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

NXA_CONTAINS_TEST_SUITE_NAMED(Base_String_Tests);

static const uinteger16 utf16String[] = {
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E,
    0x002F, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D,
    0x003E, 0x003F, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C,
    0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B,
    0x005C, 0x005D, 0x005E, 0x005F, 0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A,
    0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079,
    0x007A, 0x007B, 0x007C, 0x007D, 0x007E
};

static const character *utf8String = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

TEST(Base_String, StringContructorAndToUTF8_StringCreatedFromCharPointer_ToUTF8ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith(utf8String);

    // -- Then.
    ASSERT_TRUE(strcmp(test->toUTF8(), utf8String) == 0);
}

TEST(Base_String, StringContructorAndToUTF8_StringCreatedFromOtherString_ToUTF8ReturnsContainsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    auto otherTest = String::stringWith(test);

    // -- Then.
    ASSERT_TRUE(strcmp(otherTest->toUTF8(), utf8String) == 0);
}

TEST(Base_String, StringWithUTF16_StringCreatedFromUTF16String_ContainsCorrectValue)
{
    // -- Given.
    auto blob = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(utf16String), sizeof(utf16String));
    if (Platform::endianOrder == Platform::LitleEndian) {
        blob = Platform::convertEndiannessOfUInteger16From(blob);
    }

    // -- When.
    auto test = String::stringWithUTF16(blob);
    
    // -- Then.
    ASSERT_TRUE(strcmp(test->toUTF8(), utf8String) == 0);
}

TEST(Base_String, CharPointerCast_StringFromUTF8_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith(utf8String);

    // -- Then.
    ASSERT_TRUE(strcmp(test->operator const character*(), utf8String) == 0);
}

TEST(Base_String, ConstCharPointerCast_StringFromUTF8_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    String::PointerToConst test = String::stringWith(utf8String);

    // -- Then.
    ASSERT_TRUE(strcmp(test->operator const character*(), utf8String) == 0);
}

TEST(Base_String, Description_StringWithAValue_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith(utf8String);

    // -- Then.
    ASSERT_TRUE(strcmp(test->description()->toUTF8(), utf8String) == 0);
}

TEST(Base_String, IsEqual_TwoEqualStrings_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    auto test2 = String::stringWith(utf8String);

    // -- Then.
    ASSERT_TRUE(test->isEqualTo(test2));
}

TEST(Base_String, IsEqualWithACharacterPointer_TwoEqualStrings_ReturnsTrue)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith(utf8String);

    // -- Then.
    ASSERT_TRUE(test->isEqualTo(utf8String));
}

TEST(Base_String, Length_StringWithAValue_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith(utf8String);

    // -- Then.
    ASSERT_EQ(test->length(), 95);
}

TEST(Base_String, Length_StringWithEmptyString_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::string();

    // -- Then.
    ASSERT_EQ(test->length(), 0);
}

TEST(Base_String, Length_StringWithNoValue_ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith("");

    // -- Then.
    ASSERT_EQ(test->length(), 0);
}

TEST(Base_String, IsEmpty_StringWithAValue_ReturnFalse)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith(utf8String);

    // -- Then.
    ASSERT_FALSE(test->isEmpty());
}

TEST(Base_String, IsEmpty_StringWithEmptyString_ReturnsTrue)
{
    // -- Given.
    // -- When.
    auto test = String::string();

    // -- Then.
    ASSERT_TRUE(test->isEmpty());
}

TEST(Base_String, IsEmpty_StringWithNoValue_ReturnsTrue)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith("");

    // -- Then.
    ASSERT_TRUE(test->isEmpty());
}

TEST(Base_String, ToUTF16_StringCreatedFromUTF8String_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    auto result = test->toUTF16();

    // -- Then.
    ASSERT_EQ(test->length() * 2, result->size());
    ASSERT_EQ(sizeof(utf16String), result->size());
    ASSERT_TRUE(::memcmp(utf16String, result->data(), sizeof(utf16String)));
}

TEST(Base_String, Append_AStringToAString_ReturnsCorrectValue)
{
    // -- Given.
    auto test1 = String::stringWith("Hello This ");
    auto test2 = String::stringWith("Is A Test.");

    // -- When.
    test1->append(test2);

    // -- Then.
    ASSERT_TRUE(strcmp(test1->toUTF8(), "Hello This Is A Test.") == 0);
}

TEST(Base_String, Append_AStringToACharacterPointer_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This ");

    // -- When.
    test->append("Is A Test.");

    // -- Then.
    ASSERT_TRUE(strcmp(test->toUTF8(), "Hello This Is A Test.") == 0);
}

TEST(Base_String, SplitBySeperator_StringSeperatedBySpacesSpaces_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->splitBySeperator(' ');

    // -- Then.
    ASSERT_EQ(result->length(), 5);
    ASSERT_TRUE((*result)[0].isEqualTo("Hello"));
    ASSERT_TRUE((*result)[1].isEqualTo("This"));
    ASSERT_TRUE((*result)[2].isEqualTo("Is"));
    ASSERT_TRUE((*result)[3].isEqualTo("A"));
    ASSERT_TRUE((*result)[4].isEqualTo("Test."));
}

TEST(Base_String, Substring_FromAnIndex_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(6);

    // -- Then.
    ASSERT_TRUE(result->isEqualTo("This Is A Test."));
}

