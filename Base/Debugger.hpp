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

#pragma once

#include "Base/Uncopyable.hpp"

#include <cstdio>

// -- Macro used to stop the app in the debugger if a certain condition is met, and only in DEBUG builds.
#if DEBUG
#define NXA_LAUNCH_DEBUGGER_IF(condition) do { if (condition) { ::printf("Entering the Debugger.\n"); NxA::Debugger::launch(); }} while(0)
#else
#define NXA_LAUNCH_DEBUGGER_IF(condition, ...) do { } while (0)
#endif

namespace NxA {
    class Debugger : public Uncopyable {
    public:
        #pragma mark Constructors & Destructors
        Debugger() = delete;

        #pragma mark Class Methods
        static bool launch(void);
    };
}
