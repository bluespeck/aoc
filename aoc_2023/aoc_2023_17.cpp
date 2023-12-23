#include "../utility.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <numeric>
#include <string>
#include <vector>
#include <unordered_map>

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

enum Dir : uint8_t
{
	right = 0,
	up,
	left,
	down
};

int32_t make_hash(uint8_t i, uint8_t j, uint8_t distance_moved_straight, Dir dir)
{
	return (i << 24) | (j << 16) | (distance_moved_straight << 8) | dir;
}

using Map = std::unordered_map<int32_t, std::pair<int32_t, int32_t>>;
Map grid_map;
static constexpr std::array<int32_t, 4> ii = { 0, -1, 0, 1 };
static constexpr std::array<int32_t, 4> jj = { 1, 0, -1, 0 };
static constexpr int32_t max_val = std::numeric_limits<int32_t>::max() / 2;

void fill_path(Input& input, const Map& grid_map, int32_t hash)
{
	uint8_t i, j;
	do
	{
		i = (hash >> 24) & 0xff;
		j = (hash >> 16) & 0xff;
		switch ((hash) & 0xFF)
		{
		case right: input[i][j] = '>'; break;
		case up: input[i][j] = '^'; break;
		case left: input[i][j] = '<'; break;
		case down: input[i][j] = 'v'; break;
		}
		
		outf("{} ", grid_map.at(hash).first);
		hash = grid_map.at(hash).second;
	} while (i || j);
	outf("\n");
}

void solve_1(const Input& input)
{
	Map grid_map;
	for (uint8_t i = 0; i < input.size(); i++)
	{
		for (uint8_t j = 0; j < input[i].size(); j++)
		{
			for (Dir dir : {right, up, left, down})
			{
				for (uint8_t dist : {1, 2, 3})
				{
					int32_t hash = make_hash(i, j, dist, dir);
					if (i == 0 && j == 0)
					{
						grid_map[hash].first = 0;
						grid_map[hash].second = -1;
					}
					else
					{
						grid_map[hash].first = max_val;
						grid_map[hash].second = -1;
					}
				}
			}
		}
	}
	bool repeat = true;
	while(repeat)
	{
		repeat = false;
		for (uint8_t i = 0; i < input.size(); i++)
		{
			for (uint8_t j = 0; j < input[i].size(); j++)
			{
				for (uint8_t dist : {1, 2, 3})
				{
					for (Dir dir : {right, up, left, down})
					{
						int32_t check_origin_i = i - ii[dir];
						int32_t check_origin_j = j - jj[dir];
						if (check_origin_i < 0 || check_origin_j < 0 || check_origin_i >= input.size() || check_origin_j >= input[check_origin_i].size()) continue;
						
						int32_t hash = make_hash(i, j, dist, dir);
						uint8_t origin_i = i - ii[dir];
						uint8_t origin_j = j - jj[dir];
						if (dist - 1 == 0)
						{
							for (Dir origin_dir : {right, up, left, down})
							{
								if (origin_dir == dir || (origin_dir + 2) % 4 == dir) continue;

								for (uint8_t origin_dist : { 1, 2, 3 })
								{
									int32_t origin_hash = make_hash(origin_i, origin_j, origin_dist, origin_dir);
									if (grid_map[origin_hash].first + input[i][j] - '0' < grid_map[hash].first)
									{
										grid_map[hash].first = grid_map[origin_hash].first + input[i][j] - '0';
										grid_map[hash].second = origin_hash;
										repeat = true;
										//outf("{} {} {} {} - {}\n", i, j, dist, (uint8_t)dir, grid_map[hash].first);
									}
								}
							}
						}
						else
						{
							int32_t origin_hash = make_hash(origin_i, origin_j, dist - 1, dir);
							if (grid_map[origin_hash].first + input[i][j] - '0' < grid_map[hash].first)
							{
								grid_map[hash].first = grid_map[origin_hash].first + input[i][j] - '0';
								grid_map[hash].second = origin_hash;
								repeat = true;
								//outf("{} {} {} {} - {}\n", i, j, dist, (uint8_t)dir, grid_map[hash].first);
							}
						}

					}
				}
			}
		}
	}
	
	int32_t min_dist = max_val;
	int32_t min_hash = -1;
	for (uint8_t dist : {1, 2, 3})
	{
		for (Dir dir : {right, up, left, down})
		{
			int32_t hash = make_hash(input.size() - 1, input[input.size() - 1].size() - 1, dist, dir);
			if (!grid_map.contains(hash)) continue;

			int32_t dist = grid_map.at(hash).first;
			if (dist < min_dist)
			{
				min_dist = dist;
				min_hash = hash;
			}
		}
	}
	
	Input input_copy = input;
	fill_path(input_copy, grid_map, min_hash);
	for (auto& row : input_copy)
	{
		outf("{}\n", row);
	}
	outf("\n");
	
	outf("{}\n", min_dist);
}

