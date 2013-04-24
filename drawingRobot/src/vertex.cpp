
#include "vertex.hpp"


/*                                 1. Initialization                         */

Vertex::Vertex()
{
    v = new float[3];
	for( unsigned int i=0; i<2; i++ ){
		v[i] = 0;
	}
	v[2] = 1;
}

Vertex::Vertex( const Vertex& b )
{
    v = new float[3];
	for( unsigned int i=0; i<3; i++ ){
		v[i] = b[i];
	}
}

Vertex::Vertex( float x, float y )
{
    v = new float[2];
	set( x, y );
}

Vertex::~Vertex()
{
    delete [] v;
}

void Vertex::set( float x, float y )
{
	v[0] = x;
	v[1] = y;
	v[2] = 1;
}


/*                                  2. Operators                             */

void Vertex::operator = (const Vertex& b)
{
    for( unsigned int i=0; i<3; i++ ){
        v[i] = b[i];
    }
}

Vertex Vertex::operator + (const Vertex& b) const
{
    Vertex c( b[X]+v[X], b[Y]+v[Y] );
    return c;
}

Vertex Vertex::operator - (const Vertex& b) const
{
    Vertex c( b[X]-v[X], b[Y]-v[Y] );
    return c;
}

Vertex Vertex::operator * (const float& b) const
{
    Vertex c( v[X]*b, v[Y]*b );
    return c;
}

Vertex Vertex::operator / (const float& b) const
{
    Vertex c( v[X]/b, v[Y]/b );
    return c;
}


float Vertex::getNorm() const
{
    return( sqrt( v[X]*v[X]+v[Y]*v[Y] ) );
}

Vertex Vertex::getUnitVector() const
{
    float norm = getNorm();
    Vertex res( v[X]/norm, v[Y]/norm );

    return res;
}

void Vertex::normalize(){
    float norm = getNorm();
    set( v[X]/norm, v[Y]/norm );
}

/*                                 3. << operator                            */

ostream& operator << (ostream &out, const Vertex &vertex)
{
    out << "("
		<< vertex.v[0] << ", "
		<< vertex.v[1] << ", "
		<< vertex.v[2] << ")";
    return out;
}



float dotProduct( const Vertex& a, const Vertex& b )
{
    return a[X]*b[X]+a[Y]*b[Y];
}

