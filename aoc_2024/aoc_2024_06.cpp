#include "../utility.h"

#include <array>
#include <string>
#include <vector>
#include <print>

using input_t = std::vector<std::string>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		input.emplace_back(line);
	}
	return input;
}

void solve_1(const input_t& input)
{
	int32_t sum = 0;
	input_t scratch_grid = input;
	int32_t i = -1;
	int32_t j = -1;
	for (auto& line : input)
	{
		i++;
		j = line.find('^');
		if (j != -1)
		{
			break;
		}
	}

	int32_t di[4] = { -1, 0, 1, 0 };
	int32_t dj[4] = { 0, 1, 0, -1 };
	int32_t dir = 0;

	scratch_grid[i][j] = 'X';
	while (true)
	{
		int ii = i + di[dir];
		int jj = j + dj[dir];
		if (ii < 0 || ii >= input.size() || jj < 0 || jj >= input.size())
		{
			break;
		}

		switch (input[ii][jj])
		{
		case '#':
			dir = (dir + 1) % 4;
			break;
		default:
			i = ii;
			j = jj;
			scratch_grid[i][j] = 'X';
			break;
		}
	}

	for (auto line : scratch_grid)
	{
		sum += std::count(line.begin(), line.end(), 'X');
	}
		
	std::print("{}\n", sum);
}

void print_grid(const input_t& grid)
{
	for (int k = 0; k < grid.size(); k++)
	{
		for (int l = 0; l < grid[k].size(); l++)
		{
			std::print("{}", grid[k][l] < 16 ? char('a' + grid[k][l]) : grid[k][l]);
		}
		std::println();
	}
	std::println();
}

void solve_2(const input_t& input)
{
	int32_t sum = 0;
	int32_t start_i = -1;
	int32_t start_j = -1;
	for (auto& line : input)
	{
		start_i++;
		start_j = line.find('^');
		if (start_j != -1)
		{
			break;
		}
	}
	
	for (int k = 0; k < input.size(); k++)
	{
		for (int l = 0; l < input[k].size(); l++)
		{
			if (input[k][l] == '#') continue;

			input_t scratch_grid = input;
			scratch_grid[k][l] = '#';
			
			int32_t i = start_i;
			int32_t j = start_j;

			int32_t di[4] = { -1, 0, 1, 0 };
			int32_t dj[4] = { 0, 1, 0, -1 };
			int32_t dir = 0;

			scratch_grid[i][j] = 1 << dir;
			while (true)
			{
				//print_grid(scratch_grid);
				int ii = i + di[dir];
				int jj = j + dj[dir];
				if (ii < 0 || ii >= scratch_grid.size() || jj < 0 || jj >= scratch_grid[ii].size())
				{
					break;
				}

				if (scratch_grid[ii][jj] == '#')
				{
					dir = (dir + 1) % 4;
				}
				else if (scratch_grid[ii][jj] < 16 && (scratch_grid[ii][jj] & (1 << dir)) != 0)
				{
					sum++;
					break;
				}
				else
				{
					i = ii;
					j = jj;
					scratch_grid[i][j] = scratch_grid[i][j] < 16 ? scratch_grid[i][j] | (1 << dir) : 1 << dir;
				}
			}
		}
	}
	std::print("{}\n", sum);
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}