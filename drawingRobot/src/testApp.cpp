#include "testApp.h"
#define WHEEL_R 2.8
#define ROTATION_R 5.6
#define MOVE_FACTOR 360.0/(2*M_PI*WHEEL_R)
#define EXTRA_ROTATION 1.02
#define ROTATION_FACTOR (ROTATION_R/WHEEL_R)*EXTRA_ROTATION
#define TO_RADIANS M_PI/180.0
#define TO_DEGREES 180.0/M_PI

//--------------------------------------------------------------
void testApp::setup(){
    brickPositionPolar = toPolar(10,10);
    brickPosition.set(10,10);
    brickAngle.set(0,1);
    //When mesured, pen position is shifted 4cm, 9cm
    penOffset.set( 4, 9);
    penPosition = brickPosition + penOffset;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){

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
void testApp::mousePressed(int x, int y, int button){
    switch(button){
    case L_MOUSE:
        moveForNextPoint();
        //currentLineBegin.set( x, y );
        //sendMessage( 90*ROTATION_FACTOR, -90*ROTATION_FACTOR, PEN_UP );
        //sendMessage( 20*MOVE_FACTOR, 20*MOVE_FACTOR, PEN_UP );
        //On draw line
        // v = toPolar(x,y);
        //Rotate
        // angle = v[R] - brickPosition[R];
        //sendMessage( angle, -angle, PEN_UP );
        //brickPosition = v[R];
        //Move
        // r = v[A] - brickPosition[A];
        //sendMessage( r, r, PEN_UP );
        //brickPosition = v[A];
        break;
    case R_MOUSE:
        lastLineEnd.set( x, y );
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
    system(command);
    waitAck();
}

//--------------------------------------------------------------
void testApp::waitAck() const{
    char command[200];
    sprintf(command, "python server.py wait %d", WAIT_TIME);
    system(command);
}

//First argument is module, second is angle in radians
//--------------------------------------------------------------
Vertex testApp::toPolar(const int x, const int y){
    Vertex res;
    res[R] = sqrt(x*x + y*y);
    res[A] = atan2(x,y);
    return res;
}



void testApp::moveForNextPoint(){
    Vertex finalPosition(14, 19);
    Vertex finalVector(1,-1);
    //TODO El vector si hay que normalizarlo y hacer lo de rotar la
    //posicion del rotulador pa saber cuanto se le resta a aux position
    //finalVector.normalize();
    Vertex auxPosition = finalPosition - penOffset*finalVector;
    float distanceToAux = brickPosition.distance(auxPosition);
    Vertex currentToAux =  auxPosition - brickPosition;
    currentToAux.normalize();
    float auxAngle = acos(dotProduct(brickAngle,currentToAux))*TO_DEGREES;

    sendMessage( auxAngle*ROTATION_FACTOR, -auxAngle*ROTATION_FACTOR, PEN_UP );
    sendMessage( distanceToAux*MOVE_FACTOR, distanceToAux*MOVE_FACTOR, PEN_UP );

    Vertex auxToFinal = finalPosition - auxPosition;
    auxToFinal.normalize();
    Vertex auxToCurrent = brickPosition - auxPosition;
    auxToCurrent.normalize();
    auxAngle = 180 + acos(dotProduct(auxToFinal,auxToCurrent))*TO_DEGREES;
    sendMessage( -auxAngle*ROTATION_FACTOR, auxAngle*ROTATION_FACTOR, PEN_UP );

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
