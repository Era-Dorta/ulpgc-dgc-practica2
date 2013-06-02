#include "fractal.hpp"

#define swap(type, i, j) {type t = i; i = j; j = t;}
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
    Vertex aux;
    divisions = divisions_;
    for(int i = 0; i < 3; i++){
        Polygon::addVertex(aux);
    }
}

//--------------------------------------------------------------
void Fractal::setVertices( const Vertex& vertex0, const Vertex& vertex1){
    clear();
    Vertex aux;
    aux[X] = (vertex0[X] + vertex1[X])*0.5;
    aux[Y] = vertex0[Y] + vertex0[X] - vertex1[X];
    Polygon::addVertex(vertex0);
    Polygon::addVertex(aux);
    Polygon::addVertex(vertex1);
    Polygon::addVertex(vertex0);

    showPolygon();

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

    Vertex newVertex, normal;
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

            //Calculate normal between original vertices
            normal = v[auxIndex - 1] - v[auxIndex+1];
            swap(float, normal[X], normal[Y]);
            normal[X] = -normal[X];
            normal.normalize();
            //Normal length is inversely proportional to the number of
            //divisions
            normal = normal*(10.0/(i + 1));
            //Calculate a vertex between the two original vertices
            newVertex = (v[auxIndex - 1] + v[auxIndex+1])*INV4*2 + v[auxIndex];
            //Add the normal length to this vertex
            newVertex = newVertex + normal;
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

