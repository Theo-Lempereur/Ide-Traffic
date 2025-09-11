#include "UIManager.h"
#include <iostream>

namespace RoadSim::Render {

struct UIManager::Impl {
    sf::RenderTarget* renderTarget = nullptr;
    sf::Font font;
    bool fontLoaded = false;
    
    // UI Elements
    sf::RectangleShape simulationButton;
    sf::Text simulationButtonText;
    sf::Text modeIndicator;
    sf::Text debugText;
    
    // Layout
    unsigned int windowWidth = 1200;
    unsigned int windowHeight = 800;
    
    // Button properties
    sf::Vector2f buttonSize{120.0f, 40.0f};
    sf::Vector2f buttonPosition{0.0f, 0.0f}; // Will be calculated based on window size
    bool buttonHovered = false;
    bool buttonPressed = false;
    
    // Colors
    sf::Color developmentColor{100, 150, 255}; // Blue
    sf::Color runningColor{100, 255, 100};     // Green
    sf::Color pausedColor{255, 200, 100};     // Orange
    sf::Color buttonNormalColor{70, 70, 70};
    sf::Color buttonHoverColor{90, 90, 90};
    sf::Color buttonPressColor{50, 50, 50};
};

UIManager::UIManager() : m_impl(std::make_unique<Impl>()) {
    std::cout << "[UIManager] Created" << std::endl;
}

UIManager::~UIManager() {
    std::cout << "[UIManager] Destroyed" << std::endl;
}

void UIManager::initialize(sf::RenderTarget* target) {
    m_impl->renderTarget = target;
    
    // Try to load a font file, first from assets, then system fonts
    bool fontLoaded = false;
    
    // Try to load from assets folder
    if (m_impl->font.loadFromFile("assets/fonts/arial.ttf")) {
        fontLoaded = true;
    }
    // Try Windows system font
    else if (m_impl->font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        fontLoaded = true;
    }
    // Try another common Windows font
    else if (m_impl->font.loadFromFile("C:/Windows/Fonts/calibri.ttf")) {
        fontLoaded = true;
    }
    // Try Segoe UI
    else if (m_impl->font.loadFromFile("C:/Windows/Fonts/segoeui.ttf")) {
        fontLoaded = true;
    }
    
    if (fontLoaded) {
        m_impl->fontLoaded = true;
        std::cout << "[UIManager] Font loaded successfully" << std::endl;
    } else {
        std::cout << "[UIManager] Warning: Could not load any font file, text will not be visible" << std::endl;
        m_impl->fontLoaded = false;
    }
    
    createUI();
    std::cout << "[UIManager] Initialized" << std::endl;
}

void UIManager::createUI() {
    // Calculate button position (top-right corner)
    float buttonX = m_impl->windowWidth - m_impl->buttonSize.x - 20.0f;
    float buttonY = 20.0f;
    m_impl->buttonPosition = sf::Vector2f(buttonX, buttonY);
    
    // Setup simulation button
    m_impl->simulationButton.setSize(m_impl->buttonSize);
    m_impl->simulationButton.setPosition(m_impl->buttonPosition);
    m_impl->simulationButton.setFillColor(m_impl->buttonNormalColor);
    m_impl->simulationButton.setOutlineThickness(2.0f);
    m_impl->simulationButton.setOutlineColor(sf::Color::White);
    
    // Setup button text
    if (m_impl->fontLoaded) {
        m_impl->simulationButtonText.setFont(m_impl->font);
    }
    m_impl->simulationButtonText.setCharacterSize(16);
    m_impl->simulationButtonText.setFillColor(sf::Color::White);
    
    // Setup mode indicator (top-left corner)
    if (m_impl->fontLoaded) {
        m_impl->modeIndicator.setFont(m_impl->font);
    }
    m_impl->modeIndicator.setCharacterSize(20);
    m_impl->modeIndicator.setPosition(20.0f, 20.0f);
    
    // Setup debug text
    if (m_impl->fontLoaded) {
        m_impl->debugText.setFont(m_impl->font);
    }
    m_impl->debugText.setCharacterSize(12);
    m_impl->debugText.setFillColor(sf::Color::Yellow);
    m_impl->debugText.setPosition(20.0f, m_impl->windowHeight - 100.0f);
    
    updateSimulationButton();
    updateModeIndicator();
}

void UIManager::handleEvent(const sf::Event& event) {
    if (!m_impl->renderTarget) return;
    
    switch (event.type) {
        case sf::Event::MouseMoved: {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            sf::FloatRect buttonBounds = m_impl->simulationButton.getGlobalBounds();
            
            bool wasHovered = m_impl->buttonHovered;
            m_impl->buttonHovered = buttonBounds.contains(mousePos);
            
            if (m_impl->buttonHovered != wasHovered) {
                m_impl->simulationButton.setFillColor(
                    m_impl->buttonHovered ? m_impl->buttonHoverColor : m_impl->buttonNormalColor
                );
            }
            break;
        }
        
        case sf::Event::MouseButtonPressed: {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                sf::FloatRect buttonBounds = m_impl->simulationButton.getGlobalBounds();
                
                if (buttonBounds.contains(mousePos)) {
                    m_impl->buttonPressed = true;
                    m_impl->simulationButton.setFillColor(m_impl->buttonPressColor);
                }
            }
            break;
        }
        
        case sf::Event::MouseButtonReleased: {
            if (event.mouseButton.button == sf::Mouse::Left && m_impl->buttonPressed) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                sf::FloatRect buttonBounds = m_impl->simulationButton.getGlobalBounds();
                
                m_impl->buttonPressed = false;
                
                if (buttonBounds.contains(mousePos)) {
                    // Toggle simulation state
                    SimulationState newState;
                    switch (m_simulationState) {
                        case SimulationState::Development:
                            newState = SimulationState::Running;
                            break;
                        case SimulationState::Running:
                            newState = SimulationState::Development;
                            break;
                        case SimulationState::Paused:
                            newState = SimulationState::Running;
                            break;
                    }
                    
                    setSimulationState(newState);
                    
                    if (m_simulationCallback) {
                        m_simulationCallback(newState);
                    }
                    
                    std::cout << "[UIManager] Simulation state changed to: " << getSimulationStateText() << std::endl;
                }
                
                m_impl->simulationButton.setFillColor(
                    m_impl->buttonHovered ? m_impl->buttonHoverColor : m_impl->buttonNormalColor
                );
            }
            break;
        }
        
