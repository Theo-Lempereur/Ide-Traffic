#pragma once

#include "GameObject.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

namespace RoadSim::Core {

/**
 * @brief Scene manager that contains and manages all GameObjects
 * Provides lifecycle management and querying capabilities
 */
class Scene {
public:
    Scene(const std::string& name = "Scene");
    ~Scene();
    
    // Non-copyable
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    
    // Movable
    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;
    
    /**
     * @brief Create a new GameObject in the scene
     * @param name GameObject name
     * @return Pointer to the created GameObject
     */
    GameObject* createGameObject(const std::string& name = "GameObject");
    
    /**
     * @brief Add an existing GameObject to the scene
     * @param gameObject Unique pointer to GameObject
     * @return Raw pointer to the GameObject
     */
    GameObject* addGameObject(std::unique_ptr<GameObject> gameObject);
    
    /**
     * @brief Remove a GameObject from the scene
     * @param gameObject Pointer to GameObject to remove
     * @return True if GameObject was found and removed
     */
    bool removeGameObject(GameObject* gameObject);
    
    /**
     * @brief Remove a GameObject by ID
     * @param id GameObject ID
     * @return True if GameObject was found and removed
     */
    bool removeGameObject(size_t id);
    
    /**
     * @brief Find a GameObject by name
     * @param name GameObject name
     * @return Pointer to GameObject or nullptr if not found
     */
    GameObject* findGameObject(const std::string& name) const;
    
    /**
     * @brief Find a GameObject by ID
     * @param id GameObject ID
     * @return Pointer to GameObject or nullptr if not found
     */
    GameObject* findGameObject(size_t id) const;
    
    /**
     * @brief Find all GameObjects with a specific component
     * @tparam T Component type
     * @return Vector of GameObjects with the component
     */
    template<typename T>
    std::vector<GameObject*> findGameObjectsWithComponent() const {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        
        std::vector<GameObject*> result;
        for (const auto& gameObject : m_gameObjects) {
            if (gameObject && gameObject->hasComponent<T>()) {
                result.push_back(gameObject.get());
            }
        }
        return result;
    }
    
    /**
     * @brief Find all GameObjects matching a predicate
     * @param predicate Function that returns true for matching GameObjects
     * @return Vector of matching GameObjects
     */
    std::vector<GameObject*> findGameObjects(std::function<bool(const GameObject*)> predicate) const;
    
    /**
     * @brief Get all GameObjects in the scene
     * @return Vector of all GameObjects
     */
    std::vector<GameObject*> getAllGameObjects() const;
    
    /**
     * @brief Get number of GameObjects in the scene
     */
    size_t getGameObjectCount() const { return m_gameObjects.size(); }
    
    /**
     * @brief Update all GameObjects in the scene
     * @param deltaTime Time elapsed since last frame
     */
    void update(float deltaTime);
    
    /**
     * @brief Fixed update for all GameObjects (physics)
     * @param deltaTime Fixed physics timestep
     */
    void fixedUpdate(float deltaTime);
    
    /**
     * @brief Clear all GameObjects from the scene
     */
    void clear();
    
    /**
     * @brief Get scene name
     */
    const std::string& getName() const { return m_name; }
    
    /**
     * @brief Set scene name
     */
    void setName(const std::string& name) { m_name = name; }
    
    /**
     * @brief Check if scene is active
     */
    bool isActive() const { return m_active; }
    
    /**
     * @brief Set scene active state
     */
    void setActive(bool active) { m_active = active; }
    
    /**
     * @brief Get scene statistics
     */
    struct Statistics {
        size_t totalGameObjects = 0;
        size_t activeGameObjects = 0;
        size_t totalComponents = 0;
        double lastUpdateTime = 0.0;
        double lastFixedUpdateTime = 0.0;
    };
    
    Statistics getStatistics() const { return m_statistics; }
    
private:
    std::string m_name;
    bool m_active = true;
    
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::unordered_map<size_t, GameObject*> m_gameObjectsById;
    
    mutable Statistics m_statistics;
    
    void updateStatistics() const;
};

} // namespace RoadSim::Core