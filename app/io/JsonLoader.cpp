#include "JsonLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace RoadSim::IO {

// JsonObject implementation
JsonObject::JsonObject() : m_value(nullptr) {}

JsonObject::JsonObject(const JsonValue& value) : m_value(value) {}

JsonObject::~JsonObject() = default;

JsonObject::JsonObject(const JsonObject& other) : m_value(other.m_value) {}

JsonObject::JsonObject(JsonObject&& other) noexcept : m_value(std::move(other.m_value)) {}

JsonObject& JsonObject::operator=(const JsonObject& other) {
    if (this != &other) {
        m_value = other.m_value;
    }
    return *this;
}

JsonObject& JsonObject::operator=(JsonObject&& other) noexcept {
    if (this != &other) {
        m_value = std::move(other.m_value);
    }
    return *this;
}

bool JsonObject::isNull() const {
    return std::holds_alternative<std::nullptr_t>(m_value);
}

bool JsonObject::isBool() const {
    return std::holds_alternative<bool>(m_value);
}

bool JsonObject::isNumber() const {
    return std::holds_alternative<int64_t>(m_value) || std::holds_alternative<double>(m_value);
}

bool JsonObject::isString() const {
    return std::holds_alternative<std::string>(m_value);
}

bool JsonObject::isArray() const {
    return std::holds_alternative<std::vector<JsonObject>>(m_value);
}

bool JsonObject::isObject() const {
    return std::holds_alternative<std::map<std::string, JsonObject>>(m_value);
}

bool JsonObject::asBool() const {
    if (auto* val = std::get_if<bool>(&m_value)) {
        return *val;
    }
    return false;
}

int64_t JsonObject::asInt() const {
    if (auto* val = std::get_if<int64_t>(&m_value)) {
        return *val;
    }
    if (auto* val = std::get_if<double>(&m_value)) {
        return static_cast<int64_t>(*val);
    }
    return 0;
}

double JsonObject::asDouble() const {
    if (auto* val = std::get_if<double>(&m_value)) {
        return *val;
    }
    if (auto* val = std::get_if<int64_t>(&m_value)) {
        return static_cast<double>(*val);
    }
    return 0.0;
}

std::string JsonObject::asString() const {
    if (auto* val = std::get_if<std::string>(&m_value)) {
        return *val;
    }
    return "";
}

size_t JsonObject::size() const {
    if (auto* arr = std::get_if<std::vector<JsonObject>>(&m_value)) {
        return arr->size();
    }
    if (auto* obj = std::get_if<std::map<std::string, JsonObject>>(&m_value)) {
        return obj->size();
    }
    return 0;
}

JsonObject& JsonObject::operator[](size_t index) {
    if (auto* arr = std::get_if<std::vector<JsonObject>>(&m_value)) {
        if (index < arr->size()) {
            return (*arr)[index];
        }
    }
    static JsonObject nullObj;
    return nullObj;
}

const JsonObject& JsonObject::operator[](size_t index) const {
    if (auto* arr = std::get_if<std::vector<JsonObject>>(&m_value)) {
        if (index < arr->size()) {
            return (*arr)[index];
        }
    }
    static JsonObject nullObj;
    return nullObj;
}

JsonObject& JsonObject::operator[](const std::string& key) {
    if (auto* obj = std::get_if<std::map<std::string, JsonObject>>(&m_value)) {
        return (*obj)[key];
    }
    static JsonObject nullObj;
    return nullObj;
}

const JsonObject& JsonObject::operator[](const std::string& key) const {
    if (auto* obj = std::get_if<std::map<std::string, JsonObject>>(&m_value)) {
        auto it = obj->find(key);
        if (it != obj->end()) {
            return it->second;
        }
    }
    static JsonObject nullObj;
    return nullObj;
}

bool JsonObject::hasKey(const std::string& key) const {
    if (auto* obj = std::get_if<std::map<std::string, JsonObject>>(&m_value)) {
        return obj->find(key) != obj->end();
    }
    return false;
}

std::vector<std::string> JsonObject::getKeys() const {
    std::vector<std::string> keys;
    if (auto* obj = std::get_if<std::map<std::string, JsonObject>>(&m_value)) {
        for (const auto& pair : *obj) {
            keys.push_back(pair.first);
        }
    }
    return keys;
}

// JsonLoader implementation
struct JsonLoader::Impl {
    bool initialized = false;
    std::string lastError;
    
