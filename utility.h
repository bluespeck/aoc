
#include <cassert>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

////////////////////////
// declarations

template <typename ...Args>
void outf(const std::format_string<Args...> format_str, Args&&... args);

template <typename T>
std::pair<int32_t, int32_t> find_periodicity(std::vector<T> vec, int32_t max_period_size = 100, int32_t min_period_size = 1);

std::vector<std::string> split_string(std::string_view str, const std::string_view separator);

std::vector<std::int64_t> split_string_and_convert_to_numbers(std::string_view str, const std::string_view separator);

std::vector<std::int64_t> split_string_and_convert_to_numbers(std::string_view str, const std::string_view separator);

void add_padding(std::vector<std::string>& grid, char fill_char, size_t thickness);

using Interval = std::pair<int64_t, int64_t>;
using Interval_Vector = std::vector<Interval>;

void intersect_diff_intervals(const Interval& i1, const Interval& i2, Interval_Vector& intersection, Interval_Vector& difference);





////////////////////////
// implementations;

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
		if(pos > 0)
		{
        	result.push_back(std::string{ str.substr(0, pos) });
		}
        str = str.substr(pos + separator.length());
    }

    if (!str.empty())
        result.push_back(std::string{ str });

    return result;
}

std::vector<std::int64_t> split_string_and_convert_to_numbers(std::string_view str, const std::string_view separator)
{
    std::vector<std::int64_t> result;
    for (auto pos = str.npos; (pos = str.find(separator)) != str.npos;)
    {
		if(pos > 0)
		{
        	result.push_back(std::stoll(std::string{str.substr(0, pos)}));
		}
        str = str.substr(pos + separator.length());
    }

    if (!str.empty())
        result.push_back(std::stoll(std::string{str}));

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

void intersect_diff_intervals(const Interval& i1, const Interval& i2, Interval_Vector& intersection, Interval_Vector& difference)
{
	intersection.clear();
	difference.clear();

	// ( ) [ ]
	// [ ] ( )
	if (i1.first > i2.second || i1.second < i2.first)
	{
		difference.push_back(i1);
	}
	// [ ( ] )
	else if (i1.first <= i2.first && i1.second >= i2.first && i1.second <= i2.second)
	{
		intersection.emplace_back(i2.first, i1.second);
		if (i1.first < i2.first)
		{
			difference.emplace_back(i1.first, i2.first - 1);
		}
	}
	// ( [ ) ]
	else if (i1.first >= i2.first && i2.second >= i1.first && i2.second <= i1.second)
	{
		intersection.emplace_back(i1.first, i2.second);
		if (i2.second < i1.second)
		{
			difference.emplace_back(i2.second + 1, i1.second);
		}
	}
	// [ ( ) ]
	else if(i1.first <= i2.first && i1.second >= i2.second) 
	{
		intersection.push_back(i2);
		if (i1.first < i2.first)
		{
			difference.emplace_back(i1.first, i2.first - 1);
		}
		if (i1.second > i2.second)
		{
			difference.emplace_back(i2.second + 1, i1.second);
		}
	}
	// ( [ ] )
	else
	{
		assert(i1.first >= i2.first && i1.second <= i2.second);
		intersection.push_back(i1);
	}
}

template <typename T>
std::pair<int32_t, int32_t> find_periodicity(std::vector<T> vec, int32_t max_period_size, int32_t min_period_size)
{
	int32_t period_start = -1;
	int32_t period = -1;

	for (int32_t period_size = min_period_size; period_size < max_period_size; period_size++)
	{
		bool found = true;
		for (int32_t i = 0; i < period_size; i++)
		{
			if (vec[vec.size() - 1 - i] != vec[vec.size() - 1 - period_size - i])
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			period = period_size;
			break;
		}
	}

	for (int32_t i = 0; i < vec.size(); i++)
	{
		bool found = true;
		for (int32_t k = 0; k < period; k++)
		{
			if (vec[i + k] != vec[i + period + k])
			{
				found = false;
			}
		}
		if (found)
		{
			period_start = i;
			break;
		}
	}

	return { period_start, period };
}
