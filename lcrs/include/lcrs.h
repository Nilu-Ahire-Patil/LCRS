/*

*/

#ifndef LCRS_H
#define LCRS_H

#define LCRS_NAMESPACE_START namespace Lcrs {
#define LCRS_NAMESPACE_END }
#define LCRS_NAMESPACE using namespace Lcrs;

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <string>

#include <json/json.h>

#include "lcrsNetwork.h"

LCRS_NAMESPACE_START

class lcrs {
public:
    lcrs(const std::string& configFilePath);
    ~lcrs();

private:
    const std::string mConfigFilePath;
};

LCRS_NAMESPACE_END

#endif