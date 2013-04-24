#ifndef TEST_APP_H
#define TEST_APP_H

#pragma once

#include "polygon.h"

#define WAIT_TIME 2000
#define PEN_UP 1
#define PEN_DOWN 0
#define L_MOUSE 0
#define R_MOUSE 2

class testApp : public ofBaseApp{

    private:
        //Angle in polar coordinates, first argument is
        // module, second is angle
        Vertex brickPosition;
        Vertex lastLineEnd;
        Vertex currentLineBegin;

        Polygon polygon;

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
        void sendMessage( const int leftMotor, const int rightMotor, const int pen_up ) const;
        void waitAck() const;
        Vertex toPolar(const int x, const int y);
        void rotate();
        void move();
};


#endif
// TEST_APP_H
