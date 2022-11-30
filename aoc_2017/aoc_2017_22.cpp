#include "../outf.h"

#include <cassert>
#include <iostream>
#include <format>
#include <string>
#include <vector>

using cluster_t = std::vector<std::string>;

struct virus_carrier_t
{
    void turn_left() { direction = (direction == 0)? 3 : direction - 1; }
    void turn_right() { direction = (direction == 3) ? 0 : direction + 1; }
    void update_position_after_padding(size_t padding_thickness) { i += padding_thickness; j += padding_thickness; }
    void move_forward();

    unsigned char direction = 0; // 0u,1r,2d,3l
    int32_t i, j;
    size_t nodes_infected = 0;
};

using input_t = cluster_t;

input_t read()
{
    input_t input;
    std::string line;
    while (std::getline(std::cin, line))
    {
        input.push_back(line);
    }
    assert(input.size() % 2 == 1);
    assert(input[input.size() / 2].size() % 2 == 1);
    return input;
}

void virus_carrier_t::move_forward()
{
    static int32_t di[4] = { -1, 0, 1,  0 };
    static int32_t dj[4] = {  0, 1, 0, -1 };
    i += di[direction];
    j += dj[direction];
}

void execute_burst(cluster_t& cluster, virus_carrier_t& vbot)
{
    if (cluster[vbot.i][vbot.j] == '#')
    {
        vbot.turn_right();
        cluster[vbot.i][vbot.j] = '.';
    }
    else
    {
        vbot.turn_left();
        cluster[vbot.i][vbot.j] = '#';
        vbot.nodes_infected++;
    }

    vbot.move_forward();

    if (vbot.i <= 0 || vbot.i >= cluster.size() - 1 
        || vbot.j <= 0 || vbot.j >= cluster[0].size() - 1 )
    {
        add_padding(cluster, '.', 1);
        vbot.update_position_after_padding(1);
    }
}

void execute_burst_2(cluster_t& cluster, virus_carrier_t& vbot)
{
    auto& node = cluster[vbot.i][vbot.j];
    switch (node)
    {
    case '.':
        vbot.turn_left();
        node = 'W';
        break;
    case '#':
        vbot.turn_right();
        node = 'F';
        break;
    case 'F':
        vbot.turn_left(); vbot.turn_left();
        node = '.';
        break;
    case 'W':
        node = '#';
        vbot.nodes_infected++;
        break;
    }

    vbot.move_forward();

    if (vbot.i <= 0 || vbot.i >= cluster.size() - 1
        || vbot.j <= 0 || vbot.j >= cluster[0].size() - 1)
    {
        add_padding(cluster, '.', 1);
        vbot.update_position_after_padding(1);
    }
}

void print_cluster_and_bot(const cluster_t& cluster, const virus_carrier_t& vbot)
{
    for (size_t i = 0; i < cluster.size(); i++)
    {
        if(i == vbot.i)
        {
            for (int j = 0; j < cluster[i].size(); j++)
            {
                if (j == vbot.j || j == vbot.j + 1)
                {

                    outf("{}{}", (vbot.direction == 0 ? '^' : vbot.direction == 1 ? '>' : vbot.direction == 2 ? 'v' : '<'), cluster[i][j]);
                }
                else
                {
                    outf(" {}", cluster[i][j]);
                }
            }
        }
        else
        {
            for (auto c : cluster[i])
            {
                outf(" {}", c);
            }
        }
        outf("\n");
    }

    outf("\n");
}

void solve_1(const input_t& input)
{
    auto cluster_map = input;

    virus_carrier_t vbot;
    vbot.i = input.size() / 2;
    vbot.j = input[input.size() / 2].size() / 2;
    vbot.direction = 0; // up

    constexpr size_t num_bursts = 10'000;
    
    //print_cluster_and_bot(cluster_map, vbot);

    for (int i = 0; i < num_bursts; i++)
    {
        execute_burst(cluster_map, vbot);
    }
    //outf("Burst #{}\n", num_bursts);
    //print_cluster_and_bot(cluster_map, vbot);
    outf("Nodes infected {}\n", vbot.nodes_infected);
}

void solve_2(const input_t& input)
{
    auto cluster_map = input;

    virus_carrier_t vbot;
    vbot.i = input.size() / 2;
    vbot.j = input[input.size() / 2].size() / 2;
    vbot.direction = 0; // up

    constexpr size_t num_bursts = 10'000'000;

    //print_cluster_and_bot(cluster_map, vbot);

    for (int i = 0; i < num_bursts; i++)
    {
        execute_burst_2(cluster_map, vbot);
        //outf("Burst #{}\n", i+1);
        //print_cluster_and_bot(cluster_map, vbot);
    }
    outf("Nodes infected {}\n", vbot.nodes_infected);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}