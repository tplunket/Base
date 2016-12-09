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

#include "Base/Array.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Base_Array_Tests);

TEST(Base_Array, ArrayWith_AnArrayWithStrings_ReturnsAnArrayWithTheSameObjectsAsTheSource)
{
    // -- Given.
    MutableArray<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));

    // -- When.
    auto result = Array<String>(test);

    // -- Then.
    ASSERT_EQ(2, result.length());
    ASSERT_EQ(test[0], result[0]);
    ASSERT_EQ(test[1], result[1]);
}

TEST(Base_Array, ArrayWith_AnEmptyArray_ReturnsAnEmptyArray)
{
    // -- Given.
    MutableArray<String> test;

    // -- When.
    auto result = Array<String>(test);

    // -- Then.
    ASSERT_EQ(0, result.length());
}

TEST(Base_Array, ClassName_ArrayOfStrings_ClassNameIsReturnedCorrectly)
{
    // -- Given.
    MutableArray<String> test;

    // -- When.
    auto name = test.className();

    // -- Then.
    ASSERT_STREQ("MutableArray<String>", name);
}

TEST(Base_Array, ClassName_TwoMutableArraysOfStrings_ClassNameIsActuallyTheSamePointer)
{
    // -- Given.
    MutableArray<String> test;
    MutableArray<String> other;

    // -- When.
    auto name = test.className();
    auto otherName = other.className();

    // -- Then.
    ASSERT_EQ(otherName, name);
}

TEST(Base_Array, ClassName_TwoArraysOfStrings_ClassNameIsActuallyTheSamePointer)
{
    // -- Given.
    Array<String> test;
    Array<String> other;

    // -- When.
    auto name = test.className();
    auto otherName = other.className();

    // -- Then.
    ASSERT_EQ(otherName, name);
}

TEST(Base_Array, OperatorSquareBrackets_AccessOnConstantArray_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    MutableArray<String> test;
    test.append(String("Test"));
    Array<String> constTest(test);

    // -- Then.
    ASSERT_EQ(1, test.length());
    ASSERT_STREQ("Test", constTest[0].asUTF8());
}

TEST(Base_Array, OperatorSquareBrackets_AccessOnMutableArrayOfMutable_ReturnsTheArrayACopy)
{
    // -- Given.
    MutableArray<MutableArray<String>> test;
    MutableArray<String> test2;
    test.append(test2);
    test2.append(String("Test"));

    // -- When.
    auto result = test[0];

    // -- Then.
    ASSERT_EQ(0, result.length());
    ASSERT_STREQ("Test", test2.firstObject().asUTF8());
}

TEST(Base_Array, OperatorSquareBrackets_OutOfBoundsAccess_ThrowsException)
{
    // -- Given.
    // -- When.
    MutableArray<String> test;
    test.append(String("Test"));

    // -- Then.
    ASSERT_THROW(test[-1], NxA::AssertionFailed);
    ASSERT_THROW(test[1], NxA::AssertionFailed);
    ASSERT_THROW(test[132], NxA::AssertionFailed);
}

TEST(Base_Array, OperatorSquareBrackets_OutOfBoundsAccessOnConstantArray_ThrowsException)
{
    // -- Given.
    // -- When.
    MutableArray<String> test;
    test.append(String("Test"));
    Array<String> constTest(test);

    // -- Then.
    ASSERT_THROW(constTest[-1], NxA::AssertionFailed);
    ASSERT_THROW(constTest[1], NxA::AssertionFailed);
    ASSERT_THROW(constTest[132], NxA::AssertionFailed);
}

TEST(Base_Array, Append_AddingOneObject_AddsObjectCorrectly)
{
    // -- Given.
    MutableArray<String> test;

    // -- When.
    test.append(String("Test"));

    // -- Then.
    ASSERT_EQ(1, test.length());
    ASSERT_EQ(::strcmp(test[0].asUTF8(), "Test"), 0);
}

TEST(Base_Array, Append_AddingTwoObject_AddsObjectCorrectly)
{
    // -- Given.
    MutableArray<String> test;

    // -- When.
    test.append(String("Test"));
    test.append(String("Test2"));

    // -- Then.
    ASSERT_EQ(2, test.length());
    ASSERT_STREQ("Test", test[0].asUTF8());
    ASSERT_STREQ("Test2", test[1].asUTF8());
}

