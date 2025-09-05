# IO module - JSON/TOML serialization
add_library(RoadSim_IO STATIC
    JsonLoader.cpp
    ConfigLoader.cpp
)

target_include_directories(RoadSim_IO PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(RoadSim_IO PUBLIC cxx_std_20)

# Link with Core module
target_link_libraries(RoadSim_IO PUBLIC RoadSim_Core)

# Link optional JSON/TOML libraries if available
if(nlohmann_json_FOUND)
    target_link_libraries(RoadSim_IO PUBLIC nlohmann_json::nlohmann_json)
endif()

if(tomlplusplus_FOUND)
    target_link_libraries(RoadSim_IO PUBLIC tomlplusplus::tomlplusplus)
endif()