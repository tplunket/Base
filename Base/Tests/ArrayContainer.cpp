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

#include "Base/ArrayContainer.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Base_ArrayContainer_Tests);

TEST(Base_ArrayContainer, ArrayWith_AnArrayWithStrings_ReturnsAnArrayWithTheSameObjectsAsTheSource)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    auto result = String::Array::arrayWith(test);

    // -- Then.
    ASSERT_EQ(2, result->length());
    ASSERT_EQ(&(*test)[0], &(*result)[0]);
    ASSERT_EQ(&(*test)[1], &(*result)[1]);
}

TEST(Base_ArrayContainer, ArrayWith_AnEmptyArray_ReturnsAnEmptyArray)
{
    // -- Given.
    auto test = String::Array::array();

    // -- When.
    auto result = String::Array::arrayWith(test);

    // -- Then.
    ASSERT_EQ(0, result->length());
}

TEST(Base_ArrayContainer, ClassName_ArrayContainerOfStrings_ClassNameIsReturnedCorrectly)
{
    // -- Given.
    auto test = String::Array::array();

    // -- When.
    auto name = test->className();

    // -- Then.
    ASSERT_STREQ("NxA::Array<NxA::String>", name);
}

TEST(Base_ArrayContainer, ClassName_TwoArraysOfStrings_ClassNameIsActuallyTheSamePointer)
{
    // -- Given.
    auto test = String::Array::array();
    auto other = String::Array::array();

    // -- When.
    auto name = test->className();
    auto otherName = other->className();

    // -- Then.
    ASSERT_EQ(otherName, name);
}

TEST(Base_ArrayContainer, OperatorSquareBrackets_AccessOnConstantArray_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    auto constTest = String::Array::PointerToConst(test);

    // -- Then.
    ASSERT_EQ(1, test->length());
    ASSERT_STREQ("Test", (*constTest)[0].toUTF8());
}

TEST(Base_ArrayContainer, OperatorSquareBrackets_OutOfBoundsAccess_ThrowsException)
{
    // -- Given.
    // -- When.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));

    // -- Then.
    ASSERT_THROW((*test)[-1], NxA::AssertionFailed);
    ASSERT_THROW((*test)[1], NxA::AssertionFailed);
    ASSERT_THROW((*test)[132], NxA::AssertionFailed);
}

TEST(Base_ArrayContainer, OperatorSquareBrackets_OutOfBoundsAccessOnConstantArray_ThrowsException)
{
    // -- Given.
    // -- When.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    auto constTest = String::Array::PointerToConst(test);

    // -- Then.
    ASSERT_THROW((*constTest)[-1], NxA::AssertionFailed);
    ASSERT_THROW((*constTest)[1], NxA::AssertionFailed);
    ASSERT_THROW((*constTest)[132], NxA::AssertionFailed);
}

TEST(Base_ArrayContainer, Append_AddingOneObject_AddsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();

    // -- When.
    test->append(String::stringWith("Test"));

    // -- Then.
    ASSERT_EQ(1, test->length());
    ASSERT_EQ(::strcmp((*test)[0].toUTF8(), "Test"), 0);
}

TEST(Base_ArrayContainer, Append_AddingTwoObject_AddsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();

    // -- When.
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- Then.
    ASSERT_EQ(2, test->length());
    ASSERT_STREQ("Test", (*test)[0].toUTF8());
    ASSERT_STREQ("Test2", (*test)[1].toUTF8());
}

TEST(Base_ArrayContainer, OperatorEqual_TwoDifferentArrays_ReturnFalse)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));
    auto test2 = String::Array::array();
    test2->append(String::stringWith("Test"));
    test2->append(String::stringWith("OtherTest2"));

    // -- When.
    // -- Then.
    ASSERT_FALSE(test == test2);
}

TEST(Base_ArrayContainer, OperatorEqual_TwoEqualArrays_ReturnTrue)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));
    auto test2 = String::Array::array();
    test2->append(String::stringWith("Test"));
    test2->append(String::stringWith("Test2"));

    // -- When.
    // -- Then.
    ASSERT_TRUE(test == test2);
}

TEST(Base_ArrayContainer, Length_EmptyArray_LengthReturnsZero)
{
    // -- Given.
    // -- When.
    auto test = String::Array::array();

    // -- Then.
    ASSERT_EQ(0, test->length());
}

TEST(Base_ArrayContainer, EmptyAll_ArrayContainerWithTwoObject_RemovesAllObjects)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    test->emptyAll();

    // -- Then.
    ASSERT_EQ(0, test->length());
}

