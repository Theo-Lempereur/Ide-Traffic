#include "ConfigLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace RoadSim::IO {

struct ConfigLoader::Impl {
    bool initialized = false;
    std::string lastError;
    std::map<std::string, std::map<std::string, ConfigValue>> config;
    
    // Helper methods
    std::string trim(const std::string& str) {
        auto start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            start++;
        }
        
        auto end = str.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));
        
        return std::string(start, end + 1);
    }
    
    ConfigValue parseValue(const std::string& valueStr) {
        std::string trimmed = trim(valueStr);
        
        // Boolean values
        if (trimmed == "true" || trimmed == "True" || trimmed == "TRUE") {
            return true;
        }
        if (trimmed == "false" || trimmed == "False" || trimmed == "FALSE") {
            return false;
        }
        
        // Numeric values
        if (!trimmed.empty() && (std::isdigit(trimmed[0]) || trimmed[0] == '-' || trimmed[0] == '+')) {
            try {
                if (trimmed.find('.') != std::string::npos) {
                    return std::stod(trimmed);
                } else {
                    return static_cast<int64_t>(std::stoll(trimmed));
                }
            } catch (...) {
                // Fall through to string
            }
        }
        
        // String value (remove quotes if present)
        if (trimmed.length() >= 2 && trimmed.front() == '"' && trimmed.back() == '"') {
            return trimmed.substr(1, trimmed.length() - 2);
        }
        
        return trimmed;
    }
};

ConfigLoader::ConfigLoader() : m_impl(std::make_unique<Impl>()) {
    std::cout << "[IO] ConfigLoader created" << std::endl;
}

ConfigLoader::~ConfigLoader() {
    std::cout << "[IO] ConfigLoader destroyed" << std::endl;
}

void ConfigLoader::initialize() {
    std::cout << "[IO] ConfigLoader initialized" << std::endl;
    m_impl->initialized = true;
    loadDefaults();
}

bool ConfigLoader::loadConfig(const std::string& filePath) {
    if (!m_impl->initialized) {
        m_impl->lastError = "ConfigLoader not initialized";
        return false;
    }
    
    std::cout << "[IO] Loading configuration from: " << filePath << std::endl;
    
    // Determine file format by extension
    std::string extension = filePath.substr(filePath.find_last_of('.') + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) { return static_cast<char>(::tolower(c)); });
    
    if (extension == "toml") {
        return parseTomlFile(filePath);
    } else if (extension == "json") {
        return parseJsonFile(filePath);
    } else if (extension == "ini" || extension == "cfg") {
        return parseIniFile(filePath);
    } else {
        // Try to auto-detect format
        return parseIniFile(filePath); // Default to INI format
    }
}

bool ConfigLoader::saveConfig(const std::string& filePath) {
    if (!m_impl->initialized) {
        m_impl->lastError = "ConfigLoader not initialized";
        return false;
    }
    
    std::cout << "[IO] Saving configuration to: " << filePath << std::endl;
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        m_impl->lastError = "Could not create file: " + filePath;
        std::cerr << "[IO] " << m_impl->lastError << std::endl;
        return false;
    }
    
    // Save in INI format
    for (const auto& section : m_impl->config) {
        file << "[" << section.first << "]\n";
        
        for (const auto& keyValue : section.second) {
            file << keyValue.first << " = ";
            
            std::visit([&file](const auto& value) {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    file << "\"" << value << "\"";
                } else {
                    file << value;
                }
            }, keyValue.second);
            
            file << "\n";
        }
        
        file << "\n";
    }
    
    return file.good();
}

