#pragma once

#include <memory>
#include <random>

namespace RoadSim::Core {

/**
 * @brief Random Number Generator for deterministic simulation
 * Provides seeded random generation for reproducible results
 */
class RNG {
public:
    explicit RNG(uint32_t seed = 0);
    ~RNG();
    
    // Non-copyable
    RNG(const RNG&) = delete;
    RNG& operator=(const RNG&) = delete;
    
    // Movable
    RNG(RNG&&) = default;
    RNG& operator=(RNG&&) = default;
    
    /**
     * @brief Set the seed for reproducible results
     * @param seed Random seed value
     */
    void setSeed(uint32_t seed);
    
    /**
     * @brief Get current seed
     */
    uint32_t getSeed() const;
    
    /**
     * @brief Generate random integer in range [min, max]
     * @param min Minimum value (inclusive)
     * @param max Maximum value (inclusive)
     */
    int randomInt(int min, int max);
    
    /**
     * @brief Generate random float in range [min, max)
     * @param min Minimum value (inclusive)
     * @param max Maximum value (exclusive)
     */
    float randomFloat(float min = 0.0f, float max = 1.0f);
    
    /**
     * @brief Generate random double in range [min, max)
     * @param min Minimum value (inclusive)
     * @param max Maximum value (exclusive)
     */
    double randomDouble(double min = 0.0, double max = 1.0);
    
    /**
     * @brief Generate random boolean with given probability
     * @param probability Probability of returning true (0.0 to 1.0)
     */
    bool randomBool(double probability = 0.5);
    
    /**
     * @brief Generate random value from normal distribution
     * @param mean Mean of the distribution
     * @param stddev Standard deviation
     */
    double randomNormal(double mean = 0.0, double stddev = 1.0);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace RoadSim::Core