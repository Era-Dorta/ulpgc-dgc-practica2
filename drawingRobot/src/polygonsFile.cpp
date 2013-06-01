#include "polygonsFile.hpp"


Vertex PolygonsFile::loadVertexFromLine( char* line )
{
    char *strValue;
    Vertex vertex;

    strValue = strtok( line, " " );
    for( unsigned int i=0; i<2; i++ ){
        vertex[i] = atof( strValue );
        strValue = strtok( NULL, " " );
    }

    return vertex;
}


int PolygonsFile::load( string filePath, vector<Polygon>* polygons )
{
    const unsigned int LINE_SIZE = 250;
    char line[LINE_SIZE];
    unsigned int i, nVertexes;
    Vertex currentVertex;
    Polygon currentPolygon;

    ifstream file( filePath.c_str() );

    if( !file.is_open() ){
        return -1;
    }

    polygons->clear();

    while( file.getline( line, LINE_SIZE ) ){
        // Read the number of vertexes for current polygon.
        nVertexes = atoi( line );

        // Read all vertexes for current polygon.
        for( i=0; i<nVertexes; i++ ){
            file.getline( line, LINE_SIZE );

            currentVertex = loadVertexFromLine( line );

            currentPolygon.addVertex( currentVertex );
        }
        polygons->push_back( currentPolygon );
        currentPolygon.clear();
    }

    file.close();

    return 0;
}


int PolygonsFile::save( string filePath, vector<Polygon>* polygons )
{
    unsigned int i, j;
    Vertex currentVertex;
    Polygon currentPolygon;

    ofstream file( filePath.c_str() );

    if( !file.is_open() ){
        return -1;
    }

    for( i=0; i<polygons->size(); i++ ){
        currentPolygon = (*polygons)[i];

        file << currentPolygon.getSize() << endl;

        for( j=0; j<currentPolygon.getSize(); j++ ){
            currentVertex = currentPolygon.getTransVertex( j );

            file << currentVertex[X] << " " << currentVertex[Y] << endl;
        }
    }

    file.close();

    return 0;
}
