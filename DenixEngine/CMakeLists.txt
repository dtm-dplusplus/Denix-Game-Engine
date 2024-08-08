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

# Create Imgui Library
add_subdirectory(${CMAKE_SOURCE_DIR}/DenixEngine/contrib/imgui)

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
        ${OUTPUT_SOURCE_DIR}/contrib/imgui
        ${OUTPUT_SOURCE_DIR}/contrib/assimp/include
        ${OUTPUT_SOURCE_DIR}/contrib/stb
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} Imgui)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND}
#      XCOPY /y "$(SolutionDir)DenixEngine\\contrib\\SDL3\\bin\\SDL3.dll" "$(OutDir)"
#)

#[[add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} XCOPY /y "$$(SolutionDir)DenixEngine\\contrib\\SDL3\\bin\\SDL3.dll" "$$(OutDir)"
)]]

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Add precompiled header
#target_precompile_headers(${PROJECT_NAME} PRIVATE DenixEngine/src/depch.h)

# Add a post-build event
#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD COMMAND
#            XCOPY /y "${OUTPUT_SOURCE_DIR}/contrib/sdl3/bin/SDL3.dll" "$$(OutDir)"
#)

# Use CMAKE_BINARY_DIR to specify the output directory for the executable
#set_target_properties(${PROJECT_NAME} PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#)