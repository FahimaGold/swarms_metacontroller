#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
class ConfigFile{


     
    public:
       ConfigFile();
    
       std::string ParseFile(const std::string& filename, const std::string& property, double intensity);
};
    
