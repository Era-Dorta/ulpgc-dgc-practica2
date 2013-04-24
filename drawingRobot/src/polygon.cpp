#include "polygon.h"

int Polygon::ox = 0;
int Polygon::oy = 0;

void Polygon::AddVertex( const float& x, const float& y )
{
    v.push_back( Vertex( x-ox, y-oy ) );
}

void Polygon::DrawOpen() const
{
    for( unsigned int i=1; i<v.size(); i++ ){
        DrawLine( v[i-1], v[i] );
    }
}


void Polygon::Draw() const
{
    DrawOpen();
    //DrawLine( v[v.size()-1], v[0] );
}


void Polygon::DrawLine( const Vertex& v0, const Vertex& v1 ) const
{
    ofLine( v0[X]+ox, v0[Y]+oy, v1[X]+ox, v1[Y]+oy );
}

void Polygon::SetOrigin( const float& ox, const float& oy )
{
    Polygon::ox = ox;
    Polygon::oy = oy;
}
