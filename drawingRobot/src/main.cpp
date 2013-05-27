#include "ofMain.h"
#include "testApp.h"
#include "tests.hpp"
#include "ofAppGlutWindow.h"

//========================================================================

using namespace std;



int main( int argc, char* argv[] )
{
    ofAppGlutWindow window;

    //Board size is 165.5 cm wide and 117.7 cm high
    //Our canvas is four times that size
	ofSetupOpenGL(&window, 662,470, OF_WINDOW);			// <-------- setup the GL context
    Polygon::setOrigin( 662 >> 1, 470 >> 1 );

	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());
}
