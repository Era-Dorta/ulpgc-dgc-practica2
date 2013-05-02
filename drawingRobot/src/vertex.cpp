
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

Vertex::Vertex( float x, float y, float h )
{
    v = new float[3];
	set( x, y , h);
}

Vertex::~Vertex()
{
    delete [] v;
}

void Vertex::set( float x, float y, float h )
{
	v[0] = x;
	v[1] = y;
	v[2] = h;
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
    Vertex c( v[X]-b[X], v[Y]-b[Y] );
    return c;
}

Vertex Vertex::operator * (const Vertex& b) const
{
    Vertex c( v[Y]*b[H]-v[H]*b[Y], v[H]*b[X]-v[X]*b[H], v[X]*b[Y]-v[Y]*b[X] );
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

bool Vertex::operator == (const Vertex& b) const
{
    return (v[X] == b[X] && v[Y] == b[Y] && v[H] == b[H]);
}

Vertex Vertex::rotate( const float angle ) const
{
    Vertex res;
    float cosVal = cos(angle), sinVal = sin(angle);
    float rotationMatrix[3][3] = {
        {cosVal, -sinVal, 0},
        {sinVal, cosVal, 0},
        {0,     0,      1}
    };

    for( unsigned int i = 0; i < 3; i++){
        for( unsigned int j = 0; j < 3; j++){
            res[i] += v[j]*rotationMatrix[j][i];
        }
    }
    return res;
}

float Vertex::getNorm() const
{
    return( sqrt( v[X]*v[X]+v[Y]*v[Y]+v[H]*v[H] ) );
}

float Vertex::getNorm3() const
{
    return( sqrt( v[X]*v[X]+v[Y]*v[Y] ) );
}

Vertex Vertex::getUnitVector() const
{
    float invNorm = 1.0/getNorm();
    Vertex res( v[X]*invNorm, v[Y]*invNorm );
    return res;
}

void Vertex::normalize(){
    float invNorm = 1.0/getNorm();
    set( v[X]*invNorm, v[Y]*invNorm, v[H]*invNorm);
}

float Vertex::distance( const Vertex& b ) const{
    return sqrt( (v[X] - b[X])*(v[X] - b[X]) +
                (v[Y] - b[Y])*(v[Y] - b[Y]) );
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

