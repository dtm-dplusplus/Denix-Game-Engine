cmake_minimum_required(VERSION 3.6)

set(CMAKE_DEBUG_POSTFIX "")

# Create Development Project
project(DevProject)

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/yaml-cpp/include
)



# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create Project Executable
add_executable(${PROJECT_NAME} ${GEN_SOURCE_FILES})
add_dependencies(${PROJECT_NAME} DenixEngine)

# Create Project Solution Folder
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${PROJECT_NAME})

# Set Startup Project
set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Define linker libs
target_link_libraries(${PROJECT_NAME}
        PUBLIC DenixEngine.lib
        PUBLIC opengl32.lib
        PUBLIC SDL3::SDL3
        PUBLIC glew32.lib
        PUBLIC $<TARGET_LINKER_FILE_NAME:Imgui>
        PUBLIC  $<TARGET_LINKER_FILE_NAME:assimp>
        PUBLIC
        $<$<CONFIG:Debug>:$<TARGET_LINKER_FILE_NAME:yaml-cpp::yaml-cpp>>
        $<$<CONFIG:Release>:$<TARGET_LINKER_FILE_NAME:yaml-cpp::yaml-cpp>>
)

# Define linker lib dirs
target_link_directories(${PROJECT_NAME}
        PUBLIC $<TARGET_LINKER_FILE_DIR:DenixEngine>
        PUBLIC $<TARGET_LINKER_FILE_DIR:Imgui>
        PUBLIC ${CMAKE_BINARY_DIR}/DenixEngine/contrib/glew/lib
        PUBLIC $<TARGET_LINKER_FILE_DIR:assimp>
        PUBLIC $<TARGET_LINKER_FILE_DIR:yaml-cpp>
)

# Add Post build commands for dependencies
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:assimp> $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:yaml-cpp::yaml-cpp> $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/DenixEngine/contrib/glew/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()



# Then in some target add this to put the target in the Development folder.

