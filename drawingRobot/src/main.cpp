#include "ofMain.h"
#include "testApp.h"
#include "tests.hpp"
#include "ofAppGlutWindow.h"

//========================================================================

const unsigned int GUI_W = 300;
const unsigned int APP_W = 662;
const unsigned int APP_H = 470;

using namespace std;



int main( int argc, char* argv[] )
{
    ofAppGlutWindow window;

    //Board size is 165.5 cm wide and 117.7 cm high
    //Our canvas is four times that size
	ofSetupOpenGL(&window, GUI_W+APP_W, APP_H, OF_WINDOW);			// <-------- setup the GL context
    Polygon::setOrigin( GUI_W+(APP_W >> 1), APP_H >> 1 );

    Polygon::setCoordinatesThresholds( GUI_W+RENDER_WINDOW_BORDER,
                                       RENDER_WINDOW_BORDER,
                                       GUI_W+APP_W-RENDER_WINDOW_BORDER,
                                       APP_H-RENDER_WINDOW_BORDER );

	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp( APP_W, APP_H, GUI_W ) );
}
