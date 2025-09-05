#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <functional>

namespace RoadSim::Render {

/**
 * @brief UI Manager for handling user interface elements
 * Manages buttons, labels, and other UI components
 */
class UIManager {
public:
    enum class SimulationState {
        Development,
        Running,
        Paused
    };
    
    using SimulationCallback = std::function<void(SimulationState)>;
    
    UIManager();
    ~UIManager();
    
    // Non-copyable
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    
    // Movable
    UIManager(UIManager&&) = default;
    UIManager& operator=(UIManager&&) = default;
    
    /**
     * @brief Initialize UI with render target
     * @param target SFML render target
     */
    void initialize(sf::RenderTarget* target);
    
    /**
     * @brief Handle SFML events
     * @param event SFML event
     */
    void handleEvent(const sf::Event& event);
    
    /**
     * @brief Update UI elements
     * @param deltaTime Time elapsed since last frame
     */
    void update(float deltaTime);
    
    /**
     * @brief Render all UI elements
     */
    void render();
    
    /**
     * @brief Set current simulation state
     * @param state Current simulation state
     */
    void setSimulationState(SimulationState state);
    
    /**
     * @brief Get current simulation state
     */
    SimulationState getSimulationState() const { return m_simulationState; }
    
    /**
     * @brief Set callback for simulation state changes
     * @param callback Function to call when simulation state changes
     */
    void setSimulationCallback(SimulationCallback callback) { m_simulationCallback = callback; }
    
    /**
     * @brief Set window size for UI layout
     * @param width Window width
     * @param height Window height
     */
    void setWindowSize(unsigned int width, unsigned int height);
    
    /**
     * @brief Show/hide debug information
     * @param show True to show debug info
     */
    void setShowDebugInfo(bool show) { m_showDebugInfo = show; }
    
    /**
     * @brief Set debug information text
     * @param info Debug information string
     */
    void setDebugInfo(const std::string& info) { m_debugInfo = info; }
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    
    SimulationState m_simulationState = SimulationState::Development;
    SimulationCallback m_simulationCallback;
    
    bool m_showDebugInfo = false;
    std::string m_debugInfo;
    
    void createUI();
    void updateSimulationButton();
    void updateModeIndicator();
    std::string getSimulationStateText() const;
    std::string getButtonText() const;
    sf::Color getStateColor() const;
};

} // namespace RoadSim::Render