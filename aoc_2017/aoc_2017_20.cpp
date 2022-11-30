#include "../outf.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <format>
#include <span>
#include <string>
#include <vector>

struct vec3_t
{
    int64_t x, y, z;
};

struct point_t
{
    vec3_t p, v, a;
};

int64_t manhattan_dist(vec3_t p)
{
    return (p.x > 0 ? p.x : -p.x)
        + (p.y > 0 ? p.y : -p.y)
        + (p.z > 0 ? p.z : -p.z);
}

bool points_are_equal(vec3_t p1, vec3_t p2)
{
    return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}

using input_t = std::vector<point_t>;
input_t read()
{
    input_t input;
    std::string line;
    while (std::getline(std::cin, line))
    {
        point_t point{ {3},{2},{1} };
        auto component_strs = split_string(line, ", ");
        assert(component_strs.size() == 3);
        std::span<int64_t> comps{&point.p.x, 9};
        size_t i = 0;
        for (auto comp_str : component_strs)
        {
            auto subcomp_strs = split_string(comp_str.substr(3, comp_str.size() - 4), ",");
            assert(subcomp_strs.size() == 3);
            for (auto subcomp_str : subcomp_strs)
            {
                comps[i++] = std::stoll(subcomp_str);
            }
        }
        input.emplace_back(point);
    }

    //outf("Finished reading {} points\n", input.size());
    return input;
}


void solve_1(const input_t& input)
{
    input_t state = input;

    for (int ii = 0; ii < 10000; ii++)
    {
        for (auto& point_entry : state)
        {
            point_entry.v.x += point_entry.a.x;
            point_entry.v.y += point_entry.a.y;
            point_entry.v.z += point_entry.a.z;
            point_entry.p.x += point_entry.v.x;
            point_entry.p.y += point_entry.v.y;
            point_entry.p.z += point_entry.v.z;
        }
    }
    int64_t min_dist = manhattan_dist(state[0].p);
    size_t min_i = 0;
    for (size_t i = 1; i < state.size(); i++)
    {
        int64_t dist = manhattan_dist(state[i].p);
        if (dist < min_dist)
        {
            min_dist = dist;
            min_i = i;
        }
    }
    outf("Closest to 0 particle is {} <{},{},{}>\n", min_i, state[min_i].p.x, state[min_i].p.y, state[min_i].p.z);

}

void solve_2(const input_t& input)
{
    input_t state = input;
    for (int ii = 0; ii < 10000; ii++)
    {
        for (auto& point_entry : state)
        {
            point_entry.v.x += point_entry.a.x;
            point_entry.v.y += point_entry.a.y;
            point_entry.v.z += point_entry.a.z;
            point_entry.p.x += point_entry.v.x;
            point_entry.p.y += point_entry.v.y;
            point_entry.p.z += point_entry.v.z;
        }

        bool found_collisions;
        do
        {
            found_collisions = false;
            std::vector<size_t> identical;
            for (size_t i = 0; i < state.size() - 1; i++)
            {
                identical.clear();
                for (size_t j = i + 1; j < state.size(); j++)
                {
                    if (points_are_equal(state[i].p, state[j].p))
                    {
                        identical.push_back(j);
                    }
                }
                if (identical.size() > 0)
                {
                    //outf("Found a collision with {} particles.", identical.size());
                    found_collisions = true;
                    identical.push_back(i);
                    auto state_copy = state;
                    state.clear();
                    for (size_t k = 0; k < state_copy.size(); k++)
                    {
                        if (std::find(identical.begin(), identical.end(), k) == identical.end())
                        {
                            state.push_back(state_copy[k]);
                        }
                    }
                    //outf(" {} particles remaining.\n", state.size());
                    break;
                }
            }
        } while (found_collisions);
    }

    outf("Particles remaining: {}\n", state.size());
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}