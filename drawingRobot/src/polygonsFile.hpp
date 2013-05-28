#ifndef POLYGONS_FILE_H
#define POLYGONS_FILE_H

#include <string>
#include "polygon.hpp"
#include <fstream>
using namespace std;

class PolygonsFile
{
    private:
        Vertex loadVertexFromLine( char* line );
    public:
        void load( string filePath, vector<Polygon>* polygons );
        void save( string filePath, vector<Polygon>* polygons );
};

#endif // POLYGONS_FILE_H
