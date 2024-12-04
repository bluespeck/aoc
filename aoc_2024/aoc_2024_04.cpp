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
	int32_t count = 0;
	std::array<std::array<int32_t, 2>, 8> dirs{
		{
		{ -1, -1 },
		{ -1,  0 },
		{ -1,  1 },
		{  0, -1 },
		{  0,  1 },
		{  1, -1 },
		{  1,  0 },
		{  1,  1 }
		}
	};
	
	std::string_view xmas = "XMAS";

	for (int i = 0; i < input.size(); i++)
	{
		for (int j = 0; j < input[i].size(); j++)
		{
			if (input[i][j] != xmas[0]) continue;
			for (auto dir : dirs)
			{
				bool ok = true;
				int ii = i;
				int jj = j;
				for (char c : xmas.substr(1))
				{
					ii += dir[0];
					jj += dir[1];
					if (ii < 0 || ii >= input.size() || jj < 0 || jj >= input[ii].size() || c != input[ii][jj])
					{
						ok = false;
						break;
					}
				}
				if (ok) count++;
			}
		}
	}
	std::print("{}\n", count);
}

void solve_2(const input_t& input)
{
	std::array<std::vector<std::string>, 4> patterns =
	{
		{
			{
			"M.S",
			".A.",
			"M.S"
			},
			{
			"S.S",
			".A.",
			"M.M"
			},
			{
			"S.M",
			".A.",
			"S.M"
			},
			{
			"M.M",
			".A.",
			"S.S"
			},

		}
	};
	int32_t count = 0;
	for (int i = 1; i < input.size() - 1; i++)
	{
		for (int j = 1; j < input[i].size() - 1; j++)
		{
			for (int k = 0; k < patterns.size(); k++)
			{
				bool ok = true;
				for (int ii = 0; ii < patterns[k].size(); ii++)
				{
					for (int jj = 0; jj < patterns[k][ii].size(); jj++)
					{
						int iii = i - 1 + ii;
						int jjj = j - 1 + jj;
						if (patterns[k][ii][jj] != '.' && patterns[k][ii][jj] != input[iii][jjj])
						{
							ok = false;
							break;
						}
					}
					if (!ok) break;
				}
				if (ok) count++;
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