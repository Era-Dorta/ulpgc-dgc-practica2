#include "fractal.hpp"
#define INV4 0.25
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
    int auxIndex;
    //Divide line as many times as divisions says
    for(int i = 0; i < divisions; i++){
        auxIndex = 0;
        for(j = v.begin(); j < v.end(); j++){
            //Add three new vertices for each old vertex
            newVertex = (v[auxIndex] + v[auxIndex+1])*INV4 + v[auxIndex];
            auxIndex++;
            j = v.begin() + auxIndex;
            addVertex(newVertex, j);

            //TODO Este es el valor base, habria que sumarle en x,y el valor de
            //la normal a la recta y que sea proporcionalmente inverso
            //al numero de divisiones
            newVertex = (v[auxIndex - 1] + v[auxIndex+1])*INV4*2 + v[auxIndex];
            auxIndex++;
            j = v.begin() + auxIndex;
            addVertex(newVertex, j);

            newVertex = (v[auxIndex - 2] + v[auxIndex+1])*INV4*3 + v[auxIndex];
            auxIndex++;
            j = v.begin() + auxIndex;
            addVertex(newVertex, j);

            auxIndex++;
            j = v.begin() + auxIndex;
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

