#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "ofMain.h"
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>
#include <ostream>
#include <cmath>
using namespace std;

enum COORDINATES {
    X = 0,
    Y
};

enum POLAR {
    R = 0,
    A
};

class Vertex {
	private:
		float *v;
    public:
        // 1. Initialization
        Vertex();
        Vertex( const Vertex& b );
        ~Vertex();

		Vertex( float x, float y );
		void set( float x, float y );

        // 2. Operators
        void operator = (const Vertex& b);
        Vertex operator + (const Vertex& b ) const ;
        Vertex operator - (const Vertex& b ) const ;
        Vertex operator * (const float& b ) const ;
        Vertex operator / (const float& b ) const ;
        float operator[](int i) const { return v[i]; }
		float& operator[](int i){ return v[i]; }

        float getNorm() const ;
		Vertex getUnitVector() const ;
		void normalize();

        // 3. << operator
		friend ostream& operator<< (ostream &out, const Vertex &vertex);
};


float dotProduct( const Vertex& a, const Vertex& b );

#endif
// VERTEX_HPP

