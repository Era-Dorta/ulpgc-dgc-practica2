#include "polygon.hpp"
#include <iostream>

// Coordinates system origin initialization.
int Polygon::ox = 0;
int Polygon::oy = 0;

// Initializations for Thresholds for polygons drawable by the robot.
int Polygon::minX = 0;
int Polygon::maxX = 0;
int Polygon::minY = 0;
int Polygon::maxY = 0;


/***
    1. Initializations
***/

Polygon::Polygon()
{
    robotDrawingErrors = NONE;
}


void Polygon::clear()
{
    // Clear original and transformed vertexes.
    v.clear();
    transV.clear();

    // Clear original and transformed scalated vertexes (world coordinates).
    vScalated.clear();
    transVScalated.clear();

    // Clear original and transformed vectors.
    vectors.clear();
    transVectors.clear();

    // Set transformation matrix to identity.
    transMatrix.setIdentity();

    // By default, there's not errors for robot "drawing" an empty polygon.
    robotDrawingErrors = NONE;
}


/***
    2. Coordinates system origin administration
***/

Vertex Polygon::getOrigin()
{
    return Vertex( ox, oy );
}


void Polygon::setOrigin( const float& ox, const float& oy )
{
    Polygon::ox = ox;
    Polygon::oy = oy;
}


/***
    3. Vertexes administration
***/

unsigned int Polygon::getSize() const
{
    return v.size();
}


void Polygon::addVertexFromPixel( const float& x, const float& y )
{
    // Transform vertex from screen to world coordinates.
    Vertex vertex( x-ox, -(y-oy) );

    // Add world vertex to polygon.
    addVertex( vertex );
}


void Polygon::addVertex( const Vertex& vertex )
{
    if( vectors.size() > 0 ){
        // If new vertex is not the first one added to the polygon, update the
        // vector joining previous vertex and this one.
        Vertex aux;
        aux = vertex - v.back();
        aux.normalize();
        vectors.back().set( aux[X], aux[Y] );
        transVectors.back().set( aux[X], aux[Y] );
    }

    // Add vertex to polygon.
    v.push_back( vertex );
    transV.push_back( vertex );

    // Add scalated vertex to polygon.
    vScalated.push_back( vertex*0.25 );
    transVScalated.push_back( vertex*0.25 );

    // Insert a default vector for orienting the robot once it has finished drawing.
    Vertex vector( 0, 1 );
    vectors.push_back( vector );
    transVectors.push_back( vector );

    // If any vertex falls out of space where robot cand draw it, indicate it.
    if( !vertexInRobotRange( vertex ) ){
        robotDrawingErrors = VERTEX_OUT_OF_BORDERS;
    }
}


const Vertex Polygon::getVertex( const int& i)  const
{
    return v[i];
}


const Vertex Polygon::getTransVertex( const int& i ) const
{
    return transV[i];
}


const Vertex Polygon::getScalatedVertex( const int& i ) const
{
    return transVScalated[i];
}


Vertex Polygon::getLastVertex() const
{
    return v[v.size()-1];
}


/***
    4. Vectors administration
***/

const Vertex Polygon::getVector( const int& i ) const
{
    return transVectors[i];
}


/***
    5. Transformations
***/

void Polygon::Translate( int tx, int ty )
{
    // Create a translation matrix and compound it with the polygon's
    // transformation matrix.
    Matrix translationMatrix;
    translationMatrix.setTranslation( tx, ty );
    transMatrix = transMatrix*translationMatrix;

    // Update transformed vertexes.
    Update();
}

void Polygon::Rotate( float angle )
{
    // Create a rotation matrix and compound it with the polygon's
    // transformation matrix.
    Matrix rotMatrix;
    rotMatrix.setRotation( angle );
    transMatrix = transMatrix*rotMatrix;

    // Update transformed vertexes and vectors.
    UpdateRotation();
}

void Polygon::Scale( float sx, float sy )
{
    // Create a scale matrix and compound it with the polygon's
    // transformation matrix.
    Matrix scaleMatrix;
    scaleMatrix.setScale( sx, sy );
    transMatrix = transMatrix*scaleMatrix;

    // Update transformed vertexes.
    Update();
}


/***
    6. Updating
***/

void Polygon::Update()
{
    unsigned int i;

    robotDrawingErrors = NONE;

    // Update transformed vertexes by multiply original ones by transformation
    // matrix.
    for( i=0; i<v.size(); i++ ){
        transV[i] = v[i]*transMatrix;
        transVScalated[i] = vScalated[i]*transMatrix;

        // If any vertex falls out of space where robot cand draw it, indicate it.
        if( !vertexInRobotRange( transV[i] ) ){
            robotDrawingErrors = VERTEX_OUT_OF_BORDERS;
        }
    }
}


void Polygon::UpdateRotation()
{
    unsigned int i;

    robotDrawingErrors = NONE;

    // Update transformed vertexes and vectors by multiply original ones by
    // transformation matrix.
    for( i=0; i<v.size(); i++ ){
        transV[i] = v[i]*transMatrix;
        transVScalated[i] = vScalated[i]*transMatrix;
        transVectors[i] = vectors[i]*transMatrix;

        // If any vertex falls out of space where robot cand draw it, indicate it.
        if( !vertexInRobotRange( transV[i] ) ){
            robotDrawingErrors = VERTEX_OUT_OF_BORDERS;
        }
    }
}


/***
    7. Drawing
***/

void Polygon::draw() const
{
    for( unsigned int i=1; i<transV.size(); i++ ){
        drawLine( transV[i-1], transV[i] );
    }
}


void Polygon::drawLine( const Vertex& v0, const Vertex& v1 )
{
    // Draw line in screen coordinates.
    ofLine( v0[X]+ox, -v0[Y]+oy, v1[X]+ox, -v1[Y]+oy );
}


/***
    8. Robot drawing conditions
***/

void Polygon::setCoordinatesThresholds( const int& minX, const int& minY,
                                         const int& maxX, const int& maxY )
{
    Polygon::minX = minX;
    Polygon::minY = minY;
    Polygon::maxX = maxX;
    Polygon::maxY = maxY;
}


bool Polygon::vertexInRobotRange( Vertex vertex ) const
{
    vertex[X] += ox;
    vertex[Y] += oy;
    return ( vertex[X] >= minX ) && ( vertex[X] <= maxX ) &&
            ( vertex[Y] >= minY ) && ( vertex[Y] <= maxY );
}


bool Polygon::drawableByRobot() const
{
    return ( robotDrawingErrors == NONE );
}


/***
    9. Auxiliar methods
***/

void Polygon::showPolygon() const{
    for( unsigned int i=0; i<v.size(); i++ ){
        cout << "v[" << i << "] " << v[i] << " vector " << vectors[i] << endl;
    }
}


void Polygon::PixelToWorld( float& x, float &y )
{
    // Convert point from screen to world coordinates.
    x = x-ox;
    y = -y+oy;
}
