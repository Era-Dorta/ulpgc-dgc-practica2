
#include "matrix.hpp"

const unsigned N = 3;


/***
    1. Initialization
***/

Matrix::Matrix()
{
	setIdentity();
}

void Matrix::setIdentity(){
    for( unsigned int i=0; i<N; i++ ){
		for( unsigned int j=0; j<N; j++ ){
			if( i == j ){
				m[i][j] = 1;
			}else{
				m[i][j] = 0;
			}
		}
	}
}

void Matrix::setZeros(){
    for( unsigned int i=0; i<N; i++ ){
		for( unsigned int j=0; j<N; j++ ){
            m[i][j] = 0;
		}
	}
}


/***
    2. Transformations
***/

void Matrix::setTranslation( float tx, float ty )
{
	setIdentity();
	m[2][0] = tx;
	m[2][1] = ty;
}

void Matrix::setRotation( float angle )
{
    setIdentity();
	angle *= PI/180;
	m[0][0] = cos(angle);
	m[0][1] = -sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
}

void Matrix::setScale( float sx, float sy )
{
    setIdentity();
    m[0][0] = sx;
    m[1][1] = sy;
}


/***
    3, Operators
***/

Matrix& Matrix::operator * (const Matrix& b)
{
    Matrix* res = new Matrix;
    for( unsigned int i=0; i<N; i++ ){
        for( unsigned int j=0; j<N; j++ ){
            (*res)(i,j) = 0;
            for( unsigned int k=0; k<N; k++ ){
                (*res)(i,j) += m[i][k] * b.m[k][j];
            }
        }
    }
    return *res;
}


/***
    4. I/O
***/

ostream& operator << (ostream &out, const Matrix &matrix)
{
    out << "[";
	for( unsigned int i=0; i<N; i++ ){
		for( unsigned int j=0; j<N; j++ ){
			out << matrix( i, j ) << " ";
		}
		out << "; ";
	}
	out << "]";
    return out;
}