TEST(Base_Array, OperatorEqual_TwoDifferentArrays_ReturnFalse)
{
    // -- Given.
    MutableArray<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));
    MutableArray<String> test2;
    test2.append(String("Test"));
    test2.append(String("OtherTest2"));

    // -- When.
    // -- Then.
    ASSERT_FALSE(test == test2);
}

TEST(Base_Array, OperatorEqual_TwoEqualArrays_ReturnTrue)
{
    // -- Given.
    MutableArray<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));
    MutableArray<String> test2;
    test2.append(String("Test"));
    test2.append(String("Test2"));

    // -- When.
    // -- Then.
    ASSERT_TRUE(test == test2);
}

TEST(Base_Array, Length_EmptyArray_LengthReturnsZero)
{
    // -- Given.
    // -- When.
    MutableArray<String> test;

    // -- Then.
    ASSERT_EQ(0, test.length());
}

TEST(Base_Array, EmptyAll_MutableArrayWithTwoObject_AssignCopyStillHasElements)
{
    // -- Given.
    MutableArray<String> test1;
    Array<String> test2;
    test1.append(String("Test"));
    test1.append(String("Test2"));
    test2 = test1;

    // -- When.
    test1.removeAll();

    // -- Then.
    ASSERT_EQ(0, test1.length());
    ASSERT_EQ(2, test2.length());
}

TEST(Base_Array, EmptyAll_ImmutableArrayWithTwoObject_AssignCopyStillHasElements)
{
    // -- Given.
    MutableArray<String> test1;
    MutableArray<String> test2;
    test1.append(String("Test"));
    test1.append(String("Test2"));
    test2 = test1;

    // -- When.
    test1.removeAll();

    // -- Then.
    ASSERT_EQ(0, test1.length());
    ASSERT_EQ(2, test2.length());
}

TEST(Base_Array, EmptyAll_MutableArrayWithTwoObject_CopyStillHasElements)
{
    // -- Given.
    MutableArray<String> test1;
    test1.append(String("Test"));
    test1.append(String("Test2"));
    Array<String> test2{test1};

    // -- When.
    test1.removeAll();

    // -- Then.
    ASSERT_EQ(0, test1.length());
    ASSERT_EQ(2, test2.length());
}

TEST(Base_Array, EmptyAll_ImmutableArrayWithTwoObject_CopyStillHasElements)
{
    // -- Given.
    MutableArray<String> test1;
    test1.append(String("Test"));
    test1.append(String("Test2"));
    MutableArray<String> test2{test1};

    // -- When.
    test1.removeAll();

    // -- Then.
    ASSERT_EQ(0, test1.length());
    ASSERT_EQ(2, test2.length());
}

TEST(Base_Array, EmptyAll_ArrayWithTwoObject_RemovesAllObjects)
{
    // -- Given.
    MutableArray<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));

    // -- When.
    test.removeAll();

    // -- Then.
    ASSERT_EQ(0, test.length());
}

TEST(Base_Array, InsertAt_InsertingObjectAtTheBeginng_InsertsObjectCorrectly)
{
    // -- Given.
    MutableArray<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));

    // -- When.
    test.insertAt(String("Test3"), test.begin());

    // -- Then.
    ASSERT_EQ(3, test.length());
    ASSERT_STREQ("Test3", test[0].asUTF8());
    ASSERT_STREQ("Test", test[1].asUTF8());
    ASSERT_STREQ("Test2", test[2].asUTF8());
}

TEST(Base_Array, InsertAt_InsertingObjectAtTheEnd_InsertsObjectCorrectly)
{
    // -- Given.
    MutableArray<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));

    // -- When.
    test.insertAt(String("Test3"), test.end());

    // -- Then.
    ASSERT_EQ(3, test.length());
    ASSERT_STREQ("Test", test[0].asUTF8());
    ASSERT_STREQ("Test2", test[1].asUTF8());
    ASSERT_STREQ("Test3", test[2].asUTF8());
}

