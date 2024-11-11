#include <unordered_map>
#include <string>

class System
{
protected:
#ifdef TEST_MODE
public:
#endif
    static const std::unordered_map<std::string, const unsigned char *> getMacAddressMap();
};
