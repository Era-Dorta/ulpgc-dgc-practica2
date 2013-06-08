/***
    PolygonsFile
    ===
    Auxiliar class with methods for saving user drawings on file or loading
    them.
***/

#ifndef POLYGONS_FILE_H
#define POLYGONS_FILE_H


/***
    Includes
***/
#include <string>
#include "polygon.hpp"
#include "fractal.hpp"
#include <fstream>
using namespace std;


/***
    Main class
***/
class PolygonsFile
{
    private:
        // Get a vertex from a line in the form "coordinateX coordinateY"
        Vertex loadVertexFromLine( char* line ) const ;
    public:
        // Load from file "filePath" a vector of polygons (including fractals).
        int load( string filePath, vector< ofPtr<Polygon> >* polygons ) const ;

        // Save to file "filePath" the given vector of polygons (including
        // fractals).
        int save( string filePath, vector< ofPtr<Polygon> >* polygons ) const ;
};

#endif // POLYGONS_FILE_H
