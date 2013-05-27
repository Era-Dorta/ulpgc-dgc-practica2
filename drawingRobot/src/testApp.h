#ifndef TEST_APP_H
#define TEST_APP_H

#pragma once

#define L_MOUSE 0
#define R_MOUSE 2

#include "polygon.hpp"
#include "server.hpp"
#include <semaphore.h>
#include <fcntl.h> //O_CREAT and SEM_FAILED

enum AppMode {
    MODE_VISUALIZATION = 0,
    MODE_POLYGON_CREATION,
    MODE_TRANSLATION,
    MODE_ROTATION,
    MODE_SCALE
};

class testApp : public ofBaseApp
{
    private:
        int prevX;
        int prevY;

        std::vector< class Polygon >::iterator currentPolygon;
        std::vector< class Polygon > polygons;
        std::vector< class Polygon > toServerPolygons;

        AppMode appMode;

        Server* server;

        Polygon tempPolygon;

        //Vertex lastMousePress;
        Vertex currentMousePos;
        Vertex currentMouseWorldPos;

        int lastMouseX, lastMouseY;

        sem_t *mutex;

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawGUI();

	private:
        /*
        void sendMessage( const int leftMotor, const int rightMotor, const int pen_up ) const;
        void waitAck() const;
        Vertex toPolar(const int x, const int y);
        float calculateAngle( const Vertex& vector0, const Vertex& vector1) const;
        void moveForNextPoint(const Vertex& finalPosition, const Vertex& finalVector);
        */

        void addPolygon( Polygon polygon );
        void deleteLastPolygon();
};


#endif
// TEST_APP_H
