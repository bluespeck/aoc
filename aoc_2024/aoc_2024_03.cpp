#include "utility.h"

#include <array>
#include <string>
#include <vector>
#include <print>

using input_t = std::string;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		input += line;
	}

	return input;
}

enum class op_type_t
{
	multiplication,
	unknown
};

struct operation_t
{
	op_type_t type = op_type_t::unknown;
	int64_t o1 = -1;
	int64_t o2 = -1;
};

using param_list_t = std::vector<int64_t>;

bool try_parse_number(const std::string& str, int64_t& num)
{
	int64_t number = 0;
	for (int32_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9') return false;
		number = number * 10 + str[i] - '0';
	}
	assert(number <= 999);
	num = number;
	return true;
}

param_list_t try_parse_param_list(const std::string& str)
{
	param_list_t params;
	auto parts = split_string(str, ",");
	for (auto& part : parts)
	{
		int64_t number = 0;
		if (try_parse_number(part, number))
		{
			params.push_back(number);
		}
		else
		{
			params.clear(); // invalidate list if not a number is found
			break;
		}
	}
	return params;
}

operation_t try_parse_mul(const std::string& str, size_t& length_parsed)
{
	length_parsed = 0;
	operation_t operation;
	assert(str[0] == 'm');
	if (str[0] != 'm' || str[1] != 'u' || str[2] != 'l') return {};
	for (int32_t i = 3; i < str.size(); i++)
	{
		if(str[i] == '(')
		{
			for (int32_t j = 4; j < str.size(); j++)
			{
				if (str[j] == ')')
				{
					auto params = try_parse_param_list(str.substr(i + 1, j - i - 1));
					if (params.size() == 2)
					{
						operation.o1 = params[0];
						operation.o2 = params[1];
						operation.type = op_type_t::multiplication;
						length_parsed = j + 1;
					}
					break;
				}
			}
			if (length_parsed == 0)
			{
				length_parsed = i + 1;
			}
			break;
		}
		else
		{
			break;
		}
	}
	return operation;
}

void solve_1(const input_t& input)
{
	int64_t sum = 0;
	std::vector<operation_t> ops;
	for (size_t i = 0; i < input.size();)
	{
		auto& c = input[i];
		switch (c)
		{
		case 'm':
		{
			size_t length_parsed = 0;
			operation_t op = try_parse_mul(input.substr(i), length_parsed);
			if (op.type != op_type_t::unknown)
			{
				ops.push_back(op);
				std::println("{}",input.substr(i, length_parsed));
				i = i + length_parsed;
			}
			else
			{
				i++;
			}
			break;
		}
		default:
			i++; break;
		}
	}
	for (auto& op : ops)
	{
		switch (op.type)
		{
		case op_type_t::multiplication:
			sum += op.o1 * op.o2;
		}
	}
	std::print("{}\n", sum);
}

void solve_2(const input_t& input)
{
	bool enabled = true;
	int64_t sum = 0;
	std::vector<operation_t> ops;
	for (size_t i = 0; i < input.size();)
	{
		auto& c = input[i];
		switch (c)
		{
		case 'd':
			if (input.substr(i, 4) == "do()")
			{
				enabled = true;
				i += 4;
			}
			else if (input.substr(i, 7) == "don't()")
			{
				enabled = false;
				i += 7;
			}
			else
			{
				i++;
			}
			break;

		case 'm':
		{
			size_t length_parsed = 0;
			operation_t op = try_parse_mul(input.substr(i), length_parsed);
			if (op.type != op_type_t::unknown)
			{
				if (enabled)
				{
					ops.push_back(op);
					std::println("{}", input.substr(i, length_parsed));
				}
				i = i + length_parsed;
			}
			else
			{
				i++;
			}
			break;
		}
		default:
			i++; break;
		}
	}
	for (auto& op : ops)
	{
		switch (op.type)
		{
		case op_type_t::multiplication:
			sum += op.o1 * op.o2;
		}
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