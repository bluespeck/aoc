#include "../utility.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <ctype.h>
#include <numeric>
#include <string>
#include <ranges>
#include <vector>
#include <thread>

struct Spring_Info
{
	std::string spring_pattern;
	std::vector<int64_t> damaged_group_lenghts;
	std::int32_t total_damaged_springs;
};
using Input = std::vector<Spring_Info>;
[[nodiscard]] Input read()
{
	Input input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		Spring_Info si;
		auto tokens = split_string(line, " ");
		si.spring_pattern = tokens[0];
		si.damaged_group_lenghts = split_string_and_convert_to_numbers(tokens[1], ",");
		si.total_damaged_springs = 0;
		for (auto& group_size : si.damaged_group_lenghts)
		{
			si.total_damaged_springs += group_size;
		}
		input.emplace_back(std::move(si));
	}
	return input;
}

bool is_valid(int32_t number, const Spring_Info& record)
{
	std::string bits;
	bits.reserve(record.spring_pattern.size());
	while (number)
	{
		bits.push_back(number % 2 == 0 ? '#' : '.');
		number >>= 1;
	}
	if (std::ranges::count(bits, '#') + record.spring_pattern.size() - bits.size() > record.total_damaged_springs) return false;
	for (size_t i = bits.size(); i < record.spring_pattern.size(); i++)
	{
		bits.push_back('#');
	}
	std::ranges::reverse(bits);

	size_t i = 0;
	for (auto& group_size : record.damaged_group_lenghts)
	{
		for (; bits[i] == '.'; i++)
		{
			if (i >= record.spring_pattern.size() || record.spring_pattern[i] == '#') return false;
		}
		if (i >= record.spring_pattern.size() || record.spring_pattern[i] == '.') return false;
		
		size_t count = 1;
		for (++i; i < record.spring_pattern.size() && bits[i] == '#' && record.spring_pattern[i] != '.'; count++, i++);
		if (count != group_size) return false;
		if (i >= record.spring_pattern.size() || record.spring_pattern[i] == '#' || bits[i] != '.') return false;
		++i;
	}
	
	for(; i < record.spring_pattern.size(); i++)
	{
		if (bits[i] != '1' || record.spring_pattern[i] == '#') return false;
	}

	outf("{}\n", bits);
	return true;
}

void back_track(Spring_Info& info, size_t current_index, size_t current_group_index, size_t current_group_parsed_length, size_t& count)
{
	if (current_index == info.spring_pattern.size() && current_group_index == info.damaged_group_lenghts.size())
	{
		//outf("{}\n", info.spring_pattern);
		count++;
		return;
	}
	if (current_index == info.spring_pattern.size())
	{
		if (current_group_index == info.damaged_group_lenghts.size() - 1 && current_group_parsed_length == info.damaged_group_lenghts[current_group_index])
		{
			//outf("{}\n", info.spring_pattern);
			count++;
			return;
		}
		return;
	}
	if (current_group_index == info.damaged_group_lenghts.size())
	{
		if (std::find(info.spring_pattern.begin() + current_index, info.spring_pattern.end(), '#') == info.spring_pattern.end())
		{
			//outf("{}\n", info.spring_pattern);
			count++;
			return;
		}
		else
		{
			return;
		}
	}
	if (info.spring_pattern[current_index] == '.')
	{
		if (current_group_parsed_length > 0)
		{
			if (current_group_parsed_length < info.damaged_group_lenghts[current_group_index])
			{
				return;
			}
			back_track(info, current_index + 1, current_group_index + 1, 0, count);
		}
		else
		{
			back_track(info, current_index + 1, current_group_index, 0, count);
		}
	}
	else if (info.spring_pattern[current_index] == '#')
	{
		if (current_group_parsed_length >= info.damaged_group_lenghts[current_group_index])
		{
			return;
		}
		back_track(info, current_index + 1, current_group_index, current_group_parsed_length + 1, count);
	}
	else // ?
	{
		info.spring_pattern[current_index] = '#';
		back_track(info, current_index, current_group_index, current_group_parsed_length, count);
		info.spring_pattern[current_index] = '.';
		back_track(info, current_index, current_group_index, current_group_parsed_length, count);
		info.spring_pattern[current_index] = '?';
	}

}

void solve_1(const Input& input)
{
	size_t sum = 0;
	for (auto& record : input)
	{
		size_t count = 0;
		//int32_t limit = 1 << record.spring_pattern.size();
		//for (int32_t i = 0; i < limit; i++)
		//{
		//	count += is_valid(i, record);
		//}
		Spring_Info spring_record = record;
		back_track(spring_record, 0, 0, 0, count);
		sum += count;
	}

	outf("{}\n", sum);
}

void solve_2(const Input& input)
{
	Input larger_input;

	const std::string replacements[16] =
	{
		"....",
		"...#",
		"..#.",
		"..##",
		".#..",
		".#.#",
		".##.",
		".###",
		"#...",
		"#..#",
		"#.#.",
		"#.##",
		"##..",
		"##.#",
		"###.",
		"####"
	};

	
	std::vector<std::jthread> threads;
	std::vector<size_t> counts(input.size(), 0);
	for (size_t i = 0; i < input.size();  i++)
	{
		auto& record = input[i];
		threads.emplace_back([&record, &replacements, i, &outer_count=counts[i]]() {
			for (size_t k = 0; k < 16; k++)
			{
				Spring_Info si = record;
				for (size_t l = 0; l < 4; l++)
				{
					si.spring_pattern.push_back(replacements[k][l]);
					si.spring_pattern.append(record.spring_pattern);
					si.damaged_group_lenghts.append_range(record.damaged_group_lenghts);
				}

				size_t count = 0;
				back_track(si, 0, 0, 0, count);
				outer_count += count;
			}
			outf("record#{} count = {}\n",i, outer_count);
			});
	}
	for (auto& th: threads)
	{
		th.join();
	}
	size_t sum = 0;
	for (auto&count: counts)
	{
		sum += count;
	}
	outf("{}\n", sum);
}

int main()
{
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}