void solve_2(const Input& input)
{
	Map grid_map;
	for (uint8_t i = 0; i < input.size(); i++)
	{
		for (uint8_t j = 0; j < input[i].size(); j++)
		{
			for (Dir dir : {right, up, left, down})
			{
				for (uint8_t dist : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
				{
					int32_t hash = make_hash(i, j, dist, dir);
					if (i == 0 && j == 0)
					{
						grid_map[hash].first = 0;
						grid_map[hash].second = -1;
					}
					else
					{
						grid_map[hash].first = max_val;
						grid_map[hash].second = -1;
					}
				}
			}
		}
	}
	bool repeat = true;
	while (repeat)
	{
		repeat = false;
		for (uint8_t i = 0; i < input.size(); i++)
		{
			for (uint8_t j = 0; j < input[i].size(); j++)
			{
				for (uint8_t dist : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
				{
					for (Dir dir : {right, up, left, down})
					{
						int32_t check_origin_i = i - ii[dir];
						int32_t check_origin_j = j - jj[dir];
						if (check_origin_i < 0 || check_origin_j < 0 || check_origin_i >= input.size() || check_origin_j >= input[check_origin_i].size()) continue;

						int32_t hash = make_hash(i, j, dist, dir);
						uint8_t origin_i = i - ii[dir];
						uint8_t origin_j = j - jj[dir];
						if (dist - 1 == 0)
						{
							for (Dir origin_dir : {right, up, left, down})
							{
								if (origin_dir == dir || (origin_dir + 2) % 4 == dir) continue;

								for (uint8_t origin_dist : {4, 5, 6, 7, 8, 9, 10})
								{
									int32_t origin_hash = make_hash(origin_i, origin_j, origin_dist, origin_dir);
									if (grid_map[origin_hash].first + input[i][j] - '0' < grid_map[hash].first)
									{
										grid_map[hash].first = grid_map[origin_hash].first + input[i][j] - '0';
										grid_map[hash].second = origin_hash;
										repeat = true;
										//outf("{} {} {} {} - {}\n", i, j, dist, (uint8_t)dir, grid_map[hash].first);
									}
								}
							}
						}
						else
						{
							int32_t origin_hash = make_hash(origin_i, origin_j, dist - 1, dir);
							if (grid_map[origin_hash].first + input[i][j] - '0' < grid_map[hash].first)
							{
								grid_map[hash].first = grid_map[origin_hash].first + input[i][j] - '0';
								grid_map[hash].second = origin_hash;
								repeat = true;
								//outf("{} {} {} {} - {}\n", i, j, dist, (uint8_t)dir, grid_map[hash].first);
							}
						}

					}
				}
			}
		}
	}

	int32_t min_dist = max_val;
	int32_t min_hash = -1;
	for (uint8_t dist : {4, 5, 6, 7, 8, 9, 10})
	{
		for (Dir dir : {right, up, left, down})
		{
			int32_t hash = make_hash(input.size() - 1, input[input.size() - 1].size() - 1, dist, dir);
			if (!grid_map.contains(hash)) continue;

			int32_t dist = grid_map.at(hash).first;
			if (dist < min_dist)
			{
				min_dist = dist;
				min_hash = hash;
			}
		}
	}
	
	Input input_copy = input;
	fill_path(input_copy, grid_map, min_hash);
	for (auto& row : input_copy)
	{
		outf("{}\n", row);
	}
	outf("\n");
	
	outf("{}\n", min_dist);
}

int main()
{
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}