#pragma once

#include "Component.h"
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <iostream>

namespace RoadSim::Core {

/**
 * @brief Base class for all objects in the simulation scene
 * Provides a Unity-like GameObject system with component architecture
 */
class GameObject {
public:
    GameObject(const std::string& name = "GameObject");
    virtual ~GameObject();
    
    // Non-copyable
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    
    // Movable
    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;
    
    /**
     * @brief Add a component to this GameObject
     * @tparam T Component type
     * @tparam Args Constructor arguments
     * @return Pointer to the created component
     */
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        
        auto typeIndex = std::type_index(typeid(T));
        
        // Check if component already exists
        if (m_components.find(typeIndex) != m_components.end()) {
            std::cerr << "[GameObject] Component " << typeid(T).name() << " already exists on " << m_name << std::endl;
            return static_cast<T*>(m_components[typeIndex].get());
        }
        
        // Create and add component
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* componentPtr = component.get();
        
        component->setGameObject(this);
        m_components[typeIndex] = std::move(component);
        
        componentPtr->onAttach();
        
        std::cout << "[GameObject] Added component " << typeid(T).name() << " to " << m_name << std::endl;
        return componentPtr;
    }
    
    /**
     * @brief Get a component of specified type
     * @tparam T Component type
     * @return Pointer to component or nullptr if not found
     */
    template<typename T>
    T* getComponent() const {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        
        auto typeIndex = std::type_index(typeid(T));
        auto it = m_components.find(typeIndex);
        
        if (it != m_components.end()) {
            return static_cast<T*>(it->second.get());
        }
        
        return nullptr;
    }
    
    /**
     * @brief Remove a component of specified type
     * @tparam T Component type
     * @return True if component was removed
     */
    template<typename T>
    bool removeComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        
        auto typeIndex = std::type_index(typeid(T));
        auto it = m_components.find(typeIndex);
        
        if (it != m_components.end()) {
            it->second->onDetach();
            m_components.erase(it);
            std::cout << "[GameObject] Removed component " << typeid(T).name() << " from " << m_name << std::endl;
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief Check if GameObject has a component of specified type
     * @tparam T Component type
     */
    template<typename T>
    bool hasComponent() const {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        
        auto typeIndex = std::type_index(typeid(T));
        return m_components.find(typeIndex) != m_components.end();
    }
    
    /**
     * @brief Update all components
     * @param deltaTime Time elapsed since last frame
     */
    virtual void update(float deltaTime);
    
    /**
     * @brief Fixed update for all components (physics)
     * @param deltaTime Fixed physics timestep
     */
    virtual void fixedUpdate(float deltaTime);
    
    /**
     * @brief Get GameObject name
     */
    const std::string& getName() const { return m_name; }
    
    /**
     * @brief Set GameObject name
     */
    void setName(const std::string& name) { m_name = name; }
    
    /**
     * @brief Get GameObject active state
     */
    bool isActive() const { return m_active; }
    
    /**
     * @brief Set GameObject active state
     */
    void setActive(bool active) { m_active = active; }
    
    /**
     * @brief Get GameObject position
     */
    const sf::Vector2f& getPosition() const { return m_position; }
    
    /**
     * @brief Set GameObject position
     */
    void setPosition(const sf::Vector2f& position) { m_position = position; }
    
    /**
     * @brief Get GameObject rotation (in degrees)
     */
    float getRotation() const { return m_rotation; }
    
    /**
     * @brief Set GameObject rotation (in degrees)
     */
    void setRotation(float rotation) { m_rotation = rotation; }
    
    /**
     * @brief Get GameObject scale
     */
    const sf::Vector2f& getScale() const { return m_scale; }
    
    /**
     * @brief Set GameObject scale
     */
    void setScale(const sf::Vector2f& scale) { m_scale = scale; }
    
    /**
     * @brief Get unique GameObject ID
     */
    size_t getId() const { return m_id; }
    
private:
    static size_t s_nextId;
    
    size_t m_id;
    std::string m_name;
    bool m_active = true;
    
    // Transform data
    sf::Vector2f m_position{0.0f, 0.0f};
    float m_rotation = 0.0f;
    sf::Vector2f m_scale{1.0f, 1.0f};
    
    // Component storage
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
};

} // namespace RoadSim::Core