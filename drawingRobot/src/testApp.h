#ifndef TEST_APP_H
#define TEST_APP_H

#pragma once

#include "polygon.h"

#define WAIT_TIME 2000
#define PEN_UP 1
#define PEN_DOWN 0
#define L_MOUSE 0
#define R_MOUSE 2

enum AppMode {
    MODE_VISUALIZATION = 0,
    MODE_POLYGON_CREATION
};

class testApp : public ofBaseApp{

    private:
        //Angle in polar coordinates, first argument is
        // module, second is angle
        Vertex brickPositionPolar;
        Vertex brickPosition;
        Vertex brickAngle;
        Vertex penPosition;
        Vertex penOffset;
        Vertex lastLineEnd;
        Vertex currentLineBegin;
        Vertex yAxis;
        int prevX;
        int prevY;

        std::vector< class Polygon >::iterator currentPolygon;
        std::vector< class Polygon > polygons;

        AppMode appMode;

        Vertex currentMousePos;

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
        float calculateAngle( const Vertex& vector0, const Vertex& vector1) const;
        void moveForNextPoint(const Vertex& finalPosition, const Vertex& finalVector);

        void addPolygon();
};


#endif
// TEST_APP_H
