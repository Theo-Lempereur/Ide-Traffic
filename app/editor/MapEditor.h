#pragma once

#include <memory>
#include <vector>
#include <string>

namespace RoadSim::Editor {

/**
 * @brief Map editor for creating and modifying road networks
 * Handles nodes, routes, lanes, traffic lights, and spawn points
 */
class MapEditor {
public:
    MapEditor();
    ~MapEditor();
    
    // Non-copyable
    MapEditor(const MapEditor&) = delete;
    MapEditor& operator=(const MapEditor&) = delete;
    
    // Movable
    MapEditor(MapEditor&&) = default;
    MapEditor& operator=(MapEditor&&) = default;
    
    /**
     * @brief Initialize the map editor
     */
    void initialize();
    
    /**
     * @brief Create a new empty map
     */
    void createNewMap();
    
    /**
     * @brief Load map from file
     * @param filepath Path to map file
     */
    bool loadMap(const std::string& filepath);
    
    /**
     * @brief Save current map to file
     * @param filepath Path to save map
     */
    bool saveMap(const std::string& filepath);
    
    /**
     * @brief Add a road node at position
     * @param x X coordinate
     * @param y Y coordinate
     */
    void addNode(float x, float y);
    
    /**
     * @brief Add a road segment between two nodes
     * @param nodeId1 First node ID
     * @param nodeId2 Second node ID
     */
    void addRoad(int nodeId1, int nodeId2);
    
    /**
     * @brief Add traffic light at node
     * @param nodeId Node ID
     */
    void addTrafficLight(int nodeId);
    
    /**
     * @brief Add spawn point for vehicles
     * @param x X coordinate
     * @param y Y coordinate
     * @param type Vehicle type
     */
    void addSpawnPoint(float x, float y, const std::string& type);
    
    /**
     * @brief Remove selected elements
     */
    void removeSelected();
    
    /**
     * @brief Clear entire map
     */
    void clearMap();
    
    /**
     * @brief Update editor (handle input, etc.)
     * @param deltaTime Time elapsed since last update
     */
    void update(double deltaTime);
    
    /**
     * @brief Check if map has unsaved changes
     */
    bool hasUnsavedChanges() const;
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace RoadSim::Editor