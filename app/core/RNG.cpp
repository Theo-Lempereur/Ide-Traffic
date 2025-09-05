#include "RNG.h"
#include <iostream>
#include <chrono>

namespace RoadSim::Core {

struct RNG::Impl {
    std::mt19937 generator;
    uint32_t currentSeed;
    
    explicit Impl(uint32_t seed) : currentSeed(seed) {
        if (seed == 0) {
            // Use current time as seed if no seed provided
            currentSeed = static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count());
        }
        generator.seed(currentSeed);
    }
};

RNG::RNG(uint32_t seed) : m_impl(std::make_unique<Impl>(seed)) {
    std::cout << "[Core] RNG created with seed: " << m_impl->currentSeed << std::endl;
}

RNG::~RNG() {
    std::cout << "[Core] RNG destroyed" << std::endl;
}

void RNG::setSeed(uint32_t seed) {
    if (seed == 0) {
        seed = static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count());
    }
    
    m_impl->currentSeed = seed;
    m_impl->generator.seed(seed);
    std::cout << "[Core] RNG seed set to: " << seed << std::endl;
}

uint32_t RNG::getSeed() const {
    return m_impl->currentSeed;
}

int RNG::randomInt(int min, int max) {
    if (min > max) {
        std::swap(min, max);
    }
    
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(m_impl->generator);
}

float RNG::randomFloat(float min, float max) {
    if (min > max) {
        std::swap(min, max);
    }
    
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(m_impl->generator);
}

double RNG::randomDouble(double min, double max) {
    if (min > max) {
        std::swap(min, max);
    }
    
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(m_impl->generator);
}

bool RNG::randomBool(double probability) {
    if (probability <= 0.0) return false;
    if (probability >= 1.0) return true;
    
    std::bernoulli_distribution distribution(probability);
    return distribution(m_impl->generator);
}

double RNG::randomNormal(double mean, double stddev) {
    std::normal_distribution<double> distribution(mean, stddev);
    return distribution(m_impl->generator);
}

} // namespace RoadSim::Core