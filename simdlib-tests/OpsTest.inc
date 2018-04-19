//
//  OpsTest.inc
//  VectorialHost
//
//  Created by Daniel Doubleday on 31/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

SECTION("load")
{
    OpsTest t;
    t.testLoad();
}

SECTION("store")
{
    OpsTest t;
    t.testStore();
}

SECTION("add")
{
    OpsTest t;
    t.testAdd();
}

SECTION("subtract")
{
    OpsTest t;
    t.testSubtract();
}

SECTION("multiply")
{
    OpsTest t;
    t.testMultiply();
}

SECTION("devide")
{
    OpsTest t;
    t.testDevide();
}

SECTION("reciprocal")
{
    OpsTest t;
    t.testReciprocal();
}

SECTION("sqrt")
{
    OpsTest t;
    t.testSqrt();
}

SECTION("min")
{
    OpsTest t;
    t.testMin();
}

SECTION("max")
{
    OpsTest t;
    t.testMax();
}

SECTION("gt")
{
    OpsTest t;
    t.testCompareGT();
}

SECTION("lt")
{
    OpsTest t;
    t.testCompareLT();
}

SECTION("gte")
{
    OpsTest t;
    t.testCompareGTE();
}

SECTION("lte")
{
    OpsTest t;
    t.testCompareLTE();
}

SECTION("complement")
{
    OpsTest t;
    t.testComplement();
}

SECTION("mask xor")
{
    OpsTest t;
    t.testMaskXOr();
}

SECTION("mask or")
{
    OpsTest t;
    t.testMaskOr();
}

SECTION("mask or not")
{
    OpsTest t;
    t.testMaskOrNot();
}

SECTION("mask and")
{
    OpsTest t;
    t.testMaskAnd();
}

SECTION("mask and not")
{
    OpsTest t;
    t.testMaskAndNot();
}

SECTION("select")
{
    OpsTest t;
    t.testSelect();
}

SECTION("sum")
{
    OpsTest t;
    t.testSum();
}








