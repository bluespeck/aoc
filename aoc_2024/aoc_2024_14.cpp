#include "../utility.h"

#include <array>
#include <string>
#include <vector>
#include <print>

struct robot_t
{
	int64_t pi, pj;
	int64_t vi, vj;
};

using input_t = std::vector<robot_t>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		robot_t robot;
		auto pv = split_string(line, " ");
		{
			auto coords = split_string_and_convert_to_numbers(pv[0].substr(2), ",");
			robot.pi = coords[0];
			robot.pj = coords[1];
		}
		{
			auto coords = split_string_and_convert_to_numbers(pv[1].substr(2), ",");
			robot.vi = coords[0];
			robot.vj = coords[1];
		}

		input.push_back(robot);
	}

	return input;
}


void solve_1(const input_t& input_)
{
	int64_t prod = 0;
	input_t input = input_;
	//static constinit int64_t bathroom_width = 11;
	//static constinit int64_t bathroom_height = 7;
	static constinit int64_t bathroom_width = 101;
	static constinit int64_t bathroom_height = 103;
	for (int i = 0; i < 100; i++)
	{
		for (auto& robot : input)
		{
			robot.pi = robot.pi + robot.vi;
			if (robot.pi < 0)
			{
				robot.pi = bathroom_width + robot.pi;
			}
			else if (robot.pi >= bathroom_width)
			{
				robot.pi = robot.pi % bathroom_width;
			}

			robot.pj += robot.vj;
			if (robot.pj < 0)
			{
				robot.pj = bathroom_height + robot.pj;
			}
			else if (robot.pj >= bathroom_height)
			{
				robot.pj = robot.pj % bathroom_height;
			}
		}
	}
	int64_t q1{}, q2{}, q3{}, q4{}; // anticlockwise
	for (auto& robot : input)
	{
		if (robot.pi > bathroom_width / 2 && robot.pj > bathroom_height/2)
		{
			q1++;
		}
		else if (robot.pi > bathroom_width / 2 && robot.pj < bathroom_height / 2)
		{
			q2++;
		}
		else if (robot.pi < bathroom_width / 2 && robot.pj < bathroom_height / 2)
		{
			q3++;
		}
		else if (robot.pi < bathroom_width / 2 && robot.pj > bathroom_height / 2)
		{
			q4++;
		}
	}
	prod = q1 * q2 * q3 * q4;
	std::print("{}\n", prod);

}

void solve_2(const input_t& input_)
{
	input_t input = input_;
	static constexpr int64_t bathroom_width = 101;
	static constexpr int64_t bathroom_height = 103;
	for (int i = 0; i < 100000; i++)
	{
		std::array<int64_t, bathroom_width> counts_h;
		std::array<int64_t, bathroom_width> counts_v;
		counts_h.fill(0);
		counts_v.fill(0);
		for (auto& robot : input)
		{
			robot.pi = robot.pi + robot.vi;
			if (robot.pi < 0)
			{
				robot.pi = bathroom_width + robot.pi;
			}
			else if (robot.pi >= bathroom_width)
			{
				robot.pi = robot.pi % bathroom_width;
			}
			counts_v[robot.pi]++;

			robot.pj += robot.vj;
			if (robot.pj < 0)
			{
				robot.pj = bathroom_height + robot.pj;
			}
			else if (robot.pj >= bathroom_height)
			{
				robot.pj = robot.pj % bathroom_height;
			}
			counts_h[robot.pj]++;
		}
		
		if (std::find_if(counts_h.begin(), counts_h.end(), [](int64_t num) { return num >= 20; }) != counts_h.end() && std::find_if(counts_v.begin(), counts_v.end(), [](int64_t num) { return num >= 20; }) != counts_v.end())
		{
			std::array<std::array<char, bathroom_width>, bathroom_height> grid;
			for (auto& row : grid)
			{
				row.fill(' ');
			}
			for (auto& r : input)
			{
				grid[r.pj][r.pi] = '#';
			}
			std::println("time: {}", i + 1);
			for (auto& row : grid)
			{
				for (char c : row)
				{
					std::print("{}", c);
				}
				std::println();
			}
			std::println();
			break;
		}
	}
	
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}