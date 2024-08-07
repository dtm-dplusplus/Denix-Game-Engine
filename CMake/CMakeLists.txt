cmake_minimum_required(VERSION 3.17)
project(DenixEngine)

# Define the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/DenixEngine)
set(OUTPUT_SOURCE_DIR ${CMAKE_BINARY_DIR}/DenixEngine)
message(STATUS ${SOURCE_DIR})
message(STATUS ${OUTPUT_SOURCE_DIR})

# Copy the source files to the destination directory
# This ensures cmake source is not modifiable
file(COPY ${SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Get all source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        ${OUTPUT_SOURCE_DIR}/src/*.cpp
        ${OUTPUT_SOURCE_DIR}/src/*.h
)

# Set C++ Include Directories
include_directories(${PROJECT_NAME}
        ${OUTPUT_SOURCE_DIR}/src
        ${OUTPUT_SOURCE_DIR}/contrib/spdlog/include
        ${OUTPUT_SOURCE_DIR}/contrib/sdl3/include
        ${OUTPUT_SOURCE_DIR}/contrib/glew/include
        ${OUTPUT_SOURCE_DIR}/contrib/glm
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add linker libs
target_link_libraries(${PROJECT_NAME}
        SDL3.lib
        #glew32.lib
)
target_link_directories(${PROJECT_NAME}
        PRIVATE "${OUTPUT_SOURCE_DIR}/contrib/sdl3/lib"
        #PRIVATE "${OUTPUT_SOURCE_DIR}/contrib/glew/lib"
)
# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# Add precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE DenixEngine/src/depch.h)

# Add a post-build event
#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD COMMAND
#            XCOPY /y "${OUTPUT_SOURCE_DIR}/contrib/sdl3/bin/SDL3.dll" "$$(OutDir)"
#)

# Use CMAKE_BINARY_DIR to specify the output directory for the executable
#set_target_properties(${PROJECT_NAME} PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#)
