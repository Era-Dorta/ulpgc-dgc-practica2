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

    appMode = MODE_VISUALIZATION;
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

    if( appMode == MODE_POLYGON_CREATION ){
        Polygon::drawLine( currentPolygon->getLastVertex(), currentMousePos );
    }
    //polygon.Draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
void testApp::mouseDragged(int x, int y, int button){

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
    if(x < 60 || x > 602){
        x = prevX;
    }
    if(y < 60 || y > 410){
        y = prevY;
    }
    Vertex position, vector, currentVertex, prevVertex;
    switch(button){
    case L_MOUSE:
        cout << "L_MOUSE 1" << endl;
        currentPolygon->addVertex( x, y );
        cout << "L_MOUSE 2" << endl;
        appMode = MODE_POLYGON_CREATION;
        break;
    case R_MOUSE:
        cout << "R_MOUSE" << endl;
        currentPolygon->showPolygon();

        server->drawPolygon( &(*currentPolygon) );

        addPolygon();

        appMode = MODE_VISUALIZATION;
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



