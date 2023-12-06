#include "../utility.h"

#include <array>
#include <cassert>
#include <ctype.h>
#include <numeric>
#include <string>
#include <vector>

using input_t = std::vector<std::string>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		input.emplace_back(line);
	}
	add_padding(input, '.', 1);
	return input;
}

bool is_symbol(char c)
{
	return !std::isdigit(c) && c != '.';
}
void solve_1(const input_t& input)
{
	std::vector<int32_t> part_ids;
	for (int32_t i = 0; i < input.size(); i++)
	{
		for (int32_t j = 0; j < input[i].size(); j++)
		{
			if (std::isdigit(input[i][j]))
			{
				bool is_part = false;
				int32_t first_pos = j;
				for (int32_t k = -1; k <= 1; k++)
				{
					if (is_symbol(input[i + k][first_pos - 1]))
					{
						is_part = true;
					}
				}
				for (; j < input[i].size() && std::isdigit(input[i][j]); j++)
				{
					if (is_symbol(input[i - 1][j]) || is_symbol(input[i + 1][j]))
					{
						is_part = true;
					}
				}
				--j; //get back to last digit
				int32_t last_pos = j;
				for (int32_t k = - 1; k <= 1; k++)
				{
					if (is_symbol(input[i + k][last_pos + 1]))
					{
						is_part = true;
					}
				}
				if (is_part)
				{
					part_ids.push_back(std::stoi(input[i].substr(first_pos, last_pos - first_pos + 1)));
				}

			}
		}
	}
	int32_t sum = 0;
	sum = std::accumulate(part_ids.cbegin(), part_ids.cend(), 0);
	outf("{}\n", sum);
}

int32_t try_get_neighbor_number(const input_t& input, int32_t i, int32_t j)
{
	if (!std::isdigit(input[i][j]))
	{
		return -1;
	}

	int32_t last_pos = j;
	for (int32_t k = j; k < input[i].size() && std::isdigit(input[i][k]); k++)
	{
		last_pos = k;
	}
	int32_t first_pos = j;
	for (int32_t k = j; k > 0 && std::isdigit(input[i][k]); k--)
	{
		first_pos = k;
	}
	int32_t number = std::stoi(input[i].substr(first_pos, last_pos - first_pos + 1));
	return number;
}

std::vector < int32_t > get_neighbor_numbers(const input_t& input, int32_t i, int32_t j)
{
	std::vector < int32_t > result;
	for (int32_t k = -1; k <= 1; k += 2)
	{
		if (input[i + k][j] == '.')
		{
			for (int32_t l = -1; l <= 1; l += 2)
			{
				int32_t num = try_get_neighbor_number(input, i + k, j + l);
				if (num >= 0)
				{
					result.push_back(num);
				}
			}
		}
		else
		{
			int32_t num = try_get_neighbor_number(input, i + k, j);
			if (num >= 0)
			{
				result.push_back(num);
			}
		}
		int32_t num = try_get_neighbor_number(input, i, j + k);
		if (num >= 0)
		{
			result.push_back(num);
		}
	}
	return result;
}

void solve_2(const input_t& input)
{
	int32_t sum = 0;
	for (int32_t i = 0; i < input.size(); i++)
	{
		for (int32_t j = 0; j < input[i].size(); j++)
		{
			if (input[i][j] == '*')
			{
				auto gear_neighbors = get_neighbor_numbers(input, i, j);
				if (gear_neighbors.size() == 2)
				{
					sum += gear_neighbors[0] * gear_neighbors[1];
				}
			}
		}
	}
	outf("{}\n", sum);
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}