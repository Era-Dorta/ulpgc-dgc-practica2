
#ifndef SERVER_H
#define SERVER_H

#include "polygon.hpp"

#define USE_BRICK false

#define WAIT_TIME 2000
#define PEN_UP 1
#define PEN_DOWN 0

#define WHEEL_R 2.8
#define ROTATION_R 5.6
#define BOARD_SCALATION 0.25
#define MOVE_FACTOR 360.0/(2*M_PI*WHEEL_R)
#define EXTRA_ROTATION 1.02
#define ROTATION_FACTOR (ROTATION_R/WHEEL_R)*EXTRA_ROTATION
#define TO_RADIANS M_PI/180.0
#define TO_DEGREES 180.0/M_PI

class Server : public ofThread
{
    private:
        //String serverScript;

        Vertex brickPosition;
        Vertex brickAngle;
        Vertex penPosition;
        Vertex penOffset;
        Vertex lastLineEnd;
        Vertex currentLineBegin;
        Vertex yAxis;

        static Server* instance;

        Server();

        Server(Server const&);              // Don't Implement
        void operator=(Server const&); // Don't implement

    public:
        std::vector< Polygon > polygons;


    public:
        static Server* getInstance();

        float calculateAngle( const Vertex& vector0, const Vertex& vector1) const ;

        void moveForNextPoint( const Vertex& finalPosition, const Vertex& finalVector );

        void sendMessage( const int leftMotor, const int rightMotor, const int pen_up );

        void waitAck() const ;

        void drawPolygon( Polygon polygon ) ;

        void drawBrick() const;

        void threadedFunction();
};


//

#endif // SERVER_H
