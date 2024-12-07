#include "../utility.h"

#include <array>
#include <string>
#include <cstdlib>
#include <vector>
#include <print>

struct calibration_t
{
	int64_t result;
	std::vector<int64_t> operands;
};
using input_t = std::vector<calibration_t>;

[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		auto parts = split_string(line, ": ");
		assert(parts.size() == 2);
		calibration_t calibration;
		calibration.result = std::stoll(parts[0]);
		calibration.operands = split_string_and_convert_to_numbers(parts[1], " ");
		input.push_back(calibration);
	}

	return input;
}


void solve_1(const input_t& input)
{
	int64_t result = 0;
	for (auto& calibration : input)
	{
		int32_t num_ops = calibration.operands.size() - 1;
		int32_t num_valid = 0;
		for (int32_t i = 0; i < (1 << num_ops); i++)
		{
			int64_t sum = calibration.operands[0];
			//std::print("{} = {}", calibration.result, calibration.operands[0]);
			int64_t combination = i | (1 << num_ops);
			int64_t operand_index = 1;
			for (; combination > 1; combination >>= 1, operand_index++)
			{
				if (combination % 2 == 0)
				{
					sum += calibration.operands[operand_index];
				}
				else
				{
					sum *= calibration.operands[operand_index];

				}
				//std::print(" {} {}", (combination % 2 == 0) ? "+" : "*", calibration.operands[operand_index]);
			}
			if (sum == calibration.result)
			{
				num_valid++;
				//std::println(" true");
				break;
			}
			else
			{
				//std::println(" false");
			}
		}
		if (num_valid > 0)
		{
			result += calibration.result;
		}
	}
	std::print("{}\n", result);

}

template <int64_t EXPONENT>
constexpr std::array<int64_t, EXPONENT + 1> get_powers_of_10()
{
	std::array<int64_t, EXPONENT + 1> powers_of_10;
	powers_of_10[0] = 1;
	for (int64_t i = 1; i <= EXPONENT; i++)
	{
		powers_of_10[i] = 10 * powers_of_10[i - 1];
	}
	return powers_of_10;
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

void solve_2(const input_t& input)
{
	int64_t result = 0;
	for (auto& calibration : input)
	{
		int32_t num_ops = calibration.operands.size() - 1;
		int32_t num_valid = 0;
		int32_t num_combinations = static_cast<int32_t>(std::pow(3, num_ops));
		for (int32_t i = 0; i < num_combinations; i++)
		{
			int64_t sum = calibration.operands[0];
			//std::print("{} = {}", calibration.result, calibration.operands[0]);
			int64_t combination = i + num_combinations;
			int64_t operand_index = 1;
			for (; combination > 1; combination /= 3, operand_index++)
			{
				if (combination % 3 == 0)
				{
					sum += calibration.operands[operand_index];
				}
				else if(combination % 3 == 1)
				{
					sum *= calibration.operands[operand_index];
				}
				else
				{
					constexpr static auto powers_of_10 = get_powers_of_10<15>();
					sum = sum * powers_of_10[get_num_digits(calibration.operands[operand_index])] + calibration.operands[operand_index];
				}
				//std::print(" {} {}", ((combination % 3 == 0) ? "+" : (combination % 3 == 1) ? "*" : "||"), calibration.operands[operand_index]);
			}
			if (sum == calibration.result)
			{
				num_valid++;
				//std::println(" true");
				break;
			}
			else
			{
				//std::println(" false");
			}
		}
		if (num_valid > 0)
		{
			result += calibration.result;
		}
	}
	std::print("{}\n", result);
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}