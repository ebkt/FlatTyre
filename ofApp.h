#pragma once

#include "ofMain.h"
#include "ofxFboBlur.h"
#include "bubbleSystem.h"

#define NUM_BYTES 2

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void mouseDragged(int x, int y, int button);
        void keyPressed(int key);
    
        bubbleSystem bubSys; // initiate bubbleSystem class
    
        // Serial conversion code from Joshua Noble's "Programming Interactivity:
        // A Designer's Guide to Proccessing, Arduino and openFrameworks" (p. 229-232, 2012)
        // available at https://books.google.co.uk/books?id=sAsHA1HM1WcC&pg=PA225&lpg=PA225&dq=serial+input+from+arduino+openframeworks&source=bl&ots=KHz3UccApe&sig=mI6JR-iB4WiLT9Q-opHa-OZiNZQ&hl=en&sa=X&ved=0ahUKEwjmvtnD_I7aAhWBWhQKHW5EAKw4ChDoAQg1MAI#v=snippet&q=ofSerial&f=false
        
        void serialSetup();
        void serialUpdate();
        // a flag for whether to send our 'handshake' to arduino
        bool                    bSendSerialMessage;
        int                     countCycles;
        unsigned char           bytesRead[NUM_BYTES];

        // average the values from the sensor with array, clamp values to remove large spikes in readings
        int                     currentSample = 0;
        int                     val;
        int                     vals[8];
        int                     clampVal;
        int                     avg;
        ofSerial                serial;
    
        // ofxFboBlur declarations
        void fboSetup();
        void fboUpdate();
        ofxFboBlur gpuBlur;
        ofTexture tex;
};
