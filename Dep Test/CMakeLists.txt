cmake_minimum_required(VERSION 3.6)

#set(CMAKE_DEBUG_POSTFIX "")

# Create Development Project
project(TestProject)

include_directories(${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/yaml-cpp/include
)

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_MakeAvailable(yaml-cpp)

# Create Project Executable
add_executable(${PROJECT_NAME} TestProject.cpp)



target_link_libraries(TestProject PUBLIC yaml-cpp::yaml-cpp)