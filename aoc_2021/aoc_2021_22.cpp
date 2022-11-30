#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "outf.h"

struct Interval
{
    int32_t start = 200'000;
    int32_t stop = 200'000;
};

struct Cuboid
{
    Interval intervals[3];
    bool state = false;
};

using Input = std::vector<Cuboid>;
Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            Cuboid op;
            op.state = line[1] == 'n';
            
            auto intervals = split_string(line.substr(op.state ? 3 : 4), ",");
            assert(intervals.size() == 3);
            for (int i = 0; i < 3; i++)
            {
                auto interval_strs = split_string(intervals[i], "..");
                op.intervals[i].start = std::stol(interval_strs[0].substr(2));
                op.intervals[i].stop = std::stol(interval_strs[1]);
            }
            input.push_back(op);
        }
    }
    return input;
}

void print_cuboid(const Cuboid& e)
{
    outf("{}..{}, {}..{}, {}..{} {} total {}\n", e.intervals[0].start, e.intervals[0].stop, e.intervals[1].start, e.intervals[1].stop, e.intervals[2].start, e.intervals[2].stop, e.state,
        (e.intervals[0].stop - e.intervals[0].start + 1)
        * (e.intervals[1].stop - e.intervals[1].start + 1)
        * (e.intervals[2].stop - e.intervals[2].start + 1));
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    
    bool grid[101][101][101];
    for (int32_t i = 0; i < 101; i++)
        for (int32_t j = 0; j < 101; j++)
            for (int32_t k = 0; k < 101; k++)
            {
                grid[i][j][k] = false;
            }
    int32_t step = 0;
    for (auto& e : input)
    {
        bool found_outside = false;
        for (int32_t i = 0; i < 3; i++)
            if (e.intervals[i].start < -50 || e.intervals[i].stop > 50)
            {
                found_outside = true;
                break;
            }
        if (found_outside)
            continue;

        for(int32_t i = 50 + e.intervals[0].start; i <= 50 + e.intervals[0].stop; i++)
            for (int32_t j = 50 + e.intervals[1].start; j <= 50 + e.intervals[1].stop; j++)
                for (int32_t k = 50 + e.intervals[2].start; k <= 50 + e.intervals[2].stop; k++)
                {
                    grid[i][j][k] = e.state;
                }
        outf("\n After step{} with cuboid ", ++step);
        print_cuboid(e);
        int32_t count = 0;
        for (int32_t i = 0; i < 101; i++)
        {
            for (int32_t j = 0; j < 101; j++)
                for (int32_t k = 0; k < 101; k++)
                {
                    if (grid[i][j][k] == true)
                        count++;
                }
        }
        outf("{} cuboids.", count);
    }

    int32_t count = 0;
    for (int32_t i = 0; i < 101; i++)
    {
        for (int32_t j = 0; j < 101; j++)
            for (int32_t k = 0; k < 101; k++)
            {
                if (grid[i][j][k] == true)
                    count++;
            }
    }
    outf("{}", count);
}


