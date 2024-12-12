#include "../utility.h"

#include <cassert>
#include <array>
#include <string>
#include <vector>
#include <print>
#include <set>

using input_t = std::vector<std::string>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		input.emplace_back(line);
	}
	add_padding(input, '*', 1);
	return input;
}

void extract_region(const input_t& input, input_t& grid, int i, int j)
{
	grid[i][j] = input[i][j];
	int di[] = { 1, 0, -1, 0 };
	int dj[] = { 0, 1, 0, -1 };
	for (int k = 0; k < 4; k++)
	{
		int ii = i + di[k];
		int jj = j + dj[k];
		if (input[ii][jj] == input[i][j] && input[ii][jj] != grid[ii][jj])
		{
			extract_region(input, grid, ii, jj);
		}
	}
}

void reset_grid(input_t& grid, int n, int m)
{
	std::string empty_line(m, '*');
	grid.clear();
	for (int i = 0; i < n; i++)
	{
		grid.push_back(empty_line);
	}
}

void erase_region(const input_t& mask, input_t& grid)
{
	for (int i = 1; i < grid.size() - 1; i++)
	{
		for (int j = 1; j < grid[i].size() - 1; j++)
		{
			if (mask[i][j] == grid[i][j])
			{
				grid[i][j] = '*';
			}
		}
	}
}

int64_t compute_fence_cost(const input_t& grid)
{
	int64_t area = 0;
	int64_t perimeter = 0;
	for (int i = 1; i < grid.size() - 1; i++)
	{
		for (int j = 1; j < grid[i].size() - 1; j++)
		{
			if (grid[i][j] != '*')
			{
				area++;
				int di[] = { 1, 0, -1, 0 };
				int dj[] = { 0, 1, 0, -1 };
				for (int k = 0; k < 4; k++)
				{
					int ii = i + di[k];
					int jj = j + dj[k];
					if (grid[ii][jj] != grid[i][j])
					{
						perimeter++;
					}
				}
			}
		}
	}
	return perimeter * area;
}



void solve_1(const input_t& input_)
{
	int64_t sum = 0;

	input_t input = input_;
	input_t grid;

	for (int i = 1; i < input.size() - 1; i++)
	{
		for (int j = 1; j < input[i].size() - 1; j++)
		{
			if (input[i][j] == '*') continue;
			reset_grid(grid, input.size(), input[0].size());
			extract_region(input, grid, i, j);
			sum += compute_fence_cost(grid);
			erase_region(grid, input);
		}
	}

	std::print("{}\n", sum);

}

using point_t = std::pair<int64_t, int64_t>;
struct side_t
{
	point_t start, end;
	int dir; // up, left, down, right
};
bool operator == (const side_t& a, const side_t& b)
{
	return a.start == b.start && a.end == b.end && a.dir == b.dir;
}

point_t add_in_dir(point_t point, int dir, int64_t amount)
{
	switch (dir)
	{
	case 0: return { point.first - amount, point.second };
	case 1: return { point.first, point.second - amount };
	case 2: return { point.first + amount, point.second };
	case 3: return { point.first, point.second + amount };
	}
	return { -1, -1 };
}

bool try_grow_side(point_t point, side_t& segment)
{
	point_t ss = add_in_dir(segment.start, segment.dir, -1);
	point_t ee = add_in_dir(segment.end, segment.dir, 1);

	if (ss == point)
	{
		segment.start = ss;
		return true;
	}
	else if (ee == point)
	{
		segment.end = ee;
		return true;
	}

	return false;
}

int64_t compute_fence_cost_2(const input_t& grid)
{
	int64_t area = 0;
	std::set<point_t> perimeter_points;
	for (int i = 1; i < grid.size() - 1; i++)
	{
		for (int j = 1; j < grid[i].size() - 1; j++)
		{
			if (grid[i][j] != '*')
			{
				area++;
				int di[] = { 1, 0, -1, 0 };
				int dj[] = { 0, 1, 0, -1 };
				for (int k = 0; k < 4; k++)
				{
					int ii = i + di[k];
					int jj = j + dj[k];
					if (grid[ii][jj] != grid[i][j])
					{
						perimeter_points.insert({ i,j });
					}
				}
			}
		}
	}

	std::vector<side_t> sides;
	for (auto it = perimeter_points.begin(); it != perimeter_points.end(); it++)
	{
		auto it2 = it;
		bool grown = false;
		for (it2++; it2 != perimeter_points.end(); it2++)
		{
			for (int k = 0; k < 4; k++)
			{
				side_t side{ *it, *it, k };
				if (try_grow_side(*it2, side))
				{
					grown = true;
					sides.push_back(side);
				}
			}
		}
		if (!grown)
		{
			sides.push_back({ *it, *it, 0 });
		}
	}

	while (true)
	{
		bool grown = false;
		for (auto& side : sides)
		{
			for (auto& point : perimeter_points)
			{
				if (try_grow_side(point, side))
				{
					grown = true;
				}
			}
		}
		if (!grown)
		{
			break;
		}
	}
	int64_t num_sides = 0;
	std::vector<side_t> unique_sides;
	for (const auto& side : sides)
	{
		if (std::find(unique_sides.begin(), unique_sides.end(), side) == unique_sides.end())
		{
			unique_sides.push_back(side);
		}
	}
	return unique_sides.size() * area;
}

int64_t compute_fence_cost_2_2(const input_t& grid)
{
	int64_t area = 0;
	int64_t num_corners = 0;
	for (int i = 1; i < grid.size() - 1; i++)
	{
		for (int j = 1; j < grid[i].size() - 1; j++)
		{
			if (grid[i][j] != '*')
			{
				area++;
			}
			int count = 0;
			int di[] = { 1, 0, -1, 0 };
			int dj[] = { 0, 1, 0, -1 };
			for (int k = 0; k < 4; k++)
			{
				int ii = i + di[k];
				int jj = j + dj[k];
				if (grid[ii][jj] != grid[i][j])
				{
					count++;
				}
			}
			if (count == 2 && grid[i - 1][j] != grid[i + 1][j] && grid[i][j - 1] != grid[i][j + 1])
			{
				num_corners++;
			}
			else if (count == 3)
			{
				num_corners += 2;
			}
			else if (count == 4)
			{
				num_corners += 4;
			}
		}
	}
	for (int i = 0; i < grid.size() - 1; i++)
	{
		for (int j = 0; j < grid[i].size() - 1; j++)
		{
			if (grid[i][j] == '*' && grid[i + 1][j + 1] == '*' && grid[i + 1][j] != '*' && grid[i][j + 1] != '*')
			{
				num_corners -= 2;
			}
			else if (grid[i][j] != '*' && grid[i + 1][j + 1] != '*' && grid[i + 1][j] == '*' && grid[i][j + 1] == '*')
			{
				num_corners -= 2;
			}
		}
	}

	return num_corners * area;
}

void solve_2(const input_t& input_)
{
	int64_t sum = 0;

	input_t input = input_;
	input_t grid;

	for (int i = 1; i < input.size() - 1; i++)
	{
		for (int j = 1; j < input[i].size() - 1; j++)
		{
			if (input[i][j] == '*') continue;
			reset_grid(grid, input.size(), input[0].size());
			extract_region(input, grid, i, j);
			sum += compute_fence_cost_2_2(grid);
			erase_region(grid, input);
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