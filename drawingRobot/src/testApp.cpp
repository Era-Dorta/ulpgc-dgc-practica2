#include "testApp.h"

// Indexes for handle left arrow and right arrow key strokes (tested in
// Ubuntu).
const unsigned int KEY_LEFT_ARROW = 356;
const unsigned int KEY_RIGHT_ARROW = 358;

#include <memory>
using namespace std;

//--------------------------------------------------------------

testApp::testApp( const unsigned int& w, const unsigned int& h, const unsigned int& guiW )
{
    appW = w;
    appH = h;
    this->guiW = guiW;
}

void testApp::setup()
{
    lastMouseX = guiW+60;
    lastMouseY = 60;
    server = Server::getInstance();

    PolygonsFile polygonsFile;

    polygonsFile.load( "data/foo.txt", &polygons );
    // TODO: Copiar tambien en toServerPolygons.

    cout << "polygons.size: " << polygons.size() << endl;
    currentPolygon = polygons.begin();

    // The server thread will be waiting for new polygons to send to the NXT.
    server->startThread(true, false); // blocking, non verbose

    //addPolygon();

    setupGUI();

    appMode = MODE_POLYGON_CREATION;
}


void testApp::setupGUI()
{
    unsigned int i;

    gui = new ofxUICanvas(0,0,guiW,appH);		//ofxUICanvas(float x, float y, float width, float height)

    //gui->addWidgetDown(new ofxUILabel("OFXUI TUTORIAL", OFX_UI_FONT_LARGE));
    //gui->addWidgetDown(new ofxUISlider(304,16,0.0,255.0,100.0,"BACKGROUND VALUE"));
    //ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);

    gui->addSpacer();
    vector<string> vnames;
    for( i=0; i<N_APP_MODES; i++ ){
        vnames.push_back( appModeStr[i] );
    }
    gui->addLabel("APP MODE", OFX_UI_FONT_MEDIUM);
    appModeSelector = gui->addRadio("VR", vnames, OFX_UI_ORIENTATION_VERTICAL);
    appModeSelector->activateToggle( appModeStr[MODE_POLYGON_CREATION] );
    gui->addSpacer();

    //gui->loadSettings("GUI/guiSettings.xml");
}

