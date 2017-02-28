cmake_minimum_required (VERSION 3.6) # it may work with older versions but that would need to be tested.

project (Base)

# These following lines are passed through @varname@ blocks when using `configure_file` to generate new files.
set (Base_VERSION_MAJOR 1)
set (Base_VERSION_MINOR 0)

set (VENDOR_DIR ${CMAKE_SOURCE_DIR}/Vendor)

file (GLOB SRCS *.cpp)  # alternatively we could "set ( SRCS filea fileb filec )"
file (GLOB HDRS *.hpp)

source_group ("Source Files" FILES ${SRCS} ${HDRS})

file (GLOB INTSRC Internal/*)
source_group ("Source Files\\Internal" FILES ${INTSRC})

file (GLOB_RECURSE VENDORFILES Vendor/*.[chpp] Vendor/*.[ch])
source_group ("Vendor Files" FILES ${VENDORFILES})

set (INCDIRS
     ${CMAKE_SOURCE_DIR}/.. # all includes are <Base/...>
     ${VENDOR_DIR}/utf8rewind/include
     ${VENDOR_DIR}/boost
     )
include_directories (${INCDIRS})

add_library (Base STATIC ${SRCS} ${HDRS} ${INTSRC})
target_include_directories (Base PUBLIC ${INCDIRS})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # Required for VS2017 & C++17 (<optional> is unavailable until C++17 in MSVC)
    target_compile_options (Base PRIVATE "/std:c++latest")
    set_property (TARGET Base PROPERTY CXX_STANDARD 17)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set_property (TARGET Base PROPERTY CXX_STANDARD 14)
endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

# auto_ptr is removed in C++17 so prevent its usage in Boost
target_compile_definitions (Base PRIVATE BOOST_NO_AUTO_PTR)
set_property (TARGET Base PROPERTY CXX_STANDARD_REQUIRED ON)

# Make the Tests dir included when building
add_subdirectory (Tests)

