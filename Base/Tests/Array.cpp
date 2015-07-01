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
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "Base/String.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Base_Array_Tests);

TEST(Base_Array, OperatorSquareBrackets_AccessOnConstantArray_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    auto constTest = String::Array::PointerToConst(test);

    // -- Then.
    ASSERT_STREQ((*constTest)[0].toUTF8(), "Test");
}

TEST(Base_Array, OperatorSquareBrackets_OutOfBoundsAccess_ThrowsException)
{
    // -- Given.
    // -- When.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));

    // -- Then.
    ASSERT_THROW((*test)[-1], NxA::Exception);
    ASSERT_THROW((*test)[1], NxA::Exception);
    ASSERT_THROW((*test)[132], NxA::Exception);
}

TEST(Base_Array, OperatorSquareBrackets_OutOfBoundsAccessOnConstantArray_ThrowsException)
{
    // -- Given.
    // -- When.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    auto constTest = String::Array::PointerToConst(test);

    // -- Then.
    ASSERT_THROW((*constTest)[-1], NxA::Exception);
    ASSERT_THROW((*constTest)[1], NxA::Exception);
    ASSERT_THROW((*constTest)[132], NxA::Exception);
}

TEST(Base_Array, Append_AddingOneObject_AddsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();

    // -- When.
    test->append(String::stringWith("Test"));

    // -- Then.
    ASSERT_EQ(test->length(), 1);
    ASSERT_EQ(::strcmp((*test)[0].toUTF8(), "Test"), 0);
}

TEST(Base_Array, Append_AddingTwoObject_AddsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();

    // -- When.
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- Then.
    ASSERT_EQ(test->length(), 2);
    ASSERT_STREQ((*test)[0].toUTF8(), "Test");
    ASSERT_STREQ((*test)[1].toUTF8(), "Test2");
}

TEST(Base_Array, Length_EmptyArray_LengthReturnsZero)
{
    // -- Given.
    // -- When.
    auto test = String::Array::array();

    // -- Then.
    ASSERT_EQ(test->length(), 0);
}

TEST(Base_Array, emptyAll_ArrayWithTwoObject_RemovesAllObjects)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    test->emptyAll();

    // -- Then.
    ASSERT_EQ(test->length(), 0);
}

TEST(Base_Array, InsertAt_InsertingObjectAtTheBeginng_InsertsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    test->insertAt(String::stringWith("Test3"), test->begin());

    // -- Then.
    ASSERT_EQ(test->length(), 3);
    ASSERT_STREQ((*test)[0].toUTF8(), "Test3");
    ASSERT_STREQ((*test)[1].toUTF8(), "Test");
    ASSERT_STREQ((*test)[2].toUTF8(), "Test2");
}

TEST(Base_Array, InsertAt_InsertingObjectAtTheEnd_InsertsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    test->insertAt(String::stringWith("Test3"), test->end());

    // -- Then.
    ASSERT_EQ(test->length(), 3);
    ASSERT_STREQ((*test)[0].toUTF8(), "Test");
    ASSERT_STREQ((*test)[1].toUTF8(), "Test2");
    ASSERT_STREQ((*test)[2].toUTF8(), "Test3");
}

TEST(Base_Array, InsertAt_InsertingObjectInTheMiddle_InsertsObjectCorrectly)
{
    // -- Given.
    auto test = String::Array::array();
    test->append(String::stringWith("Test"));
    test->append(String::stringWith("Test2"));

    // -- When.
    test->insertAt(String::stringWith("Test3"), test->begin() + 1);

    // -- Then.
    ASSERT_EQ(test->length(), 3);
    ASSERT_STREQ((*test)[0].toUTF8(), "Test");
    ASSERT_STREQ((*test)[1].toUTF8(), "Test3");
    ASSERT_STREQ((*test)[2].toUTF8(), "Test2");
}

TEST(Base_Array, Find_ObjectInTheMiddle_ReturnsPositionCorrectly)
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
    auto position = test->find(object2);

    // -- Then.
    ASSERT_TRUE(position != test->end());
    ASSERT_TRUE(position == test->begin() + 1);
}

TEST(Base_Array, Remove_PositionOfObjectInTheMiddle_RemovesObjectCorrectly)
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
    test->remove(test->begin() + 1);

    // -- Then.
    ASSERT_EQ(test->length(), 2);
    ASSERT_STREQ((*test)[0].toUTF8(), "Test");
    ASSERT_STREQ((*test)[1].toUTF8(), "Test3");
}

TEST(Base_Array, Remove_ObjectInTheMiddle_RemovesObjectCorrectly)
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
    ASSERT_EQ(test->length(), 2);
    ASSERT_STREQ((*test)[0].toUTF8(), "Test");
    ASSERT_STREQ((*test)[1].toUTF8(), "Test3");
}
