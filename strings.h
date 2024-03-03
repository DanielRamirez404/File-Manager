#pragma once
#include <string>
#include <string_view>

namespace String
{
    std::string&& getLowercase(std::string&& string);
    bool contains(std::string_view container, std::string_view contained);
}
