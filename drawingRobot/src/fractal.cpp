#include "fractal.hpp"

//--------------------------------------------------------------
void Fractal::addVertex( const Vertex& vertex, std::vector<class Vertex>::iterator i )
{
    //TODO
    //Se calcula el vector de i a vertex
    //y tb de vertex a i+1
    //Y se insertar vertex en medio de los dos
    if(vectors.size() > 0){
        Vertex aux;
        aux = vertex - v.back();
        aux.normalize();
        vectors.back().set( aux[X], aux[Y] );
        transVectors.back().set( aux[X], aux[Y] );
    }
    v.insert( i, vertex );
    transV.insert( i, vertex );
    vScalated.insert(i, vertex*0.25);
    transVScalated.insert(i, vertex*0.25);
    Vertex vector( 0, 1 );
    vectors.insert(i, vector);
    transVectors.insert(i, vector);
}

//--------------------------------------------------------------
Fractal::Fractal(int divisions_)
{
    divisions = divisions_;
}

//--------------------------------------------------------------
void Fractal::divide()
{
    //Delete previous information about vertices
    clear();
    //Copy core vertices to vertices vector
    for( unsigned int i=0; i<transCoreVertices.size(); i++ ){
        Polygon::addVertex(transCoreVertices[i]);
    }

    std::vector<class Vertex>::iterator j;

    Vertex newVertex;
    //Divide line as many times as divisions says
    for(int i = 0; i < divisions; i++){
        for(j = v.begin(); j < v.end(); j++){
            //Add three new vertices for each old vertex
            addVertex(newVertex, j);
            j++;
            addVertex(newVertex, j);
            j++;
            addVertex(newVertex, j);
            j++;
        }
    }
}

//--------------------------------------------------------------
void Fractal::Update()
{
    Polygon::Update();

    //Override update methods to also update core vertices
    for( unsigned int i=0; i<coreVertices.size(); i++ ){
        transCoreVertices[i] = coreVertices[i]*transMatrix;
    }
}

//--------------------------------------------------------------
void Fractal::UpdateRotation()
{
    Polygon::UpdateRotation();

    for( unsigned int i=0; i<coreVertices.size(); i++ ){
        transCoreVertices[i] = coreVertices[i]*transMatrix;
    }
}
