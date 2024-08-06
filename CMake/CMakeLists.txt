cmake_minimum_required(VERSION 3.4)
project(DenixEngine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/DenixEngine/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DenixEngine/src/*.h"
)

# Set C++ Include Directories
include_directories(${PROJECT_NAME}
        DenixEngine/src
        DenixEngine/contrib/spdlog/include
)

# Define the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/DenixEngine)
message(STATUS ${SOURCE_DIR})

#Define output source directory
set(OUTPUT_SOURCE_DIR ${CMAKE_BINARY_DIR})
message(STATUS ${OUTPUT_SOURCE_DIR})

# Copy the source files to the destination directory
file(COPY ${SOURCE_DIR} DESTINATION ${OUTPUT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# Add precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE DenixEngine/src/depch.h)

# Use CMAKE_BINARY_DIR to specify the output directory for the executable
#set_target_properties(${PROJECT_NAME} PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#)