void ConfigLoader::loadDefaults() {
    std::cout << "[IO] Loading default configuration" << std::endl;
    
    // Window defaults
    setValue("window", "width", 1200);
    setValue("window", "height", 800);
    setValue("window", "title", std::string("RoadSim - Traffic Simulation"));
    setValue("window", "fullscreen", false);
    setValue("window", "vsync", false);
    setValue("window", "maxFPS", 60);
    
    // Simulation defaults
    setValue("simulation", "timeStep", 0.016);
    setValue("simulation", "maxSimulationTime", 3600.0);
    setValue("simulation", "maxEntities", 10000);
    setValue("simulation", "enableMultithreading", true);
    setValue("simulation", "workerThreads", 0);
    setValue("simulation", "enableStatistics", true);
    
    // Rendering defaults
    setValue("render", "enableAntialiasing", true);
    setValue("render", "antialiasingLevel", 4);
    setValue("render", "enableDebugRendering", false);
    setValue("render", "cameraSpeed", 500.0);
    setValue("render", "zoomSpeed", 1.1);
    setValue("render", "minZoom", 0.1);
    setValue("render", "maxZoom", 10.0);
    
    // Editor defaults
    setValue("editor", "enableGrid", true);
    setValue("editor", "gridSize", 50.0);
    setValue("editor", "enableSnapping", true);
    setValue("editor", "snapDistance", 10.0);
    setValue("editor", "showNodeIds", false);
    setValue("editor", "showRoadIds", false);
    setValue("editor", "defaultMapPath", std::string("maps/"));
    setValue("editor", "defaultProfilePath", std::string("profiles/"));
    
    // I/O defaults
    setValue("io", "mapsDirectory", std::string("maps/"));
    setValue("io", "profilesDirectory", std::string("profiles/"));
    setValue("io", "scenariosDirectory", std::string("scenarios/"));
    setValue("io", "outputDirectory", std::string("output/"));
    setValue("io", "enableAutoSave", true);
    setValue("io", "autoSaveInterval", 300);
    setValue("io", "enableBackups", true);
    setValue("io", "maxBackups", 5);
}

bool ConfigLoader::hasValue(const std::string& section, const std::string& key) const {
    auto sectionIt = m_impl->config.find(section);
    if (sectionIt == m_impl->config.end()) {
        return false;
    }
    
    return sectionIt->second.find(key) != sectionIt->second.end();
}

std::vector<std::string> ConfigLoader::getKeys(const std::string& section) const {
    std::vector<std::string> keys;
    
    auto sectionIt = m_impl->config.find(section);
    if (sectionIt != m_impl->config.end()) {
        for (const auto& keyValue : sectionIt->second) {
            keys.push_back(keyValue.first);
        }
    }
    
    return keys;
}

std::vector<std::string> ConfigLoader::getSections() const {
    std::vector<std::string> sections;
    
    for (const auto& section : m_impl->config) {
        sections.push_back(section.first);
    }
    
    return sections;
}

void ConfigLoader::removeValue(const std::string& section, const std::string& key) {
    auto sectionIt = m_impl->config.find(section);
    if (sectionIt != m_impl->config.end()) {
        sectionIt->second.erase(key);
    }
}

void ConfigLoader::removeSection(const std::string& section) {
    m_impl->config.erase(section);
}

void ConfigLoader::clear() {
    m_impl->config.clear();
}

std::string ConfigLoader::getLastError() const {
    return m_impl->lastError;
}

bool ConfigLoader::validateConfig(const std::string& filePath) {
    // Try to load the config file to validate it
    std::map<std::string, std::map<std::string, ConfigValue>> backup = m_impl->config;
    bool result = loadConfig(filePath);
    m_impl->config = backup; // Restore original config
    return result;
}

// Configuration structure getters and setters
ConfigLoader::WindowConfig ConfigLoader::getWindowConfig() const {
    WindowConfig config;
    config.width = getValue("window", "width", config.width);
    config.height = getValue("window", "height", config.height);
    config.title = getValue("window", "title", config.title);
    config.fullscreen = getValue("window", "fullscreen", config.fullscreen);
    config.vsync = getValue("window", "vsync", config.vsync);
    config.maxFPS = getValue("window", "maxFPS", config.maxFPS);
    return config;
}

void ConfigLoader::setWindowConfig(const WindowConfig& config) {
    setValue("window", "width", config.width);
    setValue("window", "height", config.height);
    setValue("window", "title", config.title);
    setValue("window", "fullscreen", config.fullscreen);
    setValue("window", "vsync", config.vsync);
    setValue("window", "maxFPS", config.maxFPS);
}

ConfigLoader::SimulationConfig ConfigLoader::getSimulationConfig() const {
    SimulationConfig config;
    config.timeStep = getValue("simulation", "timeStep", config.timeStep);
    config.maxSimulationTime = getValue("simulation", "maxSimulationTime", config.maxSimulationTime);
    config.maxEntities = getValue("simulation", "maxEntities", config.maxEntities);
    config.enableMultithreading = getValue("simulation", "enableMultithreading", config.enableMultithreading);
    config.workerThreads = getValue("simulation", "workerThreads", config.workerThreads);
    config.enableStatistics = getValue("simulation", "enableStatistics", config.enableStatistics);
    return config;
}

