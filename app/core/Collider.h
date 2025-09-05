#pragma once

#include "Component.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <memory>

namespace RoadSim::Core {

class Transform;

/**
 * @brief Base collider component for collision detection
 * Provides different collision shapes for GameObjects
 */
class Collider : public Component {
public:
    enum class ColliderType {
        Box,
        Circle
    };
    
    Collider(ColliderType type);
    virtual ~Collider() = default;
    
    /**
     * @brief Get collider type
     */
    ColliderType getType() const { return m_type; }
    
    /**
     * @brief Check if this collider intersects with another
     */
    virtual bool intersects(const Collider& other) const = 0;
    
    /**
     * @brief Get world bounds of the collider
     */
    virtual sf::FloatRect getBounds() const = 0;
    
    /**
     * @brief Check if a point is inside the collider
     */
    virtual bool containsPoint(const sf::Vector2f& point) const = 0;
    
    /**
     * @brief Set collider as trigger (no physical collision, only detection)
     */
    void setTrigger(bool isTrigger) { m_isTrigger = isTrigger; }
    
    /**
     * @brief Check if collider is a trigger
     */
    bool isTrigger() const { return m_isTrigger; }
    
    /**
     * @brief Set collision layer
     */
    void setLayer(int layer) { m_layer = layer; }
    
    /**
     * @brief Get collision layer
     */
    int getLayer() const { return m_layer; }
    
    // Component overrides
    void onAttach() override;
    std::string getTypeName() const override { return "Collider"; }
    
protected:
    Transform* getTransform() const { return m_transform; }
    
private:
    ColliderType m_type;
    bool m_isTrigger = false;
    int m_layer = 0;
    Transform* m_transform = nullptr;
};

/**
 * @brief Box collider for rectangular collision detection
 */
class BoxCollider : public Collider {
public:
    BoxCollider(const sf::Vector2f& size = {1.0f, 1.0f}, const sf::Vector2f& offset = {0.0f, 0.0f});
    
    /**
     * @brief Set collider size
     */
    void setSize(const sf::Vector2f& size) { m_size = size; }
    
    /**
     * @brief Get collider size
     */
    const sf::Vector2f& getSize() const { return m_size; }
    
    /**
     * @brief Set collider offset from GameObject center
     */
    void setOffset(const sf::Vector2f& offset) { m_offset = offset; }
    
    /**
     * @brief Get collider offset
     */
    const sf::Vector2f& getOffset() const { return m_offset; }
    
    // Collider overrides
    bool intersects(const Collider& other) const override;
    sf::FloatRect getBounds() const override;
    bool containsPoint(const sf::Vector2f& point) const override;
    
    std::string getTypeName() const override { return "BoxCollider"; }
    
private:
    sf::Vector2f m_size;
    sf::Vector2f m_offset;
};

/**
 * @brief Circle collider for circular collision detection
 */
class CircleCollider : public Collider {
public:
    CircleCollider(float radius = 0.5f, const sf::Vector2f& offset = {0.0f, 0.0f});
    
    /**
     * @brief Set collider radius
     */
    void setRadius(float radius) { m_radius = radius; }
    
    /**
     * @brief Get collider radius
     */
    float getRadius() const { return m_radius; }
    
    /**
     * @brief Set collider offset from GameObject center
     */
    void setOffset(const sf::Vector2f& offset) { m_offset = offset; }
    
    /**
     * @brief Get collider offset
     */
    const sf::Vector2f& getOffset() const { return m_offset; }
    
    // Collider overrides
    bool intersects(const Collider& other) const override;
    sf::FloatRect getBounds() const override;
    bool containsPoint(const sf::Vector2f& point) const override;
    
    std::string getTypeName() const override { return "CircleCollider"; }
    
private:
    float m_radius;
    sf::Vector2f m_offset;
};

} // namespace RoadSim::Core