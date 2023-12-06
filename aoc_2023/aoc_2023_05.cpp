#include "../utility.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <ctype.h>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

struct Map_Interval
{
	std::array<int64_t, 3> values;

	int64_t get_dest() const { return values[0]; }
	int64_t get_src() const { return values[1]; }
	int64_t get_range() const { return values[2]; }
};

using Plant_Map = std::vector < Map_Interval >;

struct Plant_Data
{
	std::vector< int64_t > seeds;
	Plant_Map plant_maps[7];

	Plant_Map& seed_to_soil() { return plant_maps[0]; }
	Plant_Map& soil_to_fertilizer() { return plant_maps[1]; }
	Plant_Map& fertilizer_to_water() { return plant_maps[2]; }
	Plant_Map& water_to_light() { return plant_maps[3]; }
	Plant_Map& light_to_temperature() { return plant_maps[4]; }
	Plant_Map& temperature_to_humidity() { return plant_maps[5]; }
	Plant_Map& humidity_to_location() { return plant_maps[6]; }
};

using input_t = Plant_Data;

[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	std::getline(std::cin, line);
	auto seed_tokens = split_string(line.substr(std::string("seeds: ").length()), " ");
	for (const auto& seed_str : seed_tokens)
	{
		input.seeds.push_back(std::stoll(seed_str));
	}

	int32_t current_map_index = -1;
	while (std::getline(std::cin, line))
	{
		if (line.size() == 0)
		{
			current_map_index++;
			std::getline(std::cin, line); // skip map_name line
			continue;
		}
		
		auto number_tokens = split_string(line, { " " });
		assert(number_tokens.size() == 3);
		Map_Interval map_interval;
		std::ranges::transform(number_tokens, map_interval.values.begin(), [](const std::string& num_str) -> int64_t { return std::stoll(num_str); });
				
		input.plant_maps[current_map_index].emplace_back(map_interval);

	}
	return input;
}

std::vector<int64_t> find_dest_values(const Plant_Map& map, const std::vector<int64_t>& src_values)
{
	std::vector<int64_t> dest_values;
	for (int64_t val : src_values)
	{
		bool was_mapped = false;
		for (const Map_Interval& interval : map)
		{
			if (val >= interval.get_src() && val <= interval.get_src() + interval.get_range())
			{
				dest_values.push_back(interval.get_dest() + val - interval.get_src());
				was_mapped = true;
				break;
			}
		}
		if (!was_mapped)
		{
			dest_values.push_back(val);
		}
	}
	return dest_values;
}

void solve_1(const input_t& input)
{
	std::vector<int64_t> values = input.seeds;
	for (int i = 0; i < 7; i++)
	{
		values = find_dest_values(input.plant_maps[i], values);
	}

	auto it = std::min_element(values.begin(), values.end());

	outf("{}\n", *it);
}

Interval_Vector find_dest_intervals(const Plant_Map& map, const Interval_Vector& src_intervals)
{
	Interval_Vector dest_intervals;
	
	
	for (Interval val : src_intervals)
	{
		Interval_Vector unmapped_intervals = { val };

		for (const Map_Interval& interval : map)
		{
			Interval_Vector new_unmapped_intervals;
			for (const Interval& unmapped_interval : unmapped_intervals)
			{
				Interval_Vector difference_intervals;
				Interval_Vector intersection_intervals;
				Interval interval_to_check = { interval.get_src(), interval.get_src() + interval.get_range() - 1 };
				intersect_diff_intervals(unmapped_interval, interval_to_check, intersection_intervals, difference_intervals);
				if (intersection_intervals.size())
				{
					dest_intervals.push_back( { interval.get_dest() + intersection_intervals[0].first - interval_to_check.first, interval.get_dest() + intersection_intervals[0].first - interval_to_check.first + intersection_intervals[0].second - intersection_intervals[0].first });
				}
				if (difference_intervals.size())
				{
					for (auto& new_unmapped_interval : difference_intervals)
					{
						new_unmapped_intervals.push_back(new_unmapped_interval);
					}
				}
			}
			unmapped_intervals = new_unmapped_intervals;
		}
		if (unmapped_intervals.size())
		{
			for (auto& unmapped_interval : unmapped_intervals)
			{
				dest_intervals.push_back(unmapped_interval);
			}
		}
	}
	return dest_intervals;
}

void solve_2(const input_t& input)
{
	Interval_Vector value_intervals;
	for (size_t i = 0; i < input.seeds.size(); i += 2)
	{
		value_intervals.emplace_back(input.seeds[i], input.seeds[i] + input.seeds[i+1] - 1);
	}
	
	for (int i = 0; i < 7; i++)
	{
		value_intervals = find_dest_intervals(input.plant_maps[i], value_intervals);
		outf("Mapped values step {}: ", i + 1);
		for (auto& interval : value_intervals)
		{
			outf("[{} {}] ", interval.first, interval.second);
		}
		outf("\n");

	}

	int64_t min_location = value_intervals[0].first;
	for (auto& interval : value_intervals)
	{
		if (min_location > interval.first)
		{
			min_location = interval.first;
		}
	}
	
	outf("{}\n", min_location);
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}