#ifndef TEST_APP_H
#define TEST_APP_H

#pragma once

#define L_MOUSE 0
#define R_MOUSE 2

#include "polygon.hpp"
#include "server.hpp"
#include "polygonsFile.hpp"
#include "ofxUI.h"
#include <string>

#define OFX_UI_NO_XML


enum AppMode {
    MODE_VISUALIZATION = 0,
    MODE_POLYGON_CREATION,
    MODE_TRANSLATION,
    MODE_ROTATION,
    MODE_SCALE,

    N_APP_MODES
};

const string appModeStr[N_APP_MODES] =
{
    "Visualization",
    "Create Polygon",
    "Translate",
    "Rotate",
    "Scale"
};


class testApp : public ofBaseApp
{
    private:
        unsigned int appW, appH;
        unsigned int guiW;

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

        ofxUICanvas *gui;
        ofxUIRadio *appModeSelector;

	public:
        testApp( const unsigned int& w, const unsigned int& h, const unsigned int& guiW );

		void setup();
		void setupGUI();

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

        void guiEvent(ofxUIEventArgs &e);
};


#endif
// TEST_APP_H
