#ifndef POLYGONS_FILE_H
#define POLYGONS_FILE_H

#include <string>
#include "polygon.hpp"
#include "fractal.hpp"
#include <fstream>
using namespace std;

class PolygonsFile
{
    private:
        Vertex loadVertexFromLine( char* line );
    public:
        int load( string filePath, vector< ofPtr<Polygon> >* polygons );
        int save( string filePath, vector< ofPtr<Polygon> >* polygons );
};

#endif // POLYGONS_FILE_H
