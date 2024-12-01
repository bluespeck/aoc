#include "../utility.h"

#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include <print>
#include <utility>

using input_t = struct {
	std::vector<int64_t> left;
	std::vector<int64_t> right;
};

[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		auto numbers = split_string_and_convert_to_numbers(line, "   ");
		input.left.push_back(numbers[0]);
		input.right.push_back(numbers[1]);
	}

	return input;
}


void solve_1(input_t& input)
{
	int32_t sum = 0;
	std::sort(input.left.begin(), input.left.end());
	std::sort(input.right.begin(), input.right.end());
	for (int i = 0; i < input.left.size(); i++)
	{
		sum += std::abs(input.left[i] - input.right[i]);
	}
	std::print("{}\n", sum);

}

void solve_2(input_t& input)
{
	int32_t sum = 0;
	std::sort(input.left.begin(), input.left.end());
	std::sort(input.right.begin(), input.right.end());
	
	for (int i = 0; i < input.left.size(); i++)
	{
		
		sum += input.left[i] * std::count(input.right.begin(), input.right.end(), input.left[i]);
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