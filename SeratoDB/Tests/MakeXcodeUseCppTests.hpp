//
//  MakeXcodeUseCppTests.hpp
//  Rekord Buddy
//
//  Created by Didier Malenfant on 4/27/15.
//  Copyright (c) 2013-2015 Next Audio Labs, LLC. All rights reserved.
//

#pragma once

#define XCODE_CPP_TEST_NAMED(name) void* name;

#define XCODE_LINK_TO_CPP_TEST(name) extern void* name;

#define XCODE_USE_CPP_TESTS void* UseTheseCppTest[]
