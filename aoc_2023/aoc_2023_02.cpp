#include "../outf.h"

#include <cassert>
#include <array>
#include <string>
#include <vector>

struct Cube_Set
{
	int32_t red = 0;
	int32_t green = 0;
	int32_t blue = 0;
};

struct Game
{
	int32_t id;
	std::vector<Cube_Set> cube_sets;
};

using input_t = std::vector<Game>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		auto game_line_tokens = split_string(line, { ": " });
		assert(game_line_tokens.size() == 2);
		Game game;
		game.id = std::stoi(split_string(game_line_tokens[0], " ")[1]);
		auto cube_set_strings = split_string(game_line_tokens[1], "; ");
		for (auto& cube_set_str: cube_set_strings)
		{
			Cube_Set cube_set;
			auto cube_count_strings = split_string(cube_set_str, ", ");
			for (auto& cube_count_str : cube_count_strings)
			{
				auto cube_count_tokens = split_string(cube_count_str, " ");
				assert(cube_count_tokens.size() == 2);
				int32_t count = std::stoi(cube_count_tokens[0]);
				switch (cube_count_tokens[1][0])
				{
					case 'r':
						cube_set.red = count;
						break;
					case 'g':
						cube_set.green = count;
						break;
					case 'b':
						cube_set.blue = count;
						break;
				}
			}
			game.cube_sets.emplace_back(cube_set);
		}
		input.emplace_back(game);
	}

	return input;
}


void solve_1(const input_t& input)
{
	constexpr int32_t total_red = 12;
	constexpr int32_t total_green = 13;
	constexpr int32_t total_blue = 14;

	int32_t sum = 0;
	for (auto& game : input)
	{
		bool include_game = true;
		for (const Cube_Set& cube_set: game.cube_sets)
		{
			if (cube_set.red > total_red || cube_set.green > total_green || cube_set.blue > total_blue)
			{
				include_game = false;
				break;
			}
		}
		if (include_game)
		{
			sum += game.id;
		}
	}
	outf("{}\n", sum);
}

void solve_2(const input_t& input)
{
	int32_t sum = 0;
	for (auto& game : input)
	{
		Cube_Set min_cube_set { game.cube_sets[0].red, game.cube_sets[0].green, game.cube_sets[0].blue };
		for (const Cube_Set& cube_set: game.cube_sets)
		{
			if (cube_set.red > min_cube_set.red)
			{
				min_cube_set.red = cube_set.red;
			}
			if (cube_set.green > min_cube_set.green)
			{
				min_cube_set.green = cube_set.green;
			}
			if (cube_set.blue > min_cube_set.blue)
			{
				min_cube_set.blue = cube_set.blue;
			}
		}
		sum += min_cube_set.red * min_cube_set.green * min_cube_set.blue;
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