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

        void addVertex( const Vertex& vertex, std::vector<class Vertex>::iterator i);

    public:
        Fractal( int divisions_ = 0 );
        void setVertices( const Vertex& vertex0, const Vertex& vertex1);
        void divide();

        virtual void Update();
};

#endif // FRACTAL_H
