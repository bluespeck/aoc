#include "../outf.h"


#include <array>
#include <string>
#include <vector>

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
	int32_t sum = 0;
	for (auto& str : input)
	{
		char first_digit = '-';
		char last_digit = '-';
		for (auto& c : str)
		{
			if (c >= '0' && c <= '9')
			{
				if (first_digit == '-')
				{
					first_digit = c;
				}
				last_digit = c;
			}
		}
		int32_t number = (first_digit - '0') * 10 + (last_digit - '0');
		sum += number;
	}
	outf("{}\n", sum);
	
}

int32_t get_calibrated_value(const std::string& line)
{
	const std::array<std::string, 20> spelledOutDigits = {
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9"
	};
	int32_t first_pos = static_cast<int32_t>(line.size());
	int32_t first_digit = -1;
	int32_t last_pos = -1;
	int32_t last_digit = -1;
	for (int32_t i = 0; i < spelledOutDigits.size(); i++)
	{
		const std::string& digit = spelledOutDigits[i];
		{
			int32_t pos = static_cast<int32_t>(line.rfind(digit));
			if (pos != std::string::npos && last_pos < pos)
			{
				last_pos = pos;
				last_digit = line[pos] <= '9' ? line[pos] - '0': i;
			}
		}
		{
			int32_t pos = static_cast<int32_t>(line.find(digit));
			if (pos != std::string::npos && first_pos > pos)
			{
				first_pos = pos;
				first_digit = line[pos] <= '9' ? line[pos] - '0': i;
			}
		}
	}
	
	return first_digit * 10 + last_digit;
}

void solve_2(const input_t& input)
{
	
	int32_t sum = 0;
	for (auto& str : input)
	{
		sum += get_calibrated_value(str);
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