#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // setup the bubbleSystem, serial and fbo
    bubSys.setup();
    serialSetup();
    fboSetup();

    // init parameters for ofxBox2d physics engine, serial read/write & ofxfboblur
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofDisableAntiAliasing();
    
    // set background
    ofBackground(0);
    
    // make sure there's no force applied to the ofxBox2d world
    bubSys.applyForce(false);
    
}


//--------------------------------------------------------------
void ofApp::update(){
    // update serial, bubbleSystem and fbo
    serialUpdate();
    bubSys.update();
    fboUpdate();
    
    // clamp the values from the sensor to avoid spikes triggering bubSys.applyForce()
    clampVal = ofClamp(val, 50, 380);
    
    // average the clamped values coming in from the sensor to smooth them out
    if(currentSample == 8) currentSample = 0;
        vals[currentSample] = clampVal;
        for(int i = 0; i < 8; i++){
            avg += vals[i];
        }
        avg/=8;
    
    // create deque to record the averaged readings and measure the movement between them
    deque <float> mvt;
    
    for(int i = 0; i < 12; i++){
        // push back the current avg reading
        mvt.push_back(avg);
            // check that the reading is different from nearby readings in the deque, ie. that the pump has moved
            if(mvt[i] != mvt[i+1] && mvt[i] != mvt[i+2]){
                // check the direction of movement – the force should not be applied when the pump is pulled upwards
                if(mvt[i] < mvt[i+1] && mvt[i] < mvt[i+2]){
                    // make sure the movement is > a small change from noise, and < big jumps from incorrect readings
                    if(abs(mvt[i] - mvt[i+1]) > 10 && abs(mvt[i] - mvt[i+2]) < 30 ){
                        ofResetElapsedTimeCounter();    // reset the counter (when it has been idle for 12sec the bodies will move around the screen)
                        bubSys.applyForce(true);        // applyForce to bodies in the box2d world
                    }
                }
            }
        }
    
    mvt.clear();                                    // clear the deque
    
    bubSys.applyForce(false);                       // make sure force is not applied if the above conditions are not met

    currentSample++;                                // increase the sample counter for pushing readings into the averaging array

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw the bubbleSystem to the fbo for blurring
    // code from the ofxFboBlur example
    gpuBlur.beginDrawScene();
    ofPushStyle();
        ofClear(0);
        bubSys.draw();
    ofPopStyle();
    gpuBlur.endDrawScene();

    //blur the fbo
    //blending will be disabled at this stage
    gpuBlur.performBlur();

    //draw the "clean" scene
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPushStyle();
    ofSetColor(255);
    gpuBlur.drawSceneFBO();
    ofPopStyle();

    //overlay the blur on top
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
    gpuBlur.drawBlurFbo();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);

}

//--------------------------------------------------------------
void ofApp::serialSetup(){
    
    // code from Joshua Noble's "Programming Interactivity: A Designer's Guide to Processing, Arduino & openFrameworks"
    
    countCycles = 0;                                                 // reset the cycle counter
    bSendSerialMessage = true;                                       // send a message to arduino to say oF is ready to receive serial information
    int baud = 115200;                                               // this is the rate the proximity sensor operates at
    serial.setup(0, baud);                                           // open the first device and talk to it at rate of int baud
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList(); // list the devices/ports available for serial communication
    
}

//--------------------------------------------------------------
void ofApp::serialUpdate(){

    // code from Joshua Noble's "Programming Interactivity: A Designer's Guide to Processing, Arduino & openFrameworks"
    
    // serial information is sent as bytes, which need to be converted back into usable values
    
    if(bSendSerialMessage){
        serial.writeByte('x');                                  //send a handshake to the arduino
        
                                                                // make sure there's something to write all the data to
        unsigned char bytesReturned[NUM_BYTES];
        memset(bytesReturned, 0, NUM_BYTES);
        
                                                                // keep reading until there are no bytes left to read
        while(serial.readBytes(bytesReturned, NUM_BYTES) > 0){
            val = bytesReturned[0];
            val <<= 8;                                          // shift values into correct range
            val += bytesReturned[1];
            
            bSendSerialMessage = false;                         // get ready to wait a few frames before asking again
        }
    }
    countCycles++;
    if(countCycles == 1){
        bSendSerialMessage = true;                              // send a message to the arduino to tell it oF is ready for more readings
        countCycles = 0;
    }
}

//--------------------------------------------------------------
void ofApp::fboSetup(){
    
    // declare fbo settings, modified code from ofxFboBlur example
    ofFbo::Settings s;
    
    s.width = ofGetWidth();
    s.height = ofGetHeight();
    s.internalformat = GL_RGB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;
    
    gpuBlur.setup(s, false);
    
    
}

//--------------------------------------------------------------
void ofApp::fboUpdate(){
    
    // update the fbo with desired blur settings – small changes can make enormous differences to how the bubbles look
    gpuBlur.blurOffset = 2.9;
    gpuBlur.blurPasses = 2;
    gpuBlur.numBlurOverlays = 2;
    gpuBlur.blurOverlayGain = 255;

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // this is here for debugging purposes, and also to see the force applied when arduino/sensor is not connected

    bubSys.applyForce(true);
    ofResetElapsedTimeCounter(); // once force has been applied, reset counter so the sketch exits or doesn't enter idle state
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='s'){
        ofImage screenshot;
        screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        screenshot.saveImage("screenshot_"+ofGetTimestampString()+".png"); }
}


