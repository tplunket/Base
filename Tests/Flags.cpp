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

template <typename FlagsEnum>
FlagsBuilder<FlagsEnum> operator | (FlagsEnum first, FlagsEnum second)
{
    return FlagsBuilder<FlagsEnum>(first) | second;
}

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
    Flags<TestFlag> flags(TestFlag::OneOfThese | TestFlag::SomeOthers);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags);
}

TEST(Base_Flags, HasAny_FlagsWithNothingSet_ReturnsFalse)
{
    // -- Given.
    Flags<TestFlag> flags   ;

    // -- When.
    // -- Then.
    ASSERT_FALSE(flags.hasAny(TestFlag::AndEvenMore));
}

TEST(Base_Flags, HasAny_FlagsWithOneSet_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags.hasAny(TestFlag::AndEvenMore));
}

TEST(Base_Flags, HasAny_FlagsWithOneSetButTestingMultiple_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags.hasAny(TestFlag::AndEvenMore | TestFlag::SomeOthers));
}

TEST(Base_Flags, HasAny_FlagsWithAnotherSet_ReturnsFalse)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::OneOfThese);

    // -- When.
    // -- Then.
    ASSERT_FALSE(flags.hasAny(TestFlag::AndEvenMore));
}


TEST(Base_Flags, HasAny_FlagsWithMultipleSet_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore | TestFlag::OneOfThese);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags.hasAny(TestFlag::AndEvenMore));
}

TEST(Base_Flags, HasAll_FlagsWithNothingSet_ReturnsFalse)
{
    // -- Given.
    Flags<TestFlag> flags;

    // -- When.
    // -- Then.
    ASSERT_FALSE(flags.hasAll(TestFlag::AndEvenMore | TestFlag::SomeOthers));
}

TEST(Base_Flags, HasAll_FlagsWithOneSet_ReturnsFalse)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);

    // -- When.
    // -- Then.
    ASSERT_FALSE(flags.hasAll(TestFlag::AndEvenMore | TestFlag::SomeOthers));
}

TEST(Base_Flags, HasAll_FlagsWithAnotherSet_ReturnsFalse)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::OneOfThese);

    // -- When.
    // -- Then.
    ASSERT_FALSE(flags.hasAll(TestFlag::AndEvenMore | TestFlag::SomeOthers));
}

TEST(Base_Flags, HasAll_FlagsWithMultipleSet_ReturnsTrue)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore | TestFlag::SomeOthers);

    // -- When.
    // -- Then.
    ASSERT_TRUE(flags.hasAll(TestFlag::AndEvenMore | TestFlag::SomeOthers));
}

TEST(Base_Flags, Clear_FlagsWithMultipleSet_ReturnsEmptyFlags)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore | TestFlag::SomeOthers);

    // -- When.
    flags.clear();

    // -- Then.
    ASSERT_FALSE(flags);
}

TEST(Base_Flags, OperatorOr_FlagsWithOneSetOringAnother_ReturnsFlagsWithBothSet)
{
    // -- Given.
    Flags<TestFlag> flags(TestFlag::AndEvenMore);

    // -- When.
    flags |= TestFlag::SomeOthers;

    // -- Then.
    ASSERT_TRUE(flags.hasAll(TestFlag::AndEvenMore | TestFlag::SomeOthers));
}
