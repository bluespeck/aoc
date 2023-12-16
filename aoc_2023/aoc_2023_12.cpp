#include "../utility.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <numeric>
#include <string>
#include <ranges>
#include <vector>
#include <thread>
#include <tuple>
#include <unordered_map>

struct Spring_Info
{
	std::string spring_pattern;
	std::vector<int64_t> damaged_group_lenghts;
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
		input.emplace_back(std::move(si));
	}
	return input;
}

void back_track(Spring_Info& si, size_t current_index, size_t current_group_index, size_t current_group_parsed_length, size_t& count)
{
	if (current_index == si.spring_pattern.size() && current_group_index == si.damaged_group_lenghts.size())
	{
		//outf("{}\n", info.spring_pattern);
		count++;
		return;
	}
	if (current_index == si.spring_pattern.size())
	{
		if (current_group_index == si.damaged_group_lenghts.size() - 1 && current_group_parsed_length == si.damaged_group_lenghts[current_group_index])
		{
			//outf("{}\n", info.spring_pattern);
			count++;
			return;
		}
		return;
	}
	if (current_group_index == si.damaged_group_lenghts.size())
	{
		if (std::find(si.spring_pattern.begin() + current_index, si.spring_pattern.end(), '#') == si.spring_pattern.end())
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
	if (si.spring_pattern[current_index] == '.')
	{
		if (current_group_parsed_length > 0)
		{
			if (current_group_parsed_length < si.damaged_group_lenghts[current_group_index])
			{
				return;
			}
			back_track(si, current_index + 1, current_group_index + 1, 0, count);
		}
		else
		{
			back_track(si, current_index + 1, current_group_index, 0, count);
		}
	}
	else if (si.spring_pattern[current_index] == '#')
	{
		if (current_group_parsed_length >= si.damaged_group_lenghts[current_group_index])
		{
			return;
		}
		back_track(si, current_index + 1, current_group_index, current_group_parsed_length + 1, count);
	}
	else // ?
	{
		if (current_group_parsed_length > 0)
		{
			if (current_group_parsed_length >= si.damaged_group_lenghts[current_group_index])
			{
				back_track(si, current_index + 1, current_group_index + 1, 0, count);
			}
		}
		else
		{
			back_track(si, current_index + 1, current_group_index, 0, count);
		}
		if (current_group_parsed_length < si.damaged_group_lenghts[current_group_index])
		{
			back_track(si, current_index + 1, current_group_index, current_group_parsed_length + 1, count);
		}
	}

}

size_t back_track_i(Spring_Info& si)
{
	size_t count = 0;
	size_t remaining_possible_damaged_springs = std::ranges::count_if(si.spring_pattern, [](char c) { return c != '.'; });
	size_t remaining_damaged_springs_to_find = std::accumulate(si.damaged_group_lenghts.cbegin(), si.damaged_group_lenghts.cend(), 0);
	size_t stack_index = 0;
	std::array<size_t, 110> current_group_index_stack;
	current_group_index_stack.fill(0);
	std::array<size_t, 110> current_group_parsed_length_stack;
	current_group_parsed_length_stack.fill(0);

	while (true)
	{
		if (stack_index == si.spring_pattern.size())
		{
			if (current_group_index_stack[stack_index] == si.damaged_group_lenghts.size())
			{
				//outf("{}\n", si.spring_pattern);
				count++;
			}
			else if (current_group_index_stack[stack_index] == si.damaged_group_lenghts.size() - 1 && current_group_parsed_length_stack[stack_index] == si.damaged_group_lenghts[current_group_index_stack[stack_index]])
			{
				//outf("{}\n", si.spring_pattern);
				count++;
			}
			--stack_index;
		}
		else if (current_group_index_stack[stack_index] == si.damaged_group_lenghts.size())
		{
			if (std::find(si.spring_pattern.begin() + stack_index, si.spring_pattern.end(), '#') == si.spring_pattern.end())
			{
				//outf("{}\n", si.spring_pattern);
				count++;
			}
			// we filled all our groups. should backtrack.
			--stack_index;
		}
		else if (remaining_possible_damaged_springs < remaining_damaged_springs_to_find)
		{
			// we cannot complete our quest
			if (stack_index == 0) break;
			--stack_index;
		}
		else if (si.spring_pattern[stack_index] == '.')
		{
			si.spring_pattern[stack_index] = '4';
			if (current_group_parsed_length_stack[stack_index] > 0)
			{
				if (current_group_parsed_length_stack[stack_index] >= si.damaged_group_lenghts[current_group_index_stack[stack_index]])
				{
					current_group_index_stack[stack_index + 1] = current_group_index_stack[stack_index] + 1;
					current_group_parsed_length_stack[stack_index + 1] = 0;
					++stack_index;
					continue;
					//back_track(si, current_index + 1, current_group_index + 1, 0, count);
				}
			}
			else
			{
				current_group_index_stack[stack_index + 1] = current_group_index_stack[stack_index];
				current_group_parsed_length_stack[stack_index + 1] = 0;
				++stack_index;
				continue;
				//back_track(si, current_index + 1, current_group_index, 0, count);
			}
		}
		else if (si.spring_pattern[stack_index] == '#')
		{
			remaining_possible_damaged_springs--;
			si.spring_pattern[stack_index] = '5';
			if (current_group_parsed_length_stack[stack_index] < si.damaged_group_lenghts[current_group_index_stack[stack_index]])
			{
				current_group_index_stack[stack_index + 1] = current_group_index_stack[stack_index];
				current_group_parsed_length_stack[stack_index + 1] = current_group_parsed_length_stack[stack_index] + 1;
				remaining_damaged_springs_to_find--;
				++stack_index;
				continue;
				//back_track(si, current_index + 1, current_group_index, current_group_parsed_length + 1, count);
			}
		}
		else if (si.spring_pattern[stack_index] == '?')
		{
			remaining_possible_damaged_springs--;
			si.spring_pattern[stack_index] = '1'; // pick # first
			if (current_group_parsed_length_stack[stack_index] < si.damaged_group_lenghts[current_group_index_stack[stack_index]])
			{
				current_group_index_stack[stack_index + 1] = current_group_index_stack[stack_index];
				current_group_parsed_length_stack[stack_index + 1] = current_group_parsed_length_stack[stack_index] + 1;
				remaining_damaged_springs_to_find--;
				++stack_index;
				continue;
				//back_track(si, current_index + 1, current_group_index, current_group_parsed_length + 1, count);
			}
		}

		if (si.spring_pattern[stack_index] == '2')
		{
			remaining_possible_damaged_springs++;
			si.spring_pattern[stack_index] = '?';
		}
		else if (si.spring_pattern[stack_index] == '1')
		{
			remaining_damaged_springs_to_find += (current_group_parsed_length_stack[stack_index] < si.damaged_group_lenghts[current_group_index_stack[stack_index]]);
			si.spring_pattern[stack_index] = '2';
			if (current_group_parsed_length_stack[stack_index] > 0)
			{
				if (current_group_parsed_length_stack[stack_index] >= si.damaged_group_lenghts[current_group_index_stack[stack_index]])
				{
					current_group_index_stack[stack_index + 1] = current_group_index_stack[stack_index] + 1;
					current_group_parsed_length_stack[stack_index + 1] = 0;
					++stack_index;
					continue;
					//back_track(si, current_index + 1, current_group_index + 1, 0, count);
				}
				remaining_possible_damaged_springs++;
				si.spring_pattern[stack_index] = '?';
			}
			else
			{
				current_group_index_stack[stack_index + 1] = current_group_index_stack[stack_index];
				current_group_parsed_length_stack[stack_index + 1] = 0;
				++stack_index;
				continue;
				//back_track(si, current_index + 1, current_group_index, 0, count);
			}
		}
		else if (si.spring_pattern[stack_index] == '4')
		{
			si.spring_pattern[stack_index] = '.';
		}
		else if (si.spring_pattern[stack_index] == '5')
		{
			si.spring_pattern[stack_index] = '#';
			remaining_possible_damaged_springs++;
			remaining_damaged_springs_to_find += (current_group_parsed_length_stack[stack_index] < si.damaged_group_lenghts[current_group_index_stack[stack_index]]);
		}
		else assert(false);
		if (stack_index == 0) break;
		--stack_index;
	}

	return count;
}

void solve_1(const Input& input)
{
	size_t sum = 0;
	for (auto& record : input)
	{
		size_t count = 0;
		
		Spring_Info spring_record = record;
		//back_track(spring_record, 0, 0, 0, count);
		count = back_track_i(spring_record);
		sum += count;
	}

	outf("{}\n", sum);
}

using Map = std::unordered_map<size_t, size_t>;
size_t make_key(int8_t current_index, int8_t current_group_index, int8_t current_group_parsed_length)
{
	return (current_group_parsed_length << 16) | (current_group_index << 8) | current_index;
}

size_t back_track_memo(Spring_Info& si, Map& map, int8_t current_index, int8_t current_group_index, int8_t current_group_parsed_length)
{
	if (size_t key = make_key(current_index, current_group_index, current_group_parsed_length); map.find(key) != map.end())
	{
		return map[key];
	}

	if (current_index == si.spring_pattern.size() && current_group_index == si.damaged_group_lenghts.size())
	{
		return 1;
	}
	if (current_index == si.spring_pattern.size())
	{
		if (current_group_index == si.damaged_group_lenghts.size() - 1 && current_group_parsed_length == si.damaged_group_lenghts[current_group_index])
		{
			return 1;
		}
		return 0;
	}
	if (current_group_index == si.damaged_group_lenghts.size())
	{
		return std::find(si.spring_pattern.begin() + current_index, si.spring_pattern.end(), '#') == si.spring_pattern.end();
	}

	if (si.spring_pattern[current_index] == '.')
	{
		if (current_group_parsed_length > 0)
		{
			if (current_group_parsed_length < si.damaged_group_lenghts[current_group_index])
			{
				return 0;
			}
			return back_track_memo(si, map, current_index + 1, current_group_index + 1, 0);
		}
		else
		{
			return back_track_memo(si, map, current_index + 1, current_group_index, 0);
		}
	}
	else if (si.spring_pattern[current_index] == '#')
	{
		if (current_group_parsed_length >= si.damaged_group_lenghts[current_group_index])
		{
			return 0;
		}
		back_track_memo(si, map, current_index + 1, current_group_index, current_group_parsed_length + 1);
	}
	else // '?'
	{
		size_t count = 0;
		if (current_group_parsed_length > 0)
		{
			if (current_group_parsed_length >= si.damaged_group_lenghts[current_group_index])
			{
				count = back_track_memo(si, map, current_index + 1, current_group_index + 1, 0);
			}
		}
		else
		{
			count = back_track_memo(si, map, current_index + 1, current_group_index, 0);
		}

		if (current_group_parsed_length < si.damaged_group_lenghts[current_group_index])
		{
			count += back_track_memo(si, map, current_index + 1, current_group_index, current_group_parsed_length + 1);
		}
		map[make_key(current_index, current_group_index, current_group_parsed_length)] = count;
		return count;
	}
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
		//threads.emplace_back([&record, &replacements, i, &outer_count=counts[i]]() {
			{
				Map map;
				Spring_Info si = record;
				for (size_t l = 0; l < 4; l++)
				{
					//si.spring_pattern.push_back(replacements[k][l]);
					si.spring_pattern.push_back('?');
					si.spring_pattern.append(record.spring_pattern);
					si.damaged_group_lenghts.append_range(record.damaged_group_lenghts);
				}

				size_t count = 0;
				//back_track(si, 0, 0, 0, count); // 2^95
				// count = back_track_i(si); // 2^95
				count = back_track_memo(si, map, 0, 0, 0); // faster ^^'
				//outer_count += count;
				counts[i] = count;
			}
			//outf("record#{} count = {}\n",i, outer_count);
			//});
	}
	//for (auto& th: threads)
	//{
	//	th.join();
	//}
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