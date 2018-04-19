#include "ofApp.h"
float pxs;
//--------------------------------------------------------------
void ofApp::setup(){

#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
    cout << "test???" << "\n";
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
        shaderBlurX.load("shadersGL3/shaderBlurX");
        shaderBlurY.load("shadersGL3/shaderBlurY");
        shader2.load("shadersGL3/shader2");
        //shaderGlow.load("shadersGL3/shaderGlow");
        //shaderGaussian.load("shadersGL3/shaderGaussian");
        cout << "test" << "\n";
	}else{
		shader.load("shadersGL2/shader");
        cout << "test2" << "\n";
	}
#endif

    img.allocate(320, 160, OF_IMAGE_COLOR);
    img.update();
    bumpmap.load("test_tex.jpg");
    float width     = 10;
    float height    = ofGetHeight() * .35;
    sphere.setRadius( width );
    sphere.setResolution(120);
    sphere.mapTexCoordsFromTexture(img.getTexture());
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    img2.allocate(320, 160, OF_IMAGE_COLOR);
    img2.update();
    pxs = ofRandom(2,11);
    
    //FBO
    fboBlurOnePass.allocate(ofGetWidth(), ofGetHeight());
    fboBlurTwoPass.allocate(ofGetWidth(), ofGetHeight());
    fboBlurThreePass.allocate(ofGetWidth(), ofGetHeight());
    
    
    //AUDIO
    soundStream.printDeviceList();


    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    //Basic Setup
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofDisableArbTex();
    ofSetDepthTest(false);
    

    model.loadModel("building12.obj",false);
//    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
    ofLoadImage(myTexture, "test2.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
    model.update();
    
    float p = 0;
    float high = 0;
//    float mappedScale;
    float mappedVel;
    for(int i = 0; i < bufferSize - 1; i+=2) {
        // p is incrementing left channel input based on bufferSize
        cout<<left[i]<<"\n";
        p += left[i];
    }

        mappedVel = ofMap(p, -30, 30, 0, 5 );
//      mappedScale = ofMap(p, 0, 30, 1, 2);
    
    
    
    float noiseScale = ofMap(mouseX, 0, ofGetWidth(), 0, 0.3);
    float noiseVel = ofGetElapsedTimef() * mappedVel ;
    
	ofPixels pixels = img.getPixels();
    int w = img.getWidth();
    int h = img.getHeight();

    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            float noiseVelue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
            float valc = ofMap(255 * noiseVelue, 0, 255, 200, 255);
            ofColor c(valc * 0.3, valc * 0.3, ofRandom(0, 100));
            pixels.setColor(x,y, c);
        }
    }

    float noiseScale2 = ofMap(mouseY, 0, ofGetWidth(), 0, 0.6);
    int hasBuilding = ofRandom(0,1);
    
    for(int y=0; y<h; y+= 14) {
        for(int x=0; x<w; x+= 14) {
            float val = ofRandom(0,200);
            float noiseVelue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
            float noiseVelue_pxs = ofNoise(x * 0.5, y * 0.5, noiseVel);
            for(int j = 0; j< 15* noiseVelue_pxs * 0.9; j++) {
                for(int k = 0; k< 15* noiseVelue_pxs * 0.9; k++){
                    ofColor c2(255 * noiseVelue);
                    pixels.setColor(x + j, y + k, c2);
                }
            }
        }
    }
    img.setFromPixels(pixels);
    img.update();
    
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            float r = ofMap(x, 0, w, 0, 255);
            float g = ofMap(y, 0, h, 0, 255);
            float b = 255;
            float a = 1.0;
            ofColor c(r , g, b);
            pixels.setColor(x,y, c);

        }
    }
    img2.setFromPixels(pixels);
    img2.update();