//--------------------------------------------------------------
void testApp::update(){
    //cout << "Soy main thread\n";

    // If there are polygons to copy to the server, copy them.
    if(toServerPolygons.size() > 0){
        cout << "To server poligons > 0\n";
        if(server->lock()){
            cout << "Copying polygons to server\n";
            for( unsigned int i = 0; i < toServerPolygons.size(); i++ ){
                server->polygons.push_back(toServerPolygons[i]);
            }
            server->unlock();
            toServerPolygons.clear();
        }
    }

    ofxUIToggle* appModeSelection = appModeSelector->getActive();
    for( unsigned int i=0; i<N_APP_MODES; i++ ){
        if( !appModeSelection->getName().compare( appModeStr[i] ) ){
            appMode = (AppMode)i;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    std::vector< class Polygon >::iterator it = polygons.begin();

    for( ; it != polygons.end(); ++it ){
        if( it == currentPolygon ){
            ofSetColor( 255, 255, 255 );
        }else{
            ofSetColor( 150, 150, 150 );
        }
        it->draw();
        ofSetColor( 255, 255, 255 );
    }

    tempPolygon.draw();
    Vertex origin = Polygon::getOrigin();

    Vertex currentMouseWorldPos( currentMousePos[X]-origin[X], -currentMousePos[Y]+origin[Y]  );

    if( (appMode == MODE_POLYGON_CREATION) && tempPolygon.getSize() ){
        Polygon::drawLine( tempPolygon.getLastVertex(), currentMouseWorldPos );
    }

    server->drawBrick();
    //polygon.Draw();
    //drawGUI();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch( key ){
        case 'c':
            appMode = MODE_POLYGON_CREATION;
        break;
        case 't':
            appMode = MODE_TRANSLATION;
        break;
        case 'r':
            appMode = MODE_ROTATION;
        break;
        case 's':
            appMode = MODE_SCALE;
        break;
        case KEY_LEFT_ARROW:
            if( currentPolygon == polygons.begin() ){
                currentPolygon = polygons.end();
            }
            currentPolygon--;
        break;
        case KEY_RIGHT_ARROW:
            currentPolygon++;
            if( currentPolygon == polygons.end() ){
                currentPolygon = polygons.begin();
            }
        break;
        default:
            cout << "key: " << key << endl;
        break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //Do not allow to paint more than 15 cm closer to the
    //edges, since the brick could fall off the board
    //Since canvas is world cm*4, then 15*4 = 60 pixels in canvas
    if(x < guiW+60 || x > guiW+602){
        x = lastMouseX;
    }
    if(y < 60 || y > 410){
        y = lastMouseY;
    }

    currentMousePos.set( x, y );

    //currentMousePos = currentMousePos - Polygon::getOrigin();
    lastMouseX = x;
    lastMouseY = y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    float aux;

	if( !polygons.size() ){
	     return;
	}

    //Convert (x, y) from screen space to world space.
    //Polygon::PixelToWorld( x, y );

	//renderer->PointToSpace( x, y );

    // Set one transformation or another according to pressed key.
    switch( appMode ){
		case MODE_TRANSLATION:
		    currentPolygon->Translate( x-lastMouseX, -y+lastMouseY );
		break;
        case MODE_ROTATION:
            aux = x-lastMouseX;
            currentPolygon->Rotate( aux );
        break;
        case MODE_SCALE:
            aux = x-lastMouseX;

            aux = aux ? 1+aux*0.01 : 1;
            currentPolygon->Scale( aux, aux );
        break;
        default:
        break;
    }

    // Register last mouse location.
    lastMouseX = x;
    lastMouseY = y;
}

//--------------------------------------------------------------
void testApp::addPolygon( Polygon polygon ){
    currentPolygon = polygons.insert( polygons.end(), polygon );
}

void testApp::deleteLastPolygon()
{
    polygons.pop_back();
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    bool exit = false;
    if( appMode != MODE_POLYGON_CREATION ){
        return;
    }

    //Do not allow to paint more than 15 cm closer to the
    //edges, since the brick could fall off the board
    //Since canvas is world cm*4, then 15*4 = 60 pixels in canvas
    if(x < guiW+60 || x > guiW+602){
        //return;
        exit = true;
        x = lastMouseX;
    }
    if(y < 60 || y > 410){
        //return;
        exit = true;
        y = lastMouseY;
    }

    // Register last mouse location.
    lastMouseX = x;
    lastMouseY = y;

    // TODO: Esto esta a boleo.
    if( exit ){
        return;
    }

    Vertex position, vector, currentVertex, prevVertex;
    switch(button){
    case L_MOUSE:
        cout << "L_MOUSE 1" << endl;
        tempPolygon.addVertex( x, y );
        cout << "L_MOUSE 2" << endl;
        //currentLineBegin.set( x, y );
        //appMode = MODE_POLYGON_CREATION;
        break;
    case R_MOUSE:
        cout << "R_MOUSE" << endl;
        tempPolygon.showPolygon();
        //lastLineEnd.set( x, y );

        addPolygon( tempPolygon );

        toServerPolygons.push_back(tempPolygon);

        tempPolygon.clear();

        //appMode = MODE_VISUALIZATION;
        cout << "R_MOUSE 2" << endl;
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void testApp::drawGUI()
{
    const int X_ = 10;
    const int Y_ = 20;
    unsigned int i=0;
    char currentMode[255];

    ofSetColor( 255, 255, 255 );

    // Display current app mode.
    strcpy( currentMode, "Current mode: " );
    switch( appMode ){
        case MODE_VISUALIZATION:
            strcat( currentMode, "Visualization" );
        break;
        case MODE_POLYGON_CREATION:
            strcat( currentMode, "Create polygon" );
        break;
		case MODE_TRANSLATION:
            strcat( currentMode, "Translation" );
		break;
        case MODE_ROTATION:
            strcat( currentMode, "Rotation" );
        break;
		case MODE_SCALE:
            strcat( currentMode, "Scale" );
		break;
    }
    ofDrawBitmapString( currentMode, X_, Y_+i*20 );
    i++;

    //ofDrawBitmapString( "Keep key 'h' pressed to access help", 20, h_-15 );
}

//--------------------------------------------------------------

//--------------------------------------------------------------
void testApp::exit()
{
    PolygonsFile polygonsFile;
    polygonsFile.save( "data/foo.txt", &polygons );
    server->stopThread();

    //gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}


void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "BACKGROUND VALUE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        ofBackground(slider->getScaledValue());
    }
}
