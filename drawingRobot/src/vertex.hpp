/***
    Vertex
    ===
    Class used for defining and transforming 2D vertexes.
***/

#ifndef VERTEX_HPP
#define VERTEX_HPP


/***
    Includes
***/
#include "ofMain.h"
#include "matrix.hpp"
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>
#include <ostream>
#include <cmath>
using namespace std;


/***
    Auxiliar types
***/
enum COORDINATES {
    X = 0,
    Y,
    H
};


/***
    Main class
***/

class Vertex {
	private:
        // Vertex coordinates.
		float *v;
    public:
        /***
        1. Initialization and destruction.
        ***/
        Vertex();
        Vertex( float x, float y, float h = 1 );
        Vertex( const Vertex& b );
        void set( float x, float y, float h = 1);

        ~Vertex();


        /***
        2. Operators
        ***/
        void operator = (const Vertex& b);
        Vertex operator * (const Matrix& m) const ;
        Vertex operator + (const Vertex& b ) const ;
        Vertex operator - (const Vertex& b ) const ;
        Vertex operator * (const Vertex& b ) const ;
        Vertex operator * (const float& b ) const ;
        Vertex operator / (const float& b ) const ;
        bool operator == (const Vertex& b ) const ;
        float operator[](int i) const { return v[i]; }
		float& operator[](int i){ return v[i]; }
		Vertex rotate( const float angle ) const;


        /***
        3. Utilities
        ***/
        float getNorm() const ;
        float getNorm3() const ;
		Vertex getUnitVector() const ;
		void normalize();
		float distance( const Vertex& b ) const;

        /***
        4. I/O
        ***/
		friend ostream& operator<< (ostream &out, const Vertex &vertex);
};

/***
    5. Dot product
***/
float dotProduct( const Vertex& a, const Vertex& b );

#endif
// VERTEX_HPP

