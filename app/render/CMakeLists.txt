# Render module - 2D rendering with SFML
add_library(RoadSim_Render STATIC
    Renderer.cpp
    Window.cpp
    UIManager.cpp
)

target_include_directories(RoadSim_Render PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(RoadSim_Render PUBLIC cxx_std_20)

# Link with SFML and Core module
target_link_libraries(RoadSim_Render PUBLIC
    RoadSim_Core
    sfml-graphics
    sfml-window
    sfml-system
)