TEST(Base_ArrayContainer, InsertAt_InsertingObjectAtTheBeginng_InsertsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    test->insertAt(String::stringWith("Test3"), test->begin());

    // -- Then.
    ASSERT_EQ(3, test->length());
    ASSERT_STREQ("Test3", (*test)[0].toUTF8());
    ASSERT_STREQ("Test", (*test)[1].toUTF8());
    ASSERT_STREQ("Test2", (*test)[2].toUTF8());
}

TEST(Base_ArrayContainer, InsertAt_InsertingObjectAtTheEnd_InsertsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    test->insertAt(String::stringWith("Test3"), test->end());

    // -- Then.
    ASSERT_EQ(3, test->length());
    ASSERT_STREQ("Test", (*test)[0].toUTF8());
    ASSERT_STREQ("Test2", (*test)[1].toUTF8());
    ASSERT_STREQ("Test3", (*test)[2].toUTF8());
}

TEST(Base_ArrayContainer, InsertAt_InsertingObjectInTheMiddle_InsertsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    test->insertAt(String::stringWith("Test3"), test->begin() + 1);

    // -- Then.
    ASSERT_EQ(3, test->length());
    ASSERT_STREQ("Test", (*test)[0].toUTF8());
    ASSERT_STREQ("Test3", (*test)[1].toUTF8());
    ASSERT_STREQ("Test2", (*test)[2].toUTF8());
}

TEST(Base_ArrayContainer, Find_ObjectInTheMiddleWithSameValue_ReturnsPositionCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    auto object1 = String::stringWith("Test");
    auto object2 = String::stringWith("Test2");
    auto object3 = String::stringWith("Test3");
    test->append(object1);
    test->append(object2);
    test->append(object3);

    // -- When.
    auto position = test->find(String::stringWith("Test2"));

    // -- Then.
    ASSERT_TRUE(position != test->end());
    ASSERT_TRUE(position == test->begin() + 1);
}

TEST(Base_ArrayContainer, Contains_ObjectInTheArrayWithSameValue_ReturnsTrue)
{
    // -- Given.
    auto test = String::Array::array();
    auto object1 = String::stringWith("Test");
    auto object2 = String::stringWith("Test2");
    test->append(object1);
    test->append(object2);
    auto object3 = String::stringWith("Test2");

    // -- When.
    // -- Then.
    ASSERT_TRUE(test->contains(object3));
}

TEST(Base_ArrayContainer, Contains_ObjectAlreadyInTheArray_ReturnsTrue)
{
    // -- Given.
    auto test = String::Array::array();
    auto object1 = String::stringWith("Test");
    auto object2 = String::stringWith("Test2");
    test->append(object1);
    test->append(object2);

    // -- When.
    // -- Then.
    ASSERT_TRUE(test->contains(object2));
}

TEST(Base_ArrayContainer, Contains_ObjectNotInTheArray_ReturnsFalse)
{
    // -- Given.
    auto test = String::Array::array();
    auto object1 = String::stringWith("Test");
    auto object2 = String::stringWith("Test2");
    test->append(object1);
    test->append(object2);
    auto object3 = String::stringWith("Test3");

    // -- When.
    // -- Then.
    ASSERT_FALSE(test->contains(object3));
}

TEST(Base_ArrayContainer, Find_ObjectInEndWithSameObject_ReturnsPositionCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    auto object1 = String::stringWith("Test");
    auto object2 = String::stringWith("Test2");
    auto object3 = String::stringWith("Test3");
    test->append(object1);
    test->append(object2);
    test->append(object3);

    // -- When.
    auto position = test->find(object3);

    // -- Then.
    ASSERT_TRUE(position != test->end());
    ASSERT_TRUE(position == test->begin() + 2);
}

TEST(Base_ArrayContainer, RemoveObjectAt_PositionOfObjectInTheMiddle_RemovesObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    auto object1 = String::stringWith("Test");
    auto object2 = String::stringWith("Test2");
    auto object3 = String::stringWith("Test3");
    test->append(object1);
    test->append(object2);
    test->append(object3);

    // -- When.
    test->removeObjectAt(test->begin() + 1);

    // -- Then.
    ASSERT_EQ(2, test->length());
    ASSERT_STREQ("Test", (*test)[0].toUTF8());
    ASSERT_STREQ("Test3", (*test)[1].toUTF8());
}

TEST(Base_ArrayContainer, Remove_ObjectInTheMiddle_RemovesObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    auto object1 = String::stringWith("Test");
    auto object2 = String::stringWith("Test2");
    auto object3 = String::stringWith("Test3");
    test->append(object1);
    test->append(object2);
    test->append(object3);

    // -- When.
    test->remove(object2);

    // -- Then.
    ASSERT_EQ(2, test->length());
    ASSERT_STREQ("Test", (*test)[0].toUTF8());
    ASSERT_STREQ("Test3", (*test)[1].toUTF8());
}
