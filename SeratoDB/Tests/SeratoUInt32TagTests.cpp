//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#include <GoogleMock/GoogleMock.h>

#include "SeratoDB/MakeXcodeUseCppTests.hpp"
#include "SeratoDB/SeratoUint32Tag.h"

using namespace testing;
using namespace NxA;

XCODE_CPP_TEST_NAMED(SeratoUInt32Tests);

TEST(SeratoUInt32, TagCreatedFromNormalConstructor_HasCorrectValue)
{
    // -- When.
    SeratoUInt32Tag tag('vrsn', 0xDEADBEEF);

    // -- Then.
    ASSERT_EQ(tag.value(), 0xDEADBEEF);
}
