#include <vector>
#include <string>
#include <map>

struct preset
{
    const char* title;
    const char* detailed_explanation;
    std::vector<const char*> options;
};

extern const std::map<std::string, preset> presets;
