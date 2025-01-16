cmake_minimum_required(VERSION 3.6)

# Removes d postfix so lib file names link - Should probably fix this to allow debug postfix
set(CMAKE_DEBUG_POSTFIX "")

# Create Engine Project
project(DevProject)

# Use solution folders for organisation
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Define the source directory for the project
# In the future it would be better to copy all source to the build project
set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
set(PROJECT_FILE ${PROJECT_DIR}/${PROJECT_NAME}.proj)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR})
set(PROJECT_DIR_OUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(PROJECT_CONTENT_DIR ${PROJECT_DIR}/Content)
set(PROJECT_CONTENT_DIR_OUT ${PROJECT_DIR_OUT}/Content)

set(ENGINE_NAME DenixEngine)
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/${ENGINE_NAME})
set(ENGINE_SOURCE_DIR ${ENGINE_DIR}/src)
set(ENGINE_CONTRIB_DIR ${ENGINE_DIR}/contrib)
set(ENGINE_CONTENT_DIR ${ENGINE_DIR}/res)
set(ENGINE_CONTENT_DIR_OUT ${PROJECT_DIR_OUT}/Content/Engine)

# Print out the project directories
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Project Directory: ${PROJECT_DIR}")
message(STATUS "Project File: ${PROJECT_FILE}")
message(STATUS "Project Source Directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "Project Content Directory: ${PROJECT_CONTENT_DIR}")
message(STATUS "Project Out Directory: ${PROJECT_DIR_OUT}")
message(STATUS "Project Content Directory Out: ${PROJECT_CONTENT_DIR_OUT}")

message(STATUS "Engine Directory: ${ENGINE_DIR}")
message(STATUS "Engine Source Directory: ${ENGINE_SOURCE_DIR}")
message(STATUS "Engine Contrib Directory: ${ENGINE_CONTRIB_DIR}")
message(STATUS "Engine Content Directory: ${ENGINE_CONTENT_DIR}")
message(STATUS "Engine Content Directory Out: ${ENGINE_CONTENT_DIR_OUT}")

# Create ${ENGINE_NAME} Library
add_subdirectory(${CMAKE_SOURCE_DIR}/${ENGINE_NAME})

# Copy Project & Engine files to the runtime directory
file(COPY ${PROJECT_CONTENT_DIR}/ DESTINATION ${PROJECT_CONTENT_DIR_OUT})
file(COPY ${PROJECT_FILE} DESTINATION ${PROJECT_DIR_OUT})
file(COPY ${PROJECT_DIR}/Config/ DESTINATION ${PROJECT_DIR_OUT}/Config)
file(COPY ${ENGINE_CONTENT_DIR}/ DESTINATION ${ENGINE_CONTENT_DIR_OUT})

# Get Game project source files
file(GLOB_RECURSE PROJECT_SOURCE_FILES CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.h
)

# Set Game Project include directories
include_directories(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/src
        ${ENGINE_SOURCE_DIR}
        ${ENGINE_CONTRIB_DIR}/spdlog/include
        ${ENGINE_CONTRIB_DIR}/sdl3/include
        ${ENGINE_CONTRIB_DIR}/glew/include
        ${ENGINE_CONTRIB_DIR}/glm
        ${ENGINE_CONTRIB_DIR}/imgui
        ${ENGINE_CONTRIB_DIR}/assimp/include
        ${ENGINE_CONTRIB_DIR}/stb
        ${ENGINE_CONTRIB_DIR}/yaml-cpp/include
        ${ENGINE_CONTRIB_DIR}/OpenAL/include
        ${ENGINE_CONTRIB_DIR}/physx/include
)

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create Project Executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
add_dependencies(${PROJECT_NAME} ${ENGINE_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin/debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin/release
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin/debug
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin/release
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin/debug
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin/release
)

# Create Project Solution Folder
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${PROJECT_NAME})

# Define linker lib dirs
target_link_directories(${PROJECT_NAME}
        PUBLIC $<TARGET_LINKER_FILE_DIR:${ENGINE_NAME}>
        PUBLIC $<TARGET_LINKER_FILE_DIR:Imgui>
        PUBLIC ${ENGINE_CONTRIB_DIR}/glew/lib
        PUBLIC ${ENGINE_CONTRIB_DIR}/sdl3/bin
        PUBLIC ${ENGINE_CONTRIB_DIR}/assimp/bin/${CMAKE_BUILD_TYPE}
        PUBLIC ${ENGINE_CONTRIB_DIR}/OpenAL/libs/Win64
        PUBLIC  $<$<CONFIG:Debug>:${ENGINE_CONTRIB_DIR}/physx/bin/debug>
                 $<$<CONFIG:Release>:${ENGINE_CONTRIB_DIR}/physx/bin/release>
)

# Define linker libs
target_link_libraries(${PROJECT_NAME}
        PUBLIC ${ENGINE_NAME}.lib
        PUBLIC opengl32.lib
        PUBLIC OpenAL32.lib
        PUBLIC SDL3.lib
        PUBLIC glew32.lib
        PUBLIC $<TARGET_LINKER_FILE_NAME:Imgui>

        PUBLIC  $<$<CONFIG:Debug>:${ENGINE_CONTRIB_DIR}/assimp/bin/Debug/assimp-vc143-mtd.lib>
                $<$<CONFIG:Release>:${ENGINE_CONTRIB_DIR}/assimp/bin/Release/assimp-vc143-mt.lib>
        
        PUBLIC  $<$<CONFIG:Debug>:${ENGINE_CONTRIB_DIR}/yaml-cpp/bin/Debug/yaml-cppd.lib>
                $<$<CONFIG:Release>:${ENGINE_CONTRIB_DIR}/yaml-cpp/bin/Release/yaml-cpp.lib>

        PUBLIC PhysX_64
        PUBLIC PhysXCommon_64
        PUBLIC PhysXFoundation_64
        PUBLIC PhysXExtensions_static_64
        PUBLIC PhysXPvdSDK_static_64
        PUBLIC SceneQuery_static_64
        PUBLIC PhysXCharacterKinematic_static_64
        PUBLIC PVDRuntime_64
)

# Enable Incremental linking
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
)

