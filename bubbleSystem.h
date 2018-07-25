//
//  bubbleSystem.hpp
//  BikeSerial
//
//  Created by edbkt on 11/04/2018.
//
#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class bubbleSystem{
    public:
        bubbleSystem();
    
        void setup();
        void update();
        void draw();
        void applyForce(bool _force); // applyForce function for when the pump is pumped    
    
        // create instance of ofxBox2d
        ofxBox2d box2d;
    
        // number of bubbles to be created in the space
        int numBubbles;
    
        // location for attraction point when sketch has been idle for a certain period of time
        float locX, locY;
    
        // ofxBox2d bodies need to be created in a specific way, and pushing back normal
        // ofDrawCircle/ofDrawEllipse values is not recommended in the documentation
        deque <shared_ptr<ofxBox2dCircle>> circles;

    
};
