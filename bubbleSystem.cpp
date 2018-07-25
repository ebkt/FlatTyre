//
//  bubbleSystem.cpp
//  BikeSerial
//
//  Created by edbkt on 11/04/2018.
//

#include "bubbleSystem.h"

//--------------------------------------------------------------
bubbleSystem::bubbleSystem(){

}
//--------------------------------------------------------------
void bubbleSystem::setup(){

    // initiate ofxBox2d parameters: gravity, fps, boundaries
    
    box2d.init();
    box2d.setGravity(0.0, 5.5); // set a small amount of gravity on the y axis so the bubbles fall when no force is applied
    box2d.setFPS(60.0);
    box2d.createBounds();       // the bodies created will bounce off the walls, so create boundaries
    numBubbles = 700;           // this number can be changed depending on screen size – more bubbles on a larger canvas

    locY = ofGetHeight()/3;     // declare the height at which the bubbles will congregate after the installation has been idle for a while
    
    
    // create ofxBox2dCircles w/ random radius, set physics (again, small changes completely change the way the bodies move in the space)
    for(int i = 0; i < numBubbles; i++){
        float r = ofRandom(9, 17);
        shared_ptr<ofxBox2dCircle> circle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(3.0, 0.9, 0.5); // set density, bounce and friction values for the bubbles
        circle.get()->setup(box2d.getWorld(), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()),  r); // put each bubble in a random location
        circles.push_back(circle);                                                                  // add it to the deque
    }
    
    
}

//--------------------------------------------------------------
void bubbleSystem::update(){
    
    box2d.update(); // update box2d world – this controls the movement of the shapes in the space in accordance with the settings declared in the setup
    
    locX = ofMap(sin(ofGetElapsedTimeMillis()*0.0001), -1, 1, ofGetWidth()*0.35, ofGetWidth()*0.65); // map x location for movement during idle state
    
    if(ofGetElapsedTimef() > 12){                                   // if the pump hasn't moved in a while
        for(int i = 0; i < circles.size(); i++){                    // loop through all the bubbles
            circles[i].get()->addAttractionPoint(locX, locY, 0.5);  // add a moving attraction point so they don't just sit on the bottom (boring)
            circles[i].get()->setRotation(0.001);                   // rotate the bodies slightly so they seem to swarm around the attraction point
        }
    }
}

//--------------------------------------------------------------
void bubbleSystem::draw(){
    
    for(int i = 0; i < numBubbles; i ++){
        ofFill();
        ofColor c1 = ofColor(0, 130, 100);                                              // set green colour
        ofColor c2 = ofColor(0, 80, 220);                                               // set blue colour
        ofColor col = c2.getLerped(c1, ofMap(i, 0, numBubbles-1, 0, 1));                // lerp between these two, depending on position in deque

        ofSetColor(c2.lerp(c1, ofMap(i, 0, numBubbles-1, 0, 1)));                       // set colour to lerp
        
          //////////////////////////////////////////////
         //        circles[i].get()->draw();         // this is the correct way to draw box2d bodies, but it gave me a lot of issues with
        //////////////////////////////////////////////  colours of bodies flashing and changing unexpectedly, so i've used the workaround below
        
        
        // take the location and radius of the circle at [i] and draw a circle of the correct colour there.
        // no noticeable drop in performance, but I know this isn't ideal
        ofDrawCircle(circles[i].get()->getPosition(), circles[i].get()->getRadius());
        
        // set orange-y colour and draw a qtr size circle within the larger one – gives the bodies a bit more glow and a slight celestial quality
        ofSetColor(255,    140,    86);
        ofDrawCircle(circles[i].get()->getPosition(), (circles[i].get()->getRadius()*0.4));
        
        }


}
//--------------------------------------------------------------

void bubbleSystem::applyForce(bool _force){
    
    // if the movement conditions of the pump are ment, apply force to all of the bubbles
    for(int i = 0; i < circles.size(); i++){
        if(_force){
            // i've found that a combination of attraction and repulsion forces gives the best results
            circles[i].get()->addAttractionPoint(0, 0, 0.1);                         // add weak attraction point at the top left of the sketch
            circles[i].get()->addAttractionPoint(ofGetWidth(), 0, 0.1);              // add weak attraction point at the top right of the sketch
            circles[i].get()->addRepulsionForce(ofGetWidth()/2, ofGetHeight(), 0.3); // add slightly stronger repulsion from the middle at the bottom
        }
    }
}
//--------------------------------------------------------------
