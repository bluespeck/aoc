#include "../utility.h"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <string>
#include <ranges>
#include <vector>

using Point = std::pair<int64_t, int64_t>;

struct Edge
{
	Point p1;
	Point p2;
	std::string color;
};

using Input = std::vector<Edge>;
[[nodiscard]] Input read()
{
	Input input;
	std::string line;
	Point previous_point{ 0,0 };
	while (std::getline(std::cin, line))
	{
		if (line.empty()) break;

		Point current_point = previous_point;
		auto tokens = split_string(line, " ");
		assert(tokens.size() == 3);
		int64_t offset = stoll(tokens[1]);
		switch (tokens[0][0])
		{
		case 'R':
			current_point.second += offset;
			break;
		case 'U':
			current_point.first -= offset;
			break;
		case 'L':
			current_point.second -= offset;
			break;
		case 'D':
			current_point.first += offset;
			break;
		}
		std::string color = tokens[2].substr(2, tokens[2].size() - 3);
		input.emplace_back(previous_point, current_point, color);
		previous_point = current_point;
	}
	return input;
}

void flood_fill(std::vector<std::string>& grid, int64_t i, int64_t j)
{
	if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size() || grid[i][j] == '#' || grid[i][j] == '@')
	{
		return;
	}

	grid[i][j] = '@';
	flood_fill(grid, i + 1, j);
	flood_fill(grid, i - 1, j);
	flood_fill(grid, i, j - 1);
	flood_fill(grid, i, j + 1);
}

void solve_1(const Input& input)
{
	Point min_coords = { 0,0 };
	Point max_coords = { 0,0 };
	for (auto& edge : input)
	{
		if (edge.p1.first < min_coords.first)
		{
			min_coords.first = edge.p1.first;
		}
		if (edge.p1.second < min_coords.second)
		{
			min_coords.second = edge.p1.second;
		}
		if (edge.p1.first > max_coords.first)
		{
			max_coords.first = edge.p1.first;
		}
		if (edge.p1.second > max_coords.second)
		{
			max_coords.second = edge.p1.second;
		}
	}

	// ensure we have a tile outside the trench loop in the top left corner
	min_coords.first--;
	min_coords.second--;
	// ensure that we can flood the exterior
	max_coords.first++;
	max_coords.second++;

	Input offset_input = input;
	for (auto& edge : offset_input)
	{
		if (min_coords.first < 0)
		{
			edge.p1.first -= min_coords.first;
			edge.p2.first -= min_coords.first;
		}
		if (min_coords.second < 0)
		{
			edge.p1.second -= min_coords.second;
			edge.p2.second -= min_coords.second;
		}
	}
	if (min_coords.first < 0)
	{
		max_coords.first -= min_coords.first;
	}
	if (min_coords.second < 0)
	{
		max_coords.second -= min_coords.second;
	}

	std::vector<std::string> grid(max_coords.second + 1, std::string(max_coords.first + 1, '.'));
		
	for (auto& edge : offset_input)
	{
		if (edge.p1.first == edge.p2.first)
		{ // vertical segment
			int64_t increment = edge.p1.second < edge.p2.second ? 1 : -1;
			for (int64_t i = edge.p1.second; i != edge.p2.second + increment; i += increment)
			{
				grid[i][edge.p1.first] = '#';
			}
		}
		else if (edge.p1.second == edge.p2.second)
		{ // vertical segment
			int64_t increment = edge.p1.first < edge.p2.first ? 1 : -1;
			for (int64_t j = edge.p1.first; j != edge.p2.first + increment; j += increment)
			{
				grid[edge.p1.second][j] = '#';
			}
		}
	}

	flood_fill(grid, 0, 0);
	{int64_t area = 0;
	for (auto& row : grid)
	{
		area += std::ranges::count_if(row, [](auto c) { return c != '@'; });
		outf("{}\n", row);
	}
	outf("\n");
	outf("{}\n", area);
	}
}



void solve_2(const Input& input)
{
	// Pick's formula
	// A = i + b/2 -1, where i is number of interior int-coord points, while b is the number of int-coord points on the border
	// => i = A + 1 - b/2
	// A = |S(x1y2 - y2x1)| / 2
	// num points would then be A + 1 - b/2 + b

	std::vector<Point> corrected_input{ {0,0} };
	uint64_t perimeter = 0;
	for (auto& edge : input)
	{
		Point current_point = corrected_input.back();
		uint64_t offset = std::stoll(edge.color.substr(0, 5), nullptr, 16);
		perimeter += offset;
		switch (edge.color.back())
		{
		case '0':
			current_point.second += offset;
			break;
		case '3':
			current_point.first -= offset;
			break;
		case '2':
			current_point.second -= offset;
			break;
		case '1':
			current_point.first += offset;
			break;
		}
		corrected_input.push_back(current_point);
	}

	int64_t area = 0;
	for (size_t i = 0; i < corrected_input.size() ; i++)
	{
		area += (corrected_input[i].first * corrected_input[(i + 1) % corrected_input.size()].second - corrected_input[i].second * corrected_input[(i + 1) % corrected_input.size()].first);
	}
	area >>= 1;

	outf("{}", (std::abs(area) + 1 - perimeter / 2 + perimeter));
}

int main()
{
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}