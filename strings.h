#pragma once
#include <string>
#include <string_view>

namespace String
{
    std::string&& getLowercase(std::string&& string);
    bool contains(std::string_view container, std::string_view contained);
    std::string_view getCommonSubstring(std::string_view stringA, std::string_view stringB);
}
