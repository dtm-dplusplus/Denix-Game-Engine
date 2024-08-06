project(Project)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
message(STATUS "PROJECT SOURCES" ${PROJECT_SOURCES})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_dependencies(DenixEngine)