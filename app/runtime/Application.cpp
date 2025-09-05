#include "Application.h"
#include "ThreadManager.h"
#include "../core/Simulator.h"
#include "../core/Scheduler.h"
#include "../core/Scene.h"
#include "../editor/MapEditor.h"
#include "../editor/EntityEditor.h"
#include "../render/Window.h"
#include "../render/Renderer.h"
#include "../render/UIManager.h"
#include "../io/ConfigLoader.h"

#include <iostream>
#include <chrono>
#include <thread>

namespace RoadSim::Runtime {

struct Application::Impl {
    // Subsystems
    std::unique_ptr<Core::Simulator> simulator;
    std::unique_ptr<Core::Scheduler> scheduler;
    std::unique_ptr<Core::Scene> scene;
    std::unique_ptr<Editor::MapEditor> mapEditor;
    std::unique_ptr<Editor::EntityEditor> entityEditor;
    std::unique_ptr<Render::Window> window;
    std::unique_ptr<Render::Renderer> renderer;
    std::unique_ptr<Render::UIManager> uiManager;
    std::unique_ptr<ThreadManager> threadManager;
    std::unique_ptr<IO::ConfigLoader> configLoader;
    
    // Application state
    bool initialized = false;
    bool running = false;
    bool exitRequested = false;
    Mode currentMode = Mode::Editor;
    bool debugMode = false;
    
