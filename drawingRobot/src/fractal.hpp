#ifndef FRACTAL_H
#define FRACTAL_H

#include "polygon.hpp"
using namespace std;

class Fractal : public Polygon
{
    private:
        int divisions;
        std::vector< Vertex > coreVertices;
        std::vector< Vertex > transCoreVertices;

        void addVertex( const Vertex& vertex, const unsigned int& index);
        void copyToCore();

    public:
        Fractal( int divisions_ = 0 );
        void setVertices( const Vertex& vertex0, const Vertex& vertex1);
        void divide();

        virtual void Update();

        int getDivisions() const { return divisions; };
        void setDivisions( const int& divisions_ ){ divisions = divisions_; };
};

#endif // FRACTAL_H
