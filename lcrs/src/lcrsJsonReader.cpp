#include "lcrsJsonReader.h"

LCRS_NAMESPACE

const std::string lcrsJsonReader::mFilePath;

lcrsJsonReader::lcrsJsonReader(const std::string& filePath)
    : mFilePath(filePath)
{
}