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

#include "Base/Map.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Base_Map_Tests);

using TestMapUInteger32ToString = Map<uinteger32, String>;
using TestMapUInteger32ToUInteger32 = Map<uinteger32, uinteger32>;

static const auto testString = String::stringWith("Test String");
static const auto otherString = String::stringWith("Other String");

TEST(Base_Map, ClassName_MapOfUInteger32AndUInteger32_ClassNameIsReturnedCorrectly)
{
    // -- Given.
    auto test = Map<uinteger32, uinteger32>::map();

    // -- When.
    auto name = test->className();

    // -- Then.
    ASSERT_STREQ("NxA::Map<uinteger32, uinteger32>", name);
}

TEST(Base_Map, ClassName_MapOfUInteger32AndMapOfBytesAndArrayOfStrings_ClassNameIsReturnedCorrectly)
{
    // -- Given.
    auto test = Map<uinteger32, Map<byte, String::Array>>::map();

    // -- When.
    auto name = test->className();

    // -- Then.
    ASSERT_STREQ("NxA::Map<uinteger32, NxA::Map<byte, NxA::Array<NxA::String>>>", name);
}

TEST(Base_Map, ClassName_TwoMapsOfUInteger32AndMapOfBytesAndArrayOfStrings_ClassNameIsActuallyTheSamePointer)
{
    // -- Given.
    auto test = Map<uinteger32, Map<byte, String::Array>>::map();
    auto other = Map<uinteger32, Map<byte, String::Array>>::map();

    // -- When.
    auto name = test->className();
    auto otherName = other->className();

    // -- Then.
    ASSERT_EQ(otherName, name);
}

TEST(Base_Map, SetValueForKey_IntegerValue_SetsCorrectValue)
{
    // -- Given.
    static const uinteger32 integerValue = 72;
    auto test = TestMapUInteger32ToUInteger32::map();

    // -- When.
    test->setValueForKey(integerValue, 0);

    // -- Then.
    ASSERT_EQ(integerValue, test->valueForKey(0));
}

TEST(Base_Map, SetValueForKey_ValueFromAPointer_SetsCorrectValue)
{
    // -- Given.
    static const character* otherTestString = "testString";
    auto test = TestMapUInteger32ToString::map();

    // -- When.
    test->setValueForKey(String::stringWith(otherTestString), 0);

    // -- Then.
    ASSERT_STREQ(otherTestString, test->valueForKey(0).toUTF8());
}

TEST(Base_Map, SetValueForKey_ValueFromAReference_SetsCorrectValue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();

    // -- When.
    test->setValueForKey(testString, 0);

    // -- Then.
    ASSERT_STREQ(testString->toUTF8(), test->valueForKey(0).toUTF8());
}

TEST(Base_Map, SetValueForKey_ValueFromAPointerOverAnExistingValue_SetsCorrectValue)
{
    // -- Given.
    static const character* otherTestString = "testString";
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(String::stringWith("Initial String"), 44);

    // -- When.
    test->setValueForKey(String::stringWith(otherTestString), 44);

    // -- Then.
    ASSERT_STREQ(otherTestString, test->valueForKey(44).toUTF8());
}

TEST(Base_Map, SetValueForKey_ValueFromAReferenceOverAnExistingValue_SetsCorrectValue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(testString, 44);

    // -- When.
    test->setValueForKey(otherString, 44);

    // -- Then.
    ASSERT_STREQ(otherString->toUTF8(), test->valueForKey(44).toUTF8());
}

TEST(Base_Map, ValueForKey_MapWithAGivenValue_ReturnsCorrectValue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(testString, 0x2323);
    test->setValueForKey(otherString, 0x2423);

    // -- When.
    // -- Then.
    ASSERT_STREQ(testString->toUTF8(), test->valueForKey(0x2323).toUTF8());
}

TEST(Base_Map, ValueForKey_MapWithAnotherValue_ReturnsCorrectValue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(testString, 0x2323);
    test->setValueForKey(otherString, 0x2423);

    // -- When.
    // -- Then.
    ASSERT_STREQ(otherString->toUTF8(), test->valueForKey(0x2423).toUTF8());
}

TEST(Base_Map, ValueForKey_LookingForAnUnknownKey_ThrowsException)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(String::stringWith(testString), 0x2323);
    test->setValueForKey(String::stringWith(otherString), 0x2423);

    // -- When.
    // -- Then.
    ASSERT_THROW(test->valueForKey(0x23).toUTF8(), std::exception);
}

TEST(Base_Map, ValueForKey_ConstMapWithAGivenValue_ReturnsCorrectValue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(testString, 0x2323);
    test->setValueForKey(otherString, 0x2423);
    Pointer<const TestMapUInteger32ToString> constTest = test;

    // -- When.
    // -- Then.
    ASSERT_STREQ(testString->toUTF8(), constTest->valueForKey(0x2323).toUTF8());
}

TEST(Base_Map, ValueForKey_ConstMapWithAnotherValue_ReturnsCorrectValue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(testString, 0x2323);
    test->setValueForKey(otherString, 0x2423);
    Pointer<const TestMapUInteger32ToString> constTest = test;

    // -- When.
    // -- Then.
    ASSERT_STREQ(otherString->toUTF8(), constTest->valueForKey(0x2423).toUTF8());
}

TEST(Base_Map, ValueForKey_LookingForAnUnknownKeyInConstMap_ThrowsException)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(String::stringWith(testString), 0x2323);
    test->setValueForKey(String::stringWith(otherString), 0x2423);
    Pointer<const TestMapUInteger32ToString> constTest = test;

    // -- When.
    // -- Then.
    ASSERT_THROW(constTest->valueForKey(0x23).toUTF8(), std::exception);
}

TEST(Base_Map, ContainsValueForKey_UnknownKey_ReturnsFalse)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(String::stringWith(testString), 0x2323);
    test->setValueForKey(String::stringWith(otherString), 0x2423);

    // -- When.
    // -- Then.
    ASSERT_FALSE(test->containsValueForKey(0x23));
}

TEST(Base_Map, ContainsValueForKey_KnownKey_ReturnsTrue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(String::stringWith(testString), 0x2323);
    test->setValueForKey(String::stringWith(otherString), 0x2423);

    // -- When.
    // -- Then.
    ASSERT_TRUE(test->containsValueForKey(0x2323));
}

TEST(Base_Map, Length_MapWithKeys_ReturnsCorrectValue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();
    test->setValueForKey(String::stringWith(testString), 0x2323);
    test->setValueForKey(String::stringWith(otherString), 0x2423);

    // -- When.
    // -- Then.
    ASSERT_EQ(2, test->length());
}

TEST(Base_Map, Length_MapWithNoKeys_ReturnsCorrectValue)
{
    // -- Given.
    auto test = TestMapUInteger32ToString::map();

    // -- When.
    // -- Then.
    ASSERT_EQ(0, test->length());
}
