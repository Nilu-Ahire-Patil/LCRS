#include "lcrs/test/test.h"

int main(int argc, char** argv){
    Logger::init("lcrsApp", spdlog::level::trace);
    return Test::all();
}