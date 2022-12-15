#include "../outf.h"

#include <string>
#include <vector>
#include <unordered_set>

struct point_t
{
    int64_t x;
    int64_t y;
};

struct input_t
{
    std::vector<point_t> sensors;
    std::vector<point_t> closest_beacons;
};

input_t read()
{
    input_t input;
    std::string line;

    while (std::getline(std::cin, line))
    {
        auto parts = split_string(line, ": ");
        auto sensor_parts = split_string(parts[0].substr(std::string("Sensor at ").size()), ", ");
        input.sensors.emplace_back(std::stoll(sensor_parts[0].substr(2)), std::stoll(sensor_parts[1].substr(2)));
        
        auto closest_beacon_parts = split_string(parts[1].substr(std::string("closest beacon is at ").size()), ", ");
        input.closest_beacons.emplace_back(std::stoll(closest_beacon_parts[0].substr(2)), std::stoll(closest_beacon_parts[1].substr(2)));

    }

    return input;
}

int64_t dist_manhattan(const point_t& p1, const point_t& p2)
{
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}


void solve_1(const input_t& input)
{
    int64_t y_to_check = 2000000;
    std::unordered_set<int64_t> point_xs_on_that_line;
    for (size_t i = 0; i < input.sensors.size(); i++)
    {
        int64_t dist_to_closest_beacon = dist_manhattan(input.sensors[i], input.closest_beacons[i]);
        int64_t dist_diff = std::abs(input.sensors[i].y - y_to_check);
        if ( dist_diff <= dist_to_closest_beacon)
        {
            for (int64_t x = (input.sensors[i].x - (dist_to_closest_beacon - dist_diff)); x <= (input.sensors[i].x + (dist_to_closest_beacon - dist_diff)); x++)
            {
                if (x != input.closest_beacons[i].x || y_to_check == input.closest_beacons[i].y)
                {
                    point_xs_on_that_line.insert(x);
                }
            }
        }
    }
    outf("{}\n", point_xs_on_that_line.size());
}

struct interval_t
{
    int64_t start;
    int64_t stop;
};

std::vector<interval_t> diff_interval(const interval_t& i1, const interval_t& i2)
{
    std::vector<interval_t> result;
    if (i1.stop < i2.start || i1.start > i2.stop)
    {
        result.push_back(i1);
    }
    else if (i2.start <= i1.start && i1.stop <= i2.stop)
    {
        return result;
    }
    else if (i1.start < i2.start && i2.stop <= i1.stop)
    {
        result.emplace_back(i1.start, i2.start - 1);
        if (i2.stop < i1.stop)
        {
            result.emplace_back(i2.stop + 1, i1.stop);
        }
    }
    else if(i1.start < i2.start && i2.stop > i1.stop)
    {
        result.emplace_back(i1.start, i2.start - 1);
    }
    else if (i1.stop > i2.stop && i1.start > i2.start)
    {
        result.emplace_back(i2.stop + 1, i1.stop);
    }
    return result;
}

void solve_2(const input_t& input)
{
    constexpr int64_t coord_limit = 4'000'000;
    for (int64_t y_to_check = 0; y_to_check <= coord_limit; y_to_check++)
    {
        std::vector<interval_t> distress_intervals;
        distress_intervals.emplace_back(0, coord_limit);

        for (size_t i = 0; i < input.sensors.size(); i++)
        {
            if (distress_intervals.size() == 0)
                break;
            int64_t dist_to_closest_beacon = dist_manhattan(input.sensors[i], input.closest_beacons[i]);
            int64_t dist_diff = std::abs(input.sensors[i].y - y_to_check);
            if (dist_diff <= dist_to_closest_beacon)
            {
                int64_t start = (input.sensors[i].x - (dist_to_closest_beacon - dist_diff));
                int64_t stop = (input.sensors[i].x + (dist_to_closest_beacon - dist_diff));
                std::vector<interval_t> new_intervals;
                for (auto& interval : distress_intervals)
                {
                    auto result = diff_interval(interval, { start, stop });
                    new_intervals.append_range(result);
                    //outf("Extracting ({}, {}) from ({}, {})", start, stop, interval.start, interval.stop);
                    //outf(" resulting in: ");
                    //for (auto& e : result)
                    //{
                    //    outf("({},{}) ", e.start, e.stop);
                    //}
                    //outf("\n");
                }
                distress_intervals = new_intervals;
            }
        }
        if (distress_intervals.size())
        {
            outf("{}\n", distress_intervals[0].start * 4'000'000+ y_to_check);
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