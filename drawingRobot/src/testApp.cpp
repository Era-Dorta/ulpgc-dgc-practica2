#include "testApp.h"

// Indexes for handle left arrow and right arrow key strokes (tested in
// Ubuntu).
const unsigned int KEY_LEFT_ARROW = 356;
const unsigned int KEY_RIGHT_ARROW = 358;

#include <memory>
using namespace std;

//--------------------------------------------------------------


void testApp::setup()
{
    lastMouseX = 60;
    lastMouseY = 60;

    //Creates the semaphore with permisions rw,r,r, and 0 tokens
    mutex = sem_open("mutexForServer", O_CREAT, 0644, 0);

    if(mutex == SEM_FAILED) {
      perror("testApp: error creating semaphore");
      return;
    }

    server = Server::getInstance();

    PolygonsFile polygonsFile;

    polygonsFile.load( "data/foo.txt", &polygons );
    // TODO: Copiar tambien en toServerPolygons.

    cout << "polygons.size: " << polygons.size() << endl;
    currentPolygon = polygons.begin();

    // The server thread will be waiting for new polygons to send to the NXT.
    server->startThread(true, false); // blocking, non verbose

    //addPolygon();

    appMode = MODE_POLYGON_CREATION;
}

//--------------------------------------------------------------
void testApp::update(){
    // If there are polygons to copy to the server, copy them.
    if(toServerPolygons.size() > 0){
        cout << "To server poligons > 0\n";
        if(server->lock()){
            cout << "Copying polygons to server\n";
            for( unsigned int i = 0; i < toServerPolygons.size(); i++ ){
                server->polygons.push_back(toServerPolygons[i]);
                //Increment semaphore one token for each polygon copied
                release(mutex);
            }
            server->unlock();
            toServerPolygons.clear();
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    drawEdges();

    std::vector< class Polygon >::iterator it = polygons.begin();

    for( ; it != polygons.end(); ++it ){
        cout << "polygon.size: " << it->getSize() << endl;
        if( it == currentPolygon ){
            ofSetColor( ofColor::white);
        }else{
            ofSetColor( 150, 150, 150 );
        }
        it->draw();
        ofSetColor( ofColor::white);
    }

    tempPolygon.draw();
    Vertex origin = Polygon::getOrigin();

    Vertex currentMouseWorldPos( currentMousePos[X]-origin[X], -currentMousePos[Y]+origin[Y]  );

    if( (appMode == MODE_POLYGON_CREATION) && tempPolygon.getSize() ){
        Polygon::drawLine( tempPolygon.getLastVertex(), currentMouseWorldPos );
    }

    server->drawBrick();
    //polygon.Draw();
    drawGUI();
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
    if(x < 60 || x > 602){
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
    if( appMode != MODE_POLYGON_CREATION ){
        return;
    }

    //Do not allow to paint more than 15 cm closer to the
    //edges, since the brick could fall off the board
    //Since canvas is world cm*4, then 15*4 = 60 pixels in canvas
    if(x < 60 || x > 602){
        x = lastMouseX;
    }
    if(y < 60 || y > 410){
        y = lastMouseY;
    }

    // Register last mouse location.
    lastMouseX = x;
    lastMouseY = y;

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

void testApp::drawEdges()
{
    ofSetColor(ofColor::black);
    ofFill();
    //Draw a rectangle that shows the user drawable area
    //Top
    ofRect(0, 0, 662, 60);
    //Right
    ofRect(602, 60, 60, 410);
    //Bottom
    ofRect(0, 410, 602, 60);
    //Left
    ofRect(0, 60, 60, 350);

    ofSetColor(ofColor::white);
}

//--------------------------------------------------------------

//--------------------------------------------------------------
void testApp::exit()
{
    cout << "cerrando 0\n";
    server->exit();
    server->stopThread();
    sem_close(mutex);
    sem_unlink("mutexForServer");
    cout << "cerrando 1\n";
}

//--------------------------------------------------------------
void testApp::release( sem_t* mutex_)
{
    if(sem_post(mutex) < 0) {
      perror("main thread: error on post semaphore");
      _Exit(EXIT_FAILURE);
    }
}

