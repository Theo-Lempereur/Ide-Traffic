#include "ThreadManager.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unordered_map>
#include <chrono>
#include <iostream>

namespace RoadSim::Runtime {

struct ThreadManager::Impl {
    std::vector<std::thread> workers;
    std::queue<std::pair<TaskId, Task>> taskQueue;
    std::unordered_map<TaskId, bool> taskStatus;
    
    mutable std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop{false};
    std::atomic<TaskId> nextTaskId{1};
    std::atomic<size_t> activeTasks{0};
    
    // Statistics
    std::atomic<size_t> totalTasksExecuted{0};
    std::atomic<double> totalTaskDuration{0.0};
    
    bool initialized = false;
    size_t numThreads = 0;
};

ThreadManager::ThreadManager() : m_impl(std::make_unique<Impl>()) {
    std::cout << "[Runtime] ThreadManager created" << std::endl;
}

ThreadManager::~ThreadManager() {
    shutdown();
    std::cout << "[Runtime] ThreadManager destroyed" << std::endl;
}

void ThreadManager::initialize(size_t numThreads) {
    if (m_impl->initialized) {
        std::cout << "[Runtime] ThreadManager already initialized" << std::endl;
        return;
    }
    
    if (numThreads == 0) {
        numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 4; // Fallback
    }
    
    m_impl->numThreads = numThreads;
    std::cout << "[Runtime] Initializing ThreadManager with " << numThreads << " threads" << std::endl;
    
    // Create worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        m_impl->workers.emplace_back([this, i]() {
            std::cout << "[Runtime] Worker thread " << i << " started" << std::endl;
            
            while (true) {
                std::pair<TaskId, Task> task;
                
                {
                    std::unique_lock<std::mutex> lock(m_impl->queueMutex);
                    
                    m_impl->condition.wait(lock, [this] {
                        return m_impl->stop || !m_impl->taskQueue.empty();
                    });
                    
                    if (m_impl->stop && m_impl->taskQueue.empty()) {
                        break;
                    }
                    
                    task = std::move(m_impl->taskQueue.front());
                    m_impl->taskQueue.pop();
                }
                
                // Execute task
                auto startTime = std::chrono::high_resolution_clock::now();
                m_impl->activeTasks++;
                
                try {
                    task.second(); // Execute the task
                } catch (const std::exception& e) {
                    std::cerr << "[Runtime] Task " << task.first << " threw exception: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "[Runtime] Task " << task.first << " threw unknown exception" << std::endl;
                }
                
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration<double>(endTime - startTime).count();
                
                m_impl->activeTasks--;
                m_impl->totalTasksExecuted++;
                m_impl->totalTaskDuration += duration;
                
                // Mark task as completed
                {
                    std::lock_guard<std::mutex> lock(m_impl->queueMutex);
                    m_impl->taskStatus[task.first] = true;
                }
            }
            
            std::cout << "[Runtime] Worker thread " << i << " stopped" << std::endl;
        });
    }
    
    m_impl->initialized = true;
    std::cout << "[Runtime] ThreadManager initialized successfully" << std::endl;
}

void ThreadManager::shutdown() {
    if (!m_impl->initialized) return;
    
    std::cout << "[Runtime] Shutting down ThreadManager..." << std::endl;
    
    {
        std::lock_guard<std::mutex> lock(m_impl->queueMutex);
        m_impl->stop = true;
    }
    
    m_impl->condition.notify_all();
    
    for (auto& worker : m_impl->workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    
    m_impl->workers.clear();
    m_impl->initialized = false;
    
    std::cout << "[Runtime] ThreadManager shutdown complete" << std::endl;
}

TaskId ThreadManager::submitTask(Task task) {
    if (!m_impl->initialized) {
        std::cerr << "[Runtime] ThreadManager not initialized" << std::endl;
        return 0;
    }
    
    TaskId taskId = m_impl->nextTaskId++;
    
    {
        std::lock_guard<std::mutex> lock(m_impl->queueMutex);
        m_impl->taskQueue.emplace(taskId, std::move(task));
        m_impl->taskStatus[taskId] = false;
    }
    
    m_impl->condition.notify_one();
    return taskId;
}

void ThreadManager::waitForTask(TaskId taskId) {
    if (!m_impl->initialized) return;
    
    while (!isTaskCompleted(taskId)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ThreadManager::waitForAllTasks() {
    if (!m_impl->initialized) return;
    
    while (getPendingTaskCount() > 0 || m_impl->activeTasks > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool ThreadManager::isTaskCompleted(TaskId taskId) const {
    std::lock_guard<std::mutex> lock(m_impl->queueMutex);
    auto it = m_impl->taskStatus.find(taskId);
    return it != m_impl->taskStatus.end() && it->second;
}

size_t ThreadManager::getPendingTaskCount() const {
    std::lock_guard<std::mutex> lock(m_impl->queueMutex);
    return m_impl->taskQueue.size();
}

size_t ThreadManager::getWorkerThreadCount() const {
    return m_impl->numThreads;
}

float ThreadManager::getThreadUtilization() const {
    if (m_impl->numThreads == 0) return 0.0f;
    return static_cast<float>(m_impl->activeTasks) / static_cast<float>(m_impl->numThreads);
}

void ThreadManager::setThreadPriority(int priority) {
    // TODO: Implement thread priority setting
    // This is platform-specific and requires careful implementation
    std::cout << "[Runtime] Thread priority setting not yet implemented (requested: " << priority << ")" << std::endl;
}

void ThreadManager::setAffinityOptimization(bool enabled) {
    // TODO: Implement thread affinity optimization
    // This is platform-specific and requires careful implementation
    std::cout << "[Runtime] Thread affinity optimization " << (enabled ? "enabled" : "disabled") << " (not yet implemented)" << std::endl;
}

ThreadManager::Statistics ThreadManager::getStatistics() const {
    Statistics stats;
    stats.totalTasksExecuted = m_impl->totalTasksExecuted;
    stats.currentPendingTasks = getPendingTaskCount();
    stats.activeThreads = m_impl->activeTasks;
    stats.threadUtilization = getThreadUtilization();
    
    if (stats.totalTasksExecuted > 0) {
        stats.averageTaskDuration = m_impl->totalTaskDuration / stats.totalTasksExecuted;
    }
    
    return stats;
}

} // namespace RoadSim::Runtime