    // Timing
    unsigned int targetFPS = 60;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    
    // Statistics
    Statistics stats;
    std::chrono::high_resolution_clock::time_point statsStartTime;
    size_t frameCounter = 0;
    double totalFrameTime = 0.0;
};

Application::Application() : m_impl(std::make_unique<Impl>()) {
    std::cout << "[Runtime] Application created" << std::endl;
}

Application::~Application() {
    shutdown();
    std::cout << "[Runtime] Application destroyed" << std::endl;
}

bool Application::initialize(const std::string& configPath) {
    if (m_impl->initialized) {
        std::cout << "[Runtime] Application already initialized" << std::endl;
        return true;
    }
    
    (void)configPath; // Suppress unused parameter warning - TODO: Use for config loading
    
    std::cout << "[Runtime] Initializing application..." << std::endl;
    
    try {
        // Initialize subsystems in dependency order
        
        // 1. Configuration loader
        m_impl->configLoader = std::make_unique<IO::ConfigLoader>();
        m_impl->configLoader->initialize();
        
        // TODO: Load configuration from file
        // if (!m_impl->configLoader->loadConfig(configPath)) {
        //     std::cerr << "[Runtime] Failed to load config: " << configPath << std::endl;
        // }
        
        // 2. Thread manager
        m_impl->threadManager = std::make_unique<ThreadManager>();
        m_impl->threadManager->initialize();
        
        // 3. Window and renderer
        m_impl->window = std::make_unique<Render::Window>();
        if (!m_impl->window->create(1200, 800, "RoadSim - Traffic Simulation")) {
            std::cerr << "[Runtime] Failed to create window" << std::endl;
            return false;
        }
        
        m_impl->renderer = std::make_unique<Render::Renderer>();
        m_impl->renderer->initialize(m_impl->window->getRenderWindow());
        
        // 4. UI Manager
        m_impl->uiManager = std::make_unique<Render::UIManager>();
        m_impl->uiManager->initialize(m_impl->window->getRenderWindow());
        m_impl->uiManager->setWindowSize(1200, 800);
        
        // 5. Scene management
        m_impl->scene = std::make_unique<Core::Scene>();
        
        // 6. Core simulation components
        m_impl->simulator = std::make_unique<Core::Simulator>();
        m_impl->simulator->initialize();
        
        m_impl->scheduler = std::make_unique<Core::Scheduler>();
        m_impl->scheduler->initialize();
        
        // 5. Editor components
        m_impl->mapEditor = std::make_unique<Editor::MapEditor>();
        m_impl->mapEditor->initialize();
        
        m_impl->entityEditor = std::make_unique<Editor::EntityEditor>();
        m_impl->entityEditor->initialize();
        
        // Set up UI simulation callback
        m_impl->uiManager->setSimulationCallback([this](Render::UIManager::SimulationState state) {
            switch (state) {
                case Render::UIManager::SimulationState::Development:
                    switchToEditorMode();
                    break;
                case Render::UIManager::SimulationState::Running:
                    switchToSimulationMode();
                    break;
                case Render::UIManager::SimulationState::Paused:
                    m_impl->currentMode = Mode::Paused;
                    break;
            }
        });
        
        // Set up event handling
        m_impl->window->setEventCallback([this](const sf::Event& event) {
            // Handle UI events first
            m_impl->uiManager->handleEvent(event);
            
            // TODO: Handle other custom events
            // - Editor tool selection
            // - Camera controls
        });
        
        m_impl->initialized = true;
        m_impl->lastFrameTime = std::chrono::high_resolution_clock::now();
        m_impl->statsStartTime = m_impl->lastFrameTime;
        
        std::cout << "[Runtime] Application initialized successfully" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[Runtime] Exception during initialization: " << e.what() << std::endl;
        return false;
    }
}

int Application::run() {
    if (!m_impl->initialized) {
        std::cerr << "[Runtime] Application not initialized" << std::endl;
        return -1;
    }
    
    std::cout << "[Runtime] Starting main loop..." << std::endl;
    m_impl->running = true;
    
    while (m_impl->running && m_impl->window->isOpen() && !m_impl->exitRequested) {
        auto frameStart = std::chrono::high_resolution_clock::now();
        
        // Calculate delta time
        auto deltaTime = std::chrono::duration<double>(frameStart - m_impl->lastFrameTime).count();
        m_impl->lastFrameTime = frameStart;
        
        // Handle events
        handleEvents();
        
        // Update
        auto updateStart = std::chrono::high_resolution_clock::now();
        update(deltaTime);
        auto updateEnd = std::chrono::high_resolution_clock::now();
        
        // Render
        auto renderStart = std::chrono::high_resolution_clock::now();
        render();
        auto renderEnd = std::chrono::high_resolution_clock::now();
        
        // Calculate timing statistics
        auto frameEnd = std::chrono::high_resolution_clock::now();
        double frameTime = std::chrono::duration<double>(frameEnd - frameStart).count();
        double updateTime = std::chrono::duration<double>(updateEnd - updateStart).count();
        double renderTime = std::chrono::duration<double>(renderEnd - renderStart).count();
        
        updateStatistics(frameTime);
        m_impl->stats.updateTime = updateTime;
        m_impl->stats.renderTime = renderTime;
        
        // Frame rate limiting
        if (m_impl->targetFPS > 0) {
            double targetFrameTime = 1.0 / m_impl->targetFPS;
            if (frameTime < targetFrameTime) {
                auto sleepTime = std::chrono::duration<double>(targetFrameTime - frameTime);
                std::this_thread::sleep_for(sleepTime);
            }
        }
    }
    
    std::cout << "[Runtime] Main loop ended" << std::endl;
    return 0;
}

void Application::shutdown() {
    if (!m_impl->initialized) return;
    
    std::cout << "[Runtime] Shutting down application..." << std::endl;
    
    m_impl->running = false;
    
    // Shutdown subsystems in reverse order
    if (m_impl->threadManager) {
        m_impl->threadManager->shutdown();
    }
    
    // Reset all subsystems
    m_impl->entityEditor.reset();
    m_impl->mapEditor.reset();
    m_impl->scheduler.reset();
    m_impl->simulator.reset();
    m_impl->renderer.reset();
    m_impl->window.reset();
    m_impl->threadManager.reset();
    m_impl->configLoader.reset();
    
    m_impl->initialized = false;
    std::cout << "[Runtime] Application shutdown complete" << std::endl;
}

bool Application::isRunning() const {
    return m_impl->running;
}

void Application::requestExit() {
    std::cout << "[Runtime] Exit requested" << std::endl;
    m_impl->exitRequested = true;
}

void Application::switchToSimulationMode() {
    std::cout << "[Runtime] Switching to simulation mode" << std::endl;
    m_impl->currentMode = Mode::Simulation;
    
    if (m_impl->simulator) {
        m_impl->simulator->start();
    }
}

void Application::switchToEditorMode() {
    std::cout << "[Runtime] Switching to editor mode" << std::endl;
    m_impl->currentMode = Mode::Editor;
    
    if (m_impl->simulator) {
        m_impl->simulator->pause();
    }
}

Application::Mode Application::getCurrentMode() const {
    return m_impl->currentMode;
}

bool Application::loadMap(const std::string& mapPath) {
    std::cout << "[Runtime] Loading map: " << mapPath << std::endl;
    
    if (m_impl->mapEditor) {
        return m_impl->mapEditor->loadMap(mapPath);
    }
    
    return false;
}

bool Application::saveMap(const std::string& mapPath) {
    std::cout << "[Runtime] Saving map: " << mapPath << std::endl;
    
    if (m_impl->mapEditor) {
        return m_impl->mapEditor->saveMap(mapPath);
    }
    
    return false;
}

void Application::createNewMap() {
    std::cout << "[Runtime] Creating new map" << std::endl;
    
    if (m_impl->mapEditor) {
        m_impl->mapEditor->createNewMap();
    }
}

Application::Statistics Application::getStatistics() const {
    return m_impl->stats;
}

void Application::setTargetFPS(unsigned int fps) {
    m_impl->targetFPS = fps;
    std::cout << "[Runtime] Target FPS set to " << fps << std::endl;
    
    if (m_impl->window) {
        m_impl->window->setFramerateLimit(fps);
    }
}

void Application::setDebugMode(bool enabled) {
    m_impl->debugMode = enabled;
    std::cout << "[Runtime] Debug mode " << (enabled ? "enabled" : "disabled") << std::endl;
    
    if (m_impl->renderer) {
        m_impl->renderer->setDebugMode(enabled);
    }
}

// Subsystem getters
Core::Simulator* Application::getSimulator() { return m_impl->simulator.get(); }
Editor::MapEditor* Application::getMapEditor() { return m_impl->mapEditor.get(); }
Editor::EntityEditor* Application::getEntityEditor() { return m_impl->entityEditor.get(); }
Render::Window* Application::getWindow() { return m_impl->window.get(); }
Render::Renderer* Application::getRenderer() { return m_impl->renderer.get(); }
Render::UIManager* Application::getUIManager() { return m_impl->uiManager.get(); }
Core::Scene* Application::getScene() { return m_impl->scene.get(); }
ThreadManager* Application::getThreadManager() { return m_impl->threadManager.get(); }

void Application::update(double deltaTime) {
    // Update UI Manager
    if (m_impl->uiManager) {
        m_impl->uiManager->update(static_cast<float>(deltaTime));
        
        // Sync UI state with application mode
        Render::UIManager::SimulationState uiState;
        switch (m_impl->currentMode) {
            case Mode::Editor:
                uiState = Render::UIManager::SimulationState::Development;
                break;
            case Mode::Simulation:
                uiState = Render::UIManager::SimulationState::Running;
                break;
            case Mode::Paused:
                uiState = Render::UIManager::SimulationState::Paused;
                break;
        }
        m_impl->uiManager->setSimulationState(uiState);
    }
    
    // Update scheduler
    if (m_impl->scheduler) {
        m_impl->scheduler->processScheduledTasks();
    }
    
    // Update scene
    if (m_impl->scene) {
        if (m_impl->currentMode == Mode::Simulation) {
            m_impl->scene->update(static_cast<float>(deltaTime));
            m_impl->scene->fixedUpdate(static_cast<float>(deltaTime));
        }
    }
    
    // Update simulation if running
    if (m_impl->currentMode == Mode::Simulation && m_impl->simulator) {
        m_impl->simulator->step(static_cast<float>(deltaTime));
    }
    
    // Update editors
    if (m_impl->currentMode == Mode::Editor) {
        if (m_impl->mapEditor) {
            m_impl->mapEditor->update(deltaTime);
        }
        if (m_impl->entityEditor) {
            m_impl->entityEditor->update(deltaTime);
        }
    }
}

void Application::render() {
    if (!m_impl->renderer || !m_impl->window) return;
    
    m_impl->window->clear(sf::Color(45, 45, 45));
    
    m_impl->renderer->beginFrame();
    m_impl->renderer->clear();
    
    // Render based on current mode
    switch (m_impl->currentMode) {
        case Mode::Editor:
            m_impl->renderer->renderRoads();
            m_impl->renderer->renderSpawnPoints();
            m_impl->renderer->renderTrafficLights();
            m_impl->renderer->renderEditorUI();
            break;
            
        case Mode::Simulation:
        case Mode::Paused:
            m_impl->renderer->renderRoads();
            m_impl->renderer->renderEntities();
            m_impl->renderer->renderTrafficLights();
            m_impl->renderer->renderSpawnPoints();
            break;
    }
    
    // Always render metrics in debug mode
    if (m_impl->debugMode) {
        m_impl->renderer->renderMetrics();
    }
    
    m_impl->renderer->endFrame();
    
    // Render UI on top of everything
    if (m_impl->uiManager) {
        m_impl->uiManager->render();
    }
    
    m_impl->window->display();
}

void Application::handleEvents() {
    if (m_impl->window) {
        m_impl->window->pollEvents();
    }
}

void Application::updateStatistics(double frameTime) {
    m_impl->frameCounter++;
    m_impl->totalFrameTime += frameTime;
    
    m_impl->stats.frameTime = frameTime;
    m_impl->stats.frameCount = m_impl->frameCounter;
    
    // Calculate average FPS every second
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration<double>(now - m_impl->statsStartTime).count();
    
    if (elapsed >= 1.0) {
        m_impl->stats.averageFPS = m_impl->frameCounter / elapsed;
        m_impl->frameCounter = 0;
        m_impl->totalFrameTime = 0.0;
        m_impl->statsStartTime = now;
    }
    
    // TODO: Calculate memory usage
    // m_impl->stats.memoryUsage = getCurrentMemoryUsage();
}

} // namespace RoadSim::Runtime