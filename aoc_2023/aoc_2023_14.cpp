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

void roll_rock(Input& grid, int32_t i, int32_t j)
{
	int32_t k = i - 1;
	for (; k >= 0 && grid[k][j] == '.'; k--);
	k++;
	if (k != i)
	{
		grid[k][j] = 'O';
		grid[i][j] = '.';
	}
}

bool roll_rock(Input& grid, int32_t i, int32_t j, int32_t dir)
{
	switch (dir)
	{
	case 0:
	{
		int32_t k = i - 1;
		for (; k >= 0 && grid[k][j] == '.'; k--);
		k++;
		if (k != i)
		{
			grid[k][j] = 'O';
			grid[i][j] = '.';
			return true;
		}
	} break;
	case 1:
	{
		int32_t k = j + 1;
		for (; k < grid[i].size() && grid[i][k] == '.'; k++);
		k--;
		if (k != j)
		{
			grid[i][k] = 'O';
			grid[i][j] = '.';
			return true;
		}
	} break;
	case 2:
	{
		int32_t k = i + 1;
		for (; k < grid.size() && grid[k][j] == '.'; k++);
		k--;
		if (k != i)
		{
			grid[k][j] = 'O';
			grid[i][j] = '.';
			return true;
		}
	} break;
	case 3:
	{
		int32_t k = j - 1;
		for (; k >= 0 && grid[i][k] == '.'; k--);
		k++;
		if (k != j)
		{
			grid[i][k] = 'O';
			grid[i][j] = '.';
			return true;
		}
	} break;
	}
	return false;
}

void solve_1(const Input& input)
{
	Input grid = input;
	for (int32_t i = 0; i < grid.size(); i++)
	{
		for (int32_t j = 0; j < grid[0].size(); j++)
		{
			if (grid[i][j] == 'O')
			{
				roll_rock(grid, i, j);
			}
		}
	}
	int32_t sum = 0;
	for (int32_t i = 0; i < grid.size(); i++)
	{
		//outf("{}\n", grid[i]);
		for (int32_t j = 0; j < grid[0].size(); j++)
		{
			
			if (grid[i][j] == 'O')
			{
				sum += grid.size() - i;
			}
		}

	}
	//outf("\n");
	outf("{}\n", sum);
}

void do_spin_cycle(Input& grid)
{
	for (int32_t i = 0; i < grid.size(); i++)
	{
		for (int32_t j = 0; j < grid[0].size(); j++)
		{
			if (grid[i][j] == 'O')
			{
				roll_rock(grid, i, j, 0);
			}
		}
	}
	for (int32_t j = 0; j < grid[0].size(); j++)
	{
		for (int32_t i = 0; i < grid.size(); i++)
		{
			if (grid[i][j] == 'O')
			{
				roll_rock(grid, i, j, 3);
			}
		}
	}
	for (int32_t i = grid.size() - 1; i >= 0; i--)
	{
		for (int32_t j = 0; j < grid[0].size(); j++)
		{
			if (grid[i][j] == 'O')
			{
				roll_rock(grid, i, j, 2);
			}
		}
	}
	for (int32_t j = grid[0].size() - 1; j >= 0; j--)
	{
		for (int32_t i = 0; i < grid.size(); i++)
		{
			if (grid[i][j] == 'O')
			{
				roll_rock(grid, i, j, 1);
			}
		}
	}

}
void print_grid(const Input& grid)
{
	for (int32_t i = 0; i < grid.size(); i++)
	{
		outf("{}\n", grid[i]);
	}
	outf("\n\n");
}
int32_t get_total_load(const Input& grid)
{
	int32_t sum = 0;
	for (int32_t i = 0; i < grid.size(); i++)
	{
		for (int32_t j = 0; j < grid[0].size(); j++)
		{

			if (grid[i][j] == 'O')
			{
				sum += grid.size() - i;
			}
		}
	}
	return sum;
}

void solve_2(const Input& input)
{
	Input grid = input;
	std::vector<int32_t> loads;
	loads.reserve(256);
	for (size_t i = 0; i < 256; i++)
	{
		do_spin_cycle(grid);
		loads.push_back(get_total_load(grid));
	}
	// we should have a periodic behavior by now; find the period start and size 
	auto [period_start, period] = find_periodicity(loads);

	outf("{}", loads[period_start + (1'000'000'000 - 1 - period_start) % period]);

}

int main()
{
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0; 
}