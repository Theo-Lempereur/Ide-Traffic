#pragma once

#include <memory>
#include <string>
#include <functional>

namespace RoadSim::Core {
    class Simulator;
    class Scheduler;
}

namespace RoadSim::Editor {
    class MapEditor;
    class EntityEditor;
}

namespace RoadSim::Render {
    class Window;
    class Renderer;
    class UIManager;
}

namespace RoadSim::Core {
    class Scene;
}

namespace RoadSim::IO {
    class ConfigLoader;
}

namespace RoadSim::Runtime {
    class ThreadManager;
}

namespace RoadSim::Runtime {

/**
 * @brief Main application class that orchestrates all subsystems
 * Manages the application lifecycle, coordinates between modules
 */
class Application {
public:
    Application();
    ~Application();
    
    // Non-copyable
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    
    // Movable
    Application(Application&&) = default;
    Application& operator=(Application&&) = default;
    
    /**
     * @brief Initialize the application
     * @param configPath Path to configuration file
     */
    bool initialize(const std::string& configPath = "config.toml");
    
    /**
     * @brief Run the main application loop
     */
    int run();
    
    /**
     * @brief Shutdown the application
     */
    void shutdown();
    
    /**
     * @brief Check if application is running
     */
    bool isRunning() const;
    
    /**
     * @brief Request application exit
     */
    void requestExit();
    
    /**
     * @brief Switch to simulation mode
     */
    void switchToSimulationMode();
    
    /**
     * @brief Switch to editor mode
     */
    void switchToEditorMode();
    
    /**
     * @brief Check current application mode
     */
    enum class Mode {
        Editor,
        Simulation,
        Paused
    };
    
    Mode getCurrentMode() const;
    
    /**
     * @brief Load a map file
     * @param mapPath Path to map file
     */
    bool loadMap(const std::string& mapPath);
    
    /**
     * @brief Save current map
     * @param mapPath Path to save map to
     */
    bool saveMap(const std::string& mapPath);
    
    /**
     * @brief Create a new empty map
     */
    void createNewMap();
    
    /**
     * @brief Get application statistics
     */
    struct Statistics {
        double frameTime = 0.0;
        double updateTime = 0.0;
        double renderTime = 0.0;
        size_t frameCount = 0;
        double averageFPS = 0.0;
        size_t memoryUsage = 0;
    };
    
    Statistics getStatistics() const;
    
    /**
     * @brief Set frame rate limit
     * @param fps Target FPS (0 for unlimited)
     */
    void setTargetFPS(unsigned int fps);
    
    /**
     * @brief Enable/disable debug mode
     * @param enabled Debug mode state
     */
    void setDebugMode(bool enabled);
    
    /**
     * @brief Get subsystem instances for advanced usage
     */
    Core::Simulator* getSimulator();
    Editor::MapEditor* getMapEditor();
    Editor::EntityEditor* getEntityEditor();
    Render::Window* getWindow();
    Render::Renderer* getRenderer();
    Render::UIManager* getUIManager();
    Core::Scene* getScene();
    ThreadManager* getThreadManager();
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    
    // Internal methods
    void update(double deltaTime);
    void render();
    void handleEvents();
    void updateStatistics(double frameTime);
};

} // namespace RoadSim::Runtime