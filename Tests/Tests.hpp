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

#pragma once

#include <Base/Test.hpp>

// -- This forces the linker to link the object files where these tests
// -- are defined. Otherwise they would get stripped out.
NXA_USING_TEST_SUITE_NAMED(Base_String_Tests);
NXA_USING_TEST_SUITE_NAMED(Base_Blob_Tests);
NXA_USING_TEST_SUITE_NAMED(Base_Array_Tests);
NXA_USING_TEST_SUITE_NAMED(Base_Map_Tests);
NXA_USING_TEST_SUITE_NAMED(Base_Set_Tests);

NXA_USE_TEST_SUITES_FOR_MODULE(Base) {
    Base_String_Tests,
    Base_Blob_Tests,
    Base_Set_Tests,
    Base_Array_Tests,
    Base_Map_Tests
};
