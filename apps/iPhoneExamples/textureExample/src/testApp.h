#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);

	ofTexture		texGray;
	ofTexture 		texColor;
	ofTexture		texColorAlpha;

	int 			w, h;
		
	unsigned char 	* colorPixels;
	unsigned char 	* grayPixels;
	unsigned char 	* colorAlphaPixels;	
};


