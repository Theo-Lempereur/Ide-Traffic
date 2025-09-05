#include "Renderer.h"
#include <iostream>

namespace RoadSim::Render {

struct Renderer::Impl {
    sf::RenderTarget* renderTarget = nullptr;
    sf::View camera;
    sf::Font font;
    bool debugMode = false;
    bool initialized = false;
    
    // Rendering resources
    sf::CircleShape nodeShape;
    sf::RectangleShape roadShape;
    sf::CircleShape vehicleShape;
    sf::Text debugText;
};

Renderer::Renderer() : m_impl(std::make_unique<Impl>()) {
    std::cout << "[Render] Renderer created" << std::endl;
}

Renderer::~Renderer() {
    std::cout << "[Render] Renderer destroyed" << std::endl;
}

void Renderer::initialize(sf::RenderTarget* target) {
    std::cout << "[Render] Renderer initialized" << std::endl;
    m_impl->renderTarget = target;
    m_impl->initialized = true;
    
    // Initialize camera
    if (target) {
        auto size = target->getSize();
        m_impl->camera.setSize(static_cast<float>(size.x), static_cast<float>(size.y));
        m_impl->camera.setCenter(static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f);
    }
    
    // Initialize rendering shapes
    m_impl->nodeShape.setRadius(5.0f);
    m_impl->nodeShape.setFillColor(sf::Color::White);
    m_impl->nodeShape.setOrigin(5.0f, 5.0f);
    
    m_impl->roadShape.setFillColor(sf::Color(128, 128, 128));
    
    m_impl->vehicleShape.setRadius(3.0f);
    m_impl->vehicleShape.setFillColor(sf::Color::Red);
    m_impl->vehicleShape.setOrigin(3.0f, 3.0f);
    
    // TODO: Load font for text rendering
    // if (!m_impl->font.loadFromFile("assets/fonts/default.ttf")) {
    //     std::cerr << "[Render] Warning: Could not load default font" << std::endl;
    // }
    // m_impl->debugText.setFont(m_impl->font);
    m_impl->debugText.setCharacterSize(14);
    m_impl->debugText.setFillColor(sf::Color::White);
}

void Renderer::beginFrame() {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    // Set camera view
    m_impl->renderTarget->setView(m_impl->camera);
}

void Renderer::endFrame() {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    // Reset to default view for UI rendering
    m_impl->renderTarget->setView(m_impl->renderTarget->getDefaultView());
}

void Renderer::clear(const sf::Color& color) {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    m_impl->renderTarget->clear(color);
}

void Renderer::renderRoads() {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    // TODO: Render actual road network
    // - Draw road segments as rectangles
    // - Draw lane markings
    // - Draw intersections
    
    // Placeholder: Draw a simple grid
    if (m_impl->debugMode) {
        sf::RectangleShape gridLine(sf::Vector2f(2.0f, 600.0f));
        gridLine.setFillColor(sf::Color(64, 64, 64));
        
        for (int x = 0; x < 1200; x += 50) {
            gridLine.setPosition(static_cast<float>(x), 0);
            m_impl->renderTarget->draw(gridLine);
        }
        
        gridLine.setSize(sf::Vector2f(1200.0f, 2.0f));
        for (int y = 0; y < 800; y += 50) {
            gridLine.setPosition(0, static_cast<float>(y));
            m_impl->renderTarget->draw(gridLine);
        }
    }
}

void Renderer::renderEntities() {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    // TODO: Render actual entities
    // - Draw vehicles with appropriate colors and sizes
    // - Draw pedestrians
    // - Draw cyclists
    // - Show entity states (moving, waiting, etc.)
}

void Renderer::renderTrafficLights() {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    // TODO: Render traffic lights
    // - Draw traffic light poles
    // - Show current light state (red, yellow, green)
    // - Render timing information if in debug mode
}

void Renderer::renderSpawnPoints() {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    // TODO: Render spawn points
    // - Draw spawn point markers
    // - Show spawn rates and entity types
    // - Indicate active/inactive states
}

void Renderer::renderEditorUI() {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    // TODO: Render editor UI elements
    // - Tool palette
    // - Property panels
    // - Selection indicators
    // - Grid and snap indicators
}

void Renderer::renderMetrics() {
    if (!m_impl->initialized || !m_impl->renderTarget) return;
    
    // TODO: Render simulation metrics
    // - FPS counter
    // - Entity counts
    // - Traffic flow rates
    // - Performance metrics
    
    if (m_impl->debugMode) {
        m_impl->debugText.setString("RoadSim - Debug Mode");
        m_impl->debugText.setPosition(10, 10);
        m_impl->renderTarget->draw(m_impl->debugText);
    }
}

void Renderer::setCamera(float x, float y, float zoom) {
    if (!m_impl->initialized) return;
    
    m_impl->camera.setCenter(x, y);
    
    // Apply zoom by scaling the view size
    auto size = m_impl->renderTarget->getSize();
    m_impl->camera.setSize(
        static_cast<float>(size.x) / zoom,
        static_cast<float>(size.y) / zoom
    );
}

sf::Vector2f Renderer::screenToWorld(const sf::Vector2i& screenPos) const {
    if (!m_impl->initialized || !m_impl->renderTarget) {
        return sf::Vector2f(static_cast<float>(screenPos.x), static_cast<float>(screenPos.y));
    }
    
    return m_impl->renderTarget->mapPixelToCoords(screenPos, m_impl->camera);
}

sf::Vector2i Renderer::worldToScreen(const sf::Vector2f& worldPos) const {
    if (!m_impl->initialized || !m_impl->renderTarget) {
        return sf::Vector2i(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
    }
    
    return m_impl->renderTarget->mapCoordsToPixel(worldPos, m_impl->camera);
}

void Renderer::setDebugMode(bool enabled) {
    m_impl->debugMode = enabled;
    std::cout << "[Render] Debug mode " << (enabled ? "enabled" : "disabled") << std::endl;
}

} // namespace RoadSim::Render