    // Simple JSON parsing state
    size_t pos = 0;
    std::string jsonText;
    
    void skipWhitespace() {
        while (pos < jsonText.length() && std::isspace(jsonText[pos])) {
            pos++;
        }
    }
    
    JsonObject parseValue();
    JsonObject parseObject();
    JsonObject parseArray();
    JsonObject parseString();
    JsonObject parseNumber();
    JsonObject parseLiteral();
};

JsonLoader::JsonLoader() : m_impl(std::make_unique<Impl>()) {
    std::cout << "[IO] JsonLoader created" << std::endl;
}

JsonLoader::~JsonLoader() {
    std::cout << "[IO] JsonLoader destroyed" << std::endl;
}

void JsonLoader::initialize() {
    std::cout << "[IO] JsonLoader initialized" << std::endl;
    m_impl->initialized = true;
}

bool JsonLoader::loadFromFile(const std::string& filePath, JsonObject& result) {
    if (!m_impl->initialized) {
        m_impl->lastError = "JsonLoader not initialized";
        return false;
    }
    
    std::cout << "[IO] Loading JSON from: " << filePath << std::endl;
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        m_impl->lastError = "Could not open file: " + filePath;
        std::cerr << "[IO] " << m_impl->lastError << std::endl;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    return parseFromString(content, result);
}

bool JsonLoader::saveToFile(const std::string& filePath, const JsonObject& json, bool pretty) {
    if (!m_impl->initialized) {
        m_impl->lastError = "JsonLoader not initialized";
        return false;
    }
    
    std::cout << "[IO] Saving JSON to: " << filePath << std::endl;
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        m_impl->lastError = "Could not create file: " + filePath;
        std::cerr << "[IO] " << m_impl->lastError << std::endl;
        return false;
    }
    
    std::string jsonString = toString(json, pretty);
    file << jsonString;
    
    return file.good();
}

bool JsonLoader::parseFromString(const std::string& jsonString, JsonObject& result) {
    if (!m_impl->initialized) {
        m_impl->lastError = "JsonLoader not initialized";
        return false;
    }
    
    try {
        m_impl->jsonText = jsonString;
        m_impl->pos = 0;
        
        result = m_impl->parseValue();
        return true;
        
    } catch (const std::exception& e) {
        m_impl->lastError = "JSON parsing error: " + std::string(e.what());
        std::cerr << "[IO] " << m_impl->lastError << std::endl;
        return false;
    }
}

std::string JsonLoader::toString(const JsonObject& json, bool pretty) {
    // TODO: Implement JSON serialization
    // This is a simplified placeholder implementation
    (void)pretty; // Suppress unused parameter warning
    if (json.isNull()) {
        return "null";
    } else if (json.isBool()) {
        return json.asBool() ? "true" : "false";
    } else if (json.isNumber()) {
        return std::to_string(json.asDouble());
    } else if (json.isString()) {
        return "\"" + json.asString() + "\"";
    } else {
        return "{}"; // Placeholder for complex objects
    }
}

bool JsonLoader::validateFile(const std::string& filePath) {
    JsonObject temp;
    return loadFromFile(filePath, temp);
}

std::string JsonLoader::getLastError() const {
    return m_impl->lastError;
}

bool JsonLoader::loadMapData(const std::string& filePath) {
    std::cout << "[IO] Loading map data from: " << filePath << std::endl;
    
    JsonObject mapJson;
    if (!loadFromFile(filePath, mapJson)) {
        return false;
    }
    
    // TODO: Parse map-specific data
    // - Nodes (intersections)
    // - Roads (connections between nodes)
    // - Traffic lights
    // - Spawn points
    // - Map metadata (name, description, etc.)
    
    return true;
}

bool JsonLoader::saveMapData(const std::string& filePath) {
    std::cout << "[IO] Saving map data to: " << filePath << std::endl;
    
    // TODO: Serialize current map data to JSON
    JsonObject mapJson;
    
    return saveToFile(filePath, mapJson);
}

bool JsonLoader::loadEntityProfiles(const std::string& filePath) {
    std::cout << "[IO] Loading entity profiles from: " << filePath << std::endl;
    
    JsonObject profilesJson;
    if (!loadFromFile(filePath, profilesJson)) {
        return false;
    }
    
    // TODO: Parse entity profiles
    // - Vehicle profiles (size, speed, acceleration, etc.)
    // - Pedestrian profiles
    // - Cyclist profiles
    
    return true;
}

