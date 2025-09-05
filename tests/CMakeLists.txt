# Tests module

# Find test framework (prefer Catch2, fallback to basic tests)
find_package(Catch2 QUIET)

if(Catch2_FOUND)
    # Use Catch2 if available
    add_executable(RoadSim_Tests
        test_main.cpp
        test_core.cpp
        test_editor.cpp
        test_render.cpp
        test_io.cpp
        test_runtime.cpp
    )
    
    target_link_libraries(RoadSim_Tests PRIVATE
        Catch2::Catch2WithMain
        RoadSim_Core
        RoadSim_Editor
        RoadSim_Render
        RoadSim_IO
        RoadSim_Runtime
    )
    
    # Register tests with CTest
    include(CTest)
    include(Catch)
    catch_discover_tests(RoadSim_Tests)
else()
    # Basic test executable without framework
    add_executable(RoadSim_BasicTests
        basic_tests.cpp
    )
    
    target_link_libraries(RoadSim_BasicTests PRIVATE
        RoadSim_Core
        RoadSim_Editor
        RoadSim_Render
        RoadSim_IO
        RoadSim_Runtime
    )
    
    add_test(NAME BasicTests COMMAND RoadSim_BasicTests)
endif()