# Add Post build commands for dependencies
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<$<CONFIG:Debug>:${ENGINE_CONTRIB_DIR}/SDL3/bin/Debug/SDL3.dll>
        $<$<CONFIG:Release>:${ENGINE_CONTRIB_DIR}/SDL3/bin/Release/SDL3.dll> $<TARGET_FILE_DIR:${PROJECT_NAME}>


        COMMAND ${CMAKE_COMMAND} -E copy
        $<$<CONFIG:Debug>:${ENGINE_CONTRIB_DIR}/assimp/bin/Debug/assimp-vc143-mtd.dll>
        $<$<CONFIG:Release>:${ENGINE_CONTRIB_DIR}/assimp/bin/Release/assimp-vc143-mt.dll> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        
       COMMAND ${CMAKE_COMMAND} -E copy
       $<$<CONFIG:Debug>:${ENGINE_CONTRIB_DIR}/yaml-cpp/bin/Debug/yaml-cppd.dll>
       $<$<CONFIG:Release>:${ENGINE_CONTRIB_DIR}/yaml-cpp/bin/Release/yaml-cpp.dll> $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ENGINE_CONTRIB_DIR}/glew/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<$<CONFIG:Debug>:${ENGINE_CONTRIB_DIR}/physx/bin/debug>
        $<$<CONFIG:Release>:${ENGINE_CONTRIB_DIR}/physx/bin/release> $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ENGINE_CONTRIB_DIR}/OpenAL/bin/Win64 $<TARGET_FILE_DIR:${PROJECT_NAME}>

        #COMMAND ${CMAKE_COMMAND} -E echo "Launching Nvidia PVD3"
        #COMMAND ${CMAKE_COMMAND} -E start ${ENGINE_CONTRIB_DIR}/physx/PVD3/PVD3.exe

)
