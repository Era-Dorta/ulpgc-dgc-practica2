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
    brickAngle.normalize();
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
    Vertex finalPosition(14,24);
    Vertex finalVector(1,1);
    Vertex yAxis(0,1);

    finalVector.normalize();

    //Dot product between final vector and yAxis, because
    //penOffset is defined on yAxis
    Vertex aux = finalVector*yAxis;

    //Arc sin gives us the angle between the vectors
    float finalAngle = asin(aux.getNorm());

    //Use third coordinate to control if it is clockwise or
    //anticlockwise
    if(aux[Z] < 0){
        finalAngle = -finalAngle;
    }

    //If angle is 0 but vectors are different then is a 180 degrees angle
    if(finalAngle == 0 && !(finalVector == yAxis) ){
        finalAngle = 180*TO_RADIANS;
    }

    //Rotate penOffset that angle
    Vertex currentPenOffset = penOffset.rotate(finalAngle);

    //Substract the rotated penOffset to the final position to obtain the
    //brick final position
    Vertex auxPosition = finalPosition - currentPenOffset;
    float distanceToAux = brickPosition.distance(auxPosition);

    //Vector from brick current position to final brick position
    Vertex currentToAux =  auxPosition - brickPosition;
    currentToAux.normalize();

    //Calculate the angle between where the brick is looking and
    //currentToAux vector
    float auxAngle = acos(dotProduct(brickAngle,currentToAux))*TO_DEGREES;
        cout << "auxPosition " << auxPosition << endl;
        cout << "auxAngle " << auxAngle << endl;
        cout << "distanceToAux " << distanceToAux << endl;
    //To go to auxPosition the brick must rotate auxAngle
    sendMessage( auxAngle*ROTATION_FACTOR, -auxAngle*ROTATION_FACTOR, PEN_UP );
    //Go forward the distance to auxPosition
    sendMessage( distanceToAux*MOVE_FACTOR, distanceToAux*MOVE_FACTOR, PEN_UP );

    //Calculate the angle between where the brick is looking now, after auxAngle
    //rotation, and vector auxToFinal
    auxAngle = acos(dotProduct(finalVector,currentToAux))*TO_DEGREES;
        cout << "auxAngle " << auxAngle << endl;
    //Rotate the brick to look in finalVector direction
    sendMessage( auxAngle*ROTATION_FACTOR, -auxAngle*ROTATION_FACTOR, PEN_UP );

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
