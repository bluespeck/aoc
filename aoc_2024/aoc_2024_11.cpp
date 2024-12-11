#include "../utility.h"

#include <array>
#include <string>
#include <vector>
#include <print>
#include <map>

using input_t = std::vector<int64_t>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		input = split_string_and_convert_to_numbers(line, " ");
	}

	return input;
}

int64_t get_num_digits(int64_t num)
{
	int64_t count = 0;
	while (num)
	{
		count++;
		num /= 10;
	}
	return count;
}

void print_stones(input_t& stones)
{
	for (auto& stone : stones)
	{
		std::print("{} ", stone);
		
	}
	std::println();
}

void solve_1(const input_t& input)
{
	auto stones = input;
	int num_steps = 25;

	for (int step = 0; step < num_steps; step++)
	{
		for (int64_t i = 0; i < stones.size(); i++)
		{
			auto& stone = stones[i];
			if (stone == 0)
				stone = 1;
			else if (get_num_digits(stone) % 2 == 0)
			{
				std::string stone_digits = std::to_string(stone);
				int64_t stone1 = std::stoll(stone_digits.substr(0, stone_digits.size() / 2));
				int64_t stone2 = std::stoll(stone_digits.substr(stone_digits.size() / 2));
				stones.insert(stones.begin() + i, stone1);
				stones[++i] = stone2;
			}
			else
			{
				stone *= 2024;
			}
		}
		//print_stones(stones);
		//std::println("{}", stones.size());
	}
	std::print("{}\n", stones.size());

}

std::pair<int64_t, int64_t> split_num(int64_t num, int64_t digits)
{
	int64_t power_of_10 = 1;
	for (int64_t j = 0; j < digits / 2; j++)
	{
		power_of_10 *= 10;
	}

	return { num / power_of_10, num % power_of_10 };
}

static std::map<std::pair<int64_t, int64_t>, int64_t> counters;

int64_t count(int64_t number, int64_t remaining_steps)
{
	auto num_pair = std::make_pair(number, remaining_steps);

	if (counters.contains(num_pair))
	{
		return counters[num_pair];
	}

	if (remaining_steps == 0)
	{
		//std::print("{} ", number);
		counters[num_pair] = 1;
		return 1;
	}

	if (number == 0)
	{
		auto result = count(1, remaining_steps - 1);
		counters[num_pair] = result;
		return result;
	}
	else
	{
		int64_t num_digits = get_num_digits(number);
		if (num_digits % 2 == 0)
		{
			auto [first, second] = split_num(number, num_digits);
			auto result = count(first, remaining_steps - 1) + count(second, remaining_steps - 1);
			counters[num_pair] = result;
			return result;
		}
		else
		{
			auto result = count(number * 2024, remaining_steps - 1);
			counters[num_pair] = result;
			return result;
		}
	}
}

void solve_2(const input_t& input)
{
	int64_t sum = 0;
	for (auto& num : input)
	{
		sum += count(num, 75);
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