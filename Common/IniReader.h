#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class IniReader {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_mapConfig;

public:
    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error: Cannot open INI file: " << filename << std::endl;
            return false;
        }

        std::string line, section;
        while (std::getline(file, line)) {
            // Remove leading/trailing spaces
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty() || line[0] == ';' || line[0] == '#') {
                continue; // Skip comments and empty lines
            }

            if (line[0] == '[' && line.back() == ']') {
                // Extract section name
                section = line.substr(1, line.size() - 2);
            }
            else {
                // Parse key=value pair
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);

                    // Trim spaces
                    key.erase(0, key.find_first_not_of(" \t"));
                    key.erase(key.find_last_not_of(" \t") + 1);
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);

                    // Store in config map
                    m_mapConfig[section][key] = value;
                }
            }
        }
        return true;
    }

    //get string config values
    std::string getstring(const std::string& section, const std::string& key,
            const std::string& defaultValue = "") const {
        auto secIt = m_mapConfig.find(section);
        if (secIt != m_mapConfig.end()) {
            auto keyIt = secIt->second.find(key);
            if (keyIt != secIt->second.end()) {
                return keyIt->second;
            }
        }
        return defaultValue;
    }


    //get int config values
    int getint(const std::string& section, const std::string& key,
        const int& defaultValue = 0) const {
        std::string strval = getstring(section,key, std::to_string(defaultValue));
        return atoi(strval.c_str());
    }

};

