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

#include "Base/Flags.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Base_Flags_Tests);

enum class TestFlag {
    OneOfThese,
    SomeOthers,
    AndEvenMore,

    LastFlag    // -- This is required for static assertions.
};

TEST(Base_Flags, BoolOperator_EmptyFlags_ReturnsFalse)
{
    // -- Given.
    Flags<TestFlag> flags;

    // -- When.
    // -- Then.
    ASSERT_FALSE(flags);
}

TEST(Base_Flags, BoolOperator_FlagsWithOneSet_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::OneOfThese);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags);
}

TEST(Base_Flags, BoolOperator_FlagsWithManySet_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::OneOfThese);
    flags.set(TestFlag::SomeOthers);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags);
}

TEST(Base_Flags, HasAny_FlagsWithNothingSet_ReturnsFalse)
{
    // -- Given.
    Flags<TestFlag> flags;

    // -- When.
    // -- Then.
    ASSERT_FALSE(flags.has(TestFlag::AndEvenMore));
}

TEST(Base_Flags, HasAny_FlagsWithOneSet_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags.has(TestFlag::AndEvenMore));
}

TEST(Base_Flags, HasAny_FlagsWithAnotherSet_ReturnsFalse)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::OneOfThese);

    // -- When.
    // -- Then.
    ASSERT_FALSE(flags.has(TestFlag::AndEvenMore));
}

TEST(Base_Flags, HasAny_FlagsWithMultipleSet_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);
    flags.set(TestFlag::OneOfThese);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags.has(TestFlag::AndEvenMore));
}

TEST(Base_Flags, HasAll_FlagsWithMultipleSet_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);
    flags.set(TestFlag::SomeOthers);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags.has(TestFlag::AndEvenMore));
    ASSERT_TRUE(flags.has(TestFlag::SomeOthers));
}

TEST(Base_Flags, ClearAll_FlagsWithMultipleSet_ReturnsEmptyFlags)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);
    flags.set(TestFlag::SomeOthers);

    // -- When.
    flags.clearAll();

    // -- Then.
    ASSERT_FALSE(flags);
}

TEST(Base_Flags, Set_FlagsWithOneSettingAnother_ReturnsFlagsWithBothSet)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);

    // -- When.
    flags.set(TestFlag::SomeOthers);

    // -- Then.
    ASSERT_TRUE(flags.has(TestFlag::AndEvenMore));
    ASSERT_TRUE(flags.has(TestFlag::SomeOthers));
}

TEST(Base_Flags, AndAlso_FlagsWithOneAndAlsoAnotherViaEnum_ReturnsFlagsWithBothSet)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);

    // -- When.
    auto result = flags.andAlso(TestFlag::SomeOthers);

    // -- Then.
    ASSERT_TRUE(result.has(TestFlag::AndEvenMore));
    ASSERT_TRUE(result.has(TestFlag::SomeOthers));
}

TEST(Base_Flags, AndAlso_FlagsWithOneAndAlsoAnother_ReturnsFlagsWithBothSet)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);
    Flags<TestFlag> otherFlags(TestFlag::SomeOthers);

    // -- When.
    auto result = flags.andAlso(otherFlags);

    // -- Then.
    ASSERT_TRUE(result.has(TestFlag::AndEvenMore));
    ASSERT_TRUE(result.has(TestFlag::SomeOthers));
}

TEST(Base_Flags, Clear_FlagsWithTwoSet_ReturnsFlagsWithOnlyOneSet)
{
    // -- Given.
    auto flags = Flags<TestFlag>(TestFlag::AndEvenMore).andAlso(TestFlag::SomeOthers);

    // -- When.
    flags.clear(TestFlag::AndEvenMore);

    // -- Then.
    ASSERT_FALSE(flags.has(TestFlag::AndEvenMore));
    ASSERT_TRUE(flags.has(TestFlag::SomeOthers));
}

TEST(Base_Flags, ClearAll_FlagsWithTwoSet_ReturnsFlagsWithNothingSet)
{
    // -- Given.
    auto flags = Flags<TestFlag>(TestFlag::AndEvenMore).andAlso(TestFlag::SomeOthers);

    // -- When.
    flags.clearAll();

    // -- Then.
    ASSERT_FALSE(flags.has(TestFlag::AndEvenMore));
    ASSERT_FALSE(flags.has(TestFlag::SomeOthers));
}
