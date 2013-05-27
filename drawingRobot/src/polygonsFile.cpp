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


void PolygonsFile::load( string filePath, vector<Polygon>* polygons )
{
    const unsigned int LINE_SIZE = 250;
    char line[LINE_SIZE];
    unsigned int i, nVertexes;
    Vertex currentVertex;
    Polygon currentPolygon;

    ifstream file( filePath.c_str() );

    if( !file.is_open() ){
        throw runtime_error( "ERROR opening the file [" + filePath + "]" );
    }

    while( file.getline( line, LINE_SIZE ) ){
        // Read the number of vertexes for current polygon.
        //file.getline( line, LINE_SIZE );
        //if( !file.eof() )
        nVertexes = atoi( line );
        cout << "Nº vertexes: " << nVertexes << endl;

        // Read all vertexes for current polygon.
        for( i=0; i<nVertexes; i++ ){
            file.getline( line, LINE_SIZE );

            currentVertex = loadVertexFromLine( line );

            currentPolygon.addVertex( currentVertex );

            cout << "\tVertex: " << currentVertex << endl;
        }
        polygons->push_back( currentPolygon );
        currentPolygon.clear();
        cout << endl << endl;
    }
}
