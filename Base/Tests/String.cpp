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

#include "Base/String.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Base_String_Tests);

static const byte utf16String[] = {
    0x00, 0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 0x23, 0x00, 0x24, 0x00, 0x25, 0x00, 0x26, 0x00, 0x27, 0x00, 0x28,
    0x00, 0x29, 0x00, 0x2A, 0x00, 0x2B, 0x00, 0x2C, 0x00, 0x2D, 0x00, 0x2E, 0x00, 0x2F, 0x00, 0x30, 0x00, 0x31,
    0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00, 0x35, 0x00, 0x36, 0x00, 0x37, 0x00, 0x38, 0x00, 0x39, 0x00, 0x3A,
    0x00, 0x3B, 0x00, 0x3C, 0x00, 0x3D, 0x00, 0x3E, 0x00, 0x3F, 0x00, 0x40, 0x00, 0x41, 0x00, 0x42, 0x00, 0x43,
    0x00, 0x44, 0x00, 0x45, 0x00, 0x46, 0x00, 0x47, 0x00, 0x48, 0x00, 0x49, 0x00, 0x4A, 0x00, 0x4B, 0x00, 0x4C,
    0x00, 0x4D, 0x00, 0x4E, 0x00, 0x4F, 0x00, 0x50, 0x00, 0x51, 0x00, 0x52, 0x00, 0x53, 0x00, 0x54, 0x00, 0x55,
    0x00, 0x56, 0x00, 0x57, 0x00, 0x58, 0x00, 0x59, 0x00, 0x5A, 0x00, 0x5B, 0x00, 0x5C, 0x00, 0x5D, 0x00, 0x5E,
    0x00, 0x5F, 0x00, 0x60, 0x00, 0x61, 0x00, 0x62, 0x00, 0x63, 0x00, 0x64, 0x00, 0x65, 0x00, 0x66, 0x00, 0x67,
    0x00, 0x68, 0x00, 0x69, 0x00, 0x6A, 0x00, 0x6B, 0x00, 0x6C, 0x00, 0x6D, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x70,
    0x00, 0x71, 0x00, 0x72, 0x00, 0x73, 0x00, 0x74, 0x00, 0x75, 0x00, 0x76, 0x00, 0x77, 0x00, 0x78, 0x00, 0x79,
    0x00, 0x7A, 0x00, 0x7B, 0x00, 0x7C, 0x00, 0x7D, 0x00, 0x7E
};

constexpr const character *utf8String = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

TEST(Base_String, ClassName_AString_ClassNameIsReturnedCorrectly)
{
    // -- Given.
    auto test = String::string();

    // -- When.
    auto name = test->className();

    // -- Then.
    ASSERT_STREQ("NxA::String", name);
}

TEST(Base_String, ClassName_TwoStrings_ClassNameIsActuallyTheSamePointer)
{
    // -- Given.
    auto test = String::string();
    auto other = String::string();

    // -- When.
    auto name = test->className();
    auto otherName = other->className();

    // -- Then.
    ASSERT_EQ(otherName, name);
}

TEST(Base_String, StringContructorAndToUTF8_StringCreatedFromCharPointer_ToUTF8ReturnsCorrectValue)
{
    // -- Given.
    // -- When.
    auto test = String::stringWith(utf8String);

    // -- Then.
    ASSERT_STREQ(utf8String, test->toUTF8());
}

TEST(Base_String, StringContructorAndToUTF8_StringCreatedFromOtherString_ToUTF8ReturnsContainsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    auto otherTest = String::stringWith(test);

    // -- Then.
    ASSERT_STREQ(utf8String, otherTest->toUTF8());
}

TEST(Base_String, StringWithUTF16_StringCreatedFromUTF16String_ContainsCorrectValue)
{
    // -- Given.
    auto blob = Blob::blobWithMemoryAndSize(utf16String, sizeof(utf16String));

    // -- When.
    auto test = String::stringWithUTF16(blob);
    
    // -- Then.
    ASSERT_STREQ(utf8String, test->toUTF8());
}

TEST(Base_String, Description_StringWithAValue_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    // -- Then.
    ASSERT_STREQ(utf8String, test->description()->toUTF8());
}

