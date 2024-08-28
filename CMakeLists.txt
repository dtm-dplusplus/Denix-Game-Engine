cmake_minimum_required(VERSION 3.17)

# Create Development Project
project(DevProject)

# Define Solution Output Directory
set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

# Define the source directory
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
set(GEN_PROJECT_SOURCE_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

# Create Engine Project
add_subdirectory(${CMAKE_SOURCE_DIR}/DenixEngine)


# Copy source files to the destination directory
# This ensures cmake source is not modifiable
file(COPY ${PROJECT_SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Set C++ Include Directories
file(GLOB_RECURSE GEN_SOURCE_FILES CONFIGURE_DEPENDS
        ${GEN_PROJECT_SOURCE_DIR}/src/*.cpp
        ${GEN_PROJECT_SOURCE_DIR}/src/*.h
)

include_directories(${PROJECT_NAME}
        ${GEN_PROJECT_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}/DenixEngine/src
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/spdlog/include
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/sdl3/include
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/glew/include
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/glm
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/imgui
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/assimp/include
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/stb
)



# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create Executable
add_executable(${PROJECT_NAME} ${GEN_SOURCE_FILES})
add_dependencies(${PROJECT_NAME} DenixEngine)

# Add linker libs
target_link_libraries(${PROJECT_NAME}
        PUBLIC DenixEngine.lib
        PUBLIC opengl32.lib
        PUBLIC SDL3.lib
        PUBLIC glew32.lib
        PUBLIC Imgui.lib
        PUBLIC assimp-vc143-mt.lib
)

target_link_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_BINARY_DIR}/bin/DenixEngine
        PUBLIC ${CMAKE_BINARY_DIR}/DenixEngine/contrib/sdl3/lib
        PUBLIC ${CMAKE_BINARY_DIR}/DenixEngine/contrib/glew/lib
        PUBLIC ${CMAKE_BINARY_DIR}/DenixEngine/contrib/assimp/lib
)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

