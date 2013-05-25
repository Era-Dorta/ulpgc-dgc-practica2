#include "ofMain.h"
#include "testApp.h"
#include "tests.hpp"
#include "ofAppGlutWindow.h"

//========================================================================
int main( int argc, char* argv[] )
{
    ofAppGlutWindow window;

    //Test test;

    //test.testAll();

    //return 0;

    //Tamanno de la pizarra en cm 165.5 cm de ancho por 117.7
    //Multiplicando por 4
	ofSetupOpenGL(&window, 662,470, OF_WINDOW);			// <-------- setup the GL context
    Polygon::setOrigin( 662 >> 1, 470 >> 1 );

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());
}
