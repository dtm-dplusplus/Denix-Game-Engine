project(DenixEngine)

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
set_property(TARGET Imgui PROPERTY FOLDER DenixEngine/Depend/Imgui)

# Create SDL3 Library
add_subdirectory(${CMAKE_SOURCE_DIR}/DenixEngine/contrib/SDL3)
set_property(TARGET SDL3-shared PROPERTY FOLDER DenixEngine/Depend/SDL3)
set_property(TARGET SDL_uclibc PROPERTY FOLDER DenixEngine/Depend/SDL3)

# Create Assimp Library
add_subdirectory(${CMAKE_SOURCE_DIR}/DenixEngine/contrib/assimp)
set_property(TARGET assimp PROPERTY FOLDER DenixEngine/Depend/Assimp)
set_property(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTY FOLDER DenixEngine/Depend/Assimp)
set_property(TARGET zlibstatic PROPERTY FOLDER DenixEngine/Depend/Assimp)

# Create Yaml-cpp Library
add_subdirectory(${CMAKE_SOURCE_DIR}/DenixEngine/contrib/yaml-cpp)
set_property(TARGET yaml-cpp PROPERTY FOLDER DenixEngine/Depend/Yaml-cpp)
set_property(TARGET yaml-cpp-parse PROPERTY FOLDER DenixEngine/Depend/Yaml-cpp)
set_property(TARGET yaml-cpp-read PROPERTY FOLDER DenixEngine/Depend/Yaml-cpp)
set_property(TARGET yaml-cpp-sandbox PROPERTY FOLDER DenixEngine/Depend/Yaml-cpp)
set_property(TARGET uninstall PROPERTY FOLDER DenixEngine/Depend/Yaml-cpp)
set_property(TARGET format PROPERTY FOLDER DenixEngine/Depend/Yaml-cpp)

# Get all source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        ${SOURCE_DIR}/src/*.cpp
        ${SOURCE_DIR}/src/*.h
)

# Set C++ Include Directories
include_directories(${PROJECT_NAME}
        ${SOURCE_DIR}/src
        ${SOURCE_DIR}/contrib/spdlog/include
        ${SOURCE_DIR}/contrib/SDL3/include
        ${SOURCE_DIR}/contrib/glew/include
        ${SOURCE_DIR}/contrib/glm
        ${SOURCE_DIR}/contrib/imgui
        ${OUTPUT_SOURCE_DIR}/contrib/assimp/include # Temp fix as config.h generated in binary dir
        ${SOURCE_DIR}/contrib/stb
        ${SOURCE_DIR}/contrib/yaml-cpp/include
)

# Create the Engine library
add_library(DenixEngine STATIC ${SOURCE_FILES})

# Set solution folder for DenixEngine
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER DenixEngine)

# Set dependencies for DenixEngine
add_dependencies(${PROJECT_NAME} Imgui)
add_dependencies(${PROJECT_NAME} SDL3::SDL3)
add_dependencies(${PROJECT_NAME} assimp)
add_dependencies(${PROJECT_NAME} yaml-cpp::yaml-cpp)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")