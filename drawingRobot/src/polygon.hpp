/***
    Polygon
    ===
    Class used for defining and transforming 2D polygons, drawable by NXT
    robot.
***/

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "vertex.hpp"

// Enum type for any type of error that prevents robot from drawing a polygon
// in real world.
enum RobotDrawingErrors
{
    NONE = 0,
    VERTEX_OUT_OF_BORDERS,
    IMPOSSIBLE_ANGLE
};

class Polygon
{
    private:
        // Coordinates system origin (for all polygons).
        static int ox, oy;

        // Thresholds for polygons drawable by the robot.
        static int minX, maxX;
        static int minY, maxY;

        // Original polygon vertexes.
        std::vector< Vertex > v;

        // Transformed polygon vertexes (These are obtained by multiplying
        // original ones by transformation matrix).
        std::vector< Vertex > transV;

        // Original and transformed polygon vertexes adapted to real world
        // (robot) coordinates.
        std::vector< Vertex > vScalated;
        std::vector< Vertex > transVScalated;

        // Array of normalized vectors joining each vertex with next one. ie.
        // vectors[i] = normalized( v[i+1] - v[i] );
        // transVectors[i] = normalized( transV[i+1] - transV[i] )
        // Vectors are used for robot orientation while drawing.
        std::vector< Vertex > vectors;
        std::vector< Vertex > transVectors;

        // Transformation matrix.
        Matrix transMatrix;

        // This atribute indicates whether the polygon can be drawn by the
        // robot in real world or exists any problem.
        RobotDrawingErrors robotDrawingErrors;

    public:
        /***
        1. Initializations
        ***/
        Polygon();
        void clear();


        /***
        2. Coordinates system origin administration
        ***/
        static Vertex getOrigin();
        static void setOrigin( const float& ox, const float& oy );


        /***
        3. Vertexes administration
        ***/
        unsigned int getSize() const;
        void addVertexFromPixel( const float& x, const float& y );
        void addVertex( const Vertex& vertex );
        const Vertex getVertex(const int& i) const;
        const Vertex getTransVertex(const int& i) const;
        const Vertex getScalatedVertex(const int& i) const;
        Vertex getLastVertex() const;


        /***
        4. Vectors administration
        ***/
        const Vertex getVector(const int& i) const;


        /***
        5. Transformations
        ***/
        void Translate( int tx, int ty );
        void Rotate( float angle );
        void Scale( float sx, float sy );


        /***
        6. Updating
        ***/
        void Update();
        void UpdateRotation();


        /***
        7. Drawing
        ***/
        void draw() const;
        static void drawLine( const Vertex& v0, const Vertex& vy );


        /***
        8. Robot drawing conditions
        ***/
        static void setCoordinatesThresholds( const int& minX, const int& minY,
                                                const int& maxX, const int& maxY );
        bool vertexInRobotRange( Vertex vertex ) const ;
        bool drawableByRobot() const ;

        /***
        9. Auxiliar methods
        ***/
        void showPolygon() const;
        static void PixelToWorld( float& x, float &y );

};

#endif // POLYGON_HPP
