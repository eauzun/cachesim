#include "CacheConfig.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

// Helper function to trim whitespace from both ends of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

CacheConfig parseConfig(const string& filename) {
    CacheConfig config;
    
    // Initialize with default values
    config.cache_size = 0;
    config.block_size = 0;
    config.associativity = 0;
    config.replacement_policy = "";
    config.write_policy = "";
    config.write_allocate = false;
    
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open the config file: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        // Trim the line to remove leading/trailing whitespace
        line = trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Find the '=' character
        size_t pos = line.find('=');
        if (pos != string::npos) {
            // Extract key and value, trimming whitespace from both
            string key = trim(line.substr(0, pos));
            string value = trim(line.substr(pos + 1));

            // Parse each configuration parameter
            if (key == "CACHE_SIZE") {
                config.cache_size = stoi(value);
            }
            else if (key == "BLOCK_SIZE") {
                config.block_size = stoi(value);
            }
            else if (key == "ASSOCIATIVITY") {
                config.associativity = stoi(value);
            }
            else if (key == "REPLACEMENT_POLICY") {
                config.replacement_policy = value;
            }
            else if (key == "WRITE_POLICY") {
                config.write_policy = value;
            }
            else if (key == "WRITE_ALLOCATE") {
                config.write_allocate = (stoi(value) == 1);
            }
        }
    }

    file.close();
    return config;
}
