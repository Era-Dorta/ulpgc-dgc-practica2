/***
    Class Fractal
    ===
    Special type of 2D polygon that defines the fractal known as
    "Koch snowflake". This fractal is generated based on a triangle.
***/

#ifndef FRACTAL_H
#define FRACTAL_H

#include "polygon.hpp"
using namespace std;

class Fractal : public Polygon
{
    private:
        // This sets how many subdivisions are made on the original triangle
        // for generating the final fractal.
        int divisions;

        // Vectors for original core vertices (those of the original triangle)
        // and their respectives transformed ones.
        std::vector< Vertex > coreVertices;
        std::vector< Vertex > transCoreVertices;

        /***
        1. Auxiliar methods
        ***/
        void addVertex( const Vertex& vertex, const unsigned int& index );

        // At the beginning of fractal creation, core vertexes are kept
        // temporaly in vector "Polygon::v". This method copies them to vectors
        // coreVertices and transCoreVertices.
        void copyToCore();

    public:
        /***
        2. Initialization
        ***/
        // Generates empty triangle and keeps the number of divisions.
        Fractal( int divisions_ = 0 );

        // Generates original triangle based on segment defined by user.
        void setVertices( const Vertex& vertex0, const Vertex& vertex1 );

        // Generates final fractal based on a vector of core vertexes and a
        // number of divisions.
        void set( const std::vector< Vertex >* coreTransVertexes_, int divisions_ );

        // Generates final fractal by dividing original triangle "divisions"
        // times.
        void divide();


        /***
        3. Updating
        ***/
        // Update transformed vertexes by multiplying original ones by the
        // transformation matrix.
        virtual void update();

        /***
        4. Getters and setters
        ***/
        int getDivisions() const { return divisions; };
        void setDivisions( const int& divisions_ ){ divisions = divisions_; };
        const std::vector< Vertex >* getTransCoreVertices();
};

#endif // FRACTAL_H
