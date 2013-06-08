/***
    Main rutine
    ===
    Setup the GL context and runs the "Drawing Robot" program.
***/

#include "ofMain.h"
#include "testApp.h"
#include "tests.hpp"
#include "ofAppGlutWindow.h"
using namespace std;

//========================================================================

// Width of the GUI (left tools menu).
const unsigned int GUI_W = 300;

// Main drawing subwindow dimensions. Dimensions of full app window are
// (APP_W+GUI_W, APP_H).
// ===
// Board size is 165.5 cm wide and 117.7 cm high.
// Our canvas is four times that size
const unsigned int APP_W = 662;
const unsigned int APP_H = 470;


int main( int argc, char* argv[] )
{
    // Setup the GL context.
    ofAppGlutWindow window;
	ofSetupOpenGL( &window, GUI_W+APP_W, APP_H, OF_WINDOW );

	// Set the coordinates system origin at the center of the drawing
	// subwindow.
    Polygon::setOrigin( GUI_W+(APP_W >> 1), APP_H >> 1 );

    // Those polygons falling outside of the drawing subwindow can't be drawn
    // by robot. Let's inform Polygon class about those limits.
    Polygon::setCoordinatesThresholds( GUI_W+RENDER_WINDOW_BORDER,
                                       RENDER_WINDOW_BORDER,
                                       GUI_W+APP_W-RENDER_WINDOW_BORDER,
                                       APP_H-RENDER_WINDOW_BORDER );

	// Run app.
	ofRunApp( new testApp( APP_W, APP_H, GUI_W ) );
}
