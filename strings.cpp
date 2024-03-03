#include "strings.h"
#include <string>
#include <algorithm>
#include <utility>

std::string&& String::getLowercase(std::string&& string)
{
    std::transform
    (
        string.begin(), string.end(), string.begin(), [](char myChar) 
        { 
            return tolower(myChar); 
        }
    );

    return std::move(string);
}

bool String::contains(std::string_view container, std::string_view contained)
{
    return container.find(contained) != std::string_view::npos;
}
