
#include "server.hpp"


Server* Server::instance = 0;// Inicializar el puntero

Server::Server()
{
    //serverScript = "server.py"

    yAxis.set(0,1);
    brickPosition.set(0,0);
    brickAngle.set(0,1);
    brickAngle.normalize();
    penPosition = brickPosition + penOffset;
    //When mesured, pen position is shifted 4cm, 9cm
    penOffset.set(4,9);
}


Server* Server::getInstance()
{
    if( instance == NULL ){
        instance = new Server;
    }
    return instance;
}

//--------------------------------------------------------------
float Server::calculateAngle( const Vertex& vector0, const Vertex& vector1) const
{

    Vertex vertexCopy0 = vector0, vertexCopy1 = vector1;
    vertexCopy0[H] = 0;
    vertexCopy1[H] = 0;

    //Cross product between final vector0 and vector1
    Vertex aux = vertexCopy0*vertexCopy1;

    //Arc sin gives us the angle between the vectors
    float resAngle = acos(dotProduct(vertexCopy0, vertexCopy1));

    //Use third coordinate to control if it is clockwise or
    //anticlockwise
    if(aux[H] > 0){
        resAngle = -resAngle;
    }
    return resAngle*TO_DEGREES;
}

void Server::moveForNextPoint( const Vertex& finalPosition, const Vertex& finalVector )
{
    //Angle between final vector and yAxis, because
    //penOffset is defined on yAxis
    float finalAngle = calculateAngle(yAxis, finalVector);

    //Rotate penOffset that angle
    Vertex currentPenOffset = penOffset.rotate(finalAngle*TO_RADIANS);

    //Substract the rotated penOffset to the final position to obtain the
    //brick final position
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


void Server::sendMessage( const int leftMotor, const int rightMotor, const int pen_up ) const{
    char command[200];
    sprintf(command, "python server.py send %d %d %d", leftMotor, rightMotor, pen_up);
    system(command);
    waitAck();
}

void Server::waitAck() const
{
    char command[200];
    sprintf(command, "python server.py wait %d", WAIT_TIME);
    system(command);
}

void Server::drawPolygon( const Polygon* currentPolygon )
{
    Vertex currentVertex;
    Vertex prevVertex = currentPolygon->getVertex(0);
    float distance;

    //Place brick in the first position of the poligon
    moveForNextPoint(prevVertex, currentPolygon->getVector(0) );
    unsigned int i = 1;
    //Iterate for all other vertices, but last one
    for(; i < currentPolygon->getSize() - 1; i++){
        //Advance until next vertex
        currentVertex = currentPolygon->getVertex(i);
        distance = prevVertex.distance(currentVertex);
        sendMessage(distance*MOVE_FACTOR, distance*MOVE_FACTOR, PEN_DOWN);
        brickPosition[X] += brickAngle[X]*distance;
        brickPosition[Y] += brickAngle[Y]*distance;

        //Move the brick so it can draw next line
        moveForNextPoint(currentVertex, currentPolygon->getVector(i));
        prevVertex = currentVertex;
    }

    //Advance to draw the last line
    currentVertex = currentPolygon->getVertex(i);
    distance = prevVertex.distance(currentVertex);
    sendMessage(distance*MOVE_FACTOR, distance*MOVE_FACTOR, PEN_DOWN);
    brickPosition[X] += brickAngle[X]*distance;
    brickPosition[Y] += brickAngle[Y]*distance;
}
