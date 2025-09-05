#include "Window.h"
#include <iostream>

namespace RoadSim::Render {

struct Window::Impl {
    std::unique_ptr<sf::RenderWindow> window;
    EventCallback eventCallback;
    bool initialized = false;
    std::string currentTitle;
};

Window::Window() : m_impl(std::make_unique<Impl>()) {
    std::cout << "[Render] Window created" << std::endl;
}

Window::~Window() {
    if (m_impl->window && m_impl->window->isOpen()) {
        m_impl->window->close();
    }
    std::cout << "[Render] Window destroyed" << std::endl;
}

bool Window::create(unsigned int width, unsigned int height, const std::string& title, bool fullscreen) {
    std::cout << "[Render] Creating window: " << width << "x" << height << " - " << title << std::endl;
    
    try {
        sf::Uint32 style = fullscreen ? sf::Style::Fullscreen : (sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
        
        m_impl->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title, style);
        
        if (!m_impl->window->isOpen()) {
            std::cerr << "[Render] Failed to create window" << std::endl;
            return false;
        }
        
        m_impl->currentTitle = title;
        m_impl->initialized = true;
        
        // Set default window properties
        m_impl->window->setFramerateLimit(60);
        m_impl->window->setVerticalSyncEnabled(false);
        
        std::cout << "[Render] Window created successfully" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[Render] Exception creating window: " << e.what() << std::endl;
        return false;
    }
}

void Window::close() {
    if (m_impl->window && m_impl->window->isOpen()) {
        std::cout << "[Render] Closing window" << std::endl;
        m_impl->window->close();
    }
}

bool Window::isOpen() const {
    return m_impl->window && m_impl->window->isOpen();
}

void Window::pollEvents() {
    if (!m_impl->window) return;
    
    sf::Event event;
    while (m_impl->window->pollEvent(event)) {
        // Handle default events
        switch (event.type) {
            case sf::Event::Closed:
                close();
                break;
                
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    close();
                }
                break;
                
            case sf::Event::Resized:
                // Update window view on resize
                m_impl->window->setView(sf::View(sf::FloatRect(0, 0, 
                    static_cast<float>(event.size.width), 
                    static_cast<float>(event.size.height))));
                break;
                
            default:
                break;
        }
        
        // Call custom event callback if set
        if (m_impl->eventCallback) {
            m_impl->eventCallback(event);
        }
    }
}

void Window::clear(const sf::Color& color) {
    if (m_impl->window) {
        m_impl->window->clear(color);
    }
}

void Window::display() {
    if (m_impl->window) {
        m_impl->window->display();
    }
}

sf::RenderWindow* Window::getRenderWindow() {
    return m_impl->window.get();
}

sf::Vector2u Window::getSize() const {
    if (m_impl->window) {
        return m_impl->window->getSize();
    }
    return sf::Vector2u(0, 0);
}

void Window::setSize(unsigned int width, unsigned int height) {
    if (m_impl->window) {
        m_impl->window->setSize(sf::Vector2u(width, height));
        std::cout << "[Render] Window resized to " << width << "x" << height << std::endl;
    }
}

void Window::setTitle(const std::string& title) {
    if (m_impl->window) {
        m_impl->window->setTitle(title);
        m_impl->currentTitle = title;
        std::cout << "[Render] Window title changed to: " << title << std::endl;
    }
}

void Window::setFramerateLimit(unsigned int limit) {
    if (m_impl->window) {
        m_impl->window->setFramerateLimit(limit);
        std::cout << "[Render] Framerate limit set to " << limit << " FPS" << std::endl;
    }
}

void Window::setVerticalSyncEnabled(bool enabled) {
    if (m_impl->window) {
        m_impl->window->setVerticalSyncEnabled(enabled);
        std::cout << "[Render] VSync " << (enabled ? "enabled" : "disabled") << std::endl;
    }
}

void Window::setEventCallback(EventCallback callback) {
    m_impl->eventCallback = std::move(callback);
    std::cout << "[Render] Event callback set" << std::endl;
}

sf::Vector2i Window::getMousePosition() const {
    if (m_impl->window) {
        return sf::Mouse::getPosition(*m_impl->window);
    }
    return sf::Vector2i(0, 0);
}

bool Window::isKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key);
}

bool Window::isMouseButtonPressed(sf::Mouse::Button button) const {
    return sf::Mouse::isButtonPressed(button);
}

} // namespace RoadSim::Render