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
    img3.allocate(320, 160, OF_IMAGE_COLOR);
    img3.update();
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
    
    model.loadModel("2.dae");
    model.setPosition(ofGetWidth() * 0.85, (float)ofGetHeight() * 0.60 , -100);
    model.setScale(2, 2, 2);
    model2.loadModel("city.obj");
    model2.setPosition(ofGetWidth() * 0.35, (float)ofGetHeight() -200, -200);
    model2.setScale(5,5,5);
    light1.enable();
    light1.setPosition(ofVec3f(-50,0,100));
    light1.lookAt(ofVec3f(0,0,0));

    light2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    light2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));

    light3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    light3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    //---------Object Drawing
    for (int i=0; i<NUM; i++) {
        stars[i].setPosition(ofRandom(-500,500), ofRandom(-500,500), ofRandom(-500,500));
        stars[i].set(ofRandom(0.1,0.5), 10);
    };
    
    plane.set(800, 800, 80, 80);
    plane.mapTexCoordsFromTexture(img.getTexture());
    
    //secondScene = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    model.update();
    model2.update();
    float p = 0;
    float high = 0;
//    float mappedScale;
    float mappedVel;
    for(int i = 0; i < bufferSize - 1; i+=2) {
        // p is incrementing left channel input based on bufferSize

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
    
    
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            float r = ofMap(x, 0, w, 0, 155);
            float g = ofMap(y, 0, h, 0, 100);
            float b = 200;
            float a = 0.8;
            ofColor c(r , g, b);
            pixels.setColor(x,y, c);
            
        }
    }
    img3.setFromPixels(pixels);
    img3.update();
//    cout<<"?????"<<"\n";
    
    light2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                       ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);

    light3.setPosition( cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                       sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                       cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                       );
    
    //secondScene_start = ofGetElapsedTimef();
    secondScene_time = ofGetElapsedTimef() - secondScene_start;
    
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
    
    //---------Star Drawing

    if(pressed ==1){
        
        
//        ofPushMatrix();
//        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
//        ofRotateY(ofGetElapsedTimef() / 5);
//        for (int i=0; i< NUM; i++) {
//            ofSetColor(ofRandom(100,255),ofRandom(100,255),ofRandom(100,255));
//            stars[i].draw();
//        }
//        ofPopMatrix();
        
        
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableDepthTest();
        light.enable();
        ofEnableSeparateSpecularLight();
        ofTranslate(0, 0, ofGetElapsedTimef() * 10 * secondScene_time);
        ofRotate(sin(secondScene_time)*15);


        //shader2.begin();
        ofSetColor(255);
        ofBackground(180, 180, 200);
        

        fboBlurOnePass.clear();
        fboBlurTwoPass.clear();
        fboBlurThreePass.clear();
        //ofTranslate(ofGetMouseX(), ofGetMouseY());
        //shader2.setUniformTexture("colormap", img2, 2);
        //shader2.setUniform1f("maxHeight", mouseX);
        ofColor materialColor = ofColor::red;
        shader2.setUniform4f("uMaterialColor", ofColor(materialColor));
        ofPushMatrix();
//        ofTranslate(mouseX,mouseY);
//        plane.draw(ofPolyRenderMode::OF_MESH_FILL);
        model.drawWireframe();
        model2.drawWireframe();
        ofPopMatrix();
        
        //model.draw(ofPolyRenderMode::OF_MESH_FILL);
        ofDisableDepthTest();
        //        model.drawWireframe();
        //shader2.end();
        //fbo4.end();

        
        //fbo4.clear();
        
        shader.begin();
        
        ofPushMatrix();
        
        // translate plane into center screen.
        float tx = ofGetWidth() / 2;
        float ty = ofGetHeight() / 2;
        ofTranslate(tx, ty+150);
        
        // the mouse/touch Y position changes the rotation of the plane.
        float percentY = mouseY / (float)ofGetHeight();
        float rotation = ofMap(percentY, 0, 1, -60, 60, true) + 60;
        ofRotate(90, 1, 0, 0);
        shader.setUniformTexture("colormap", img3, 3);
        //shader.setUniformTexture("colormap", img2, 2);
        shader.setUniform1f("maxHeight", mouseX);
        plane.drawWireframe();
        ofPopMatrix();
        

        light.setAmbientColor(ofFloatColor::red);
        light.setAmbientColor(ofFloatColor::blue);

        shader.end();
    }
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
        ofLoadImage(myTexture, "test2.jpg");
        ofEnableDepthTest();
        pressed = 1;
        secondScene_start = ofGetElapsedTimef();
        //secondScene_time = ofGetElapsedTimef() - secondScene_start;
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