void ConfigLoader::setSimulationConfig(const SimulationConfig& config) {
    setValue("simulation", "timeStep", config.timeStep);
    setValue("simulation", "maxSimulationTime", config.maxSimulationTime);
    setValue("simulation", "maxEntities", config.maxEntities);
    setValue("simulation", "enableMultithreading", config.enableMultithreading);
    setValue("simulation", "workerThreads", config.workerThreads);
    setValue("simulation", "enableStatistics", config.enableStatistics);
}

ConfigLoader::RenderConfig ConfigLoader::getRenderConfig() const {
    RenderConfig config;
    config.enableAntialiasing = getValue("render", "enableAntialiasing", config.enableAntialiasing);
    config.antialiasingLevel = getValue("render", "antialiasingLevel", config.antialiasingLevel);
    config.enableDebugRendering = getValue("render", "enableDebugRendering", config.enableDebugRendering);
    config.cameraSpeed = getValue("render", "cameraSpeed", config.cameraSpeed);
    config.zoomSpeed = getValue("render", "zoomSpeed", config.zoomSpeed);
    config.minZoom = getValue("render", "minZoom", config.minZoom);
    config.maxZoom = getValue("render", "maxZoom", config.maxZoom);
    return config;
}

void ConfigLoader::setRenderConfig(const RenderConfig& config) {
    setValue("render", "enableAntialiasing", config.enableAntialiasing);
    setValue("render", "antialiasingLevel", config.antialiasingLevel);
    setValue("render", "enableDebugRendering", config.enableDebugRendering);
    setValue("render", "cameraSpeed", config.cameraSpeed);
    setValue("render", "zoomSpeed", config.zoomSpeed);
    setValue("render", "minZoom", config.minZoom);
    setValue("render", "maxZoom", config.maxZoom);
}

ConfigLoader::EditorConfig ConfigLoader::getEditorConfig() const {
    EditorConfig config;
    config.enableGrid = getValue("editor", "enableGrid", config.enableGrid);
    config.gridSize = getValue("editor", "gridSize", config.gridSize);
    config.enableSnapping = getValue("editor", "enableSnapping", config.enableSnapping);
    config.snapDistance = getValue("editor", "snapDistance", config.snapDistance);
    config.showNodeIds = getValue("editor", "showNodeIds", config.showNodeIds);
    config.showRoadIds = getValue("editor", "showRoadIds", config.showRoadIds);
    config.defaultMapPath = getValue("editor", "defaultMapPath", config.defaultMapPath);
    config.defaultProfilePath = getValue("editor", "defaultProfilePath", config.defaultProfilePath);
    return config;
}

void ConfigLoader::setEditorConfig(const EditorConfig& config) {
    setValue("editor", "enableGrid", config.enableGrid);
    setValue("editor", "gridSize", config.gridSize);
    setValue("editor", "enableSnapping", config.enableSnapping);
    setValue("editor", "snapDistance", config.snapDistance);
    setValue("editor", "showNodeIds", config.showNodeIds);
    setValue("editor", "showRoadIds", config.showRoadIds);
    setValue("editor", "defaultMapPath", config.defaultMapPath);
    setValue("editor", "defaultProfilePath", config.defaultProfilePath);
}

ConfigLoader::IOConfig ConfigLoader::getIOConfig() const {
    IOConfig config;
    config.mapsDirectory = getValue("io", "mapsDirectory", config.mapsDirectory);
    config.profilesDirectory = getValue("io", "profilesDirectory", config.profilesDirectory);
    config.scenariosDirectory = getValue("io", "scenariosDirectory", config.scenariosDirectory);
    config.outputDirectory = getValue("io", "outputDirectory", config.outputDirectory);
    config.enableAutoSave = getValue("io", "enableAutoSave", config.enableAutoSave);
    config.autoSaveInterval = getValue("io", "autoSaveInterval", config.autoSaveInterval);
    config.enableBackups = getValue("io", "enableBackups", config.enableBackups);
    config.maxBackups = getValue("io", "maxBackups", config.maxBackups);
    return config;
}

void ConfigLoader::setIOConfig(const IOConfig& config) {
    setValue("io", "mapsDirectory", config.mapsDirectory);
    setValue("io", "profilesDirectory", config.profilesDirectory);
    setValue("io", "scenariosDirectory", config.scenariosDirectory);
    setValue("io", "outputDirectory", config.outputDirectory);
    setValue("io", "enableAutoSave", config.enableAutoSave);
    setValue("io", "autoSaveInterval", config.autoSaveInterval);
    setValue("io", "enableBackups", config.enableBackups);
    setValue("io", "maxBackups", config.maxBackups);
}

