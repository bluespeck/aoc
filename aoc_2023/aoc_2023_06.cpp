#include "../utility.h"

#include <array>
#include <cassert>
#include <string>
#include <vector>

struct Race
{
	int64_t time;
	int64_t distance;
};

using input_t = std::vector<Race>;

[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;

	std::getline(std::cin, line);
	auto times = split_string_and_convert_to_numbers(line.substr(std::string("Time:       ").length()), " ");
	
	std::getline(std::cin, line);
	auto distances = split_string_and_convert_to_numbers(line.substr(std::string("Distance:   ").length()), " ");
	
	assert(times.size() == distances.size());
	
	for (size_t i = 0; i < times.size(); i++)
	{
		input.push_back({ times[i], distances[i] });
	}
	return input;
}

void solve_1(const input_t& input)
{
	std::vector<int64_t> counts;

	for (auto& race : input)
	{
		int64_t count = 0;
		int64_t t = race.time;
		for (int64_t i = 1; i <= t - 1; i++)
		{
			int64_t dist = i * (t - i);
			if (dist > race.distance)
			{
				count++;
			}
		}
		counts.push_back(count);
	}

	int64_t p = 1;
	for (auto& count : counts)
	{
		p *= count;
	}
	outf("{}\n", p);
}

void solve_2(const input_t& input)
{
	std::string timeStr;
	std::string distanceStr;
	for (auto& race: input)
	{
		timeStr += std::to_string(race.time);
		distanceStr += std::to_string(race.distance);
	}
	int64_t time = std::stoll(timeStr);
	int64_t distance = std::stoll(distanceStr);
		
	int64_t first_time = -1;
	int64_t last_time = -1;
	for (int64_t i = 1; i <= time - 1; i++)
	{
		int64_t dist = i * (time - i);
		if (dist > distance)
		{
			first_time = i;
			break;
		}
	}
	for (int64_t i = time - 1; i > 1; i--)
	{
		int64_t dist = i * (time - i);
		if (dist > distance)
		{
			last_time = i;
			break;
		}
	}

	outf("{}\n", last_time - first_time + 1);
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}