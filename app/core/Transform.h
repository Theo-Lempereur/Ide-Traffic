#pragma once

#include "Component.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace RoadSim::Core {

/**
 * @brief Transform component for position, rotation, and scale
 * Essential component for spatial representation of GameObjects
 */
class Transform : public Component {
public:
    Transform(const sf::Vector2f& position = {0.0f, 0.0f}, 
              float rotation = 0.0f, 
              const sf::Vector2f& scale = {1.0f, 1.0f});
    
    virtual ~Transform() = default;
    
    /**
     * @brief Get world position
     */
    const sf::Vector2f& getPosition() const { return m_position; }
    
    /**
     * @brief Set world position
     */
    void setPosition(const sf::Vector2f& position) { 
        m_position = position;
        m_transformDirty = true;
    }
    
    /**
     * @brief Set world position
     */
    void setPosition(float x, float y) { 
        setPosition({x, y});
    }
    
    /**
     * @brief Translate by offset
     */
    void translate(const sf::Vector2f& offset) {
        m_position += offset;
        m_transformDirty = true;
    }
    
    /**
     * @brief Get rotation in degrees
     */
    float getRotation() const { return m_rotation; }
    
    /**
     * @brief Set rotation in degrees
     */
    void setRotation(float rotation) { 
        m_rotation = rotation;
        m_transformDirty = true;
    }
    
    /**
     * @brief Rotate by angle in degrees
     */
    void rotate(float angle) {
        m_rotation += angle;
        m_transformDirty = true;
    }
    
    /**
     * @brief Get scale
     */
    const sf::Vector2f& getScale() const { return m_scale; }
    
    /**
     * @brief Set scale
     */
    void setScale(const sf::Vector2f& scale) { 
        m_scale = scale;
        m_transformDirty = true;
    }
    
    /**
     * @brief Set uniform scale
     */
    void setScale(float scale) { 
        setScale({scale, scale});
    }
    
    /**
     * @brief Get forward direction vector (based on rotation)
     */
    sf::Vector2f getForward() const;
    
    /**
     * @brief Get right direction vector (based on rotation)
     */
    sf::Vector2f getRight() const;
    
    /**
     * @brief Get SFML transform matrix for rendering
     */
    const sf::Transform& getTransformMatrix() const;
    
    /**
     * @brief Transform a point from local to world space
     */
    sf::Vector2f transformPoint(const sf::Vector2f& localPoint) const;
    
    /**
     * @brief Transform a point from world to local space
     */
    sf::Vector2f inverseTransformPoint(const sf::Vector2f& worldPoint) const;
    
    // Component overrides
    std::string getTypeName() const override { return "Transform"; }
    
private:
    sf::Vector2f m_position;
    float m_rotation; // in degrees
    sf::Vector2f m_scale;
    
    // Cached transform matrix
    mutable sf::Transform m_transformMatrix;
    mutable bool m_transformDirty = true;
    
    void updateTransformMatrix() const;
};

} // namespace RoadSim::Core