#include <unordered_map>
#include <string>

class System
{
protected:
    static const std::unordered_map<std::string, const unsigned char *> getMacAddressMap();
};
