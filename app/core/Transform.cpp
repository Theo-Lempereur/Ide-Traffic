#include "Transform.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace RoadSim::Core {

Transform::Transform(const sf::Vector2f& position, float rotation, const sf::Vector2f& scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {
}



sf::Vector2f Transform::getForward() const {
    float radians = m_rotation * static_cast<float>(M_PI) / 180.0f;
    return sf::Vector2f(std::cos(radians), std::sin(radians));
}

sf::Vector2f Transform::getRight() const {
    float radians = (m_rotation + 90.0f) * static_cast<float>(M_PI) / 180.0f;
    return sf::Vector2f(std::cos(radians), std::sin(radians));
}

const sf::Transform& Transform::getTransformMatrix() const {
    if (m_transformDirty) {
        updateTransformMatrix();
        m_transformDirty = false;
    }
    return m_transformMatrix;
}

sf::Vector2f Transform::transformPoint(const sf::Vector2f& localPoint) const {
    return getTransformMatrix().transformPoint(localPoint);
}

sf::Vector2f Transform::inverseTransformPoint(const sf::Vector2f& worldPoint) const {
    return getTransformMatrix().getInverse().transformPoint(worldPoint);
}

void Transform::updateTransformMatrix() const {
    m_transformMatrix = sf::Transform::Identity;
    
    // Apply transformations in order: scale, rotate, translate
    m_transformMatrix.scale(m_scale);
    m_transformMatrix.rotate(m_rotation);
    m_transformMatrix.translate(m_position);
}

} // namespace RoadSim::Core