cmake_minimum_required(VERSION 3.27)
project(RoadSim VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)

# Optional: Find ImGui-SFML
find_package(ImGui-SFML QUIET)

# Optional: Find nlohmann_json
find_package(nlohmann_json QUIET)

# Optional: Find toml++
find_package(tomlplusplus QUIET)

# Create main executable
add_executable(RoadSim
    app/main.cpp
)

# Add subdirectories for each module
add_subdirectory(app/core)
add_subdirectory(app/editor)
add_subdirectory(app/render)
add_subdirectory(app/io)
add_subdirectory(app/runtime)

# Link libraries
target_link_libraries(RoadSim PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    RoadSim_Core
    RoadSim_Editor
    RoadSim_Render
    RoadSim_IO
    RoadSim_Runtime
)

# Link optional libraries if found
if(ImGui-SFML_FOUND)
    target_link_libraries(RoadSim PRIVATE ImGui-SFML::ImGui-SFML)
    target_compile_definitions(RoadSim PRIVATE HAVE_IMGUI_SFML)
endif()

if(nlohmann_json_FOUND)
    target_link_libraries(RoadSim PRIVATE nlohmann_json::nlohmann_json)
    target_compile_definitions(RoadSim PRIVATE HAVE_NLOHMANN_JSON)
endif()

if(tomlplusplus_FOUND)
    target_link_libraries(RoadSim PRIVATE tomlplusplus::tomlplusplus)
    target_compile_definitions(RoadSim PRIVATE HAVE_TOMLPLUSPLUS)
endif()

# Set output directory
set_target_properties(RoadSim PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Enable testing
enable_testing()
add_subdirectory(tests)

# Compiler-specific options
if(MSVC)
    target_compile_options(RoadSim PRIVATE /W4)
else()
    target_compile_options(RoadSim PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Debug/Release configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(RoadSim PRIVATE DEBUG_BUILD)
else()
    target_compile_definitions(RoadSim PRIVATE RELEASE_BUILD)
endif()