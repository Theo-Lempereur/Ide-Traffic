#include "Scheduler.h"
#include <iostream>
#include <queue>
#include <vector>
#include <chrono>

namespace RoadSim::Core {

struct ScheduledTask {
    TaskFunction function;
    TimePoint executeTime;
    Duration interval; // For recurring tasks (0 for one-time tasks)
    bool isRecurring = false;
};

struct Scheduler::Impl {
    std::priority_queue<ScheduledTask> taskQueue;
    std::vector<TaskFunction> immediateTasks;
    Duration timeStep{16}; // Default 16ms (~60 FPS)
    TimePoint startTime;
    bool active = false;
};

Scheduler::Scheduler() : m_impl(std::make_unique<Impl>()) {
    std::cout << "[Core] Scheduler created" << std::endl;
}

Scheduler::~Scheduler() {
    std::cout << "[Core] Scheduler destroyed" << std::endl;
}

void Scheduler::initialize() {
    std::cout << "[Core] Scheduler initialized with default settings" << std::endl;
    m_impl->timeStep = std::chrono::milliseconds(16); // Default 60 FPS
    m_impl->startTime = std::chrono::steady_clock::now();
    m_impl->active = true;
}

void Scheduler::initialize(Duration timeStep) {
    std::cout << "[Core] Scheduler initialized with time step: " << timeStep.count() << "ms" << std::endl;
    m_impl->timeStep = timeStep;
    m_impl->startTime = std::chrono::steady_clock::now();
    m_impl->active = true;
}

void Scheduler::scheduleImmediate(TaskFunction task) {
    if (!m_impl->active) return;
    m_impl->immediateTasks.push_back(std::move(task));
}

void Scheduler::scheduleDelayed(TaskFunction task, Duration delay) {
    if (!m_impl->active) return;
    
    ScheduledTask scheduledTask;
    scheduledTask.function = std::move(task);
    scheduledTask.executeTime = std::chrono::steady_clock::now() + delay;
    scheduledTask.isRecurring = false;
    
    // TODO: Implement proper priority queue insertion
    // m_impl->taskQueue.push(scheduledTask);
}

void Scheduler::scheduleRecurring(TaskFunction task, Duration interval) {
    if (!m_impl->active) return;
    
    ScheduledTask scheduledTask;
    scheduledTask.function = std::move(task);
    scheduledTask.executeTime = std::chrono::steady_clock::now() + interval;
    scheduledTask.interval = interval;
    scheduledTask.isRecurring = true;
    
    // TODO: Implement proper priority queue insertion
    // m_impl->taskQueue.push(scheduledTask);
}

void Scheduler::processScheduledTasks() {
    // TODO: Process all scheduled tasks based on timing
}

void Scheduler::processTasks() {
    if (!m_impl->active) return;
    
    // Process immediate tasks
    for (auto& task : m_impl->immediateTasks) {
        try {
            task();
        } catch (const std::exception& e) {
            std::cerr << "[Core] Task execution error: " << e.what() << std::endl;
        }
    }
    m_impl->immediateTasks.clear();
    
    // TODO: Process scheduled tasks from priority queue
    // - Check if tasks are ready to execute
    // - Execute ready tasks
    // - Reschedule recurring tasks
}

void Scheduler::clearTasks() {
    std::cout << "[Core] Scheduler tasks cleared" << std::endl;
    m_impl->immediateTasks.clear();
    // TODO: Clear priority queue
    // while (!m_impl->taskQueue.empty()) {
    //     m_impl->taskQueue.pop();
    // }
}

TimePoint Scheduler::getCurrentTime() const {
    return std::chrono::steady_clock::now();
}

bool Scheduler::isActive() const {
    return m_impl->active;
}

} // namespace RoadSim::Core