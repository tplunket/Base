#Building boost libraries

This folder contains version 1.59.0 of the boost libraries.

1. Download the unix tar ball (not the ZIP, that has CR/LF line endings and will not work).
2. Un-tar it.
3. Modify the code from file ```boost/libs/filesystem/src/operations.cpp b/boost/libs/filesystem/src/operations.cpp``` at line 1414 so that the fallback code is always executed. Otherwise you will get a linking error telling you that ```fchmodat``` is not available in OS X 10.9.
4. cd to ```boost_1_59_0/```.
5. Copy ```./boost``` to ```Vendor/boost/boost```

For OS X:

1. Run ```./bootstrap.sh toolset=clang```.
2. Run ```./b2 toolset=clang --without-mpi cxxflags="-arch x86_64 -fvisibility=hidden -fvisibility-inlines-hidden -std=c++14 -stdlib=libc++ -ftemplate-depth=512" linkflags="-stdlib=libc++" link=static stage``` which puts the output libs in ```./stage/lib```.
3. Copy ```./stage/lib``` to ```Vendor/boost/lib/mac_x86_64```.