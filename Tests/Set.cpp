//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
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
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "Base/Set.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Base_Set_Tests);

TEST(Base_Set, SetWith_ASetWithStrings_ReturnsAnSetWithTheSameObjectsAsTheSource)
{
    // -- Given.
    MutableSet<String> test;
    test.append(String("Test"));

    // -- When.
    auto result = Set<String>(test);

    // -- Then.
    ASSERT_EQ(1, result.length());
    ASSERT_EQ(test.anyObject(), result.anyObject());
}

TEST(Base_Set, SetWith_AnEmptySet_ReturnsAnEmptySet)
{
    // -- Given.
    MutableSet<String> test;

    // -- When.
    auto result = Set<String>(test);

    // -- Then.
    ASSERT_EQ(0, result.length());
}

TEST(Base_Set, ClassName_SetOfStrings_ClassNameIsReturnedCorrectly)
{
    // -- Given.
    MutableSet<String> test;

    // -- When.
    auto name = test.className();

    // -- Then.
    ASSERT_STREQ("MutableSet<String>", name);
}

TEST(Base_Set, ClassName_TwoMutableSetsOfStrings_ClassNameIsActuallyTheSamePointer)
{
    // -- Given.
    MutableSet<String> test;
    MutableSet<String> other;

    // -- When.
    auto name = test.className();
    auto otherName = other.className();

    // -- Then.
    ASSERT_EQ(otherName, name);
}

TEST(Base_Set, ClassName_TwoSetsOfStrings_ClassNameIsActuallyTheSamePointer)
{
    // -- Given.
    Set<String> test;
    Set<String> other;

    // -- When.
    auto name = test.className();
    auto otherName = other.className();

    // -- Then.
    ASSERT_EQ(otherName, name);
}

TEST(Base_Set, OperatorSquareBrackets_AccessOnConstantSet_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    MutableSet<String> test;
    test.append(String("Test"));
    Set<String> constTest(test);

    // -- Then.
    ASSERT_EQ(1, test.length());
    ASSERT_STREQ("Test", constTest.anyObject().asUTF8());
}

TEST(Base_Set, Append_AddingOneObject_AddsObjectCorrectly)
{
    // -- Given.
    MutableSet<String> test;

    // -- When.
    test.append(String("Test"));

    // -- Then.
    ASSERT_EQ(1, test.length());
    ASSERT_EQ(::strcmp(test.anyObject().asUTF8(), "Test"), 0);
}

TEST(Base_Set, OperatorEqual_TwoDifferentSets_ReturnFalse)
{
    // -- Given.
    MutableSet<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));
    MutableSet<String> test2;
    test2.append(String("Test"));
    test2.append(String("OtherTest2"));

    // -- When.
    // -- Then.
    ASSERT_FALSE(test == test2);
}

TEST(Base_Set, OperatorEqual_TwoEqualSets_ReturnTrue)
{
    // -- Given.
    MutableSet<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));
    MutableSet<String> test2;
    test2.append(String("Test"));
    test2.append(String("Test2"));

    // -- When.
    // -- Then.
    ASSERT_TRUE(test == test2);
}

TEST(Base_Set, EmptyAll_MutableSetWithTwoObject_AssignCopyStillHasElements)
{
    // -- Given.
    MutableSet<String> test1;
    Set<String> test2;
    test1.append(String("Test"));
    test1.append(String("Test2"));
    test2 = test1;

    // -- When.
    test1.removeAll();

    // -- Then.
    ASSERT_EQ(0, test1.length());
    ASSERT_EQ(2, test2.length());
}

TEST(Base_Set, EmptyAll_ImmutableSetWithTwoObject_AssignCopyStillHasElements)
{
    // -- Given.
    MutableSet<String> test1;
    Set<String> test2;
    test1.append(String("Test"));
    test1.append(String("Test2"));
    test2 = test1;

    // -- When.
    test1.removeAll();

    // -- Then.
    ASSERT_EQ(0, test1.length());
    ASSERT_EQ(2, test2.length());
}

TEST(Base_Set, EmptyAll_MutableSetWithTwoObject_CopyStillHasElements)
{
    // -- Given.
    MutableSet<String> test1;
    test1.append(String("Test"));
    test1.append(String("Test2"));
    Set<String> test2{test1};

    // -- When.
    test1.removeAll();

    // -- Then.
    ASSERT_EQ(0, test1.length());
    ASSERT_EQ(2, test2.length());
}

TEST(Base_Set, EmptyAll_ImmutableSetWithTwoObject_CopyStillHasElements)
{
    // -- Given.
    MutableSet<String> test1;
    test1.append(String("Test"));
    test1.append(String("Test2"));
    MutableSet<String> test2{test1};

    // -- When.
    test1.removeAll();

    // -- Then.
    ASSERT_EQ(0, test1.length());
    ASSERT_EQ(2, test2.length());
}

TEST(Base_Set, Length_EmptySet_LengthReturnsZero)
{
    // -- Given.
    // -- When.
    MutableSet<String> test;

    // -- Then.
    ASSERT_EQ(0, test.length());
}

TEST(Base_Set, EmptyAll_SetWithTwoObject_RemovesAllObjects)
{
    // -- Given.
    MutableSet<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));

    // -- When.
    test.removeAll();

    // -- Then.
    ASSERT_EQ(0, test.length());
}

TEST(Base_Set, Find_ObjectInTheMiddleWithSameValue_ReturnsPositionCorrectly)
{
    // -- Given.
    MutableSet<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    auto object3 = String("Test3");
    test.append(object1);
    test.append(object2);
    test.append(object3);

    // -- When.
    auto position = test.find(String("Test2"));

    // -- Then.
    ASSERT_TRUE(position != test.end());
}

TEST(Base_Set, Contains_ObjectInTheSetWithSameValue_ReturnsTrue)
{
    // -- Given.
    MutableSet<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    test.append(object1);
    test.append(object2);
    auto object3 = String("Test2");

    // -- When.
    // -- Then.
    ASSERT_TRUE(test.contains(object3));
}

TEST(Base_Set, Contains_ObjectAlreadyInTheSet_ReturnsTrue)
{
    // -- Given.
    MutableSet<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    test.append(object1);
    test.append(object2);

    // -- When.
    // -- Then.
    ASSERT_TRUE(test.contains(object2));
}

TEST(Base_Set, Contains_ObjectNotInTheSet_ReturnsFalse)
{
    // -- Given.
    MutableSet<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    test.append(object1);
    test.append(object2);
    auto object3 = String("Test3");

    // -- When.
    // -- Then.
    ASSERT_FALSE(test.contains(object3));
}

TEST(Base_Set, Find_ObjectInEndWithSameObject_ReturnsPositionCorrectly)
{
    // -- Given.
    MutableSet<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    auto object3 = String("Test3");
    test.append(object1);
    test.append(object2);
    test.append(object3);

    // -- When.
    auto position = test.find(object3);

    // -- Then.
    ASSERT_TRUE(position != test.end());
}
