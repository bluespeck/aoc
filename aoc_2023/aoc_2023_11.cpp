#include "../utility.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <ctype.h>
#include <numeric>
#include <string>
#include <ranges>
#include <vector>

struct Position
{
	int32_t i, j;
};
using Pos_Vec = std::vector<Position>;
using Input = std::vector<std::string>;
[[nodiscard]] Input read()
{
	Input input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		input.push_back(line);
	}
	return input;
}

int64_t distance(Position p1, Position p2)
{
	return std::abs(p1.i - p2.i) + std::abs(p1.j - p2.j);
}

void solve_1(const Input& input)
{
	Input grid = input;
	{
		Input new_grid;
		for (size_t i = 0; i < grid[0].size(); i++)
		{
			bool found = false;
			for (size_t j = 0; j < grid.size(); j++)
			{
				if (grid[j][i] == '#')
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				for (size_t j = 0; j < grid.size(); j++)
				{
					grid[j].insert(i, 1, '.');
				}
				i++;
			}
		}

		for (auto& row : grid)
		{
			new_grid.push_back(row);
			if (std::ranges::count(row, '#') == 0)
			{
				new_grid.push_back(row);
			}
		}
		grid = new_grid;
	}

	Pos_Vec positions;
	for (int32_t i = 0; i < grid.size(); i++)
	{	
		for (int32_t j = 0; j < grid[i].size(); j++)
		{
			if (grid[i][j] == '#')
			{
				positions.push_back({ i, j });
			}
		}
	}
	int64_t sum = 0;
	for (auto& gal : positions)
	{
		for (auto& other : positions)
		{
			if (gal.i == other.i && gal.j == other.j) break;
			sum += distance(gal, other);
		}
	}

	outf("{}\n", sum);
}

void solve_2(const Input& input)
{
	int32_t expansion_rate = 1'000'000 - 1;
	Input grid = input;
	Pos_Vec positions;
	for (int32_t i = 0; i < grid.size(); i++)
	{	
		for (int32_t j = 0; j < grid[i].size(); j++)
		{
			if (grid[i][j] == '#')
			{
				positions.push_back({ i, j });
			}
		}
	}
	
	Pos_Vec updated_positions;
	updated_positions = positions;
	for (size_t j = 0; j < grid[0].size(); j++)
	{
		bool found = false;
		for (size_t i = 0; i < grid.size(); i++)
		{
			if (grid[i][j] == '#')
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			for (size_t k = 0; k < positions.size();  k++)
			{
				if (positions[k].j > j)
				{
					updated_positions[k].j += expansion_rate;
				}
			}
		}
	}
	
	int32_t i = 0;

	for (auto& row : grid)
	{
		if (std::ranges::count(row, '#') == 0)
		{
			for (size_t k = 0; k < positions.size();  k++)
			{
				if (positions[k].i > i)
				{
					updated_positions[k].i += expansion_rate;
				}
			}
		}
		i++;
	}

	positions = updated_positions;

	int64_t sum = 0;
	for (auto& gal : positions)
	{
		for (auto& other : positions)
		{
			if (gal.i == other.i && gal.j == other.j) break;
			sum += distance(gal, other);
		}
	}

	outf("{}\n", sum);
}

int main()
{
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}