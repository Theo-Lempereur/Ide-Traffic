#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <variant>

namespace RoadSim::IO {

// Configuration value types
using ConfigValue = std::variant<bool, int64_t, double, std::string>;

/**
 * @brief Configuration management system
 * Handles loading, parsing, and managing application configuration
 * Supports TOML, JSON, and INI formats
 */
class ConfigLoader {
public:
    ConfigLoader();
    ~ConfigLoader();
    
    // Non-copyable
    ConfigLoader(const ConfigLoader&) = delete;
    ConfigLoader& operator=(const ConfigLoader&) = delete;
    
    // Movable
    ConfigLoader(ConfigLoader&&) = default;
    ConfigLoader& operator=(ConfigLoader&&) = default;
    
    /**
     * @brief Initialize the configuration loader
     */
    void initialize();
    
    /**
     * @brief Load configuration from file
     * @param filePath Path to configuration file
     * @return Success status
     */
    bool loadConfig(const std::string& filePath);
    
    /**
     * @brief Save configuration to file
     * @param filePath Path to save configuration
     * @return Success status
     */
    bool saveConfig(const std::string& filePath);
    
    /**
     * @brief Load default configuration
     */
    void loadDefaults();
    
    /**
     * @brief Get configuration value
     * @param section Configuration section
     * @param key Configuration key
     * @param defaultValue Default value if not found
     */
    template<typename T>
    T getValue(const std::string& section, const std::string& key, const T& defaultValue) const;
    
    /**
     * @brief Set configuration value
     * @param section Configuration section
     * @param key Configuration key
     * @param value Value to set
     */
    template<typename T>
    void setValue(const std::string& section, const std::string& key, const T& value);
    
    // Template specializations
    template<> int getValue<int>(const std::string& section, const std::string& key, const int& defaultValue) const;
    template<> void setValue<int>(const std::string& section, const std::string& key, const int& value);
    template<> bool getValue<bool>(const std::string& section, const std::string& key, const bool& defaultValue) const;
    template<> void setValue<bool>(const std::string& section, const std::string& key, const bool& value);
    template<> double getValue<double>(const std::string& section, const std::string& key, const double& defaultValue) const;
    template<> void setValue<double>(const std::string& section, const std::string& key, const double& value);
    template<> std::string getValue<std::string>(const std::string& section, const std::string& key, const std::string& defaultValue) const;
    template<> void setValue<std::string>(const std::string& section, const std::string& key, const std::string& value);
    
    /**
     * @brief Check if configuration key exists
     * @param section Configuration section
     * @param key Configuration key
     */
    bool hasValue(const std::string& section, const std::string& key) const;
    
    /**
     * @brief Get all keys in a section
     * @param section Configuration section
     */
    std::vector<std::string> getKeys(const std::string& section) const;
    
    /**
     * @brief Get all sections
     */
    std::vector<std::string> getSections() const;
    
    /**
     * @brief Remove configuration value
     * @param section Configuration section
     * @param key Configuration key
     */
    void removeValue(const std::string& section, const std::string& key);
    
    /**
     * @brief Remove entire section
     * @param section Configuration section
     */
    void removeSection(const std::string& section);
    
    /**
     * @brief Clear all configuration
     */
    void clear();
    
    /**
     * @brief Get last error message
     */
    std::string getLastError() const;
    
    /**
     * @brief Validate configuration file
     * @param filePath Path to configuration file
     */
    bool validateConfig(const std::string& filePath);
    
    // Convenience methods for common configuration sections
    
    /**
     * @brief Get window configuration
     */
    struct WindowConfig {
        int width = 1200;
        int height = 800;
        std::string title = "RoadSim";
        bool fullscreen = false;
        bool vsync = false;
        int maxFPS = 60;
    };
    
    WindowConfig getWindowConfig() const;
    void setWindowConfig(const WindowConfig& config);
    
    /**
     * @brief Get simulation configuration
     */
    struct SimulationConfig {
        double timeStep = 0.016; // 60 FPS
        double maxSimulationTime = 3600.0; // 1 hour
        int maxEntities = 10000;
        bool enableMultithreading = true;
        int workerThreads = 0; // Auto-detect
        bool enableStatistics = true;
    };
    
    SimulationConfig getSimulationConfig() const;
    void setSimulationConfig(const SimulationConfig& config);
    
    /**
     * @brief Get rendering configuration
     */
    struct RenderConfig {
        bool enableAntialiasing = true;
        int antialiasingLevel = 4;
        bool enableDebugRendering = false;
        double cameraSpeed = 500.0;
        double zoomSpeed = 1.1;
        double minZoom = 0.1;
        double maxZoom = 10.0;
    };
    
    RenderConfig getRenderConfig() const;
    void setRenderConfig(const RenderConfig& config);
    
    /**
     * @brief Get editor configuration
     */
    struct EditorConfig {
        bool enableGrid = true;
        double gridSize = 50.0;
        bool enableSnapping = true;
        double snapDistance = 10.0;
        bool showNodeIds = false;
        bool showRoadIds = false;
        std::string defaultMapPath = "maps/";
        std::string defaultProfilePath = "profiles/";
    };
    
    EditorConfig getEditorConfig() const;
    void setEditorConfig(const EditorConfig& config);
    
    /**
     * @brief Get I/O configuration
     */
    struct IOConfig {
        std::string mapsDirectory = "maps/";
        std::string profilesDirectory = "profiles/";
        std::string scenariosDirectory = "scenarios/";
        std::string outputDirectory = "output/";
        bool enableAutoSave = true;
        int autoSaveInterval = 300; // 5 minutes
        bool enableBackups = true;
        int maxBackups = 5;
    };
    
    IOConfig getIOConfig() const;
    void setIOConfig(const IOConfig& config);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    
    // Helper methods
    std::string makeKey(const std::string& section, const std::string& key) const;
    bool parseTomlFile(const std::string& filePath);
    bool parseJsonFile(const std::string& filePath);
    bool parseIniFile(const std::string& filePath);
};

// Template implementations
template<typename T>
T ConfigLoader::getValue(const std::string& section, const std::string& key, const T& defaultValue) const {
    std::string fullKey = makeKey(section, key);
    
    // Implementation will be in the .cpp file
    // This is a placeholder that returns the default value
    return defaultValue;
}

template<typename T>
void ConfigLoader::setValue(const std::string& section, const std::string& key, const T& value) {
    std::string fullKey = makeKey(section, key);
    
    // Implementation will be in the .cpp file
    // This is a placeholder
}

} // namespace RoadSim::IO