#include "../utility.h"

#include <array>
#include <string>
#include <vector>

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

enum Dir
{
	right = 0,
	up,
	left,
	down
};

struct Tile_Info
{
	int32_t total_passes;
	std::array<int32_t, 4> beam_passes;
};

using Tracker_Grid = std::vector<std::vector<Tile_Info>>;

void print_grid(const Tracker_Grid& grid)
{
	for (auto& row : grid)
	{
		for (auto& c : row)
		{
			if (c.total_passes != 0)
			{
				outf("{}", c.total_passes);
			}
			else
			{
				outf(".");
			}
		}
		outf("\n");
	}
	outf("\n");
}

void light_up_tile(const Input& grid, Tracker_Grid& out_grid, int32_t i, int32_t j, Dir dir)
{
	static constexpr int32_t i_increment[4] = { 0, -1, 0, 1 };
	static constexpr int32_t j_increment[4] = { 1, 0, -1, 0 };

	if (i < 0 || i >= grid.size() || j < 0 || j >= grid[i].size() || out_grid[i][j].beam_passes[dir] != 0)
	{
		return;
	}
	
	out_grid[i][j].total_passes++;
	out_grid[i][j].beam_passes[dir]++;

	switch (grid[i][j])
	{
	case '.':
		light_up_tile(grid, out_grid, i + i_increment[dir], j + j_increment[dir], dir);
		break;
	case '|':
		switch (dir)
		{
		case right:
		case left:
			light_up_tile(grid, out_grid, i - 1, j, up);
			light_up_tile(grid, out_grid, i + 1, j, down);
			return;
		case up:
		case down:
			light_up_tile(grid, out_grid, i + i_increment[dir], j + j_increment[dir], dir);
			return;
		}
		break;
	case '-':
		switch (dir)
		{
		case right:
		case left:
			light_up_tile(grid, out_grid, i + i_increment[dir], j + j_increment[dir], dir);
			return;
		case up:
		case down:
			light_up_tile(grid, out_grid, i, j - 1, left);
			light_up_tile(grid, out_grid, i, j + 1, right);
			return;
		}
		break;
	case '/':
		switch (dir)
		{
		case right:
			light_up_tile(grid, out_grid, i - 1, j, up);
			return;
		case left:
			light_up_tile(grid, out_grid, i + 1, j, down);
			return;
		case up:
			light_up_tile(grid, out_grid, i, j + 1, right);
			return;
		case down:
			light_up_tile(grid, out_grid, i, j - 1, left);
			return;
		}
		break;
	case '\\':
		switch (dir)
		{
		case right:
			light_up_tile(grid, out_grid, i + 1, j, down);
			return;
		case left:
			light_up_tile(grid, out_grid, i - 1, j, up);
			return;
		case up:
			light_up_tile(grid, out_grid, i, j - 1, left);
			return;
		case down:
			light_up_tile(grid, out_grid, i, j + 1, right);
			return;
		}
		break;
	}
}

int32_t light_up_grid(const Input& input, int32_t start_i, int32_t start_j, Dir dir)
{
	Tracker_Grid grid;

	for (auto& row : input)
	{
		grid.push_back(std::vector(row.size(), Tile_Info{ 0, {} }));
	}

	light_up_tile(input, grid, start_i, start_j, dir);

	int32_t count = 0;
	for (auto& row : grid)
	{
		for (auto& c : row)
		{
			if (c.total_passes != 0)
			{	
				count++;
			}
		}
	}

	//print_grid(grid);

	return count;
}

void solve_1(const Input& input)
{
	outf("{}\n", light_up_grid(input, 0, 0, right));
}


void solve_2(const Input& input)
{
	int32_t max_count = 0;
	for (int32_t i = 0; i < input.size(); i++)
	{
		int32_t count = light_up_grid(input, i, 0, right);
		if (count > max_count)
		{
			max_count = count;
		}
		count = light_up_grid(input, i, input[i].size() - 1, left);
		if (count > max_count)
		{
			max_count = count;
		}
	}
	for (int32_t i = 0; i < input[0].size(); i++)
	{
		int32_t count = light_up_grid(input, 0, i, down);
		if (count > max_count)
		{
			max_count = count;
		}
		count = light_up_grid(input, input[i].size() - 1, i, up);
		if (count > max_count)
		{
			max_count = count;
		}
	}

	outf("{}", max_count);

}

int main()
{
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}