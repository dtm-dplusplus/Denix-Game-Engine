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

# Create Imgui Library
add_library(Imgui STATIC
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imconfig.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_demo.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_draw.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_internal.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_tables.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_widgets.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imstb_rectpack.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imstb_textedit.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imstb_truetype.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/misc/cpp/imgui_stdlib.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/misc/cpp/imgui_stdlib.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_sdl3.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_sdl3.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_opengl3.cpp
)
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
)



add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} Imgui)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND}
#      XCOPY /y "$(SolutionDir)DenixEngine\\contrib\\SDL3\\bin\\SDL3.dll" "$(OutDir)"
#)

#add_custom_command(
#        TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} XCOPY /y "$$(SolutionDir)DenixEngine\\contrib\\SDL3\\bin\\SDL3.dll" "$$(OutDir)"
#)

# Add linker libs
target_link_libraries(${PROJECT_NAME}
        PUBLIC SDL3.lib
        PUBLIC opengl32.lib
        PUBLIC glew32.lib
        PUBLIC Imgui
)

target_link_directories(${PROJECT_NAME}
        PUBLIC ${OUTPUT_SOURCE_DIR}/contrib/sdl3/lib
        PUBLIC ${OUTPUT_SOURCE_DIR}/contrib/glew/lib
)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

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
