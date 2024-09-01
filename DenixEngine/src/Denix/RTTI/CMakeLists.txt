project(RTTISubsystem)

# Define the source directory
set(GEN_RTTI_SOURCE_DIR ${CMAKE_BINARY_DIR}/DenixEngine/src/Denix/RTTI)
message(GEN_RTTI_SOURCE_DIR: ${GEN_RTTI_SOURCE_DIR})

# Set the C++ flags to enable multi-processor compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_BINARY_DIR}/DenixEngine/contrib/yaml-cpp-master/include)

# Create Executable
add_library(${PROJECT_NAME}
        ${GEN_RTTI_SOURCE_DIR}/AbstractProperty.cpp
        ${GEN_RTTI_SOURCE_DIR}/AbstractProperty.h
        ${GEN_RTTI_SOURCE_DIR}/BaseObject.cpp
        ${GEN_RTTI_SOURCE_DIR}/BaseObject.h
        ${GEN_RTTI_SOURCE_DIR}/Property.cpp
        ${GEN_RTTI_SOURCE_DIR}/Property.h
        ${GEN_RTTI_SOURCE_DIR}/PropertySystem.cpp
        ${GEN_RTTI_SOURCE_DIR}/PropertySystem.h
        ${GEN_RTTI_SOURCE_DIR}/PropertyTypes.h
        ${GEN_RTTI_SOURCE_DIR}/RTTI.cpp
        ${GEN_RTTI_SOURCE_DIR}/RTTI.h
        ${GEN_RTTI_SOURCE_DIR}/RTTIClass.cpp
        ${GEN_RTTI_SOURCE_DIR}/RTTIClass.h
        ${GEN_RTTI_SOURCE_DIR}/TestClasses.cpp
        ${GEN_RTTI_SOURCE_DIR}/TestClasses.h
        ${GEN_RTTI_SOURCE_DIR}/TypedProperty.cpp
        ${GEN_RTTI_SOURCE_DIR}/TypedProperty.h
)

# Enable Edit and Continue for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
endif()

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/DenixEngine/contrib/yaml-cpp-master/lib)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    $<$<CONFIG:Debug>:yaml-cppd.lib>
    $<$<CONFIG:Release>:yaml-cpp.lib>
)

# Then in some target add this to put the target in the Development folder.
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER DenixEngine/Subsystem)