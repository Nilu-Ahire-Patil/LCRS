#include <iostream>
#include <sstream>
#include <string>

#include "lcrs/lcrs.h"

class Test : public System
{
public:
    static int system();
    static int logger();
    static int all();
};