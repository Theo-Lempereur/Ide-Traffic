#include "runtime/Application.h"
#include <iostream>
#include <memory>

int main()
{
    try {
        std::cout << "=== RoadSim - Traffic Simulation IDE ===" << std::endl;
        std::cout << "Initializing application..." << std::endl;
        
        // Create the main application
        auto app = std::make_unique<RoadSim::Runtime::Application>();
        
        // Initialize with default config
        if (!app->initialize("config.toml")) {
            std::cerr << "Failed to initialize application!" << std::endl;
            return 1;
        }
        
        std::cout << "Application initialized successfully!" << std::endl;
        std::cout << "Features available:" << std::endl;
        std::cout << "- GameObject system with Transform and Collider components" << std::endl;
        std::cout << "- Scene management for simulation objects" << std::endl;
        std::cout << "- UI with simulation control button (Start/Stop)" << std::endl;
        std::cout << "- Mode indicator (Development/Running)" << std::endl;
        std::cout << "- Map editor and entity editor" << std::endl;
        std::cout << "- Real-time simulation with SFML rendering" << std::endl;
        std::cout << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "- Click 'Start Sim' button to begin simulation" << std::endl;
        std::cout << "- Click 'Stop Sim' button to return to editor mode" << std::endl;
        std::cout << "- ESC or close window to exit" << std::endl;
        std::cout << std::endl;
        
        // Run the main application loop
        int result = app->run();
        
        std::cout << "Application finished with code: " << result << std::endl;
        return result;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred!" << std::endl;
        return 1;
    }
}