#include "GameObject.h"
#include <iostream>

namespace RoadSim::Core {

// Static member initialization
size_t GameObject::s_nextId = 1;

GameObject::GameObject(const std::string& name) 
    : m_id(s_nextId++), m_name(name) {
    std::cout << "[GameObject] Created " << m_name << " (ID: " << m_id << ")" << std::endl;
}

GameObject::~GameObject() {
    std::cout << "[GameObject] Destroying " << m_name << " (ID: " << m_id << ")" << std::endl;
    
    // Components will be automatically destroyed due to unique_ptr
    // But we call onDetach for cleanup
    for (auto& [typeIndex, component] : m_components) {
        if (component) {
            component->onDetach();
        }
    }
    
    m_components.clear();
}

void GameObject::update(float deltaTime) {
    if (!m_active) return;
    
    // Update all active components
    for (auto& [typeIndex, component] : m_components) {
        if (component && component->isActive()) {
            component->update(deltaTime);
        }
    }
}

void GameObject::fixedUpdate(float deltaTime) {
    if (!m_active) return;
    
    // Fixed update for all active components
    for (auto& [typeIndex, component] : m_components) {
        if (component && component->isActive()) {
            component->fixedUpdate(deltaTime);
        }
    }
}

} // namespace RoadSim::Core