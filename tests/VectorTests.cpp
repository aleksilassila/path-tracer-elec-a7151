#include "doctest.h"
#include "../src/utils/vector.hpp"


TEST_CASE("Testing equals") {
    Vector v = {};
    CHECK((v == Vector(0, 0, 0)));
    CHECK((Vector() == Vector(0, 0, 0)));
    CHECK((Vector(1, 1, 1) == Vector(1, 1, 1)));
    CHECK_FALSE((Vector(1, 1, 1) == Vector(3, 4, 5)));
    CHECK_FALSE((Vector(1, 1, 1) == Vector(-1, 1, 1)));
    CHECK_FALSE((Vector(1, 1, 1) == Vector(1, -1, 1)));
    CHECK_FALSE((Vector(1, 1, 1) == Vector(1, 1, -1)));
}

TEST_CASE("Testing unequals") {
    CHECK((Vector(1,1,1) != Vector(3, 4, 5)));
    CHECK_FALSE((Vector(1,1,1) != Vector(1, 1, 1)));
    CHECK_FALSE((Vector(0,0,0) != Vector(0, 0, 0)));
}

TEST_CASE("Testing addition and subtraction") {
    Vector vectorA = Vector(1,1,1);
    Vector vectorB = Vector(-3,2,-5);
    Vector vectorC = Vector(5,7,12);

    CHECK((vectorA + vectorB == Vector(-2, 3, -4)));
    CHECK((vectorB + vectorC == Vector(2, 9, 7)));
    CHECK((vectorC + vectorA == Vector(6, 8, 13)));
    CHECK((vectorC + vectorA == vectorA + vectorC));

    CHECK((vectorA - vectorB == Vector(4,-1,6)));
    CHECK((vectorB - vectorC == Vector(-8,-5,-17)));
    CHECK((vectorC - vectorA == Vector(4,6,11)));
    CHECK((vectorB - vectorA == Vector(-4,1,-6)));
    CHECK((vectorC - vectorB == Vector(8,5,17)));
    CHECK((vectorA - vectorC == Vector(-4,-6,-11)));
}

TEST_CASE("Testing += operator") {
    Vector vector = Vector(1,1,1);
    vector += Vector(3, -4, 5);

    CHECK((vector == Vector(4, -3, 6)));
}

TEST_CASE("Testing multiplication by scalar") {
    Vector vector = Vector(1,1,1);

    CHECK((vector * 3 == Vector(3,3,3)));
    CHECK((vector * -1 == Vector(-1,-1,-1)));
    CHECK((vector * 0.5 == Vector(0.5,0.5,0.5)));
    CHECK((vector * 0 == Vector(0,0,0)));
}

TEST_CASE("Testing vector length") {
    Vector vectorA = Vector(1,1,1);
    Vector vectorB = Vector(3,4,5);
    Vector vectorC = Vector(5,7,12);

    CHECK((vectorA.Len() == 1.7320508075688772));
    CHECK((vectorB.Len() == 7.0710678118654755));
    CHECK((vectorC.Len() == 14.7648230602334));
}

TEST_CASE("Testing transformation to unit vector") {
    Vector vectorA = Vector(1,1,1);
    Vector vectorB = Vector(-3,4,-5);
    Vector vectorC = Vector(5,-7,12);
    Vector vectorD = Vector(0,0,0);

    CHECK((vectorA.Norm() == Vector(0.57735026918962584, 0.57735026918962584, 0.57735026918962584)));
    CHECK((vectorB.Norm() == Vector(-0.42426406871192851, 0.56568542494923801, -0.70710678118654746)));
    CHECK((vectorC.Norm() == Vector(0.33864273073929818, -0.47409982303501752, 0.8127425537743157)));
    CHECK((vectorD.Norm() == Vector(0, 0, 0)));
}


TEST_CASE("Testing dot product") {
    Vector vectorA = Vector(1,1,1);
    Vector vectorB = Vector(-3,2,-5);
    Vector vectorC = Vector(5,7,12);

    CHECK((vectorA * vectorB == -6));
    CHECK((vectorB * vectorC == -61));
    CHECK((vectorC * vectorA == 24));
    CHECK((vectorC * vectorA == vectorA * vectorC));
}

TEST_CASE("Testing cross product") {
    Vector vectorA = Vector(1,1,1);
    Vector vectorB = Vector(-3,2,-5);
    Vector vectorC = Vector(5,7,12);

    CHECK((vectorA % vectorB == Vector(-7,2,5)));
    CHECK((vectorB % vectorC == Vector(59,11,-31)));
    CHECK((vectorC % vectorA == Vector(-5,7,-2)));
    CHECK((vectorA % vectorC == Vector(5,-7,2)));
}

TEST_CASE("Testing distance") {
    Vector vectorA = Vector(1,1,1);
    Vector vectorB = Vector(-3,4,-5);
    Vector vectorC = Vector(5,7,12);

    CHECK((vectorA.Distance(vectorB) == 7.810249675906654));
    CHECK((vectorB.Distance(vectorC) == 19.026297590440446));
    CHECK((vectorC.Distance(vectorA) == 13.152946437965905));
    CHECK((vectorA.Distance(vectorC) == vectorC.Distance(vectorA)));
}

