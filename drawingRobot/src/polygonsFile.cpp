#include "polygonsFile.hpp"

Vertex PolygonsFile::loadVertexFromLine( char* line ) const
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


int PolygonsFile::load( string filePath, vector< ofPtr<Polygon> >* polygons ) const
{
    const unsigned int LINE_SIZE = 250;
    char line[LINE_SIZE];
    unsigned int i, nVertexes;
    Vertex currentVertex;
    Polygon currentPolygon;
    Fractal currentFractal;

    std::vector< Vertex > coreTransVertexes;

    ifstream file( filePath.c_str() );

    if( !file.is_open() ){
        return -1;
    }

    polygons->clear();

    while( file.getline( line, LINE_SIZE ) ){
        if( line[0] == 'P' ){
            // Read the number of vertexes for current polygon.
            file.getline( line, LINE_SIZE );
            nVertexes = atoi( line );

            // Read all vertexes for current polygon.
            for( i=0; i<nVertexes; i++ ){
                file.getline( line, LINE_SIZE );

                currentVertex = loadVertexFromLine( line );

                currentPolygon.addVertex( currentVertex );
            }
            polygons->push_back( (ofPtr<Polygon>)( new Polygon( currentPolygon ) ) );
            currentPolygon.clear();
        }else{
            // Read the number of core vertexes for current fractal.
            file.getline( line, LINE_SIZE );
            nVertexes = atoi( line );

            // Read all core vertexes for current fractal.
            coreTransVertexes.clear();
            for( i=0; i<nVertexes; i++ ){
                file.getline( line, LINE_SIZE );

                currentVertex = loadVertexFromLine( line );

                coreTransVertexes.push_back( currentVertex );
                //currentFractal.addVertex( currentVertex );
            }

            // Read the number of divisions for current fractal.
            file.getline( line, LINE_SIZE );

            currentFractal.set( &coreTransVertexes, atoi( line ) );

            polygons->push_back( (ofPtr<Polygon>)( dynamic_cast<Polygon *>( new Fractal( currentFractal ) ) ) );
            currentFractal.clear();
        }
    }

    file.close();

    return 0;
}


int PolygonsFile::save( string filePath, vector< ofPtr<Polygon> >* polygons ) const
{
    unsigned int i, j;
    Vertex currentVertex;
    Polygon currentPolygon;
    Fractal currentFractal;
    const std::vector< Vertex >* coreVertices;

    ofstream file( filePath.c_str() );

    if( !file.is_open() ){
        return -1;
    }

    for( i=0; i<polygons->size(); i++ ){
        if( ((*polygons)[i])->getType() == POLYGON ){
            currentPolygon = *((*polygons)[i]);

            file << "P" << endl
                 << currentPolygon.getSize() << endl;

            for( j=0; j<currentPolygon.getSize(); j++ ){
                currentVertex = currentPolygon.getTransVertex( j );

                file << currentVertex[X] << " " << currentVertex[Y] << endl;
            }
        }else{
            currentFractal = *(dynamic_cast<Fractal *>( ( (*polygons)[i] ).get() ) );

            coreVertices = currentFractal.getTransCoreVertices();

            // Mark 'F' for Fractal.
            file << "F" << endl
                 << coreVertices->size() << endl;

            for( j=0; j<coreVertices->size(); j++ ){
                currentVertex = (*coreVertices)[j];
                file << currentVertex[X] << " " << currentVertex[Y] << endl;
            }

            file << currentFractal.getDivisions() << endl;
        }
    }

    file.close();

    return 0;
}
