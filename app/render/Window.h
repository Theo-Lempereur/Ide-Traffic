#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <functional>

namespace RoadSim::Render {

using EventCallback = std::function<void(const sf::Event&)>;

/**
 * @brief Window management for the application
 * Handles SFML window creation, events, and basic input
 */
class Window {
public:
    Window();
    ~Window();
    
    // Non-copyable
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    
    // Movable
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;
    
    /**
     * @brief Create and initialize the window
     * @param width Window width
     * @param height Window height
     * @param title Window title
     * @param fullscreen Create fullscreen window
     */
    bool create(unsigned int width, unsigned int height, const std::string& title, bool fullscreen = false);
    
    /**
     * @brief Close the window
     */
    void close();
    
    /**
     * @brief Check if window is open
     */
    bool isOpen() const;
    
    /**
     * @brief Poll events from the window
     */
    void pollEvents();
    
    /**
     * @brief Clear the window
     * @param color Clear color
     */
    void clear(const sf::Color& color = sf::Color::Black);
    
    /**
     * @brief Display the window contents
     */
    void display();
    
    /**
     * @brief Get the SFML render window
     */
    sf::RenderWindow* getRenderWindow();
    
    /**
     * @brief Get window size
     */
    sf::Vector2u getSize() const;
    
    /**
     * @brief Set window size
     * @param width New width
     * @param height New height
     */
    void setSize(unsigned int width, unsigned int height);
    
    /**
     * @brief Set window title
     * @param title New title
     */
    void setTitle(const std::string& title);
    
    /**
     * @brief Set framerate limit
     * @param limit FPS limit (0 for unlimited)
     */
    void setFramerateLimit(unsigned int limit);
    
    /**
     * @brief Enable/disable vertical sync
     * @param enabled VSync state
     */
    void setVerticalSyncEnabled(bool enabled);
    
    /**
     * @brief Set event callback for custom event handling
     * @param callback Event callback function
     */
    void setEventCallback(EventCallback callback);
    
    /**
     * @brief Get mouse position relative to window
     */
    sf::Vector2i getMousePosition() const;
    
    /**
     * @brief Check if key is currently pressed
     * @param key Key to check
     */
    bool isKeyPressed(sf::Keyboard::Key key) const;
    
    /**
     * @brief Check if mouse button is currently pressed
     * @param button Mouse button to check
     */
    bool isMouseButtonPressed(sf::Mouse::Button button) const;
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace RoadSim::Render