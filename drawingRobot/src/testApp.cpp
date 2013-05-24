#include "testApp.h"

//#include <thread>
#include <memory>
using namespace std;

//--------------------------------------------------------------

void task1(string msg)
{
    cout << "task1: hola" << endl;
}

void testApp::setup()
{
    prevX = 60;
    prevY = 60;
    server = Server::getInstance();

    //thread t1(task1);
    //t1.join();

    addPolygon();

    appMode = MODE_POLYGON_CREATION;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    std::vector< class Polygon >::iterator it = polygons.begin();

    for( ; it != polygons.end(); ++it ){
        it->draw();
    }

    if( (appMode == MODE_POLYGON_CREATION) && currentPolygon->getSize() ){
        Polygon::drawLine( currentPolygon->getLastVertex(), currentMousePos );
    }

    //polygon.Draw();
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
        x = prevX;
    }
    if(y < 60 || y > 410){
        y = prevY;
    }
    currentMousePos.set( x, y );

    currentMousePos = currentMousePos - Polygon::getOrigin();
    prevX = x;
    prevY = y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    float aux;

	if( !polygons.size() ){
	     return;
	}

    // Convert (x, y) from screen space to world space.
    //Vertex origin = Polygon::getOrigin();
    //x = x+origin[X];
    //y = y+origin[Y];
	//renderer->PointToSpace( x, y );

    // Set one transformation or another according to pressed key.
    switch( appMode ){
		case MODE_TRANSLATION:
		    currentPolygon->Translate( x-lastMouseX, y-lastMouseY );
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
void testApp::addPolygon(){
    class Polygon polygon;

    currentPolygon = polygons.insert( polygons.end(), polygon );
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //Do not allow to paint more than 15 cm closer to the
    //edges, since the brick could fall off the board
    //Since canvas is world cm*4, then 15*4 = 60 pixels in canvas

    if( appMode != MODE_POLYGON_CREATION ){
        return;
    }

    if(x < 60 || x > 602){
        x = prevX;
    }
    if(y < 60 || y > 410){
        y = prevY;
    }
    Vertex position, vector, currentVertex, prevVertex;
    //float distance;
    switch(button){
    case L_MOUSE:
        cout << "L_MOUSE 1" << endl;
        currentPolygon->addVertex( x, y );
        cout << "L_MOUSE 2" << endl;
        //currentLineBegin.set( x, y );
        //appMode = MODE_POLYGON_CREATION;
        break;
    case R_MOUSE:
        cout << "R_MOUSE" << endl;
        currentPolygon->showPolygon();
        //lastLineEnd.set( x, y );


        server->drawPolygon( &(*currentPolygon) );

        addPolygon();

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


//--------------------------------------------------------------