TEST(Base_Array, InsertAt_InsertingObjectInTheMiddle_InsertsObjectCorrectly)
{
    // -- Given.
    MutableArray<String> test;
    test.append(String("Test"));
    test.append(String("Test2"));

    // -- When.
    test.insertAt(String("Test3"), test.begin() + 1);

    // -- Then.
    ASSERT_EQ(3, test.length());
    ASSERT_STREQ("Test", test[0].asUTF8());
    ASSERT_STREQ("Test3", test[1].asUTF8());
    ASSERT_STREQ("Test2", test[2].asUTF8());
}

TEST(Base_Array, Find_ObjectInTheMiddleWithSameValue_ReturnsPositionCorrectly)
{
    // -- Given.
    MutableArray<String> test;
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
    ASSERT_TRUE(position == (test.begin() + 1));
}

TEST(Base_Array, Contains_ObjectInTheArrayWithSameValue_ReturnsTrue)
{
    // -- Given.
    MutableArray<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    test.append(object1);
    test.append(object2);
    auto object3 = String("Test2");

    // -- When.
    // -- Then.
    ASSERT_TRUE(test.contains(object3));
}

TEST(Base_Array, Contains_ObjectAlreadyInTheArray_ReturnsTrue)
{
    // -- Given.
    MutableArray<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    test.append(object1);
    test.append(object2);

    // -- When.
    // -- Then.
    ASSERT_TRUE(test.contains(object2));
}

TEST(Base_Array, Contains_ObjectNotInTheArray_ReturnsFalse)
{
    // -- Given.
    MutableArray<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    test.append(object1);
    test.append(object2);
    auto object3 = String("Test3");

    // -- When.
    // -- Then.
    ASSERT_FALSE(test.contains(object3));
}

TEST(Base_Array, Find_ObjectInEndWithSameObject_ReturnsPositionCorrectly)
{
    // -- Given.
    MutableArray<String> test;
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
    ASSERT_TRUE(position == (test.begin() + 2));
}

TEST(Base_Array, RemoveObjectAt_PositionOfObjectInTheMiddle_RemovesObjectCorrectly)
{
    // -- Given.
    MutableArray<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    auto object3 = String("Test3");
    test.append(object1);
    test.append(object2);
    test.append(object3);

    // -- When.
    test.removeObjectAt(test.begin() + 1);

    // -- Then.
    ASSERT_EQ(2, test.length());
    ASSERT_STREQ("Test", test[0].asUTF8());
    ASSERT_STREQ("Test3", test[1].asUTF8());
}

TEST(Base_Array, Remove_ObjectInTheMiddle_RemovesObjectCorrectly)
{
    // -- Given.
    MutableArray<String> test;
    auto object1 = String("Test");
    auto object2 = String("Test2");
    auto object3 = String("Test3");
    test.append(object1);
    test.append(object2);
    test.append(object3);

    // -- When.
    test.remove(object2);

    // -- Then.
    ASSERT_EQ(2, test.length());
    ASSERT_STREQ("Test", test[0].asUTF8());
    ASSERT_STREQ("Test3", test[1].asUTF8());
}

TEST(Base_Array, Sort_SortedArray_ReturnsUnchangedArray)
{
    // -- Given.
    MutableArray<String> test;
    auto object1 = String("ATest");
    auto object2 = String("BTest2");
    auto object3 = String("CTest3");
    test.append(object1);
    test.append(object2);
    test.append(object3);

    // -- When.
    test.sort();

    // -- Then.
    ASSERT_EQ(3, test.length());
    ASSERT_STREQ("ATest", test[0].asUTF8());
    ASSERT_STREQ("BTest2", test[1].asUTF8());
    ASSERT_STREQ("CTest3", test[2].asUTF8());
}

TEST(Base_Array, Sort_UnsortedArray_ReturnsASortedArray)
{
    // -- Given.
    MutableArray<String> test;
    auto object1 = String("CTest");
    auto object2 = String("ATest2");
    auto object3 = String("BTest3");
    test.append(object1);
    test.append(object2);
    test.append(object3);

    // -- When.
    test.sort();

    // -- Then.
    ASSERT_EQ(3, test.length());
    ASSERT_STREQ("ATest2", test[0].asUTF8());
    ASSERT_STREQ("BTest3", test[1].asUTF8());
    ASSERT_STREQ("CTest", test[2].asUTF8());
}
