project(DenixEngine)

# Define the source directory
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/DenixEngine)
set(ENGINE_SOURCE_DIR ${ENGINE_DIR}/src)
set(ENGINE_CONTRIB_DIR ${ENGINE_DIR}/contrib)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Create Imgui Library
add_subdirectory(${ENGINE_DIR}/contrib/imgui)
set_property(TARGET Imgui PROPERTY FOLDER Depend/Imgui)

# Get all source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        ${ENGINE_SOURCE_DIR}/*.cpp
        ${ENGINE_SOURCE_DIR}/*.h
)

# Set C++ Include Directories
include_directories(${PROJECT_NAME}
        ${ENGINE_SOURCE_DIR}
        ${ENGINE_CONTRIB_DIR}/spdlog/include
        ${ENGINE_CONTRIB_DIR}/SDL3/include
        ${ENGINE_CONTRIB_DIR}/glew/include
        ${ENGINE_CONTRIB_DIR}/glm
        ${ENGINE_CONTRIB_DIR}/imgui
        ${ENGINE_CONTRIB_DIR}/assimp/include
        ${ENGINE_CONTRIB_DIR}/stb
        ${ENGINE_CONTRIB_DIR}/yaml-cpp/include
        ${ENGINE_CONTRIB_DIR}/OpenAL/include
        ${ENGINE_CONTRIB_DIR}/physx/include
)

# Create Audio Subsystem Library
file(GLOB_RECURSE AUDIO_SOURCE CONFIGURE_DEPENDS
        ${ENGINE_SOURCE_DIR}/Denix/Audio/*.cpp
        ${ENGINE_SOURCE_DIR}/Denix/Audio/*.h
)
add_library(AudioSubsystem STATIC ${AUDIO_SOURCE})
set_property(TARGET AudioSubsystem PROPERTY FOLDER DenixEngine/Audio)

# Create the Engine library & Solution Folder
add_library(DenixEngine STATIC ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER DenixEngine)

# Add Precompiled Header - We Can add multiple precompiled headers
target_precompile_headers(DenixEngine PUBLIC ${ENGINE_SOURCE_DIR}/depch.h)


# Set dependencies for DenixEngine
add_dependencies(${PROJECT_NAME}
        Imgui
        AudioSubsystem
)

# Enable Edit and Continue for MSVC
#[[
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()]]

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
)