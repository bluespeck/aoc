#include "../outf.h"

#include <array>
#include <cassert>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct valve_t
{
    int64_t flow_rate;
    std::vector<size_t> linked_valves;
};

struct input_t
{
    std::vector<valve_t> valves;
    std::unordered_map<std::string, size_t> valve_name_to_index;
    std::unordered_map<size_t, std::string> index_to_valve_name;
};

input_t read()
{
    input_t input;
    std::vector<std::vector<std::string>> links;
    std::string line;
    while (std::getline(std::cin, line))
    {
        valve_t valve;
        auto half_split_parts = split_string(line, "; ");
        {
            auto parts = split_string(half_split_parts[0], " ");
            valve.flow_rate = std::stoll(parts[4].substr(std::string("rate=").size()));
            input.valve_name_to_index[parts[1]] = input.valves.size();
            input.index_to_valve_name[input.valves.size()] = parts[1];
        }
        {
            size_t pos_to_check = std::string("tunnels lead to valve").size();
            auto parts = half_split_parts[1][pos_to_check] == 's' ? split_string(half_split_parts[1].substr(std::string("tunnels lead to valves ").size()), ", ")
                : std::vector<std::string>{half_split_parts[1].substr(pos_to_check+1)};
            links.push_back({});
            for (auto valve_name : parts)
            {
                links.back().push_back(valve_name);
            }
        }
        input.valves.push_back(valve);
    }

    for (size_t i = 0; i < input.valves.size(); i++)
    {
        for(auto& e: links[i])
        { 
            input.valves[i].linked_valves.push_back(input.valve_name_to_index[e]);
        }
    }

    return input;
}

using path_t = std::vector<std::pair<size_t, bool>>;

size_t compute_flow(const input_t& input, const path_t& path)
{
    size_t total_flow = 0;
    size_t pressure_per_minute = 0;
    size_t current_time = 1; // the move from AA
    size_t max_time = 30;
    for (auto [index, open] : path)
    {
        if (current_time >= 30)
            break;
        outf("== Minute {:2} ==\n", current_time);
        outf("Move to {}. ", input.index_to_valve_name.at(index));
        outf("Pressure per minute {}. ", pressure_per_minute);
        outf("Total pressue until minute thirty is {}.\n\n", total_flow);
        current_time++;
        if (open)
        {
            outf("== Minute {:2} ==\n", current_time);
            current_time ++;
            pressure_per_minute += input.valves[index].flow_rate;
            total_flow += input.valves[index].flow_rate * (max_time - current_time + 1);
            outf("Opening {}. ", input.index_to_valve_name.at(index));
            outf("Pressure per minute {}. ", pressure_per_minute);
            outf("Total pressue until minute thirty is {}.\n\n", total_flow);
        }
        
    }
    return total_flow;
}

constexpr int time_between_valves = 1;
constexpr int time_to_open = 1;

void backtrack_1(const input_t& input, const std::vector<std::vector<int>>& dists, std::vector<size_t>& current_path, int64_t flow, int64_t time_left, int64_t& max_flow)
{
    if (time_left < 0)
    {
        //outf("Found pressure = {}\n", flow);
        if (max_flow < flow)
            max_flow = flow;
        return;
    }
    bool found = false;
    for (size_t i = 0; i < dists[current_path.back()].size(); i++)
    {
        int64_t new_time_left = time_left - dists[current_path.back()][i] * time_between_valves - time_to_open;
        int64_t added_benefit = input.valves[i].flow_rate * new_time_left;
        if (added_benefit > 0
            && std::find(current_path.begin(), current_path.end(), i) == current_path.end())
        {
            found = true;
            current_path.push_back(i);
            backtrack_1(input, dists, current_path, flow + added_benefit, new_time_left, max_flow);
            current_path.pop_back();
        }
    }
    if (!found)
    {
        //outf("Found pressure = {}\n", flow);
        if (max_flow < flow)
            max_flow = flow;
    }
}

