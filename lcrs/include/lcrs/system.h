#include <unordered_map>
#include <string>

class System
{
public:
    static const std::unordered_map<std::string, const unsigned char *> getMacAddressMap();
};
