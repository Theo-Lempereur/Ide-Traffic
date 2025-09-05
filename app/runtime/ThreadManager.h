#pragma once

#include <memory>
#include <functional>
#include <future>
#include <thread>
#include <vector>
#include <string>

namespace RoadSim::Runtime {

using Task = std::function<void()>;
using TaskId = size_t;

/**
 * @brief Thread pool manager for parallel simulation tasks
 * Handles background processing, async operations, and worker threads
 */
class ThreadManager {
public:
    ThreadManager();
    ~ThreadManager();
    
    // Non-copyable
    ThreadManager(const ThreadManager&) = delete;
    ThreadManager& operator=(const ThreadManager&) = delete;
    
    // Movable
    ThreadManager(ThreadManager&&) = default;
    ThreadManager& operator=(ThreadManager&&) = default;
    
    /**
     * @brief Initialize thread manager
     * @param numThreads Number of worker threads (0 = auto-detect)
     */
    void initialize(size_t numThreads = 0);
    
    /**
     * @brief Shutdown thread manager and wait for all tasks
     */
    void shutdown();
    
    /**
     * @brief Submit a task for async execution
     * @param task Task function to execute
     * @return Task ID for tracking
     */
    TaskId submitTask(Task task);
    
    /**
     * @brief Submit a task with return value
     * @param task Task function to execute
     * @return Future for the result
     */
    template<typename F, typename... Args>
    auto submitTaskWithResult(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>;
    
    /**
     * @brief Wait for a specific task to complete
     * @param taskId Task ID to wait for
     */
    void waitForTask(TaskId taskId);
    
    /**
     * @brief Wait for all pending tasks to complete
     */
    void waitForAllTasks();
    
    /**
     * @brief Check if a task is completed
     * @param taskId Task ID to check
     */
    bool isTaskCompleted(TaskId taskId) const;
    
    /**
     * @brief Get number of pending tasks
     */
    size_t getPendingTaskCount() const;
    
    /**
     * @brief Get number of active worker threads
     */
    size_t getWorkerThreadCount() const;
    
    /**
     * @brief Get thread utilization percentage
     */
    float getThreadUtilization() const;
    
    /**
     * @brief Set thread priority for worker threads
     * @param priority Thread priority level
     */
    void setThreadPriority(int priority);
    
    /**
     * @brief Enable/disable thread affinity optimization
     * @param enabled Affinity optimization state
     */
    void setAffinityOptimization(bool enabled);
    
    /**
     * @brief Get performance statistics
     */
    struct Statistics {
        size_t totalTasksExecuted = 0;
        size_t currentPendingTasks = 0;
        size_t activeThreads = 0;
        double averageTaskDuration = 0.0;
        double threadUtilization = 0.0;
    };
    
    Statistics getStatistics() const;
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

// Template implementation
template<typename F, typename... Args>
auto ThreadManager::submitTaskWithResult(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;
    
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    std::future<return_type> result = task->get_future();
    
    submitTask([task](){ (*task)(); });
    
    return result;
}

} // namespace RoadSim::Runtime