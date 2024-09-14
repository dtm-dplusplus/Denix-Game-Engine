project(DenixEngine)

# Define Output Directories
set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

# Define the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/DenixEngine)
set(OUTPUT_SOURCE_DIR ${CMAKE_BINARY_DIR}/DenixEngine)

# Copy the source files to the destination directory
# This ensures cmake source is not modifiable
file(COPY ${SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create Imgui Library
add_subdirectory(${CMAKE_SOURCE_DIR}/DenixEngine/contrib/imgui)

# Create SDL3 Library
add_subdirectory(${CMAKE_SOURCE_DIR}/DenixEngine/contrib/SDL3)

# Get all source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        ${OUTPUT_SOURCE_DIR}/src/*.cpp
        ${OUTPUT_SOURCE_DIR}/src/*.h
)

# Set C++ Include Directories
include_directories(${PROJECT_NAME}
        ${OUTPUT_SOURCE_DIR}/src
        ${OUTPUT_SOURCE_DIR}/contrib/spdlog/include
        ${OUTPUT_SOURCE_DIR}/contrib/SDL3/include
        ${OUTPUT_SOURCE_DIR}/contrib/glew/include
        ${OUTPUT_SOURCE_DIR}/contrib/glm
        ${OUTPUT_SOURCE_DIR}/contrib/imgui
        ${OUTPUT_SOURCE_DIR}/contrib/assimp/include
        ${OUTPUT_SOURCE_DIR}/contrib/stb
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/yaml-cpp-master/include
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} Imgui)
add_dependencies(${PROJECT_NAME} SDL3::SDL3)

# Set the solution folder for the target
set_property(TARGET SDL3-shared PROPERTY FOLDER DenixEngine/Depend/SDL3)
set_property(TARGET SDL_uclibc PROPERTY FOLDER DenixEngine/Depend/SDL3)

# Add Yaml CPP Static Def
target_compile_definitions(${PROJECT_NAME} PUBLIC YAML_CPP_STATIC_DEFINE)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Then in some target add this to put the target in the Development folder.
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER DenixEngine)