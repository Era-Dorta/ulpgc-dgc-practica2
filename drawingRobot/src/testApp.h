#ifndef TEST_APP_H
#define TEST_APP_H

#pragma once

#include "polygon.hpp"
#include "server.hpp"
#include "polygonsFile.hpp"
#include <semaphore.h>
#include <fcntl.h> //O_CREAT and SEM_FAILED
#include <cstdlib> //exit function
using namespace std;
// This macro tells the GUI not to load or save its current state to a XML
// file.
#define OFX_UI_NO_XML

#include "ofxUI.h"
// User Interface Library/Addon for openFrameworks created by rezaali :
// https://github.com/rezaali/ofxUI
#include <string>

// Auxiliar macros for handling mouse button strokes.
#define L_MOUSE 0
#define R_MOUSE 2

// Different app modes.
enum AppMode {
    MODE_VISUALIZATION = 0,
    MODE_POLYGON_CREATION,
    MODE_TRANSLATION,
    MODE_ROTATION,
    MODE_SCALE,

    N_APP_MODES
};


// The GUI displays to the user one of the following strings according to the
// current app mode.
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
        // Dimensions of the sub-window where polygons are rendered.
        unsigned int appW, appH;

        // GUI width. The GUI takes up a vertical panel on the left of the app,
        // so its height equals to appH.
        unsigned int guiW;

        // Mouse position during the last frame.
        int lastMouseX, lastMouseY;

        // Container for all polygons drawn by user.
        std::vector< class Polygon > polygons;

        // Iterator pointing to current or active polygon. Transformations
        // such as translations, rotations, etc will affect this polygon.
        std::vector< class Polygon >::iterator currentPolygon;

        // Container for polygons waiting to be copied to the NXT server.
        std::vector< class Polygon > toServerPolygons;

        // Current app mode.
        AppMode appMode;

        // Pointer to the server which will send the drawing commands to the
        // NXT.
        Server* server;

        // Polygon which the user is creating.
        Polygon tempPolygon;

        // Vertex lastMousePress;
        Vertex currentMousePos;
        Vertex currentMouseWorldPos;

        // Pointer to GUI.
        ofxUICanvas* gui;

        // Pointer to GUI radio for selecting app mode.
        ofxUIRadio* appModeSelector;

        // Text input for submiting saving / loading file.
        ofxUITextInput* fileInput;

        // Pointers to buttons for saving / loading.
        ofxUILabelButton* savingButton;
        ofxUILabelButton* loadingButton;

        // Pointer to warning label (used when a file is not found).
        ofxUILabel* fileNotFoundLabel;

        ofxUILabelButton* deletingButton;

        sem_t* mutex;

	public:
        // 1. Initializations
        testApp( const unsigned int& w, const unsigned int& h, const unsigned int& guiW );
		void setup();
		void setupGUI();

        // 2. Events
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// Listener for ofxUI GUI library
		void guiEvent(ofxUIEventArgs &e);

        // 3. Updating and drawing
        void update();
        void draw();

        // 4. Exit
		void exit();

		// 5. Auxiliar methods
        bool pointOnRenderWindow( const int& x, const int& y );

	private:

        // 6. Polygons administration
        void addPolygon( Polygon polygon );
        void deleteLastPolygon();
        void deleteCurrentPolygon();

        void release(sem_t* mutex);
        void drawEdges();
};


#endif
// TEST_APP_H
