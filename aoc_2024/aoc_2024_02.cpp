#include "../utility.h"

#include <array>
#include <string>
#include <vector>
#include <print>
#include <math.h>

using input_t = std::vector<std::vector<int64_t>>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		input.emplace_back(split_string_and_convert_to_numbers(line, " "));
	}

	return input;
}


void solve_1(const input_t& input)
{
	int32_t sum = 0;
	for (auto& report : input)
	{
		bool increasing = (report[0] <= report[1]);
		bool decreasing = (report[0] >= report[1]);
		bool safe = true;
		for (int i = 0; i < report.size() - 1; i++)
		{
			bool new_increasing = (report[i] < report[i + 1]);
			bool new_decreasing = (report[i] > report[i + 1]);
			if (new_increasing != increasing || new_decreasing != decreasing)
			{
				safe = false;
			}
			int64_t diff = std::abs(report[i] - report[i + 1]);
			if (diff < 1 || diff > 3)
			{
				safe = false;
			}
		}
		sum += !!safe;
	}
	std::print("{}\n", sum);

}

void solve_2(const input_t& input)
{
	int32_t sum = 0;
	for (auto& report_ : input)
	{
		bool safe;
		for (int level = 0; level < report_.size(); level++)
		{
			auto report = report_;
			report.erase(report.begin() + level);
			bool increasing = (report[0] <= report[1]);
			safe = true;
			for (int i = 0; i < report.size() - 1; i++)
			{
				bool new_increasing = (report[i] <= report[i + 1]);
				if (new_increasing != increasing)
				{
					safe = false;
					break;
				}
				int64_t diff = std::abs(report[i] - report[i + 1]);
				if (diff < 1 || diff > 3)
				{
					safe = false;
					break;
				}
			}
			if (safe) break;
		}
		sum += !!safe;
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