void solve_1(const input_t& input)
{
    std::vector<std::vector<int>> dist_matrix;

    auto init_matrix = [size = input.valves.size()](std::vector<std::vector<int>>& matrix)
    {
        matrix.resize(size);
        for (auto& e : matrix)
            e.resize(size, 999'999);
    };
    
    init_matrix(dist_matrix);
    //  + input.valves[index].flow_rate * (time_limit - time_distance - time_to_open)
    for (size_t i = 0; i < input.valves.size(); i++)
    {
        for (size_t index : input.valves[i].linked_valves)
        {   
            dist_matrix[i][index] = 1;
            dist_matrix[index][i] = 1;
            //outf("{} to {}({}) --- \n", input.index_to_valve_name.at(i),
            //    input.index_to_valve_name.at(index),
            //    dist_matrix[i][index]);
        }
        dist_matrix[i][i] = 9999;
    }

    for (size_t k = 0; k < dist_matrix.size(); k++)
    {
        for (size_t i = 0; i < dist_matrix.size(); i++)
        {
            for (size_t j = 0; j < dist_matrix.size(); j++)
            {
                if (i != j && k != i && k != j 
                    && dist_matrix[i][j] > dist_matrix[i][k] + dist_matrix[k][j])
                {
                    //outf("{} to {}({})  ->  {} to {}({}) to {}({})\n", 
                    //    input.index_to_valve_name.at(i),
                    //    input.index_to_valve_name.at(j),
                    //    dist_matrix[i][j],
                    //    input.index_to_valve_name.at(i),
                    //    input.index_to_valve_name.at(k),
                    //    dist_matrix[i][k],
                    //    input.index_to_valve_name.at(j),
                    //    dist_matrix[k][j]);
                    dist_matrix[i][j] = dist_matrix[i][k] + dist_matrix[k][j];
                }

            }
        }
    }

    //size_t current_valve = input.valve_name_to_index.at("AA");
    //int time_left = time_limit;
    //size_t total_flow = 0;
    //std::unordered_set<size_t> opened_valves;
    //while (true)
    //{
    //    size_t best_choice = 9999;
    //    int64_t max_benefit = 0;
    //    for (size_t i = 0; i < dist_matrix[current_valve].size(); i++)
    //    {
    //        if (opened_valves.contains(i))
    //        {
    //            continue;
    //        }
    //
    //        int64_t benefit = input.valves[i].flow_rate * (time_left - dist_matrix[current_valve][i] * time_between_valves - time_to_open);
    //        if (max_benefit < benefit)
    //        {
    //            max_benefit = benefit;
    //            best_choice = i;
    //        }
    //    }
    //    
    //    if (best_choice == 9999)
    //        break;
    //
    //    total_flow += max_benefit;
    //    outf("Chose to open {}\n", input.index_to_valve_name.at(best_choice));
    //    time_left -= dist_matrix[current_valve][best_choice] + time_to_open;
    //    opened_valves.insert(best_choice);
    //    current_valve = best_choice;
    //    if (time_left < 0)
    //        break;
    //}

    //outf("Total pressure flow is {}.\n", total_flow);

    constexpr int time_limit_1 = 30;
    int64_t max_flow = 0;
    std::vector<size_t> path = { input.valve_name_to_index.at("AA") };
    backtrack_1(input, dist_matrix, path, 0, time_limit_1, max_flow);
    outf("Total pressure flow is {}.\n", max_flow);
}


input_t _input;
std::vector<std::vector<int>> _dists;
std::vector<size_t> _current_path_1;
std::vector<size_t> _current_path_2;
int64_t _max_flow;
void backtrack_2(int64_t flow_1, int64_t flow_2, int64_t time_left_1, int64_t time_left_2)
{
    if (time_left_1 < 0 && time_left_2 < 0)
    {
        //outf("Found pressure = {}\n", flow);
        if (_max_flow < flow_1 + flow_2)
        {
            _max_flow = flow_1 + flow_2;
            outf("-- Total pressure flow is {}.\n", _max_flow);
        }
        return;
    }

    
    bool found = false;
    for (size_t i = 0; i < _dists[_current_path_1.back()].size(); i++)
    {
        int64_t new_time_left = time_left_1 - _dists[_current_path_1.back()][i] * time_between_valves - time_to_open;
        int64_t added_benefit = _input.valves[i].flow_rate * new_time_left;
        if (added_benefit > 0
            && std::find(_current_path_1.begin(), _current_path_1.end(), i) == _current_path_1.end()
            && std::find(_current_path_2.begin(), _current_path_2.end(), i) == _current_path_2.end()
            )
        {
            found = true;
            _current_path_1.push_back(i);
            backtrack_2(flow_1 + added_benefit, flow_2, new_time_left, time_left_2);
            _current_path_1.pop_back();
        }
    }
    for (size_t i = 0; i < _dists[_current_path_2.back()].size(); i++)
    {
        int64_t new_time_left = time_left_2 - _dists[_current_path_2.back()][i] * time_between_valves - time_to_open;
        int64_t added_benefit = _input.valves[i].flow_rate * new_time_left;
        if (added_benefit > 0
            && std::find(_current_path_1.begin(), _current_path_1.end(), i) == _current_path_1.end()
            && std::find(_current_path_2.begin(), _current_path_2.end(), i) == _current_path_2.end())
        {
            found = true;
            _current_path_2.push_back(i);
            backtrack_2(flow_1, flow_2 + added_benefit, time_left_1, new_time_left);
            _current_path_2.pop_back();
        }
    }
    
    if (!found)
    {
        if (_max_flow < flow_1 + flow_2)
        {
            _max_flow = flow_1 + flow_2;
            outf("-- Total pressure flow is {}.\n", _max_flow);
        }
    }
    

}

void solve_2(const input_t& input)
{
    std::vector<std::vector<int>> dist_matrix;

    auto init_matrix = [size = input.valves.size()](std::vector<std::vector<int>>& matrix)
    {
        matrix.resize(size);
        for (auto& e : matrix)
            e.resize(size, 999'999);
    };

    init_matrix(dist_matrix);
    for (size_t i = 0; i < input.valves.size(); i++)
    {
        for (size_t index : input.valves[i].linked_valves)
        {
            dist_matrix[i][index] = 1;
            dist_matrix[index][i] = 1;
            //outf("{} to {}({}) --- \n", input.index_to_valve_name.at(i),
            //    input.index_to_valve_name.at(index),
            //    dist_matrix[i][index]);
        }
        dist_matrix[i][i] = 9999;
    }

    for (size_t k = 0; k < dist_matrix.size(); k++)
    {
        for (size_t i = 0; i < dist_matrix.size(); i++)
        {
            for (size_t j = 0; j < dist_matrix.size(); j++)
            {
                if (i != j && k != i && k != j
                    && dist_matrix[i][j] > dist_matrix[i][k] + dist_matrix[k][j])
                {
                    dist_matrix[i][j] = dist_matrix[i][k] + dist_matrix[k][j];
                }

            }
        }
    }

    constexpr int time_limit_2 = 26;
    _max_flow = 0;
    _current_path_1 = { input.valve_name_to_index.at("AA") };
    _current_path_2 = { input.valve_name_to_index.at("AA") };
    _dists = dist_matrix;
    _input = input;
    backtrack_2(0, 0, time_limit_2, time_limit_2);
    outf("Total pressure flow is {}.\n", _max_flow);
}

int main()
{
    input_t input = read();
    //compute_flow(input, {
    //    { input.valve_name_to_index.at("DD"), true},
    //    { input.valve_name_to_index.at("CC"), false},
    //    { input.valve_name_to_index.at("BB"), true},
    //    { input.valve_name_to_index.at("AA"), false},
    //    { input.valve_name_to_index.at("II"), false},
    //    { input.valve_name_to_index.at("JJ"), true},
    //    { input.valve_name_to_index.at("II"), false},
    //    { input.valve_name_to_index.at("AA"), false},
    //    { input.valve_name_to_index.at("DD"), false},
    //    { input.valve_name_to_index.at("EE"), false},
    //    { input.valve_name_to_index.at("FF"), false},
    //    { input.valve_name_to_index.at("GG"), false},
    //    { input.valve_name_to_index.at("HH"), true},
    //    { input.valve_name_to_index.at("GG"), false},
    //    { input.valve_name_to_index.at("FF"), false},
    //    { input.valve_name_to_index.at("EE"), true},
    //    { input.valve_name_to_index.at("DD"), false},
    //    { input.valve_name_to_index.at("CC"), true}
    //    });

    solve_1(input);
    solve_2(input);

    return 0;
}