TEST(Base_String, Substring_FromAnIndexAtTheEndOfTheString_ReturnsEmptyString)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(21);

    // -- Then.
    ASSERT_TRUE(result->isEqualTo(""));
}

TEST(Base_String, Substring_FromAnIndexPastTheEndOfTheString_ReturnsEmptyString)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(245);

    // -- Then.
    ASSERT_TRUE(result->isEqualTo(""));
}

TEST(Base_String, Substring_FromAnIndexToAnother_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(6, 10);

    // -- Then.
    ASSERT_TRUE(result->isEqualTo("This"));
}

TEST(Base_String, Substring_FromAnIndexToAnotherAtTheEndOfTheString_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(6, 21);

    // -- Then.
    ASSERT_TRUE(result->isEqualTo("This Is A Test."));
}

TEST(Base_String, Substring_FromAnIndexToAnotherPastTheEndOfTheString_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(6, 234);

    // -- Then.
    ASSERT_TRUE(result->isEqualTo("This Is A Test."));
}

TEST(Base_String, Substring_FromAnIndexAndToAnotherBothPastTheEndOfTheString_ReturnsEmptyString)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(245, 255);

    // -- Then.
    ASSERT_TRUE(result->isEqualTo(""));
}

TEST(Base_String, Substring_FromAnIndexAndToAnotherInInverserOrderTheEndOfTheString_ReturnsEmptyString)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When/Then.
    ASSERT_THROW(test->subString(244, 25), NxA::Exception);
}

TEST(Base_String, HasPrefix_StringWithAGivenPrefix_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    // -- Then.
    ASSERT_TRUE(test->hasPrefix("Hello"));
}

TEST(Base_String, HasPrefix_StringWithoutAGivenPrefix_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    // -- Then.
    ASSERT_FALSE(test->hasPrefix("Hello2"));
}

TEST(Base_String, HasPrefix_StringWithAGivenPrefixAndAStringWithThatPrefix_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");
    auto test2 = String::stringWith("Hello");

    // -- When.
    // -- Then.
    ASSERT_TRUE(test->hasPrefix(test2));
}

TEST(Base_String, HasPrefixR_StringWithoutAGivenPrefixAndAStringWithThatPrefix_eturnsFalse)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");
    auto test2 = String::stringWith("Hello2");

    // -- When.
    // -- Then.
    ASSERT_FALSE(test->hasPrefix(test2));
}

TEST(Base_String, HasPostfix_StringWithAGivenPostfix_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    // -- Then.
    ASSERT_TRUE(test->hasPostfix("Test."));
}

TEST(Base_String, HasPostfix_StringWithoutAGivenPostfix_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    // -- Then.
    ASSERT_FALSE(test->hasPostfix("Test2"));
}

TEST(Base_String, HasPostfix_StringWithAGivenPostfixAndAStringWithThatPostfix_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");
    auto test2 = String::stringWith("Test.");

    // -- When.
    // -- Then.
    ASSERT_TRUE(test->hasPostfix(test2));
}

TEST(Base_String, HasPostfix_StringWithoutAGivenPostfixAndAStringWithThatPostfix_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");
    auto test2 = String::stringWith("Test2");

    // -- When.
    // -- Then.
    ASSERT_FALSE(test->hasPostfix(test2));
}

TEST(Base_String, IndexOfFirstOccurenceOf_StringWithoutAGivenSubStringAndStringWithThatSubString_ReturnsEndOfStringIndex)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A This Test.");
    auto test2 = String::stringWith("Thit");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->indexOfFirstOccurenceOf(test2), test->length());
}

TEST(Base_String, IndexOfFirstOccurenceOf_StringWithAGivenSubStringAndStringWithThatSubString_ReturnsCorrectIndex)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A This Test.");
    auto test2 = String::stringWith("This");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->indexOfFirstOccurenceOf(test2), 6);
}

TEST(Base_String, IndexOfLastOccurenceOf_StringWithoutAGivenSubStringAndStringWithThatSubString_ReturnsEndOfStringIndex)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A This Test.");
    auto test2 = String::stringWith("Thit");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->indexOfLastOccurenceOf(test2), test->length());
}

TEST(Base_String, IndexOfLastOccurenceOf_StringWithAGivenSubStringAndStringWithThatSubString_ReturnsCorrectIndex)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A This Test.");
    auto test2 = String::stringWith("This");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->indexOfLastOccurenceOf(test2), 16);
}

TEST(Base_String, IndexOfFirstOccurenceOf_StringWithoutAGivenSubStringAndCharacterPointergWithThatSubString_ReturnsEndOfStringIndex)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A This Test.");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->indexOfFirstOccurenceOf("Thit"), test->length());
}

TEST(Base_String, IndexOfFirstOccurenceOf_StringWithAGivenSubStringAndCharacterPointerWithThatSubString_ReturnsCorrectIndex)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A This Test.");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->indexOfFirstOccurenceOf("This"), 6);
}

TEST(Base_String, IndexOfLastOccurenceOf_StringWithoutAGivenSubStringAndCharacterPointerWithThatSubString_ReturnsEndOfStringIndex)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A This Test.");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->indexOfLastOccurenceOf("Thit"), test->length());
}

TEST(Base_String, IndexOfLastOccurenceOf_StringWithAGivenSubStringAndCharacterPointerWithThatSubString_ReturnsCorrectIndex)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A This Test.");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->indexOfLastOccurenceOf("This"), 16);
}
