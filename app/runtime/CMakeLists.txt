# Runtime module - Thread management and application lifecycle
add_library(RoadSim_Runtime STATIC
    ThreadManager.cpp
    Application.cpp
)

target_include_directories(RoadSim_Runtime PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(RoadSim_Runtime PUBLIC cxx_std_20)

# Link with all other modules
target_link_libraries(RoadSim_Runtime PUBLIC
    RoadSim_Core
    RoadSim_Editor
    RoadSim_Render
    RoadSim_IO
)