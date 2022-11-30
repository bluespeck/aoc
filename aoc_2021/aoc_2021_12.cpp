#include <iostream>
#include <string>
#include <vector>
#include "outf.h"

using Input = std::vector<std::pair<std::string, std::string>>;

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            auto vec = split_string(line, "-");
            input.emplace_back( vec[0], vec[1]);
        }
    }
    return input;
}

void literally_back_track(const Input& input, std::vector<std::string>& current_path, int32_t& valid_path_count, bool small_caves_can_repeat_once)
{
    if (current_path.back() == "end")
    {
        //for (auto& cave : current_path)
        //    outf("{} ", cave);
        //outf("\n");
        valid_path_count++;
        return;
    }

    for (auto& edge : input)
    {
        if (current_path.back() != edge.first && current_path.back() != edge.second)
            continue;
        std::string source = edge.first;
        std::string dest = edge.second;
        if (current_path.back() == edge.second)
        {
            source = edge.second;
            dest = edge.first;
        }
        
        if (dest[0] >= 97 && std::find(current_path.begin(), current_path.end(), dest) != current_path.end())
        {
            if (small_caves_can_repeat_once && dest != "start" && dest != "end")
            {
                current_path.push_back(dest);
                literally_back_track(input, current_path, valid_path_count, false);
                current_path.pop_back();
            }
        }
        else
        {
            current_path.push_back(dest);
            literally_back_track(input, current_path, valid_path_count, small_caves_can_repeat_once);
            current_path.pop_back();
        }
    }

}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    
    int32_t valid_paths_count = 0;

    std::vector<std::string> current_path;
    current_path.push_back("start");
    literally_back_track(input, current_path, valid_paths_count, false);
    
    outf("{}", valid_paths_count);
}

void solve_2(const Input& input)
{
    
    outf("\nPart {}: ", 2);

    int32_t valid_paths_count = 0;

    std::vector<std::string> current_path;
    current_path.push_back("start");
    literally_back_track(input, current_path, valid_paths_count, true);

    outf("{}", valid_paths_count);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
