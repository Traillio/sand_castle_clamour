#include "utils.h"

std::string utils::isolateFileName(const std::string& filePath)
{
    size_t lastSlash = filePath.rfind('/') + 1;
    size_t lastPeriod = filePath.rfind('.');
    return filePath.substr(lastSlash, lastPeriod - lastSlash);
}