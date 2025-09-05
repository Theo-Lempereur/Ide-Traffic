#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace RoadSim::Render {

/**
 * @brief 2D renderer for traffic simulation visualization
 * Handles real-time rendering of maps, entities, and UI elements
 */
class Renderer {
public:
    Renderer();
    ~Renderer();
    
    // Non-copyable
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    // Movable
    Renderer(Renderer&&) = default;
    Renderer& operator=(Renderer&&) = default;
    
    /**
     * @brief Initialize renderer with render target
     * @param target SFML render target (window)
     */
    void initialize(sf::RenderTarget* target);
    
    /**
     * @brief Begin frame rendering
     */
    void beginFrame();
    
    /**
     * @brief End frame rendering
     */
    void endFrame();
    
    /**
     * @brief Clear screen with background color
     * @param color Background color
     */
    void clear(const sf::Color& color = sf::Color(45, 45, 45));
    
    /**
     * @brief Render road network
     */
    void renderRoads();
    
    /**
     * @brief Render traffic entities (vehicles, pedestrians, cyclists)
     */
    void renderEntities();
    
    /**
     * @brief Render traffic lights
     */
    void renderTrafficLights();
    
    /**
     * @brief Render spawn points
     */
    void renderSpawnPoints();
    
    /**
     * @brief Render editor UI elements
     */
    void renderEditorUI();
    
    /**
     * @brief Render simulation metrics overlay
     */
    void renderMetrics();
    
    /**
     * @brief Set camera position and zoom
     * @param x Camera X position
     * @param y Camera Y position
     * @param zoom Zoom level
     */
    void setCamera(float x, float y, float zoom);
    
    /**
     * @brief Convert screen coordinates to world coordinates
     * @param screenPos Screen position
     */
    sf::Vector2f screenToWorld(const sf::Vector2i& screenPos) const;
    
    /**
     * @brief Convert world coordinates to screen coordinates
     * @param worldPos World position
     */
    sf::Vector2i worldToScreen(const sf::Vector2f& worldPos) const;
    
    /**
     * @brief Enable/disable debug rendering
     * @param enabled Debug rendering state
     */
    void setDebugMode(bool enabled);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace RoadSim::Render