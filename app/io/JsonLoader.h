#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <variant>

namespace RoadSim::IO {

// JSON value types
using JsonValue = std::variant<
    std::nullptr_t,
    bool,
    int64_t,
    double,
    std::string,
    std::vector<class JsonObject>,
    std::map<std::string, class JsonObject>
>;

/**
 * @brief Simple JSON object wrapper
 * Provides basic JSON parsing and serialization capabilities
 */
class JsonObject {
public:
    JsonObject();
    JsonObject(const JsonValue& value);
    ~JsonObject();
    
    // Copy and move constructors
    JsonObject(const JsonObject& other);
    JsonObject(JsonObject&& other) noexcept;
    JsonObject& operator=(const JsonObject& other);
    JsonObject& operator=(JsonObject&& other) noexcept;
    
    /**
     * @brief Check if object is null
     */
    bool isNull() const;
    
    /**
     * @brief Check if object is a boolean
     */
    bool isBool() const;
    
    /**
     * @brief Check if object is a number
     */
    bool isNumber() const;
    
    /**
     * @brief Check if object is a string
     */
    bool isString() const;
    
    /**
     * @brief Check if object is an array
     */
    bool isArray() const;
    
    /**
     * @brief Check if object is an object
     */
    bool isObject() const;
    
    /**
     * @brief Get value as boolean
     */
    bool asBool() const;
    
    /**
     * @brief Get value as integer
     */
    int64_t asInt() const;
    
    /**
     * @brief Get value as double
     */
    double asDouble() const;
    
    /**
     * @brief Get value as string
     */
    std::string asString() const;
    
    /**
     * @brief Get array size
     */
    size_t size() const;
    
    /**
     * @brief Access array element
     */
    JsonObject& operator[](size_t index);
    const JsonObject& operator[](size_t index) const;
    
    /**
     * @brief Access object member
     */
    JsonObject& operator[](const std::string& key);
    const JsonObject& operator[](const std::string& key) const;
    
    /**
     * @brief Check if object has key
     */
    bool hasKey(const std::string& key) const;
    
    /**
     * @brief Get all object keys
     */
    std::vector<std::string> getKeys() const;
    
private:
    JsonValue m_value;
};

/**
 * @brief JSON file loader and parser
 * Handles loading, parsing, and saving JSON files for configuration and data
 */
class JsonLoader {
public:
    JsonLoader();
    ~JsonLoader();
    
    // Non-copyable
    JsonLoader(const JsonLoader&) = delete;
    JsonLoader& operator=(const JsonLoader&) = delete;
    
    // Movable
    JsonLoader(JsonLoader&&) = default;
    JsonLoader& operator=(JsonLoader&&) = default;
    
    /**
     * @brief Initialize the JSON loader
     */
    void initialize();
    
    /**
     * @brief Load JSON from file
     * @param filePath Path to JSON file
     * @param result Output JSON object
     * @return Success status
     */
    bool loadFromFile(const std::string& filePath, JsonObject& result);
    
    /**
     * @brief Save JSON to file
     * @param filePath Path to save file
     * @param json JSON object to save
     * @param pretty Enable pretty printing
     * @return Success status
     */
    bool saveToFile(const std::string& filePath, const JsonObject& json, bool pretty = true);
    
    /**
     * @brief Parse JSON from string
     * @param jsonString JSON string to parse
     * @param result Output JSON object
     * @return Success status
     */
    bool parseFromString(const std::string& jsonString, JsonObject& result);
    
    /**
     * @brief Convert JSON object to string
     * @param json JSON object to convert
     * @param pretty Enable pretty printing
     * @return JSON string
     */
    std::string toString(const JsonObject& json, bool pretty = true);
    
    /**
     * @brief Validate JSON file
     * @param filePath Path to JSON file
     * @return Validation result
     */
    bool validateFile(const std::string& filePath);
    
    /**
     * @brief Get last error message
     */
    std::string getLastError() const;
    
    /**
     * @brief Load map data from JSON
     * @param filePath Path to map JSON file
     * @return Success status
     */
    bool loadMapData(const std::string& filePath);
    
    /**
     * @brief Save map data to JSON
     * @param filePath Path to save map JSON
     * @return Success status
     */
    bool saveMapData(const std::string& filePath);
    
    /**
     * @brief Load entity profiles from JSON
     * @param filePath Path to profiles JSON file
     * @return Success status
     */
    bool loadEntityProfiles(const std::string& filePath);
    
    /**
     * @brief Save entity profiles to JSON
     * @param filePath Path to save profiles JSON
     * @return Success status
     */
    bool saveEntityProfiles(const std::string& filePath);
    
    /**
     * @brief Load simulation scenario from JSON
     * @param filePath Path to scenario JSON file
     * @return Success status
     */
    bool loadScenario(const std::string& filePath);
    
    /**
     * @brief Save simulation scenario to JSON
     * @param filePath Path to save scenario JSON
     * @return Success status
     */
    bool saveScenario(const std::string& filePath);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace RoadSim::IO