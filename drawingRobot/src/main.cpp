#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( int argc, char* argv[] )
{
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context
    Polygon::SetOrigin( 1024 >> 1, 768 >> 1 );
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());
}
