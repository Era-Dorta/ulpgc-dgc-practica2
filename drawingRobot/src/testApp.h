#pragma once

#include "ofMain.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

#define WAIT_TIME 2000

class testApp : public ofBaseApp{

    private:


	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
        void sendMessage( const int x, const int y, const int pen_up ) const;
        void waitAck() const;
};
