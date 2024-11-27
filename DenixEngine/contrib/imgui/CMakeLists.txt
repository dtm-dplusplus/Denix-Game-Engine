project(Imgui)

# Define the source directory
set(ENGINE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/DenixEngine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include Header
include_directories(${PROJECT_NAME}#${CMAKE_CURRENT_SOURCE_DIR}
        ${ENGINE_SOURCE_DIR}/contrib/imgui
        PRIVATE ${ENGINE_SOURCE_DIR}/contrib/SDL3/include
)

# Create Imgui Library
add_library(Imgui STATIC
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imgui.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imgui.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imconfig.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imgui_demo.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imgui_draw.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imgui_internal.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imgui_tables.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imgui_widgets.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imstb_rectpack.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imstb_textedit.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/imstb_truetype.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/misc/cpp/imgui_stdlib.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/misc/cpp/imgui_stdlib.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_sdl3.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_sdl3.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_opengl3.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/implot.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/implot.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/implot_internal.h
        ${ENGINE_SOURCE_DIR}/contrib/imgui/implot_items.cpp
        ${ENGINE_SOURCE_DIR}/contrib/imgui/implot_demo.cpp
)

# Add Dependency to support the SDL3 Backend
#add_dependencies(${PROJECT_NAME} SDL3::SDL3)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
