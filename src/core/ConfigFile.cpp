#include "ConfigFile.h"


ConfigFile::ConfigFile() {
    // Constructor implementation
}

std::string ConfigFile::ParseFile(const std::string& filename, const std::string& property, double intensity) {
    std::ifstream file(filename);
    std::string pfsm = "";
    std::cout<<"inside parsing file"<<std::endl;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key, value;

            while (std::getline(iss >> std::ws, key, ':')) {
                if (key == property) {
                    std::getline(iss >> std::ws, value, ',');
                    double parsedIntensity = std::stod(value);
                    if (parsedIntensity == intensity) {
                        std::getline(iss >> std::ws, pfsm);
                        size_t prefixPos = pfsm.find(": ");
                        if (prefixPos != std::string::npos) {
                            pfsm = pfsm.substr(prefixPos + 2);
                        }
                        break;
                    }
                }
            }

            if (!pfsm.empty()) {
                break;
            }
        }

        file.close();
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }

    return pfsm;
}