#include "../utility.h"

#include <algorithm>
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

struct coords_t
{
	int32_t i = 0;
	int32_t j = 0;
};
using antenna_coords_t = std::vector<coords_t>;

bool are_valid_coords(const input_t& grid, const coords_t& coords)
{
	return (coords.i >= 0 && coords.i < grid.size() && coords.j >= 0 && coords.j < grid[coords.i].size());
}

char get_frequency(const input_t& input, const antenna_coords_t& coords, int32_t index)
{
	auto [i, j] = coords[index];
	if( are_valid_coords(input, coords[index]))
	{
		return input[i][j];
	}
	return '.';
}

void add_antinode_if_valid(const input_t& input, antenna_coords_t& antinodes, const coords_t& antinode)
{
	if (are_valid_coords(input, antinode))
	{
		auto it = std::find_if(antinodes.begin(), antinodes.end(), [antinode](auto coord) { return antinode.i == coord.i && antinode.j == coord.j; });
		if (it == antinodes.end())
		{
			antinodes.push_back(antinode);
		}
	}
}

void solve_1(const input_t& input)
{
	antenna_coords_t antennas;
	antenna_coords_t antinodes;

	for (int32_t i = 0; i < input.size(); i++)
	{
		for (int32_t j = 0; j < input[i].size(); j++)
		{
			if (input[i][j] != '.')
			{
				antennas.emplace_back(i, j);
			}
		}
	}
	
	for (int32_t i = 0; i < antennas.size(); i++)
	{
		char frequency_i = get_frequency(input, antennas, i);
		if (frequency_i == '.') continue;
		for (int32_t j = 0; j < antennas.size(); j++)
		{
			if (i == j) continue;
			char frequency_j = get_frequency(input, antennas, j);
			if (frequency_j == '.') continue;
			if (frequency_i == frequency_j)
			{
				static constinit int32_t distance_multiplier = 2;

				int32_t ii = antennas[i].i + distance_multiplier * (antennas[j].i - antennas[i].i);
				int32_t ij = antennas[i].j + distance_multiplier * (antennas[j].j - antennas[i].j);
				add_antinode_if_valid(input, antinodes, { ii, ij });

				int32_t ji = antennas[j].i + distance_multiplier * (antennas[i].i - antennas[j].i);
				int32_t jj = antennas[j].j + distance_multiplier * (antennas[i].j - antennas[j].j);
				add_antinode_if_valid(input, antinodes, { ji, jj });
			}
		}
	}
	std::print("{}\n", antinodes.size());
}

void solve_2(const input_t& input)
{
	antenna_coords_t antennas;
	antenna_coords_t antinodes;

	for (int32_t i = 0; i < input.size(); i++)
	{
		for (int32_t j = 0; j < input[i].size(); j++)
		{
			if (input[i][j] != '.')
			{
				antennas.emplace_back(i, j);
			}
		}
	}

	for (int32_t i = 0; i < antennas.size(); i++)
	{
		char frequency_i = get_frequency(input, antennas, i);
		if (frequency_i == '.') continue;
		for (int32_t j = 0; j < antennas.size(); j++)
		{
			if (i == j) continue;
			char frequency_j = get_frequency(input, antennas, j);
			if (frequency_j == '.') continue;
			if (frequency_i == frequency_j)
			{
				for (int32_t multiplier = 1; multiplier < input.size(); multiplier++)
				{
					int32_t ii = antennas[i].i + multiplier * (antennas[j].i - antennas[i].i);
					int32_t ij = antennas[i].j + multiplier * (antennas[j].j - antennas[i].j);
					add_antinode_if_valid(input, antinodes, { ii, ij });

					int32_t ji = antennas[j].i + multiplier * (antennas[i].i - antennas[j].i);
					int32_t jj = antennas[j].j + multiplier * (antennas[i].j - antennas[j].j);
					add_antinode_if_valid(input, antinodes, { ji, jj });
				}
			}
		}

	}
	std::print("{}\n", antinodes.size());
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}