bool intersect_cuboids(const Cuboid& a, const Cuboid& b, std::vector<Cuboid>& fragmented_a, std::vector<Cuboid>& fragmented_b)
{
    // this considers that b comes after a in the sequence

    Interval new_intervals[3][3];
    for (int32_t i = 0; i < 3; i++)
    {
        auto [a1, a2] = a.intervals[i];
        auto [b1, b2] = b.intervals[i];
        if (a2 < b1 || b2 < a1) // a1 a2 b1 b2 | b1 b2 a1 a2
            return false;

        if (a1 <= b1 && b2 <= a2) // a1 b1 b2 a2
        {
            new_intervals[i][0] = { a1, b1 - 1 };
            new_intervals[i][1] = { b1, b2 };
            new_intervals[i][2] = { b2 + 1, a2 };
        }
        else if (b1 <= a1 && a2 <= b2) // b1 a1 a2 b2
        {
            new_intervals[i][0] = { b1, a1 - 1 };
            new_intervals[i][1] = { a1, a2 };
            new_intervals[i][2] = { a2 + 1, b2 };
        }
        else if (b1 <= a1 && a1 <= b2 && b2 <= a2) // b1 a1 b2 a2
        {
                new_intervals[i][0] = { b1, a1 - 1 };
                new_intervals[i][1] = { a1, b2 };
                new_intervals[i][2] = { b2 + 1, a2 };
        }
        else if (a1 <= b1 && b1 <= a2 && a2 <= b2) // a1 b1 a2 b2
        {
            new_intervals[i][0] = { a1, b1 - 1 };
            new_intervals[i][1] = { b1, a2 };
            new_intervals[i][2] = { a2 + 1, b2 };
        }
        else
        {
            return false;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (new_intervals[0][i].start <= new_intervals[0][i].stop
                    && new_intervals[1][j].start <= new_intervals[1][j].stop
                    && new_intervals[2][k].start <= new_intervals[2][k].stop)
                {
                    // box should be considered;
                    Cuboid cuboid;
                    cuboid.intervals[0] = new_intervals[0][i];
                    cuboid.intervals[1] = new_intervals[1][j];
                    cuboid.intervals[2] = new_intervals[2][k];

                    bool in_a = true;
                    bool in_b = true;
                    for (int ii = 0; ii < 3; ii++)
                    {
                        if (cuboid.intervals[ii].start > a.intervals[ii].stop || cuboid.intervals[ii].start < a.intervals[ii].start
                            || cuboid.intervals[ii].stop > a.intervals[ii].stop || cuboid.intervals[ii].stop < a.intervals[ii].start)
                        {
                            in_a = false;
                        }
                        if (cuboid.intervals[ii].start > b.intervals[ii].stop || cuboid.intervals[ii].start < b.intervals[ii].start
                            || cuboid.intervals[ii].stop > b.intervals[ii].stop || cuboid.intervals[ii].stop < b.intervals[ii].start)
                        {
                            in_b = false;
                        }
                    }

                    auto* vector_ref = &fragmented_a;
                    if (in_b)
                    {
                        cuboid.state = b.state;
                        //if (!in_a) // remains with previous value otherwise (fragmented_a)
                        { // keep the intersection cuboid in a, even if its state changes to that of b
                            vector_ref = &fragmented_b; 
                        }
                    }
                    else if (in_a)
                    {
                        cuboid.state = a.state;
                        vector_ref = &fragmented_a;
                    }
                    else
                    {
                        //outf("Discarding ");
                        //print_cuboid(cuboid);
                        continue;
                    }
                    if (i == 1 && j == 1 && k == 1)
                    {
                        // this is the intersection cuboid; don't merge it with anything to keep it visible
                        vector_ref->push_back(cuboid);
                        continue;
                    }
                    // try to merge cuboids if possible
                    if (vector_ref->size() > 0)
                    {
                        auto& prev = vector_ref->back();
                        int ii = -1;
                        int count = 0;
                        for (int l = 0; l < 3; l++)
                        {
                            if (prev.intervals[l].start == cuboid.intervals[l].start && prev.intervals[l].stop == cuboid.intervals[l].stop)
                                count++;
                            else
                                ii = l;
                        }
                        if (count == 2)
                        {
                            if (prev.intervals[ii].start == (cuboid.intervals[ii].stop + 1) && prev.state == cuboid.state)
                            {
                                prev.intervals[ii].start = cuboid.intervals[ii].start;
                                continue;
                            }
                            else if (prev.intervals[ii].stop == (cuboid.intervals[ii].start - 1) && prev.state == cuboid.state)
                            {
                                prev.intervals[ii].stop = cuboid.intervals[ii].stop;
                                continue;
                            }
                        }
                    }
                    vector_ref->push_back(cuboid);
                }
            }
        }
    }
    return true;
}