TEST(Base_String, OperatorEqual_TwoEqualStrings_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);
    auto test2 = String::stringWith(utf8String);

    // -- When.
    // -- Then.
    ASSERT_TRUE(test == test2);
}

TEST(Base_String, OperatorNotEqual_TwoUnequalStrings_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);
    auto test2 = String::stringWith(utf8String);
    test2->append("more");

    // -- When.
    // -- Then.
    ASSERT_TRUE(test != test2);
}

TEST(Base_String, OperatorNotEqual_TwoEqualStrings_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith(utf8String);
    auto test2 = String::stringWith(utf8String);

    // -- When.
    // -- Then.
    ASSERT_FALSE(test != test2);
}

TEST(Base_String, OperatorEqual_TwoUnequalStrings_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith(utf8String);
    auto test2 = String::stringWith(utf8String);
    test2->append("more");

    // -- When.
    // -- Then.
    ASSERT_FALSE(test == test2);
}

TEST(Base_String, OperatorEqual_AStringAndAnEqualCharacterPointer_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test == utf8String);
}

TEST(Base_String, OperatorEqual_AStringAndAnUnequalCharacterPointer_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith(utf8String);
    test->append("more");

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == utf8String);
}

TEST(Base_String, OperatorNotEqual_AStringAndAnEqualCharacterPointer_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test != utf8String);
}

TEST(Base_String, OperatorNotEqual_AStringAndAnUnequalCharacterPointer_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);
    test->append("more");

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != utf8String);
}

TEST(Base_String, OperatorEqual_ACharacterPointerAndAnEqualString_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    // -- Then.
    ASSERT_TRUE(utf8String == *test);
}

TEST(Base_String, OperatorEqual_ACharacterPointerAndAnUnequalString_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith(utf8String);
    test->append("more");

    // -- When.
    // -- Then.
    ASSERT_FALSE(utf8String == *test);
}

TEST(Base_String, OperatorNotEqual_ACharacterPointerAndAnEqualString_ReturnsFalse)
{
    // -- Given.
    auto test = String::stringWith(utf8String);

    // -- When.
    // -- Then.
    ASSERT_FALSE(utf8String != *test);
}

TEST(Base_String, OperatorNotEqual_ACharacterPointerAndAnUnequalString_ReturnsTrue)
{
    // -- Given.
    auto test = String::stringWith(utf8String);
    test->append("more");

    // -- When.
    // -- Then.
    ASSERT_TRUE(utf8String != *test);
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
    ASSERT_EQ(0, ::memcmp(utf16String, result->data(), sizeof(utf16String)));
}

TEST(Base_String, Append_AStringToAString_ReturnsCorrectValue)
{
    // -- Given.
    auto test1 = String::stringWith("Hello This ");
    auto test2 = String::stringWith("Is A Test.");

    // -- When.
    test1->append(test2);

    // -- Then.
    ASSERT_STREQ("Hello This Is A Test.", test1->toUTF8());
}

TEST(Base_String, Append_AStringToACharacterPointer_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This ");

    // -- When.
    test->append("Is A Test.");

    // -- Then.
    ASSERT_STREQ("Hello This Is A Test.", test->toUTF8());
}

TEST(Base_String, SplitBySeperator_StringSeperatedBySpacesSpaces_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->splitBySeperator(' ');

    // -- Then.
    ASSERT_EQ(result->length(), 5);
    ASSERT_EQ("Hello", (*result)[0]);
    ASSERT_EQ("This", (*result)[1]);
    ASSERT_EQ("Is", (*result)[2]);
    ASSERT_EQ("A", (*result)[3]);
    ASSERT_EQ("Test.", (*result)[4]);
}

TEST(Base_String, Substring_FromAnIndex_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(6);

    // -- Then.
    ASSERT_EQ("This Is A Test.", *result);
}

TEST(Base_String, Substring_FromAnIndex_UTFReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Ἀριστοτέλης");
    
    // -- When.
    auto result = test->utfSeek(6);
    
    // -- Then.
    ASSERT_EQ("τέλης", *result);
}

TEST(Base_String, Substring_FromAnIndexAtTheEndOfTheString_ReturnsEmptyString)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(21);

    // -- Then.
    ASSERT_EQ("", *result);
}

TEST(Base_String, Substring_FromAnIndexPastTheEndOfTheString_ReturnsEmptyString)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(245);

    // -- Then.
    ASSERT_EQ("", *result);
}

