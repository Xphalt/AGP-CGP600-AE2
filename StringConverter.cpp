#include "StringConverter.h"

std::wstring StringConverter::StringToWide(std::string _string)
{
    std::wstring wideString(_string.begin(), _string.end());

    return wideString;
}