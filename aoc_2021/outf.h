#include <algorithm>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

template <typename ...Args>
void outf(const std::_Fmt_string<Args...> format_str, Args&&... args)
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

template <typename Inner_Type>
void pad_vec_vec(std::vector<std::vector<Inner_Type>>& vec_vec, Inner_Type val, size_t count = 1)
{
    auto sz1 = vec_vec.size();
    auto sz2 = vec_vec[0].size();
    for (size_t i = 0; i < count; i++)
    {
        vec_vec.push_back({});
        vec_vec.back().resize(sz2, val);
    }
    std::rotate(vec_vec.begin(), vec_vec.end() - count, vec_vec.end());
    for (size_t i = 0; i < count; i++)
    {
        vec_vec.push_back({});
        vec_vec.back().resize(sz2, val);
    }
    for (auto& row : vec_vec)
    {
        std::vector<Inner_Type> padding(count, val);
        row.insert(row.begin(), padding.begin(), padding.end());
        row.insert(row.end(), padding.begin(), padding.end());
    }
}

void pad_vec_str(std::vector<std::string>& vec_str, char ch, size_t count = 1)
{
    auto sz1 = vec_str.size();
    auto sz2 = vec_str[0].size();
    for (size_t i = 0; i < count; i++)
    {
        vec_str.push_back({});
        vec_str.back().resize(sz2, ch);
    }
    std::rotate(vec_str.begin(), vec_str.end() - count, vec_str.end());
    for (size_t i = 0; i < count; i++)
    {
        vec_str.push_back({});
        vec_str.back().resize(sz2, ch);
    }
    for (auto& row : vec_str)
    {
        std::string padding(count, ch);
        row.insert(row.begin(), padding.begin(), padding.end());
        row.insert(row.end(), padding.begin(), padding.end());
    }
}
