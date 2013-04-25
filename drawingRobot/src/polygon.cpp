#include "polygon.h"

int Polygon::ox = 0;
int Polygon::oy = 0;

void Polygon::addVertex( const float& x, const float& y )
{
    Vertex vertex( x-ox, y-oy );
    cout << "push_back 1" << endl;
    v.push_back( vertex );
    cout << "push_back 2" << endl;
}

void Polygon::draw() const
{
    for( unsigned int i=1; i<v.size(); i++ ){
        drawLine( v[i-1], v[i] );
    }
}


void Polygon::drawLine( const Vertex& v0, const Vertex& v1 ) const
{
    ofLine( v0[X]+ox, v0[Y]+oy, v1[X]+ox, v1[Y]+oy );
}

void Polygon::setOrigin( const float& ox, const float& oy )
{
    Polygon::ox = ox;
    Polygon::oy = oy;
}


unsigned int Polygon::getSize() const
{
    return v.size();
}
