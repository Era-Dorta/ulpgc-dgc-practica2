/***
    Class Matrix
    ===
    Used for defining 2D transformation matrixes.
***/

#ifndef MATRIX_HPP
#define MATRIX_CPP

/***
    Includes
***/
#include <ofMain.h>
#include <cmath>
#include <iostream>
using namespace std;


/***
    Main class
***/
class Matrix {
	private:
		float m[3][3];
    public:
        /***
        1. Initialization
        ***/
        Matrix(); // Calls SetIdentity();
        void setIdentity();
        void setZeros();


        /***
        2. Transformations
        ***/
		void setTranslation( float tx, float ty );
		void setRotation( float angle );
		void setScale( float sx, float sy );


        /***
        3, Operators
        ***/
        Matrix& operator * (const Matrix& b);
		float& operator()(int i, int j){ return m[i][j]; }
		float operator()(int i, int j) const { return m[i][j]; }
};


/***
    4. I/O
***/
ostream& operator<< (ostream &out, const Matrix &matrix);

#endif
// MATRIX_HPP
