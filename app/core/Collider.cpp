#include "Collider.h"
#include "Transform.h"
#include "GameObject.h"
#include <cmath>
#include <algorithm>

namespace RoadSim::Core {

// Base Collider implementation
Collider::Collider(ColliderType type) : m_type(type) {
}

void Collider::onAttach() {
    // Get Transform component from the same GameObject
    m_transform = getGameObject()->getComponent<Transform>();
    if (!m_transform) {
        // If no Transform exists, create one
        m_transform = getGameObject()->addComponent<Transform>();
    }
}

// BoxCollider implementation
BoxCollider::BoxCollider(const sf::Vector2f& size, const sf::Vector2f& offset)
    : Collider(ColliderType::Box), m_size(size), m_offset(offset) {
}

bool BoxCollider::intersects(const Collider& other) const {
    if (!getTransform()) return false;
    
    switch (other.getType()) {
        case ColliderType::Box: {
            const BoxCollider& otherBox = static_cast<const BoxCollider&>(other);
            sf::FloatRect thisBounds = getBounds();
            sf::FloatRect otherBounds = otherBox.getBounds();
            return thisBounds.intersects(otherBounds);
        }
        case ColliderType::Circle: {
            const CircleCollider& otherCircle = static_cast<const CircleCollider&>(other);
            
            // Get circle center
            sf::Vector2f circleCenter = otherCircle.getGameObject()->getPosition() + otherCircle.getOffset();
            float radius = otherCircle.getRadius();
            
            // Get box bounds
            sf::FloatRect boxBounds = getBounds();
            
            // Find closest point on box to circle center
            float closestX = std::max(boxBounds.left, std::min(circleCenter.x, boxBounds.left + boxBounds.width));
            float closestY = std::max(boxBounds.top, std::min(circleCenter.y, boxBounds.top + boxBounds.height));
            
            // Calculate distance
            float dx = circleCenter.x - closestX;
            float dy = circleCenter.y - closestY;
            float distanceSquared = dx * dx + dy * dy;
            
            return distanceSquared <= (radius * radius);
        }
    }
    return false;
}

sf::FloatRect BoxCollider::getBounds() const {
    if (!getTransform()) return sf::FloatRect();
    
    sf::Vector2f position = getTransform()->getPosition() + m_offset;
    sf::Vector2f scale = getTransform()->getScale();
    sf::Vector2f scaledSize = {m_size.x * scale.x, m_size.y * scale.y};
    
    return sf::FloatRect(
        position.x - scaledSize.x * 0.5f,
        position.y - scaledSize.y * 0.5f,
        scaledSize.x,
        scaledSize.y
    );
}

bool BoxCollider::containsPoint(const sf::Vector2f& point) const {
    sf::FloatRect bounds = getBounds();
    return bounds.contains(point);
}

// CircleCollider implementation
CircleCollider::CircleCollider(float radius, const sf::Vector2f& offset)
    : Collider(ColliderType::Circle), m_radius(radius), m_offset(offset) {
}

bool CircleCollider::intersects(const Collider& other) const {
    if (!getTransform()) return false;
    
    switch (other.getType()) {
        case ColliderType::Circle: {
            const CircleCollider& otherCircle = static_cast<const CircleCollider&>(other);
            
            sf::Vector2f thisCenter = getTransform()->getPosition() + m_offset;
        sf::Vector2f otherCenter = otherCircle.getGameObject()->getPosition() + otherCircle.m_offset;
        
        float thisRadius = m_radius * std::max(getTransform()->getScale().x, getTransform()->getScale().y);
        float otherRadius = otherCircle.m_radius * std::max(otherCircle.getGameObject()->getScale().x, otherCircle.getGameObject()->getScale().y);
            
            float dx = thisCenter.x - otherCenter.x;
            float dy = thisCenter.y - otherCenter.y;
            float distanceSquared = dx * dx + dy * dy;
            float radiusSum = thisRadius + otherRadius;
            
            return distanceSquared <= (radiusSum * radiusSum);
        }
        case ColliderType::Box: {
            // Use box-circle intersection (implemented in BoxCollider)
            return other.intersects(*this);
        }
    }
    return false;
}

sf::FloatRect CircleCollider::getBounds() const {
    if (!getTransform()) return sf::FloatRect();
    
    sf::Vector2f position = getTransform()->getPosition() + m_offset;
    float scaledRadius = m_radius * std::max(getTransform()->getScale().x, getTransform()->getScale().y);
    
    return sf::FloatRect(
        position.x - scaledRadius,
        position.y - scaledRadius,
        scaledRadius * 2.0f,
        scaledRadius * 2.0f
    );
}

bool CircleCollider::containsPoint(const sf::Vector2f& point) const {
    if (!getTransform()) return false;
    
    sf::Vector2f center = getTransform()->getPosition() + m_offset;
    float scaledRadius = m_radius * std::max(getTransform()->getScale().x, getTransform()->getScale().y);
    
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    float distanceSquared = dx * dx + dy * dy;
    
    return distanceSquared <= (scaledRadius * scaledRadius);
}

} // namespace RoadSim::Core