void solve_2_bad(const Input& input)
{
    outf("\nPart {}: ", 2);
  
    std::vector<Cuboid> evaluated_cuboids;
    int32_t step = 0;
    for (auto& input_cuboid : input)
    {
        outf("\nStep {}\n", ++step);
        if (evaluated_cuboids.empty())
        {
            evaluated_cuboids.push_back(input_cuboid);
        }
        else
        {
            std::vector<Cuboid> generated_evaluated_cuboids;
            
            std::vector<Cuboid> input_fragments;
            input_fragments.push_back(input_cuboid);
            // at each intersection between a new cuboid (input cuboid) and a cuboid from the previous step, 
            // we use up some of the input cuboid, resulting at each step with some input leftover that has to be used with 
            // the rest of the cuboids from the previous step.
            // If at the end of this process we are left with some input leftovers, those get added to the vector of processed cuboids,
            // to be used in the next step (for the next input cuboid).
            for (auto& ef : evaluated_cuboids)
            {
                bool intersected = false;
                std::vector<Cuboid> evaluated_fragments_remaining;
                evaluated_fragments_remaining.push_back(ef);

                std::vector<Cuboid> input_fragments_remaining;
                for (auto& ic : input_fragments)
                {
                    std::vector<Cuboid> resulting_cuboids_evaluated;
                    std::vector<Cuboid> resulting_cuboids_input_fragments;
                    if (intersect_cuboids(ef, ic, resulting_cuboids_evaluated, resulting_cuboids_input_fragments))
                    {
                        generated_evaluated_cuboids.insert(generated_evaluated_cuboids.end(), resulting_cuboids_evaluated.begin(), resulting_cuboids_evaluated.end());
                        input_fragments_remaining.insert(input_fragments_remaining.end(), resulting_cuboids_input_fragments.begin(), resulting_cuboids_input_fragments.end());
                        intersected = true;

                        //outf("\nIntersecting ");
                        //print_cuboid(ef);
                        //outf(" and ");
                        //print_cuboid(ic);
                        //outf(" with results:\n");
                        //for (auto& e : resulting_cuboids_evaluated)
                        //{
                        //    print_cuboid(e);
                        //}
                        //outf("still to check: \n");
                        //for (auto& e : resulting_cuboids_input_fragments)
                        //{
                        //    print_cuboid(e);
                        //}
                        //outf("\n");
                    }
                    else
                    {
                        input_fragments_remaining.push_back(ic);
                    }
                }
                if (!intersected)
                {
                    generated_evaluated_cuboids.push_back(ef);
                    // input_fragments remains unchanged and will be used for subsequent tests in this step
                }
                else
                {
                    input_fragments = input_fragments_remaining;
                }
            }
            evaluated_cuboids = generated_evaluated_cuboids;
            evaluated_cuboids.insert(evaluated_cuboids.end(), input_fragments.begin(), input_fragments.end());
            while (true)
            {
                bool found_conflicts = false;
                for (size_t i = 0; !found_conflicts && i < evaluated_cuboids.size() - 1; i++)
                {
                    for (size_t j = i + 1; !found_conflicts && j < evaluated_cuboids.size(); j++)
                    {
                        std::vector<Cuboid> resulting_cuboids_evaluated;
                        std::vector<Cuboid> resulting_cuboids_input_fragments;
                        if (intersect_cuboids(evaluated_cuboids[i], evaluated_cuboids[j], resulting_cuboids_evaluated, resulting_cuboids_input_fragments))
                        {
                            found_conflicts = true;
                            evaluated_cuboids.erase(evaluated_cuboids.begin() + j);
                            evaluated_cuboids.erase(evaluated_cuboids.begin() + i);
                            evaluated_cuboids.insert(evaluated_cuboids.end(), resulting_cuboids_evaluated.begin(), resulting_cuboids_evaluated.end());
                            evaluated_cuboids.insert(evaluated_cuboids.end(), resulting_cuboids_input_fragments.begin(), resulting_cuboids_input_fragments.end());
                        }
                    }
                }
                if (!found_conflicts)
                    break;
            }
        }

        outf("After step {}, {} cuboids:\n", step, evaluated_cuboids.size());
        int64_t count_on = 0;
        for (size_t i = 0; i < evaluated_cuboids.size(); i++)
        {
            auto& e = evaluated_cuboids[i];
            //print_cuboid(e);
            if (e.state)
            {
                count_on += int64_t(e.intervals[0].stop - e.intervals[0].start + 1)
                    * int64_t(e.intervals[1].stop - e.intervals[1].start + 1)
                    * int64_t(e.intervals[2].stop - e.intervals[2].start + 1);
            }
        }
        outf("Total on {}\n", count_on);
    }

    
    outf("Final cuboids:\n");
    int64_t count_on = 0;
    for (auto & e : evaluated_cuboids)
    {
        //print_cuboid(e);
        if (e.state)
        {
            count_on += int64_t(e.intervals[0].stop - e.intervals[0].start + 1)
                * int64_t(e.intervals[1].stop - e.intervals[1].start + 1)
                * int64_t(e.intervals[2].stop - e.intervals[2].start + 1);
        }
    }
    outf("{}", count_on);
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);

    std::vector<Cuboid> evaluated_cuboids;
    int32_t step = 0;
    for (auto& input_cuboid : input)
    {
        outf("\nStep {}\n", ++step);
        
        std::vector<Cuboid> resulting_cuboids;
        for (auto& e : evaluated_cuboids)
        {
            bool cuboids_overlap = true;
            for (int i = 0; i < 3; i++)
            {
                if (e.intervals[i].stop < input_cuboid.intervals[i].start || e.intervals[i].start > input_cuboid.intervals[i].stop)
                {
                    cuboids_overlap = false;
                    break;
                }
            }
            if (!cuboids_overlap)
            {
                resulting_cuboids.push_back(e);
                continue;
            }
            for (int i = 0; i < 3; i++)
            {
                if (e.intervals[i].start <= input_cuboid.intervals[i].start)
                {
                    Cuboid new_cuboid = e;
                    new_cuboid.intervals[i].stop = input_cuboid.intervals[i].start - 1;
                    resulting_cuboids.push_back(new_cuboid);
                    e.intervals[i].start = new_cuboid.intervals[i].stop + 1;
                }
                if (e.intervals[i].stop >= input_cuboid.intervals[i].stop)
                {
                    Cuboid new_cuboid = e;
                    new_cuboid.intervals[i].start = input_cuboid.intervals[i].stop + 1;
                    resulting_cuboids.push_back(new_cuboid);
                    e.intervals[i].stop = new_cuboid.intervals[i].start - 1;
                }
            }
        }
        resulting_cuboids.push_back(input_cuboid);
        evaluated_cuboids = resulting_cuboids;
        int64_t count_on = 0;
        for (auto& e : evaluated_cuboids)
        {
            //print_cuboid(e);
            if (e.state)
            {
                count_on += int64_t(e.intervals[0].stop - e.intervals[0].start + 1)
                    * int64_t(e.intervals[1].stop - e.intervals[1].start + 1)
                    * int64_t(e.intervals[2].stop - e.intervals[2].start + 1);
            }
        }
        outf("After step {}: {} cuboids are lit.\n", step, count_on);
    }

    int64_t count_on = 0;
    for (auto& e : evaluated_cuboids)
    {
        //print_cuboid(e);
        if (e.state)
        {
            count_on += int64_t(e.intervals[0].stop - e.intervals[0].start + 1)
                * int64_t(e.intervals[1].stop - e.intervals[1].start + 1)
                * int64_t(e.intervals[2].stop - e.intervals[2].start + 1);
        }
    }
    outf("{}", count_on);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