        default:
            break;
    }
}

void UIManager::update(float deltaTime) {
    (void)deltaTime; // Suppress unused parameter warning
    
    // Update UI animations or states if needed
    updateModeIndicator();
    updateSimulationButton();
    
    if (m_showDebugInfo) {
        m_impl->debugText.setString(m_debugInfo);
    }
}

void UIManager::render() {
    if (!m_impl->renderTarget) return;
    
    // Render mode indicator
    m_impl->renderTarget->draw(m_impl->modeIndicator);
    
    // Render simulation button
    m_impl->renderTarget->draw(m_impl->simulationButton);
    m_impl->renderTarget->draw(m_impl->simulationButtonText);
    
    // Render debug info if enabled
    if (m_showDebugInfo && !m_debugInfo.empty()) {
        m_impl->renderTarget->draw(m_impl->debugText);
    }
}

void UIManager::setSimulationState(SimulationState state) {
    m_simulationState = state;
    updateSimulationButton();
    updateModeIndicator();
}

void UIManager::setWindowSize(unsigned int width, unsigned int height) {
    m_impl->windowWidth = width;
    m_impl->windowHeight = height;
    
    // Recalculate button position (top-right corner)
    float buttonX = static_cast<float>(width) - m_impl->buttonSize.x - 20.0f;
    float buttonY = 20.0f;
    m_impl->buttonPosition = sf::Vector2f(buttonX, buttonY);
    m_impl->simulationButton.setPosition(m_impl->buttonPosition);
    
    // Update debug text position
    m_impl->debugText.setPosition(20.0f, static_cast<float>(height) - 100.0f);
}

void UIManager::updateSimulationButton() {
    std::string buttonText = getButtonText();
    m_impl->simulationButtonText.setString(buttonText);
    
    // Center text in button
    sf::FloatRect textBounds = m_impl->simulationButtonText.getLocalBounds();
    sf::Vector2f buttonPos = m_impl->simulationButton.getPosition();
    sf::Vector2f buttonSize = m_impl->simulationButton.getSize();
    
    m_impl->simulationButtonText.setPosition(
        buttonPos.x + (buttonSize.x - textBounds.width) * 0.5f - textBounds.left,
        buttonPos.y + (buttonSize.y - textBounds.height) * 0.5f - textBounds.top
    );
}

void UIManager::updateModeIndicator() {
    std::string modeText = "Mode: " + getSimulationStateText();
    m_impl->modeIndicator.setString(modeText);
    m_impl->modeIndicator.setFillColor(getStateColor());
}

std::string UIManager::getSimulationStateText() const {
    switch (m_simulationState) {
        case SimulationState::Development: return "Development";
        case SimulationState::Running: return "Running";
        case SimulationState::Paused: return "Paused";
    }
    return "Unknown";
}

std::string UIManager::getButtonText() const {
    switch (m_simulationState) {
        case SimulationState::Development: return "Start Sim";
        case SimulationState::Running: return "Stop Sim";
        case SimulationState::Paused: return "Resume";
    }
    return "Unknown";
}

sf::Color UIManager::getStateColor() const {
    switch (m_simulationState) {
        case SimulationState::Development: return m_impl->developmentColor;
        case SimulationState::Running: return m_impl->runningColor;
        case SimulationState::Paused: return m_impl->pausedColor;
    }
    return sf::Color::White;
}

} // namespace RoadSim::Render