#include "../utility.h"

#include <array>
#include <cassert>
#include <string>
#include <vector>

using Input = std::vector<std::string>;
[[nodiscard]] Input read()
{
	Input input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line.empty()) break;
		input = split_string(line, ",");
	}
	return input;
}

void solve_1(const Input& input)
{
	int64_t sum = 0;
	for (auto& str : input)
	{
		int32_t current_value = 0;
		for (char c : str)
		{
			current_value += c;
			current_value *= 17;
			current_value %= 256;
		}
		sum += current_value;
	}
	
	outf("{}\n", sum);
}

struct Lens
{
	std::string label;
	int32_t focal_length;
};

using Box = std::vector<Lens>;
using Box_Array = std::array<Box, 256>;

void remove_lens(Box& box, const std::string& label)
{
	for (size_t i = 0; i < box.size(); i++)
	{
		if (box[i].label == label)
		{
			for (size_t j = i; j < box.size() - 1; j++)
			{
				box[j] = box[j + 1];
			}
			box.pop_back();
			break;
		}
	}
}

void print_boxes(Box_Array& boxes)
{
	for (size_t i = 0; i < boxes.size(); i++)
	{
		if (boxes[i].empty()) continue;
		outf("Box {}: ", i);
		for (auto& lens : boxes[i])
		{
			outf("[{} {}] ", lens.label, lens.focal_length);
		}
		outf("\n");
	}
	outf("\n");
}

void solve_2(const Input& input)
{
	Box_Array boxes;
	for (auto& str : input)
	{
		int32_t box_index = 0;
		for (char c : str)
		{
			if (c == '-')
			{
				remove_lens(boxes[box_index], str.substr(0, str.size() - 1));
				break;
			}
			else if (c == '=')
			{
				auto tokens = split_string(str, "=");
				assert(tokens.size() == 2);
				bool found = false;
				for (auto& lens : boxes[box_index])
				{
					if (lens.label == tokens[0])
					{
						found = true;
						lens.focal_length = std::stoi(tokens[1]);
						break;
					}
				}
				if (!found)
				{
					boxes[box_index].push_back(Lens{ tokens[0], std::stoi(tokens[1]) });
				}
				break;
			}
			else
			{
				box_index += c;
				box_index *= 17;
				box_index %= 256;
			}
		}
		//outf("After {}\n", str);

		//print_boxes(boxes);
		
	}

	int64_t sum = 0;
	for (size_t i = 0; i < boxes.size(); i++)
	{
		for (size_t j = 0; j < boxes[i].size(); j++)
		{
			int32_t focusing_power = (1 + i) * (j + 1) * boxes[i][j].focal_length;
			sum += focusing_power;
		}
	}

	outf("{}", sum);

}

int main()
{
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}