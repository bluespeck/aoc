#include "../utility.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <ctype.h>
#include <numeric>
#include <string>
#include <ranges>
#include <vector>
#include <thread>

using Pattern = std::vector<std::string>;
using Input = std::vector<Pattern>;
[[nodiscard]] Input read()
{
	Input input;
	std::string line;
	Pattern pattern;
	while (std::getline(std::cin, line))
	{
		if (line.empty())
		{
			input.push_back(pattern);
			pattern.clear();
			continue;
		}
		pattern.push_back(line);
	}
	input.push_back(pattern);
	return input;
}


struct Reflection_Line
{
	int32_t i;
	int32_t j;
	int32_t is_along_i;
	int32_t reflection_size;
};

std::vector<Reflection_Line> find_reflection_lines(const Pattern& pattern)
{
	std::vector<Reflection_Line> reflections;
	// horiz
	for (int32_t i = 1; i < pattern.size(); i++)
	{
		int32_t reflection_size = 0;
		for (int32_t ii = 0; i - ii - 1 >= 0 && i + ii < pattern.size(); ii++)
		{
			bool is_reflected = true;
			for (int32_t j = 0; j < pattern[i].size(); j++)
			{
				if (pattern[i - ii - 1][j] != pattern[i + ii][j])
				{
					is_reflected = false;
					break;
				}
			}
			if (!is_reflected)
			{
				break;
			}
			reflection_size += is_reflected;
		}
		if ((i <= pattern.size() / 2 && reflection_size == i) || reflection_size == pattern.size() - i)
		{
			reflections.emplace_back(i, 0, true, reflection_size );
		}
	}

	// vert
	for (int32_t j = 1; j < pattern[0].size(); j++)
	{
		int32_t reflection_size = 0;
		for (int32_t jj = 0; j - jj - 1 >= 0 && j + jj < pattern[0].size(); jj++)
		{
			bool is_reflected = true;
			for (int32_t i = 0; i < pattern.size(); i++)
			{
				if (pattern[i][j - jj - 1] != pattern[i][j + jj])
				{
					is_reflected = false;
					break;
				}
			}
			if (!is_reflected)
			{
				break;
			}
			reflection_size++;
		}
		if ((j <= pattern[0].size() / 2 && reflection_size == j) || reflection_size == pattern[0].size() - j)
		{
			reflections.emplace_back(0, j, false, reflection_size);
		}
	}
	return reflections;
}

void solve_1(const Input& input)
{
	size_t col_sum = 0;
	size_t row_sum = 0;
	for (auto& pattern : input)
	{
		// horiz
		auto reflection_lines = find_reflection_lines(pattern);
		assert(reflection_lines.size() > 0);
		row_sum += reflection_lines[0].is_along_i ? reflection_lines[0].i : 0;
		col_sum += !reflection_lines[0].is_along_i ? reflection_lines[0].j : 0;
	}

	outf("{}\n", 100 * row_sum + col_sum);
}



void solve_2(const Input& input)
{
	size_t col_sum = 0;
	size_t row_sum = 0;
	auto new_input = input;
	for (auto& pattern : new_input)
	{
		auto base_reflection_lines = find_reflection_lines(pattern);
		for (auto& row : pattern)
		{
			bool smudge_found = false;
			for (auto& c : row)
			{
				c = c == '.' ? '#' : '.';
				auto reflection_lines = find_reflection_lines(pattern);
				c = c == '.' ? '#' : '.';
				assert(reflection_lines.size() <= 2);
				if (reflection_lines.size() > 0)
				{
					if (reflection_lines[0].is_along_i != base_reflection_lines[0].is_along_i
						|| (base_reflection_lines[0].is_along_i && base_reflection_lines[0].i != reflection_lines[0].i)
						|| (!base_reflection_lines[0].is_along_i && base_reflection_lines[0].j != reflection_lines[0].j)
						)
					{
						row_sum += reflection_lines[0].is_along_i ? reflection_lines[0].i : 0;
						col_sum += !reflection_lines[0].is_along_i ? reflection_lines[0].j : 0;
						smudge_found = true;
						break;
					}
					else if(reflection_lines.size() == 2)
					{
						row_sum += reflection_lines[1].is_along_i ? reflection_lines[1].i : 0;
						col_sum += !reflection_lines[1].is_along_i ? reflection_lines[1].j : 0;
						smudge_found = true;
						break;
					}
				}
			}
			if (smudge_found) break;
		}
	}

	outf("{}\n", 100 * row_sum + col_sum);
}

int main()
{
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}