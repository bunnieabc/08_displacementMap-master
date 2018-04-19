#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#define NUM 50

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
    ofFbo fbo4, fbo5, fbo6;
    ofPlanePrimitive plane;
    ofSpherePrimitive sphere;
    ofImage img, img2, img3;
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
    ofxAssimpModelLoader model2;
    ofImage myTexture;
    ofLight    light;
    ofLight light1;
    ofLight light2;
    ofLight light3;
    ofMesh mesh;
    
    ofSpherePrimitive stars[NUM];
    
    int pressed = 0;
    float secondScene_time;
    float secondScene_start;
    
};
