#pragma once

#include <memory>
#include <string>
#include <vector>

namespace RoadSim::Editor {

/**
 * @brief Entity editor for configuring traffic entities and scenarios
 * Handles vehicle profiles, pedestrian behavior, and traffic scenarios
 */
class EntityEditor {
public:
    EntityEditor();
    ~EntityEditor();
    
    // Non-copyable
    EntityEditor(const EntityEditor&) = delete;
    EntityEditor& operator=(const EntityEditor&) = delete;
    
    // Movable
    EntityEditor(EntityEditor&&) = default;
    EntityEditor& operator=(EntityEditor&&) = default;
    
    /**
     * @brief Initialize the entity editor
     */
    void initialize();
    
    /**
     * @brief Create new vehicle profile
     * @param name Profile name
     * @param maxSpeed Maximum speed
     * @param acceleration Acceleration rate
     * @param size Vehicle size
     */
    void createVehicleProfile(const std::string& name, float maxSpeed, float acceleration, float size);
    
    /**
     * @brief Create new pedestrian profile
     * @param name Profile name
     * @param walkSpeed Walking speed
     * @param crossingBehavior Crossing behavior type
     */
    void createPedestrianProfile(const std::string& name, float walkSpeed, const std::string& crossingBehavior);
    
    /**
     * @brief Create new cyclist profile
     * @param name Profile name
     * @param speed Cycling speed
     * @param laneBehavior Lane usage behavior
     */
    void createCyclistProfile(const std::string& name, float speed, const std::string& laneBehavior);
    
    /**
     * @brief Create traffic scenario
     * @param name Scenario name
     * @param duration Scenario duration in seconds
     */
    void createScenario(const std::string& name, float duration);
    
    /**
     * @brief Add entity spawn rule to scenario
     * @param scenarioName Target scenario
     * @param entityType Type of entity to spawn
     * @param spawnRate Spawn rate per minute
     * @param spawnPointId Spawn point ID
     */
    void addSpawnRule(const std::string& scenarioName, const std::string& entityType, float spawnRate, int spawnPointId);
    
    /**
     * @brief Load entity profiles from file
     * @param filepath Path to profiles file
     */
    bool loadProfiles(const std::string& filepath);
    
    /**
     * @brief Save entity profiles to file
     * @param filepath Path to save profiles
     */
    bool saveProfiles(const std::string& filepath);
    
    /**
     * @brief Load scenario from file
     * @param filepath Path to scenario file
     */
    bool loadScenario(const std::string& filepath);
    
    /**
     * @brief Save scenario to file
     * @param filepath Path to save scenario
     */
    bool saveScenario(const std::string& filepath);
    
    /**
     * @brief Update editor state
     * @param deltaTime Time elapsed since last update
     */
    void update(double deltaTime);
    
    /**
     * @brief Get list of available vehicle profiles
     */
    std::vector<std::string> getVehicleProfiles() const;
    
    /**
     * @brief Get list of available scenarios
     */
    std::vector<std::string> getScenarios() const;
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace RoadSim::Editor