#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    brickPosition.set(0,0);
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
        currentLineBegin.set( x, y );
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
