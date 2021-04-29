#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofEnableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	int v_span = 1;
	int u_span = 60;
	for (int v = 0; v <= 360; v += v_span) {

		int u_start = v;
		int next_u = v + v_span;
		for (int u = u_start; u < u_start + 360; u += u_span) {

			this->base_location_list.push_back(this->make_point(200, 50, u, v));
			this->base_location_list.push_back(this->make_point(200, 50, u + u_span, v));
			this->base_location_list.push_back(this->make_point(200, 50, next_u + u_span, v + v_span));
			this->base_location_list.push_back(this->make_point(200, 50, next_u, v + v_span));

			next_u += u_span;
		}
	}

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 3;
	while (this->log_list.size() < 3500) {

		int font_location_index = ofRandom(this->base_location_list.size());
		vector<glm::vec3> log;
		log.push_back(this->base_location_list[font_location_index]);
		this->log_list.push_back(log);
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		this->life_list.push_back(ofRandom(20, 40));
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0035, ofGetFrameNum() * 0.003)), 0, 1, -360, 360);
		auto theta = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0035, (ofGetFrameNum() + 10000) * 0.003)), 0, 1, -360, 360);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec3(radius * cos(deg * DEG_TO_RAD) * sin(theta * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD) * sin(theta * DEG_TO_RAD), radius * cos(theta * DEG_TO_RAD)));
		while (this->log_list[i].size() > 100) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	//ofRotateY(ofGetFrameNum() * 0.25);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 30) {

			ofSetLineWidth(1.5);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1.5));
		}

		ofNoFill();
		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();

		ofFill();
		ofDrawSphere(this->log_list[i].front(), 1);
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}