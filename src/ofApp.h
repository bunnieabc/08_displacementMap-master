#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp{
	public:
		
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

    ofShader shader, shaderBlurX, shaderBlurY, shaderGlow, shaderGaussian, shader2;
    ofFbo fboBlurOnePass;
    ofFbo fboBlurTwoPass;
    ofFbo fboBlurThreePass;
    ofFbo fbo4;
    //ofPlanePrimitive plane;
    ofSpherePrimitive sphere;
    ofImage img, img2;
    ofImage bumpmap;
    GLUquadricObj *quadric;
    int first = 0;
    ofFbo fbo;
    ofLight pointLight;
    ofVec3f center;
    
    
    //------Audio-------
    ofSoundStream soundStream;
    void audioIn( float * input, int bufferSize, int nChannels );
    vector <float> left;
    vector <float> right;
    int bufferSize = 1024;
    int bufferCounter;
    
    // load city building
    ofxAssimpModelLoader model;
    ofImage myTexture;
    ofLight    light;
    
    int pressed = 0;
};
