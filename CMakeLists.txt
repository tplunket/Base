cmake_minimum_required (VERSION 3.7) # it may work with older versions but that needs to be tested.

project (Base)

# These following lines are passed through @varname@ blocks when using `configure_file` to generate
# new files.
set (Base_VERSION_MAJOR 1)
set (Base_VERSION_MINOR 0)

file (GLOB SRCS *.cpp)  # alternatively we could "set ( SRCS filea fileb filec )"
file (GLOB HDRS *.hpp)

file (GLOB INTSRC Internal/*)

source_group("Source Files" FILES ${SRCS} ${HDRS})
source_group("Source Files\\Internal" FILES ${INTSRC})

include_directories(${CMAKE_SOURCE_DIR}/..) # all #includes are <Base/...>
add_library (Base STATIC ${SRCS} ${HDRS} ${INTSRC})
target_include_directories(Base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..) # all #includes are <Base/...>
add_subdirectory (Tests)

