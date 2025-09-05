# Editor module - Map and entity editing
add_library(RoadSim_Editor STATIC
    MapEditor.cpp
    EntityEditor.cpp
)

target_include_directories(RoadSim_Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(RoadSim_Editor PUBLIC cxx_std_20)

# Link with Core module
target_link_libraries(RoadSim_Editor PUBLIC RoadSim_Core)