//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//

#include "Persistence/ObjectID.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Persistence_ObjectID_Tests);

TEST(Persistence_ObjectID, ClassName_ObjectID_TestEquality)
{
    // -- Given.
    auto obj0 = ObjectID(1, 0);
    auto obj1 = ObjectID(1, 0);

    // -- When.
    // -- Then.
    ASSERT_TRUE(obj0 == obj1);
}

TEST(Persistence_ObjectID, ClassName_ObjectID_TestInequality)
{
    // -- Given.
    auto obj0 = ObjectID(1, 0);
    auto obj1 = ObjectID(1, 1);

    // -- When.
    // -- Then.
    ASSERT_TRUE(obj0 != obj1);
}

TEST(Persistence_ObjectID, ClassName_ObjectID_TestComparison)
{
    // -- Given.
    auto obj0 = ObjectID(1, 0);
    auto obj1 = ObjectID(1, 1);

    // -- When.
    // -- Then.
    ASSERT_TRUE(obj0 < obj1);
    ASSERT_FALSE(obj0 > obj1);
}

