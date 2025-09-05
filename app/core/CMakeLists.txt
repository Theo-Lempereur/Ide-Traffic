# Core module - Simulation engine
add_library(RoadSim_Core STATIC
    Simulator.cpp
    Scheduler.cpp
    RNG.cpp
)

target_include_directories(RoadSim_Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(RoadSim_Core PUBLIC cxx_std_20)