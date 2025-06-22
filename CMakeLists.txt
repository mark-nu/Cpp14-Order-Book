cmake_minimum_required(VERSION 3.10)
project(MyApp)

# Set C++ standard if provided
if(DEFINED CXX_STANDARD)
    set(CMAKE_CXX_STANDARD ${CXX_STANDARD})
endif()

# Collect all custom compile flags into CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT_FLAGS} ${DEBUG_FLAGS} ${SAN_FLAGS} ${EXTRA_OPTIONS}")

# Print all flags for debug
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")

# Convert a string of sources to a list (space-separated paths)
separate_arguments(SOURCE_LIST)

# Add executable target
add_executable(${TARGET_NAME} ${SOURCE_LIST})
