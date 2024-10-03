cmake_minimum_required(VERSION 3.6)

# Removes d postfix so lib file names link - Should probably fix this to allow debug postfix
set(CMAKE_DEBUG_POSTFIX "")

# Create the DenixEngine Project
# In future builds the name will be passed as an argument to the cmake file
project(DevProject)

# Use solution folders for organisation
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Define the source directory for the project
# In the future it would be better to copy all source to the build project
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})

# Create DenixEngine Library
add_subdirectory(${CMAKE_SOURCE_DIR}/DenixEngine)

# Copy resource files to the destination directory
# The engine uses the root of the runtime directory to scan for assets
# Unlike source files which are linked via the solution
file(COPY ${PROJECT_SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Set C++ Include Directories
file(GLOB_RECURSE PROJECT_SOURCE_FILES CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.h
)

include_directories(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/src
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
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
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
