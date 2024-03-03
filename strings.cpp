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

std::string_view String::getCommonSubstring(std::string_view stringA, std::string_view stringB)
{
    size_t commonCharacters{ 0 };

    for (size_t i{ 0 }; i < stringA.size() && i < stringB.size(); ++i)
    {
        if (stringA[i] != stringB[i])
            break;
        
        ++commonCharacters;
    }

    std::string_view commonSubstring { stringA };
    commonSubstring.remove_suffix( stringA.size() - commonCharacters);

    return commonSubstring;
}
