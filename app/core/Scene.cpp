#include "Scene.h"
#include <iostream>
#include <algorithm>
#include <chrono>

namespace RoadSim::Core {

Scene::Scene(const std::string& name) : m_name(name) {
    std::cout << "[Scene] Created scene: " << m_name << std::endl;
}

Scene::~Scene() {
    std::cout << "[Scene] Destroying scene: " << m_name << std::endl;
    clear();
}

GameObject* Scene::createGameObject(const std::string& name) {
    auto gameObject = std::make_unique<GameObject>(name);
    GameObject* ptr = gameObject.get();
    
    m_gameObjectsById[ptr->getId()] = ptr;
    m_gameObjects.push_back(std::move(gameObject));
    
    std::cout << "[Scene] Created GameObject " << name << " in scene " << m_name << std::endl;
    return ptr;
}

GameObject* Scene::addGameObject(std::unique_ptr<GameObject> gameObject) {
    if (!gameObject) return nullptr;
    
    GameObject* ptr = gameObject.get();
    m_gameObjectsById[ptr->getId()] = ptr;
    m_gameObjects.push_back(std::move(gameObject));
    
    std::cout << "[Scene] Added GameObject " << ptr->getName() << " to scene " << m_name << std::endl;
    return ptr;
}

bool Scene::removeGameObject(GameObject* gameObject) {
    if (!gameObject) return false;
    
    return removeGameObject(gameObject->getId());
}

bool Scene::removeGameObject(size_t id) {
    // Remove from ID map
    auto idIt = m_gameObjectsById.find(id);
    if (idIt == m_gameObjectsById.end()) {
        return false;
    }
    
    GameObject* gameObject = idIt->second;
    m_gameObjectsById.erase(idIt);
    
    // Remove from vector
    auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
        [id](const std::unique_ptr<GameObject>& obj) {
            return obj && obj->getId() == id;
        });
    
    if (it != m_gameObjects.end()) {
        std::cout << "[Scene] Removed GameObject " << (*it)->getName() << " from scene " << m_name << std::endl;
        m_gameObjects.erase(it);
        return true;
    }
    
    return false;
}

GameObject* Scene::findGameObject(const std::string& name) const {
    for (const auto& gameObject : m_gameObjects) {
        if (gameObject && gameObject->getName() == name) {
            return gameObject.get();
        }
    }
    return nullptr;
}

GameObject* Scene::findGameObject(size_t id) const {
    auto it = m_gameObjectsById.find(id);
    return (it != m_gameObjectsById.end()) ? it->second : nullptr;
}

std::vector<GameObject*> Scene::findGameObjects(std::function<bool(const GameObject*)> predicate) const {
    std::vector<GameObject*> result;
    
    for (const auto& gameObject : m_gameObjects) {
        if (gameObject && predicate(gameObject.get())) {
            result.push_back(gameObject.get());
        }
    }
    
    return result;
}

std::vector<GameObject*> Scene::getAllGameObjects() const {
    std::vector<GameObject*> result;
    result.reserve(m_gameObjects.size());
    
    for (const auto& gameObject : m_gameObjects) {
        if (gameObject) {
            result.push_back(gameObject.get());
        }
    }
    
    return result;
}

void Scene::update(float deltaTime) {
    if (!m_active) return;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Update all active GameObjects
    for (const auto& gameObject : m_gameObjects) {
        if (gameObject && gameObject->isActive()) {
            gameObject->update(deltaTime);
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    m_statistics.lastUpdateTime = std::chrono::duration<double>(endTime - startTime).count();
    
    updateStatistics();
}

void Scene::fixedUpdate(float deltaTime) {
    if (!m_active) return;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Fixed update for all active GameObjects
    for (const auto& gameObject : m_gameObjects) {
        if (gameObject && gameObject->isActive()) {
            gameObject->fixedUpdate(deltaTime);
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    m_statistics.lastFixedUpdateTime = std::chrono::duration<double>(endTime - startTime).count();
}

void Scene::clear() {
    std::cout << "[Scene] Clearing " << m_gameObjects.size() << " GameObjects from scene " << m_name << std::endl;
    
    m_gameObjects.clear();
    m_gameObjectsById.clear();
    
    updateStatistics();
}

void Scene::updateStatistics() const {
    m_statistics.totalGameObjects = m_gameObjects.size();
    m_statistics.activeGameObjects = 0;
    m_statistics.totalComponents = 0;
    
    for (const auto& gameObject : m_gameObjects) {
        if (gameObject) {
            if (gameObject->isActive()) {
                m_statistics.activeGameObjects++;
            }
            // Note: We can't easily count components without exposing internal structure
            // This could be improved by adding a getComponentCount() method to GameObject
        }
    }
}

} // namespace RoadSim::Core