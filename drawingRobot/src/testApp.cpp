#include "testApp.h"
#define WHEEL_R 2.8
#define ROTATION_R 5.6
#define MOVE_FACTOR 360.0/(2*M_PI*WHEEL_R)
#define EXTRA_ROTATION 1.02
#define ROTATION_FACTOR (ROTATION_R/WHEEL_R)*EXTRA_ROTATION
#define TO_RADIANS M_PI/180.0
#define TO_DEGREES 180.0/M_PI
#define BOARD_SCALATION 0.25
//#include <thread>
#include <memory>
using namespace std;

//--------------------------------------------------------------

void task1(string msg)
{
    cout << "task1: hola" << endl;
}

void testApp::setup(){
    yAxis.set(0,1);
    brickPositionPolar = toPolar(10,10);
    brickPosition.set(0,0);
    brickAngle.set(0,1);
    brickAngle.normalize();
    penPosition = brickPosition + penOffset;
    //When mesured, pen position is shifted 4cm, 9cm
    penOffset.set(4,9);
    //thread t1(task1);
    //t1.join();

    addPolygon();
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
    Vertex position, vector, currentVertex, prevVertex;
    float distance;
    switch(button){
    case L_MOUSE:
        cout << "L_MOUSE 1" << endl;
        currentPolygon->addVertex( x, y );
        cout << "L_MOUSE 2" << endl;
        currentLineBegin.set( x, y );
        break;
    case R_MOUSE:
        cout << "R_MOUSE" << endl;
        currentPolygon->showPolygon();
        lastLineEnd.set( x, y );
        prevVertex = currentPolygon->getVertex(0);
        cout << "Brick Position " << brickPosition << " Brick Angle " << brickAngle << endl;
        cout << "Move to " << prevVertex << " with vector " << currentPolygon->getVector(0) << endl;
        moveForNextPoint(prevVertex, currentPolygon->getVector(0));
        cout << "Brick Position " << brickPosition << " Brick Angle " << brickAngle << endl;
        for(unsigned int i = 1; i < currentPolygon->getSize(); i++){
            currentVertex = currentPolygon->getVertex(i);
            distance = prevVertex.distance(currentVertex);
            sendMessage(distance*MOVE_FACTOR, distance*MOVE_FACTOR, PEN_DOWN);
            brickPosition[X] += brickAngle[X]*distance;
            brickPosition[Y] += brickAngle[Y]*distance;
            cout << "Brick Position " << brickPosition << " Brick Angle " << brickAngle << " distance " <<  distance << endl;
            cout << "Move to " << currentVertex << " with vector " << currentPolygon->getVector(i) << endl;
            moveForNextPoint(currentVertex, currentPolygon->getVector(i));
            cout << "Brick Position " << brickPosition << " Brick Angle " << brickAngle << endl;
            prevVertex = currentVertex;
        }

        if( !currentPolygon->getSize() ){
            currentPolygon->addVertex( x, y );
            addPolygon();
        }
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
void testApp::sendMessage( const int leftMotor, const int rightMotor, const int pen_up ) const{
    char command[200];
    sprintf(command, "python server.py send %d %d %d", leftMotor, rightMotor, pen_up);
    //system(command);
    waitAck();
}

//--------------------------------------------------------------
void testApp::waitAck() const{
    char command[200];
    sprintf(command, "python server.py wait %d", WAIT_TIME);
    //system(command);
}

//First argument is module, second is angle in radians
//--------------------------------------------------------------
Vertex testApp::toPolar(const int x, const int y){
    Vertex res;
    res[R] = sqrt(x*x + y*y);
    res[A] = atan2(x,y);
    return res;
}

//--------------------------------------------------------------
float testApp::calculateAngle( const Vertex& vector0, const Vertex& vector1) const{

    Vertex vertexCopy0 = vector0, vertexCopy1 = vector1;
    vertexCopy0[H] = 0;
    vertexCopy1[H] = 0;

    //Cross product between final vector0 and vector1
    Vertex aux = vertexCopy0*vertexCopy1;

    //Arc sin gives us the angle between the vectors
    float resAngle = asin(aux.getNorm3());

    //Use third coordinate to control if it is clockwise or
    //anticlockwise
    if(aux[H] > 0){
        resAngle = -resAngle;
    }

    //If angle is 0 but vectors are different then is a 180 degrees angle
    if(resAngle == 0 && !(vertexCopy0 == vertexCopy1) ){
        return resAngle = 180;
    }

    return resAngle*TO_DEGREES;
}

void testApp::moveForNextPoint( const Vertex& finalPosition, const Vertex& finalVector ){
    //Angle between final vector and yAxis, because
    //penOffset is defined on yAxis
    float finalAngle = calculateAngle(yAxis, finalVector);
    cout << "yAxis " << yAxis << endl;
    cout << "finalVector " << finalVector << endl;

cout << "finalAngle " << finalAngle << endl;
    //Rotate penOffset that angle
    Vertex currentPenOffset = penOffset.rotate(finalAngle*TO_RADIANS);

    //Substract the rotated penOffset to the final position to obtain the
    //brick final position
    cout << "currentpenoffset " << currentPenOffset << endl;
    Vertex auxPosition = finalPosition - currentPenOffset;

    //If position and angle are the same then do nothing
    if(auxPosition == brickPosition && finalVector == brickAngle){
        return;
    }

    float distanceToAux = brickPosition.distance(auxPosition);

    //Vector from brick current position to final brick position
    Vertex currentToAux =  auxPosition - brickPosition;
    currentToAux.normalize();

    //Calculate the angle between where the brick is looking and
    //currentToAux vector
    float rotationAngle = calculateAngle(brickAngle, currentToAux);

    //To go to auxPosition the brick must rotate rotationAngle
    sendMessage( rotationAngle*ROTATION_FACTOR, -rotationAngle*ROTATION_FACTOR, PEN_UP );
    //Go forward the distance to auxPosition
    sendMessage( distanceToAux*MOVE_FACTOR, distanceToAux*MOVE_FACTOR, PEN_UP );

    //Calculate the angle between where the brick is looking now, after rotationAngle
    //rotation, and vector auxToFinal
    rotationAngle = calculateAngle(currentToAux, finalVector);

    //Rotate the brick to look in finalVector direction
    sendMessage( rotationAngle*ROTATION_FACTOR, -rotationAngle*ROTATION_FACTOR, PEN_UP );

    //Update brick position and direction
    brickPosition = auxPosition;
    brickAngle = finalVector;
}

//--------------------------------------------------------------
void testApp::rotate(){
    //Necesito
    //vector actual
    //vector objectivo
    //Si hacemos el producto vectorial, podemos mirar el signo de la z
    //y con eso ya sabemos si es hacia derecha o i entre los dos nos da
}

//--------------------------------------------------------------
void testApp::move(){
}
