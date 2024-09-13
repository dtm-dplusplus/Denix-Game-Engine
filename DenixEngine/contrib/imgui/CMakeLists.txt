project(Imgui)

# Define Output Directories
set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/DenixEngine/depend/${PROJECT_NAME})
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/lib")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/lib")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/lib")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/lib")
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/lib")
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/lib")

# Define the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/DenixEngine)
set(OUTPUT_SOURCE_DIR ${CMAKE_BINARY_DIR}/DenixEngine)
message(STATUS ${SOURCE_DIR})
message(STATUS ${OUTPUT_SOURCE_DIR})

# Copy the source files to the destination directory
# This ensures cmake source is not modifiable
file(COPY ${SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include Header
include_directories(${PROJECT_NAME}${CMAKE_CURRENT_SOURCE_DIR}
        ${OUTPUT_SOURCE_DIR}/contrib/imgui
        PRIVATE ${CMAKE_BINARY_DIR}/DenixEngine/contrib/SDL3/include
)

# Create Imgui Library
add_library(Imgui STATIC
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imconfig.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_demo.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_draw.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_internal.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_tables.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imgui_widgets.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imstb_rectpack.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imstb_textedit.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/imstb_truetype.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/misc/cpp/imgui_stdlib.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/misc/cpp/imgui_stdlib.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_sdl3.cpp
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_sdl3.h
        ${OUTPUT_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_opengl3.cpp
)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Set the solution folder for the target
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER DenixEngine/Depend/${PROJECT_NAME})

# Then in some target add this to put the target in the Development folder.
#set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Development)