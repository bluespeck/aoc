#include "../utility.h"

#include <algorithm>
#include <array>
#include <string>
#include <vector>
#include <print>
#include <unordered_map>

using input_t = struct
{
	std::array<std::array<int64_t, 100>, 100> ordering;
	std::vector<std::vector<int64_t>> to_print;
};
[[nodiscard]] input_t read()
{
	input_t input;
	for (auto& a : input.ordering)
	{
		a.fill(0);
	}
	std::string line;
	bool reading_ordering = true;
	while (std::getline(std::cin, line))
	{
		if (line.size() == 0)
		{
			reading_ordering = false;
			continue;
		}
		if(reading_ordering)
		{
			auto pages = split_string_and_convert_to_numbers(line, "|");
			assert(pages.size() == 2);
			input.ordering[pages[0]][pages[1]] = 1;
		}
		else
		{
			input.to_print.push_back(split_string_and_convert_to_numbers(line, ","));
		}
	}

	return input;
}


void solve_1(const input_t& input)
{
	int64_t sum = 0;
	
	for (auto& p : input.to_print)
	{
		bool valid = true;
		for (int i = 0; i < p.size(); i++)
		{
			for (int j = 0; j < p.size(); j++)
			{
				if (i < j && input.ordering[p[j]][p[i]] == 1)
				{
					valid = false;
					break;
				}
				else if (i > j && input.ordering[p[i]][p[j]] == 1)
				{
					valid = false;
					break;
				}
			}
		}
		if (valid)
		{
			sum += p[p.size() / 2];
		}
	}
	std::print("{}\n", sum);
}

void solve_2(const input_t& input)
{
	int64_t sum = 0;
	for (auto& p : input.to_print)
	{
		for (int i = 0; i < p.size(); i++)
		{
			bool valid = true;
			for (int j = 0; j < p.size(); j++)
			{
				if (i < j && input.ordering[p[j]][p[i]] == 1 ||
					i > j && input.ordering[p[i]][p[j]] == 1)
				{
					auto update = p;
					std::sort(update.begin(), update.end(), [&](int64_t a, int64_t b)
						{
							return input.ordering[a][b] == 1;
						});
					sum += update[update.size() / 2];
					valid = false;
					break;
				}
			}
			if (!valid)
			{
				break;
			}
		}
	}
	std::println("{}", sum);
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}