TEST(Base_String, Substring_FromAnIndexToAnother_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(6, 10);

    // -- Then.
    ASSERT_EQ("This", *result);
}

TEST(Base_String, Substring_FromAnIndexToAnotherAtTheEndOfTheString_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(6, 21);

    // -- Then.
    ASSERT_EQ("This Is A Test.", *result);
}

TEST(Base_String, Substring_FromAnIndexToAnotherPastTheEndOfTheString_ReturnsCorrectValue)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(6, 234);

    // -- Then.
    ASSERT_EQ("This Is A Test.", result);
}

TEST(Base_String, Substring_FromAnIndexAndToAnotherBothPastTheEndOfTheString_ReturnsEmptyString)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When.
    auto result = test->subString(245, 255);

    // -- Then.
    ASSERT_EQ("", *result);
}

TEST(Base_String, Substring_FromAnIndexAndToAnotherInInverserOrderTheEndOfTheString_ReturnsEmptyString)
{
    // -- Given.
    auto test = String::stringWith("Hello This Is A Test.");

    // -- When/Then.
    ASSERT_THROW(test->subString(244, 25), NxA::AssertionFailed);
}

TEST(Base_String, LowerCaseString_StringWithUpperCaseCharacters_ReturnsOneWithLowerCaseUTFCharacters)
{
    // -- Given.
    auto test = String::stringWith("Mp3 grüßEN");
    
    // -- When.
    auto result = test->lowerCaseString();
    
    // -- Then.
    ASSERT_STREQ("mp3 grüßen", result->toUTF8());
}

TEST(Base_String, UpperCaseString_StringWithLowerCaseCharacters_ReturnsOneWithUpperCaseUTFCharacters)
{
    // -- Given.
    auto test = String::stringWith("Mp3 grüßENß");
    
    // -- When.
    auto result = test->upperCaseString();
    
    // -- Then.
    ASSERT_STREQ("MP3 GRÜSSENSS", result->toUTF8());
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

TEST(Base_String, LengthOf_ConstCharacterPointer_ReturnsCorrectLength)
{
    // -- Given.
    auto test = "123456789";

    // -- When.
    // -- Then.
    ASSERT_EQ(String::lengthOf(test), 9);
}

TEST(Base_String, HashFor_ConstCharacterPointerAndStringPointer_AreEqual)
{
    // -- Given.
    auto testStr = String::stringWith("Hello");
    auto testPtr = "Hello";

    // -- When.
    // -- Then.
    ASSERT_EQ(String::hashFor(testPtr), testStr->hash());
}

TEST(Base_String, StaticClassHash_AString_ReturnsExpectedValue)
{
    // -- Given.
    // -- When.
    // -- Then.
    ASSERT_EQ(String::staticClassHash(), String::hashFor("NxA::String"));
}

TEST(Base_String, ClassHash_AString_ReturnsExpectedValue)
{
    // -- Given.
    auto test = String::stringWith("Hello");

    // -- When.
    // -- Then.
    ASSERT_EQ(test->classHash(), String::staticClassHash());
}

TEST(Base_String, IntegerValue_AStringWithAnInteger_ReturnsCorrectValue)
{
    // -- Given.
    auto testStr = String::stringWith("1345");

    // -- When.
    // -- Then.
    ASSERT_EQ(testStr->integerValue(), 1345);
}

TEST(Base_String, IntegerValue_AStringWithANegativeInteger_ReturnsCorrectValue)
{
    // -- Given.
    auto testStr = String::stringWith("-341345");

    // -- When.
    // -- Then.
    ASSERT_EQ(testStr->integerValue(), -341345);
}

TEST(Base_String, IntegerValue_AStringWithAInvalidInteger_ReturnsPartOfTheNumber)
{
    // -- Given.
    auto testStr = String::stringWith("-341d345");

    // -- When.
    // -- Then.
    ASSERT_EQ(testStr->integerValue(), -341);
}

TEST(Base_String, IntegerValue_AStringWithAInvalidString_ReturnsZero)
{
    // -- Given.
    auto testStr = String::stringWith("d-341d345");

    // -- When.
    // -- Then.
    ASSERT_EQ(testStr->integerValue(), 0);
}
