#pragma once

#include <memory>
#include <vector>

namespace RoadSim::Core {

/**
 * @brief Main simulation engine with fixed time step
 * Handles deterministic simulation of traffic entities
 */
class Simulator {
public:
    Simulator();
    ~Simulator();
    
    // Non-copyable
    Simulator(const Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;
    
    // Movable
    Simulator(Simulator&&) = default;
    Simulator& operator=(Simulator&&) = default;
    
    /**
     * @brief Initialize the simulation with given parameters
     */
    void initialize();
    
    /**
     * @brief Run one simulation step
     * @param deltaTime Fixed time step in seconds
     */
    void step(float deltaTime);
    
    /**
     * @brief Start the simulation
     */
    void start();
    
    /**
     * @brief Pause the simulation
     */
    void pause();
    
    /**
     * @brief Stop and reset the simulation
     */
    void stop();
    
    /**
     * @brief Check if simulation is running
     */
    bool isRunning() const;
    
    /**
     * @brief Get current simulation time
     */
    double getCurrentTime() const;
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace RoadSim::Core