//    cout<<"?????"<<"\n";
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);
    //ofFbo fboBlurOnePass;
    //ofFbo fboBlurTwoPass;
    
    float blur = ofMap(mouseX, 0, ofGetWidth(), 0, 10, true);
    // bind our texture. in our shader this will now be tex0 by default
    // so we can just go ahead and access it there.
    
    fboBlurOnePass.begin();
    {
        ofClear(0,0,0,0);
    }
    fboBlurOnePass.end();
    
    
    img.getTexture().bind();
    fboBlurOnePass.begin();
    shader.begin();
    shader.setUniformTexture("colormap", img2, 2);
    shader.setUniform1f("maxHeight", mouseX);

    
    ofPushMatrix();
    
    // translate plane into center screen.
    float tx = ofGetWidth() / 2;
    float ty = ofGetHeight() / 2;
    ofTranslate(tx, ty, ofGetElapsedTimef() * 30);

    // the mouse/touch Y position changes the rotation of the plane.
    float percentY = mouseY / (float)ofGetHeight();
    float rotation = ofMap(percentY, 0, 1, -60, 60, true) + 60;
    //ofRotate(rotation, 1, 0, 0);
    ofRotateY(360 * sinf(float(ofGetFrameNum())/500.0f));
    ofRotate(-90,1,0,0);

    sphere.draw(ofPolyRenderMode::OF_MESH_FILL);
    sphere.drawWireframe();
    ofPopMatrix();
    
    shader.end();
    fboBlurOnePass.end();
    //ofSetColor(ofColor::white);
    //img.draw(0, 0);
    //ofPushStyle();
    ofSetColor(ofColor::white);
    //ofDrawRectangle(0, 0, 0, ofGetWidth(), ofGetHeight());
    //ofPopStyle();
    //fboBlurOnePass.draw(0,0);
    
    
    //////////////// Gaussian Shader ////////////////
    /*
    fboBlurTwoPass.begin();
    {
        ofClear(0,0,0,0);
    }
    fboBlurTwoPass.end();
    fboBlurTwoPass.begin();
    
    shaderGaussian.begin();
    shaderGaussian.setUniformTexture("screenshot", fboBlurOnePass.getTexture(), 2);
    shaderGaussian.setUniform3f("resolution", (float)ofGetWidth(), (float)ofGetHeight(), 0.0);
    shaderGaussian.setUniform1f("u_sigma", 100);
    shaderGaussian.setUniform1f("u_width", (float)ofGetWidth()/30);
    //shaderBlurX.setUniform1f("iTime", ofGetElapsedTimef());
    //shaderBlurX.setUniform2f("iResolution", (float)ofGetWidth(), (float)ofGetHeight());
    //shaderGaussian.setUniform1f("blurAmnt", blur);
    
    fboBlurOnePass.draw(0, 0);
    shaderGaussian.end();
    fboBlurTwoPass.end();
    
    fboBlurTwoPass.draw(0, 0);
    */
    
    
    
    //////////////// Second Shader /////////////////
    fboBlurTwoPass.begin();
    {
        ofClear(0,0,0,0);
    }
    fboBlurTwoPass.end();
    fboBlurTwoPass.begin();
    
    shaderBlurX.begin();
    shaderBlurX.setUniformTexture("screenshot", fboBlurOnePass.getTexture(), 1);
    //shaderBlurX.setUniform1f("iTime", ofGetElapsedTimef());
    //shaderBlurX.setUniform2f("iResolution", (float)ofGetWidth(), (float)ofGetHeight());
    shaderBlurX.setUniform1f("blurAmnt", blur );
    
    fboBlurOnePass.draw(0, 0);
    
    shaderBlurX.end();
    
    fboBlurTwoPass.end();
    //fboBlurTwoPass.draw(0, 0);
    
    //////////////// third shader //////////////////
    
    fboBlurThreePass.begin();
    {
        ofClear(0,0,0,0);
    }
    fboBlurThreePass.end();
    fboBlurThreePass.begin();
    
    shaderBlurY.begin();
    shaderBlurY.setUniformTexture("screenshot2", fboBlurTwoPass.getTexture(), 1);
    shaderBlurY.setUniform1f("blurAmnt", blur );
    
    fboBlurTwoPass.draw(0, 0);
    
    shaderBlurY.end();
    
    fboBlurThreePass.end();
    
    //----------------------------------------------------------
    //ofSetColor(ofColor::white);
    fboBlurThreePass.draw(0, 0);
    //ofSetColor(ofColor::white);
    //fboBlurOnePass.clear();
    //fboBlurTwoPass.clear();
    fboBlurOnePass.draw(0, 0);
    //fboBlurThreePass.draw(0, 0);
    //fboBlurOnePass.draw(0, 0);
    
    
    
    //----------------------------------------------------------
    /*
    fbo4.begin();
    {
        ofClear(0,0,0,0);
    }
    fbo4.end();
    
    fbo4.begin();*/
    
    if(pressed ==1){
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableDepthTest();
        
        light.enable();
        ofEnableSeparateSpecularLight();

        shader.begin();
        ofSetColor(255);
        ofBackground(255, 255, 255);
        
        fboBlurOnePass.clear();
        fboBlurTwoPass.clear();
        fboBlurThreePass.clear();
        //ofTranslate(ofGetMouseX(), ofGetMouseY());
        shader.setUniformTexture("colormap", img2, 2);
        shader.setUniform1f("maxHeight", mouseX);
        
        ofPushMatrix();
        ofTranslate(mouseX,mouseY);
        model.drawFaces();
        ofPopMatrix();

        //model.draw(ofPolyRenderMode::OF_MESH_FILL);
        ofDisableDepthTest();
//        model.drawWireframe();
        shader.end();
        //fbo4.end();
        
        
        
        //fbo4.clear();
    }
    
    //----------------------------------------------------------
    
    
    
    
}


//--------------------------------------------------------------

void ofApp::audioIn( float * input, int bufferSize, int nChannels ){
    

    for ( int i = 0; i < bufferSize; i++ ) {
        left[i]     = input[i*2]*.5;
        right[i]    = input[i*2+1]*.5;
    }
    
    bufferCounter++;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='a'){
        ofSetFrameRate(60);
        ofSetVerticalSync(true);
        ofSetDepthTest(true);
//        model.enableColors();
//        model.enableTextures();
//        model.enableTextures();
        model.loadModel("building12.obj");
        ofLoadImage(myTexture, "test2.jpg");
        ofDisableArbTex();
        ofEnableDepthTest();
        pressed = 1;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
