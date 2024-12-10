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
	add_padding(input, '.', 1);
	return input;
}

void parse_trails_1(input_t& input, int i, int j, int64_t& count)
{
	if (input[i][j] == '9')
	{
		input[i][j] = '.';
		count++;
		return;
	}

	const int di[4] = {-1, 0, 1, 0};
	const int dj[4] = { 0, -1, 0, 1 };
	
	for (int k = 0; k < 4; k++)
	{
		int ii = i + di[k];
		int jj = j + dj[k];
		if (input[ii][jj] == '.' || input[ii][jj] - input[i][j] != 1)
			continue;
		
		parse_trails_1(input, ii, jj, count);
	}
}

void parse_trails_2(input_t& input, int i, int j, int64_t& count)
{
	if (input[i][j] == '9')
	{
		//input[i][j] = '.';
		count++;
		return;
	}

	const int di[4] = { -1, 0, 1, 0 };
	const int dj[4] = { 0, -1, 0, 1 };

	for (int k = 0; k < 4; k++)
	{
		int ii = i + di[k];
		int jj = j + dj[k];
		if (input[ii][jj] == '.' || input[ii][jj] - input[i][j] != 1)
			continue;

		parse_trails_2(input, ii, jj, count);
	}
}

void solve_1(const input_t& input)
{
	int64_t count = 0;

	for (int i = 1; i < input.size(); i++)
	{
		for (int j = 1; j < input[i].size(); j++)
		{
			if (input[i][j] == '0')
			{
				input_t grid = input;
				int64_t c = 0;
				parse_trails_1(grid, i, j, c);
				//std::println("score {}", c);
				count += c;
			}
		}
	}
	
	std::print("{}\n", count);

}

void solve_2(const input_t& input)
{
	int64_t count = 0;

	for (int i = 1; i < input.size(); i++)
	{
		for (int j = 1; j < input[i].size(); j++)
		{
			if (input[i][j] == '0')
			{
				input_t grid = input;
				int64_t c = 0;
				parse_trails_2(grid, i, j, c);
				//std::println("score {}", c);
				count += c;
			}
		}
	}

	std::print("{}\n", count);
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}