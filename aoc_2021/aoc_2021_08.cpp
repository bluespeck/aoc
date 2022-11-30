#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "outf.h"

struct Entry
{
    std::vector<std::string> in;
    std::array<std::string, 4> out;
};
using Input = std::vector<Entry>;

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            Entry entry;
            auto in_out = split_string(line, " | ");
            entry.in = split_string(in_out[0], " ");
            auto out_strs = split_string(in_out[1], " ");
            for (int i = 0; i < 4; i++)
            {
                entry.out[i] = out_strs[i];
            }
            input.push_back(entry);
        }
    }
    return input;
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    int count = 0;
    for (auto e : input)
    {
        for (auto str : e.out)
        {
            auto sz = str.size();
            if (sz == 2 || sz == 4 || sz == 3 || sz == 7)
                count++;
        }
    }
    outf("{}", count);
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);
    int64_t sum = 0;
    std::unordered_map<uint8_t, std::vector<int8_t>> digit_to_segments;
    //  0000
    // 1    6
    // 1    6
    //  2222
    // 3    5
    // 3    5
    //  4444
    // 
    // 0 -> 0 1 3 4 5 6
    // 1 -> 5 6
    // 2 -> 0 2 3 4 6
    // 3 -> 0 2 4 5 6
    // 4 -> 1 2 5 6
    // 5 -> 0 1 2 4 5
    // 6 -> 0 1 2 3 4 5
    // 7 -> 0 5 6
    // 8 -> 0 1 2 3 4 5 6
    // 9 -> 0 1 2 4 5 6
    digit_to_segments[0] = { 0, 1, 3, 4, 5, 6 };
    digit_to_segments[1] = { 5, 6 };
    digit_to_segments[2] = { 0, 2, 3, 4, 6 };
    digit_to_segments[3] = { 0, 2, 4, 5, 6 };
    digit_to_segments[4] = { 1, 2, 5, 6 };
    digit_to_segments[5] = { 0, 1, 2, 4, 5 };
    digit_to_segments[6] = { 0, 1, 2, 3, 4, 5 };
    digit_to_segments[7] = { 0, 5, 6 };
    digit_to_segments[8] = { 0, 1, 2, 3, 4, 5, 6 };
    digit_to_segments[9] = { 0, 1, 2, 4, 5, 6 };

    for (auto entry : input)
    {
        std::unordered_map<int8_t, std::unordered_set<char>> digit_to_wires;
        std::unordered_map<int8_t, char> segment_to_wire;

        for (auto str : entry.in)
        {
            auto sz = str.size();
            if (sz == 2)
            {
                digit_to_wires[1].insert(str.begin(), str.end());
            }
            else if (sz == 4)
            {
                digit_to_wires[4].insert(str.begin(), str.end());
            }
            else if (sz == 3)
            {
                digit_to_wires[7].insert(str.begin(), str.end());
            }
            else if (sz == 7)
            {
                digit_to_wires[8].insert(str.begin(), str.end());
            }
            else
            {
                // 0 6 9 -> 6 segments
                // 2 3 5 -> 5 segments
            }
        }

        // deduce segment 0 from difference between digits 7 and 1
        for (auto ch : digit_to_wires[7])
        {
            if (!digit_to_wires[1].contains(ch))
            {
                segment_to_wire[0] = ch;
                break;
            }
        }
        // deduce possible segments for 3 from the segments of 7; from 2, 3 and 5, only 3 has all of them
        for (auto str : entry.in)
        {
            auto sz = str.size();
            if (sz == 5) // digits 2 3 or 5
            {
                bool found_digit_three = true;
                for (auto ch : digit_to_wires[7])
                {
                    if (!str.contains(ch))
                    {
                        found_digit_three = false;
                        break;
                    }
                }
                if (found_digit_three)
                {
                    digit_to_wires[3].insert(str.begin(), str.end());
                    break;
                }
            }
        }

        // deduce segment 1 since it appears in digit 4 but not in digit 3
        for (auto ch : digit_to_wires[4])
        {
            if (!digit_to_wires[3].contains(ch))
            {
                segment_to_wire[1] = ch;
                break;
            }
        }
        // deduce the segments of digit 5 since from 2, 3, 5, only it has segment 1

        for (auto str : entry.in)
        {
            auto sz = str.size();
            if (sz == 5) // digits 2 3 or 5
            {
                if (str.contains(segment_to_wire[1]))
                {
                    digit_to_wires[5].insert(str.begin(), str.end());
                    break;
                }
            }
        }

        // deduce segment 6 from difference between digits 3 and 5
        for (auto ch : digit_to_wires[3])
        {
            if (!digit_to_wires[5].contains(ch))
            {
                segment_to_wire[6] = ch;
                break;
            }
        }
        // deduce segment 3 from difference between digits 5 and 8
        for (auto ch : digit_to_wires[8])
        {
            if (!digit_to_wires[5].contains(ch) && ch != segment_to_wire[6])
            {
                segment_to_wire[3] = ch;
                break;
            }

        }
        // deduce segment 4 from difference between digits 5 and 4
        for (auto ch : digit_to_wires[5])
        {
            if (!digit_to_wires[4].contains(ch) && ch != segment_to_wire[0])
            {
                segment_to_wire[4] = ch;
                break;
            }

        }
        // deduce segment 2 from difference between digits 4 and 1
        for (auto ch : digit_to_wires[4])
        {
            if (!digit_to_wires[1].contains(ch) && ch != segment_to_wire[1])
            {
                segment_to_wire[2] = ch;
                break;
            }
        }
        // deduce the last segment, segment 5
        for (auto e : "abcdefg")
        {
            bool found = false;
            for (int8_t i = 0; i < 5; i++)
            {
                if (segment_to_wire[i] == e)
                {
                    found = true;
                    break;
                }
            }
            if (!found && segment_to_wire[6] != e)
            {
                segment_to_wire[5] = e;
                break;
            }
        }

        std::unordered_map<std::string, int8_t> sorted_wires_to_digit;
        for (int8_t i = 0; i < 10; i++)
        {
            std::string wires;
            for (auto e : digit_to_segments[i])
            {
                wires += segment_to_wire[e];
            }
            std::sort(wires.begin(), wires.end());
            sorted_wires_to_digit[wires] = i;
        }

        int64_t number = 0;
        for (auto& e : entry.out)
        {
            std::sort(e.begin(), e.end());
            assert(sorted_wires_to_digit.contains(e));
            number = number * 10 + sorted_wires_to_digit[e];
        }
        sum += number;
    }
    outf("{}", sum);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