bool JsonLoader::saveEntityProfiles(const std::string& filePath) {
    std::cout << "[IO] Saving entity profiles to: " << filePath << std::endl;
    
    // TODO: Serialize entity profiles to JSON
    JsonObject profilesJson;
    
    return saveToFile(filePath, profilesJson);
}

bool JsonLoader::loadScenario(const std::string& filePath) {
    std::cout << "[IO] Loading scenario from: " << filePath << std::endl;
    
    JsonObject scenarioJson;
    if (!loadFromFile(filePath, scenarioJson)) {
        return false;
    }
    
    // TODO: Parse scenario data
    // - Spawn rules
    // - Traffic patterns
    // - Simulation parameters
    // - Events and triggers
    
    return true;
}

bool JsonLoader::saveScenario(const std::string& filePath) {
    std::cout << "[IO] Saving scenario to: " << filePath << std::endl;
    
    // TODO: Serialize scenario data to JSON
    JsonObject scenarioJson;
    
    return saveToFile(filePath, scenarioJson);
}

// Simple JSON parsing implementation
JsonObject JsonLoader::Impl::parseValue() {
    skipWhitespace();
    
    if (pos >= jsonText.length()) {
        throw std::runtime_error("Unexpected end of JSON");
    }
    
    char c = jsonText[pos];
    
    switch (c) {
        case '{': return parseObject();
        case '[': return parseArray();
        case '"': return parseString();
        case 't':
        case 'f':
        case 'n': return parseLiteral();
        default:
            if (c == '-' || std::isdigit(c)) {
                return parseNumber();
            }
            throw std::runtime_error("Unexpected character: " + std::string(1, c));
    }
}

JsonObject JsonLoader::Impl::parseObject() {
    // TODO: Implement object parsing
    pos++; // Skip '{'
    skipWhitespace();
    
    std::map<std::string, JsonObject> obj;
    
    if (pos < jsonText.length() && jsonText[pos] == '}') {
        pos++; // Skip '}'
        return JsonObject(obj);
    }
    
    // Simplified: just skip to closing brace
    while (pos < jsonText.length() && jsonText[pos] != '}') {
        pos++;
    }
    if (pos < jsonText.length()) pos++; // Skip '}'
    
    return JsonObject(obj);
}

JsonObject JsonLoader::Impl::parseArray() {
    // TODO: Implement array parsing
    pos++; // Skip '['
    skipWhitespace();
    
    std::vector<JsonObject> arr;
    
    if (pos < jsonText.length() && jsonText[pos] == ']') {
        pos++; // Skip ']'
        return JsonObject(arr);
    }
    
    // Simplified: just skip to closing bracket
    while (pos < jsonText.length() && jsonText[pos] != ']') {
        pos++;
    }
    if (pos < jsonText.length()) pos++; // Skip ']'
    
    return JsonObject(arr);
}

JsonObject JsonLoader::Impl::parseString() {
    pos++; // Skip opening quote
    
    std::string str;
    while (pos < jsonText.length() && jsonText[pos] != '"') {
        str += jsonText[pos];
        pos++;
    }
    
    if (pos < jsonText.length()) pos++; // Skip closing quote
    
    return JsonObject(str);
}

JsonObject JsonLoader::Impl::parseNumber() {
    std::string numStr;
    
    while (pos < jsonText.length() && 
           (std::isdigit(jsonText[pos]) || jsonText[pos] == '.' || 
            jsonText[pos] == '-' || jsonText[pos] == '+' || 
            jsonText[pos] == 'e' || jsonText[pos] == 'E')) {
        numStr += jsonText[pos];
        pos++;
    }
    
    if (numStr.find('.') != std::string::npos || 
        numStr.find('e') != std::string::npos || 
        numStr.find('E') != std::string::npos) {
        return JsonObject(std::stod(numStr));
    } else {
        return JsonObject(static_cast<int64_t>(std::stoll(numStr)));
    }
}

JsonObject JsonLoader::Impl::parseLiteral() {
    if (jsonText.substr(pos, 4) == "true") {
        pos += 4;
        return JsonObject(true);
    } else if (jsonText.substr(pos, 5) == "false") {
        pos += 5;
        return JsonObject(false);
    } else if (jsonText.substr(pos, 4) == "null") {
        pos += 4;
        return JsonObject(nullptr);
    }
    
    throw std::runtime_error("Invalid literal");
}

} // namespace RoadSim::IO