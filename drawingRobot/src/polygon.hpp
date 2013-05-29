#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "vertex.hpp"

class Polygon
{
    private:
        static int ox, oy;
        std::vector< Vertex > v;
        std::vector< Vertex > transV;
        std::vector< Vertex > vScalated;
        std::vector< Vertex > vectors;

        Matrix transMatrix;

    public:

        void addVertex( const float& x, const float& y );
        void addVertex( const Vertex& vertex );

        void draw() const;

        static void setOrigin( const float& ox, const float& oy );
        static Vertex getOrigin();

        unsigned int getSize() const;

        const Vertex getVertex(const int& i) const;

        const Vertex getScalatedVertex(const int& i) const;

        const Vertex getVector(const int& i) const;

        void showPolygon() const;

        static void drawLine( const Vertex& v0, const Vertex& vy );

        Vertex getLastVertex() const;

        static void PixelToWorld( float& x, float &y );
        void clear();

        // Transformations
        void Translate( int tx, int ty );
        void Rotate( float angle );
        void Scale( float sx, float sy );

        void Update();
};


//

#endif // POLYGON_H
