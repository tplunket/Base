cmake_minimum_required (VERSION 3.7) # it may work with older versions but that needs to be tested.

project (Base)

# These following lines are passed through @varname@ blocks when using `configure_file` to generate
# new files.
set (Base_VERSION_MAJOR 1)
set (Base_VERSION_MINOR 0)

file (GLOB SRCS *.cpp)  # alternatively we could "set ( SRCS filea fileb filec )"
file (GLOB HDRS *.hpp)

source_group("Source Files" FILES ${SRCS} ${HDRS})

include_directories(${CMAKE_SOURCE_DIR}/..) # all #includes are <Base/...>
add_library (Base STATIC ${SRCS} ${HDRS})


