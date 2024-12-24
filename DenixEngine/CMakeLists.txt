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
        ${SOURCE_DIR}/contrib/assimp/include
        ${SOURCE_DIR}/contrib/stb
        ${SOURCE_DIR}/contrib/yaml-cpp/include
        ${SOURCE_DIR}/contrib/OpenAL/include
)

# Create the Engine library
add_library(DenixEngine STATIC ${SOURCE_FILES})

# Add Precompiled Header - We Can add multiple precompiled headers
target_precompile_headers(DenixEngine PUBLIC ${SOURCE_DIR}/src/depch.h)


# Set solution folder for DenixEngine
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER DenixEngine)

# Set dependencies for DenixEngine
add_dependencies(${PROJECT_NAME} Imgui)
#add_dependencies(${PROJECT_NAME} SDL3::SDL3)
#add_dependencies(${PROJECT_NAME} assimp)
#add_dependencies(${PROJECT_NAME} yaml-cpp::yaml-cpp)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")