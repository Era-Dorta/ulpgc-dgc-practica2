#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "vertex.hpp"

class Polygon
{
    private:
        static int ox, oy;
        std::vector< Vertex > v;

    public:

        void AddVertex( const float& x, const float& y );

        void DrawOpen() const;
        void Draw() const;

        static void SetOrigin( const float& ox, const float& oy );

    private:
        void DrawLine( const Vertex& v0, const Vertex& vy ) const;
};


//

#endif // POLYGON_H