// Helper methods
std::string ConfigLoader::makeKey(const std::string& section, const std::string& key) const {
    return section + "." + key;
}

bool ConfigLoader::parseTomlFile(const std::string& filePath) {
    // TODO: Implement proper TOML parsing
    // For now, fall back to INI parsing
    std::cout << "[IO] TOML parsing not yet implemented, falling back to INI" << std::endl;
    return parseIniFile(filePath);
}

bool ConfigLoader::parseJsonFile(const std::string& filePath) {
    // TODO: Implement JSON config parsing
    // For now, fall back to INI parsing
    std::cout << "[IO] JSON config parsing not yet implemented, falling back to INI" << std::endl;
    return parseIniFile(filePath);
}

bool ConfigLoader::parseIniFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        m_impl->lastError = "Could not open file: " + filePath;
        std::cerr << "[IO] " << m_impl->lastError << std::endl;
        return false;
    }
    
    std::string line;
    std::string currentSection;
    
    while (std::getline(file, line)) {
        line = m_impl->trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        // Section header
        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }
        
        // Key-value pair
        size_t equalPos = line.find('=');
        if (equalPos != std::string::npos) {
            std::string key = m_impl->trim(line.substr(0, equalPos));
            std::string value = m_impl->trim(line.substr(equalPos + 1));
            
            if (!currentSection.empty() && !key.empty()) {
                m_impl->config[currentSection][key] = m_impl->parseValue(value);
            }
        }
    }
    
    return true;
}

// Template specialization implementations
template<>
int ConfigLoader::getValue<int>(const std::string& section, const std::string& key, const int& defaultValue) const {
    auto sectionIt = m_impl->config.find(section);
    if (sectionIt != m_impl->config.end()) {
        auto keyIt = sectionIt->second.find(key);
        if (keyIt != sectionIt->second.end()) {
            if (auto* val = std::get_if<int64_t>(&keyIt->second)) {
                return static_cast<int>(*val);
            }
        }
    }
    return defaultValue;
}

template<>
void ConfigLoader::setValue<int>(const std::string& section, const std::string& key, const int& value) {
    m_impl->config[section][key] = static_cast<int64_t>(value);
}

template<>
bool ConfigLoader::getValue<bool>(const std::string& section, const std::string& key, const bool& defaultValue) const {
    auto sectionIt = m_impl->config.find(section);
    if (sectionIt != m_impl->config.end()) {
        auto keyIt = sectionIt->second.find(key);
        if (keyIt != sectionIt->second.end()) {
            if (auto* val = std::get_if<bool>(&keyIt->second)) {
                return *val;
            }
        }
    }
    return defaultValue;
}

template<>
void ConfigLoader::setValue<bool>(const std::string& section, const std::string& key, const bool& value) {
    m_impl->config[section][key] = value;
}

template<>
double ConfigLoader::getValue<double>(const std::string& section, const std::string& key, const double& defaultValue) const {
    auto sectionIt = m_impl->config.find(section);
    if (sectionIt != m_impl->config.end()) {
        auto keyIt = sectionIt->second.find(key);
        if (keyIt != sectionIt->second.end()) {
            if (auto* val = std::get_if<double>(&keyIt->second)) {
                return *val;
            }
            if (auto* val = std::get_if<int64_t>(&keyIt->second)) {
                return static_cast<double>(*val);
            }
        }
    }
    return defaultValue;
}

template<>
void ConfigLoader::setValue<double>(const std::string& section, const std::string& key, const double& value) {
    m_impl->config[section][key] = value;
}

template<>
std::string ConfigLoader::getValue<std::string>(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    auto sectionIt = m_impl->config.find(section);
    if (sectionIt != m_impl->config.end()) {
        auto keyIt = sectionIt->second.find(key);
        if (keyIt != sectionIt->second.end()) {
            if (auto* val = std::get_if<std::string>(&keyIt->second)) {
                return *val;
            }
        }
    }
    return defaultValue;
}

template<>
void ConfigLoader::setValue<std::string>(const std::string& section, const std::string& key, const std::string& value) {
    m_impl->config[section][key] = value;
}

} // namespace RoadSim::IO