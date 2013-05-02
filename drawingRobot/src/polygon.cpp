#include "polygon.h"

int Polygon::ox = 0;
int Polygon::oy = 0;

void Polygon::addVertex( const float& x, const float& y )
{
    Vertex vertex( x-ox, -(y-oy) );
    cout << "push_back 1" << endl;

    cout << "push_back 2" << endl;

    if(vectors.size() > 0){
        Vertex aux;
        aux = vertex - v.back();
        aux.normalize();
        vectors.back().set( aux[X], aux[Y] );
    }
    v.push_back( vertex );
    Vertex vector( 0, 1 );
    vectors.push_back(vector);
}

void Polygon::draw() const
{
    for( unsigned int i=1; i<v.size(); i++ ){
        drawLine( v[i-1], v[i] );
    }
}


void Polygon::drawLine( const Vertex& v0, const Vertex& v1 ) const
{
    ofLine( v0[X]+ox, -v0[Y]+oy, v1[X]+ox, -v1[Y]+oy );
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

const Vertex Polygon::getVertex(const int& i) const{
    return v[i];
}

const Vertex Polygon::getVector(const int& i) const{
    return vectors[i];
}
