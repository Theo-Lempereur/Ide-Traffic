#pragma once

#include <functional>
#include <memory>
#include <chrono>

namespace RoadSim::Core {

using TaskFunction = std::function<void()>;
using TimePoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::milliseconds;

/**
 * @brief Task scheduler for simulation events and parallel processing
 * Manages fixed time step scheduling and task queuing
 */
class Scheduler {
public:
    Scheduler();
    ~Scheduler();
    
    // Non-copyable
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    
    // Movable
    Scheduler(Scheduler&&) = default;
    Scheduler& operator=(Scheduler&&) = default;
    
    /**
     * @brief Initialize scheduler with default settings
     */
    void initialize();
    
    /**
     * @brief Initialize scheduler with fixed time step
     * @param timeStep Fixed time step in milliseconds
     */
    void initialize(Duration timeStep);
    
    /**
     * @brief Schedule a task to run immediately
     * @param task Function to execute
     */
    void scheduleImmediate(TaskFunction task);
    
    /**
     * @brief Schedule a task to run after a delay
     * @param task Function to execute
     * @param delay Delay before execution
     */
    void scheduleDelayed(TaskFunction task, Duration delay);
    
    /**
     * @brief Schedule a recurring task
     * @param task Function to execute
     * @param interval Interval between executions
     */
    void scheduleRecurring(TaskFunction task, Duration interval);
    
    /**
     * @brief Process all scheduled tasks
     */
    void processScheduledTasks();
    
    /**
     * @brief Process all pending tasks
     */
    void processTasks();
    
    /**
     * @brief Clear all scheduled tasks
     */
    void clearTasks();
    
    /**
     * @brief Get current scheduler time
     */
    TimePoint getCurrentTime() const;
    
    /**
     * @brief Check if scheduler is active
     */
    bool isActive() const;
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace RoadSim::Core