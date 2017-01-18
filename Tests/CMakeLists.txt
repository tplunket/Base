cmake_minimum_required (VERSION 3.7) # it may work with older versions but that needs to be tested.

project (BaseTest)

# These following lines are passed through @varname@ blocks when using `configure_file` to generate
# new files.
set (BaseTest_VERSION_MAJOR 1)
set (BaseTest_VERSION_MINOR 0)

file (GLOB SRCS *.cpp *.hpp)

source_group("Source Files" FILES ${SRCS})

include_directories(${Base_SOURCE_DIR}/..) # all #includes are <Base/...>
add_executable (BaseTest ${SRCS})
set_property(TARGET BaseTest PROPERTY CXX_STANDARD 17)
set_property(TARGET BaseTest PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries (BaseTest LINK_PUBLIC Base)

