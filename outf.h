
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

template <typename ...Args>
void outf(const std::format_string<Args...> format_str, Args&&... args)
{
    std::cout << std::format(format_str, std::forward<Args>(args)...);
}

std::vector<std::string> split_string(std::string_view str, const std::string_view separator)
{
    std::vector<std::string> result;
    for (auto pos = str.npos; (pos = str.find(separator)) != str.npos;)
    {
        result.push_back(std::string{ str.substr(0, pos) });
        str = str.substr(pos + separator.length());
    }

    if (!str.empty())
        result.push_back(std::string{ str });

    return result;
}

void add_padding(std::vector<std::string>& grid, char fill_char, size_t thickness)
{
    std::string filler_horizontal(thickness, fill_char);
    std::string filler_row(thickness * 2 + grid[0].size(), fill_char);

    for (auto& row : grid)
    {
        row.insert(row.begin(), filler_horizontal.begin(), filler_horizontal.end());
        row.insert(row.end(), filler_horizontal.begin(), filler_horizontal.end());
    }

    for (size_t i = 0; i < thickness; i++)
    {
        grid.insert(grid.begin(), filler_row);
    }
    for (size_t i = 0; i < thickness; i++)
    {
        grid.insert(grid.end(), filler_row);
    }
}