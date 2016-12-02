# Base
A simple, lightweight &amp; cross-platform base library.

This contains basic `String`, `File`, `Array`, `Map`, etc.. Objects are value-types that use the pimpl idiom for simple moving/copying. Some types provide both mutable and non-mutable types which share implementations but allow code to keep references to non-mutable objects without any risk of them being mutated elsewhere.

Basic unit test support using GoogleMock is provided. All classes will have 100% coverage eventually :)

This library is released under the MIT license.

Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
