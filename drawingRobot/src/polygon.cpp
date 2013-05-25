#include "polygon.hpp"
#include <iostream>
int Polygon::ox = 0;
int Polygon::oy = 0;


void Polygon::addVertex( const float& x, const float& y )
{
    Vertex vertex( x-ox, -(y-oy) );

    if(vectors.size() > 0){
        Vertex aux;
        aux = vertex - v.back();
        aux.normalize();
        vectors.back().set( aux[X], aux[Y] );
    }
    v.push_back( vertex );
    transV.push_back( vertex );
    vScalated.push_back(vertex*0.25);
    Vertex vector( 0, 1 );
    vectors.push_back(vector);
}

void Polygon::draw() const
{
    for( unsigned int i=1; i<transV.size(); i++ ){
        drawLine( transV[i-1], transV[i] );
    }
}


void Polygon::drawLine( const Vertex& v0, const Vertex& v1 )
{
    ofLine( v0[X]+ox, -v0[Y]+oy, v1[X]+ox, -v1[Y]+oy );
}

void Polygon::setOrigin( const float& ox, const float& oy )
{
    Polygon::ox = ox;
    Polygon::oy = oy;
}

Vertex Polygon::getOrigin()
{
    return Vertex( ox, oy );
}


unsigned int Polygon::getSize() const
{
    return v.size();
}

const Vertex Polygon::getVertex(const int& i) const{
    return vScalated[i];
}

const Vertex Polygon::getVector(const int& i) const{
    return vectors[i];
}

void Polygon::showPolygon() const{
    for( unsigned int i=0; i<v.size(); i++ ){
        cout << "v[" << i << "] " << v[i] << " vector " << vectors[i] << endl;
    }
}

Vertex Polygon::getLastVertex() const
{
    return v[v.size()-1];
}


void Polygon::PixelToWorld( float& x, float &y )
{
    x = x-ox;
    y = -y+oy;
}

void Polygon::clear()
{
    v.clear();
    transV.clear();
    vectors.clear();

    transMatrix.setIdentity();
}


void Polygon::Translate( int tx, int ty )
{
    Matrix translationMatrix;
    translationMatrix.setTranslation( tx, ty );
    transMatrix = transMatrix*translationMatrix;

    Update();
}

void Polygon::Rotate( float angle )
{
    Matrix rotMatrix;
    rotMatrix.setRotation( angle );
    transMatrix = transMatrix*rotMatrix;

    Update();
}

void Polygon::Scale( float sx, float sy )
{
    Matrix scaleMatrix;
    scaleMatrix.setScale( sx, sy );
    transMatrix = transMatrix*scaleMatrix;

    Update();
}


void Polygon::Update()
{
    unsigned int i;

    for( i=0; i<v.size(); i++ ){
        transV[i] = v[i]*transMatrix;
    }
}
