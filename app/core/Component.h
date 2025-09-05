#pragma once

#include <memory>
#include <typeinfo>
#include <string>

namespace RoadSim::Core {

class GameObject;

/**
 * @brief Base class for all components that can be attached to GameObjects
 * Provides a Unity-like component system architecture
 */
class Component {
public:
    Component() = default;
    virtual ~Component() = default;
    
    // Non-copyable
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    
    // Movable
    Component(Component&&) = default;
    Component& operator=(Component&&) = default;
    
    /**
     * @brief Called when component is added to a GameObject
     */
    virtual void onAttach() {}
    
    /**
     * @brief Called when component is removed from a GameObject
     */
    virtual void onDetach() {}
    
    /**
     * @brief Called every frame to update the component
     * @param deltaTime Time elapsed since last frame
     */
    virtual void update(float deltaTime) { (void)deltaTime; }
    
    /**
     * @brief Called for physics updates (fixed timestep)
     * @param deltaTime Fixed physics timestep
     */
    virtual void fixedUpdate(float deltaTime) { (void)deltaTime; }
    
    /**
     * @brief Get the GameObject this component is attached to
     */
    GameObject* getGameObject() const { return m_gameObject; }
    
    /**
     * @brief Check if component is active
     */
    bool isActive() const { return m_active; }
    
    /**
     * @brief Set component active state
     */
    void setActive(bool active) { m_active = active; }
    
    /**
     * @brief Get component type name for debugging
     */
    virtual std::string getTypeName() const { return typeid(*this).name(); }
    
protected:
    friend class GameObject;
    
    /**
     * @brief Set the GameObject this component belongs to (called by GameObject)
     */
    void setGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
    
private:
    GameObject* m_gameObject = nullptr;
    bool m_active = true;
};

} // namespace RoadSim::Core