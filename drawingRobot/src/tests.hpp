#ifndef TESTS_H
#define TESTS_H

#include "polygon.hpp"
#include <cassert>

class Test
{
    private:
        Vertex originalVertex, resultVertex;
        Matrix transMatrix;
        float x, y, tx, ty, sx, sy, angle;

    public:
        void testAll();
        void testTranslations();
        void testRotations();
        void testScales();

        void testCompoundTrasnformations();
};




#endif // TESTS_H
