/***
    Server
    ===
    Singlenton class for sending drawing commands to NXT robot.
***/

#ifndef SERVER_H
#define SERVER_H

/***
    Includes
***/
#include "polygon.hpp"
#include <semaphore.h>
#include <fcntl.h> //O_CREAT and SEM_FAILED
#include <cstdlib> //exit function
using namespace std;

/***
    Auxiliar macros
***/
// Defines whether we are sending commands to a real NXT or simply doing a
// simulation.
#define USE_BRICK false

// Time that the server waits for a response from the NXT.
#define WAIT_TIME 2000

// Pen position.
#define PEN_DOWN 0
#define PEN_UP 1

// Various factors.
#define WHEEL_R 2.8
#define ROTATION_R 5.6
#define BOARD_SCALATION 0.25
#define MOVE_FACTOR 360.0/(2*M_PI*WHEEL_R)
#define EXTRA_ROTATION 1.02
#define ROTATION_FACTOR (ROTATION_R/WHEEL_R)*EXTRA_ROTATION

// Utility macros used when transforming between radians and degrees.
#define TO_RADIANS M_PI/180.0
#define TO_DEGREES 180.0/M_PI


/***
    Main class
***/
class Server : public ofThread
{
    private:
        // Various brick parameters.
        Vertex brickPosition;
        Vertex brickAngle;
        Vertex penPosition;
        Vertex penOffset;
        Vertex lastLineEnd;
        Vertex currentLineBegin;
        Vertex yAxis;

        // Pointer to unique server instance (Singlenton pattern).
        static Server* instance;

        // Mutex for shared data access control.
        sem_t *mutex;

        /***
        1. Private constructors (Singlenton pattern)
        ***/
        Server();

        // Not implemented (singlenton pattern).
        Server(Server const&);

        // Not implemented (singlenton pattern).
        void operator=(Server const&);


        /***
        2. Shared data access control
        ***/
        void wait( sem_t * mutex_ );
        void release( sem_t* mutex_ );

    public:
        // Polygons to be drawn by NXT.
        std::vector< ofPtr<Polygon> > polygons;

        /***
        3. Initialization
        ***/
        // Get unique instance of Server class (Singlenton pattern).
        static Server* getInstance();


        /***
        4. NXT communication
        ***/
        void moveForNextPoint( const Vertex& finalPosition, const Vertex& finalVector );
        void sendMessage( const int leftMotor, const int rightMotor, const int pen_up );
        void waitAck() const ;


        /***
        5. Drawing
        ***/
        void drawPolygon( ofPtr<Polygon> polygon );
        void drawBrick() const;


        /***
        6. Server main loop
        ***/
        void threadedFunction();


        /***
        7. Finalization
        ***/
        void exit();


        /***
        8. Auxiliar methods
        ***/
        float calculateAngle( const Vertex& vector0, const Vertex& vector1) const ;
};


//

#endif // SERVER_H
