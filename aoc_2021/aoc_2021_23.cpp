#include <cassert>
#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include "outf.h"

using Slot = std::array<char, 2>;
using Input = std::array<Slot, 6>;


Input read_input()
{
    Input input;
    //while (std::cin)
    //{
    //    std::string line;
    //    std::getline(std::cin, line);
    //    if (line.size())
    //    {
    //        input.push_back(line);
    //    }
    //}
    
    input[0] = { '.', '.' };
    input[1] = { 'C', 'C' };
    input[2] = { 'B', 'D' };
    input[3] = { 'A', 'A' };
    input[4] = { 'D', 'B' };
    input[5] = { '.', '.' };
    return input;
}



void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    using Slot = std::pair<char, char>;
    
    using Move_Coord = std::pair<std::pair<int8_t, int8_t>, std::pair<int8_t, int8_t>>;
    
    int32_t a = 1;
    int32_t b = 10;
    int32_t c = 100;
    int32_t d = 1000;

    int32_t score = 0;
    //score += 7 * a; // a* cc bd *a db **
    //score += 7 * a; // aa cc bd ** db **
    //score += 7 * c; // aa *c bd *c db **
    //score += 7 * c; // aa ** bd cc db **
    //score += 5 * b; // aa *b *d cc db **
    //score += 5 * d; // aa *b *d cc *b *d
    //score += 9 * b; // aa bb *d cc ** *d
    //score += 4 * d; // aa bb *d cc *d **
    //score += 7 * d;
    //score += 5 * b;
    //score += 5 * b;
    //score += 4 * a;
    //score += 4 * a;

    //score += 7 * a; // a* cc bd *a db **
    //score += 7 * a; // aa cc bd ** db **
    //score += 4 * b; // aa cc *d *b db **
    //score += 4 * d; // aa cc *d *b ** *d
    //score += 5 * b; // aa cc *d bb ** *d
    //score += 8 * d; // aa cc ** bb *d *d
    //score += 3 * d; // aa cc ** bb dd **
    //score += 5 * b; // aa cc *b *b dd **
    //score += 5 * b; // aa cc bb ** dd **
    //score += 7 * c; // aa *c bb *c dd **
    //score += 7 * c; // aa ** bb cc dd **
    //score += 4 * a; // a* *a bb cc dd **
    //score += 4 * a; // ** aa bb cc dd **
    //outf("{}", score);

    std::vector<int32_t> energy_to_move;
    //energy_to_move.push_back(d * distances[{ {4, 1}, { 5,0 } }]);   // ** cc bd aa *b *d
    //energy_to_move.push_back(b * distances[{ {2, 1}, { 1,0 } }]);   // b* cc *d aa *b *d
    //energy_to_move.push_back(b * distances[{ {4, 0}, { 0,1 } }]);   // bb cc *d aa ** *d
    //energy_to_move.push_back(d * distances[{ {2, 0}, { 4,0 } }]);   // bb cc ** aa *d *d
    //energy_to_move.push_back(d * distances[{ {5, 0}, { 4,1 } }]);   // bb cc ** aa dd **
    //energy_to_move.push_back(b * distances[{ {0, 1}, { 2,0 } }]);   // b* cc *b aa dd **
    //energy_to_move.push_back(b * distances[{ {0, 0}, { 2,1 } }]);   // ** cc bb aa dd **
    //energy_to_move.push_back(a * distances[{ {3, 1}, { 0,0 } }]);   // a* cc bb *a db **
    //energy_to_move.push_back(a * distances[{ {3, 0}, { 0,1 } }]);   // aa cc bb ** dd **
    //energy_to_move.push_back(c * distances[{ {1, 1}, { 3,0 } }]);   // aa *c bb *c dd **
    //energy_to_move.push_back(c * distances[{ {1, 0}, { 3,1 } }]);   // aa ** bb cc dd **
    //energy_to_move.push_back(a * distances[{ {0, 1}, { 1,0 } }]);   // a* *a bb cc dd **
    //energy_to_move.push_back(a * distances[{ {0, 0}, { 1,1 } }]);   // ** aa bb cc dd **
    //
    //

    energy_to_move.push_back(d * 2); // left
    energy_to_move.push_back(b * 3); // right
    energy_to_move.push_back(d * 3); // into room
    energy_to_move.push_back(b * 2); // left
    energy_to_move.push_back(d * 7); // into room
    energy_to_move.push_back(b * 3); // into room
    energy_to_move.push_back(b * 6); // into room
    energy_to_move.push_back(a * 2); // right
    energy_to_move.push_back(a * 7); // second leftomos hallway spot
    energy_to_move.push_back(c * 7); // into room
    energy_to_move.push_back(c * 7); // into room
    energy_to_move.push_back(a * 3); // into room
    energy_to_move.push_back(a * 6); // into room

    for (auto& e : energy_to_move)
    {
        score += e;
    }
    outf("{}", score);
    
}

struct Node
{
    size_t inner_index;
    size_t outer_index;
    char content;
    char type; // .ABCD
};

using Nodes = std::vector<Node>;
using Distances = std::array<std::array<int32_t, 23>, 23>;
using Path_Bottlenecks = std::array<std::array<std::vector<size_t>, 23>, 23>;

Path_Bottlenecks build_path_bottlenecks_data(const Nodes& nodes)
{
    Path_Bottlenecks bottlenecks;
    // process hallway nodes
    for (size_t i = 0; i < 23; i++)
    {
        for (size_t j = i + 1; j < 23; j++)
        {
            if (i < 7 && j < 7)
            {
                for (size_t k = i + 1; k < j; k++)
                {
                    bottlenecks[i][j].push_back(k);
                    bottlenecks[j][i].push_back(k);
                }
            }
        }
    }
    // process room nodes and use previously processed paths
    for (size_t i = 0; i < 22; i++)
    {
        for (size_t j = i + 1; j < 23; j++)
        {
            if (i >= 7)
            {
                for (size_t k = 0; k < nodes[i].inner_index; k++)
                {
                    bottlenecks[i][j].push_back(7 + 4 * (nodes[i].type - 'A') + k);
                    bottlenecks[j][i].push_back(7 + 4 * (nodes[i].type - 'A') + k);
                }
            }

            if (j >= 7)
            {
                for (size_t k = 0; k < nodes[j].inner_index; k++)
                {
                    bottlenecks[i][j].push_back(7 + 4 * (nodes[j].type - 'A') + k);
                    bottlenecks[j][i].push_back(7 + 4 * (nodes[j].type - 'A') + k);
                }
            }

            if (i < 7 && j >= 7)
            {
                size_t first_hallway_before_room = nodes[j].type - 'A' + 1;
                first_hallway_before_room += i > first_hallway_before_room ? 1 : 0;
                if (i != first_hallway_before_room)
                {
                    bottlenecks[i][j].push_back(first_hallway_before_room);
                    bottlenecks[j][i].push_back(first_hallway_before_room);
                }
                bottlenecks[i][j].insert(bottlenecks[i][j].end(), bottlenecks[i][first_hallway_before_room].begin(), bottlenecks[i][first_hallway_before_room].end());
                bottlenecks[j][i].insert(bottlenecks[j][i].end(), bottlenecks[i][first_hallway_before_room].begin(), bottlenecks[i][first_hallway_before_room].end());
            }
            else if (i >= 7 && j < 7)
            {
                size_t first_hallway_before_room = nodes[i].type - 'A' + 1;
                first_hallway_before_room += j > first_hallway_before_room ? 1 : 0;
                if (j != first_hallway_before_room)
                {
                    bottlenecks[i][j].push_back(first_hallway_before_room);
                    bottlenecks[j][i].push_back(first_hallway_before_room);
                }
                bottlenecks[i][j].insert(bottlenecks[i][j].end(), bottlenecks[j][first_hallway_before_room].begin(), bottlenecks[j][first_hallway_before_room].end());
                bottlenecks[j][i].insert(bottlenecks[j][i].end(), bottlenecks[j][first_hallway_before_room].begin(), bottlenecks[j][first_hallway_before_room].end());
            }
            else if (i >= 7 && j >= 7)
            {
                if (nodes[i].type != nodes[j].type)
                {
                    size_t first_hallway_after_room_i = nodes[i].type - 'A' + 1 + 1;
                    size_t first_hallway_before_room_j = nodes[j].type - 'A' + 1; 
                    for (size_t k = first_hallway_after_room_i; k <= first_hallway_before_room_j; k++)
                    {
                        bottlenecks[i][j].push_back(k);
                        bottlenecks[j][i].push_back(k);
                    }
                }
            }

        }
    }

    return bottlenecks;
}

Distances process_node_distances(const Nodes& nodes)
{
    // H0 -1- H1 -2- H2 -2- H3 -2- H4 -2- H5 -1- H6
    //         \     /\     /\     /\     /
    //          2   2  2   2  2   2  2   2
    //           \ /    \ /    \ /    \ /
    //            A0     B0     C0     D0
    //            |      |      |      |
    //            1      1      1      1
    //            |      |      |      |
    //            A1     B1     C1     D1
    //            |      |      |      |
    //            1      1      1      1
    //            |      |      |      |
    //            A2     B2     C2     D2
    //            |      |      |      |
    //            1      1      1      1
    //            |      |      |      |
    //            A3     B3     C3     D3
    // all in all, 23 nodes; init the distnaces between the nodes

    Distances distances = {};

    distances[0][1] = distances[1][0] = 1;
    distances[5][6] = distances[6][5] = 1;
    // hallway nodes
    for (size_t i = 1; i < 5; i++)
    {
        distances[i][i + 1] = distances[i + 1][i] = 2;
        
        distances[7 + (i - 1) * 4][i] = distances[i][7 + (i - 1) * 4] = 2;
        distances[7 + (i - 1) * 4][i+1] = distances[i+1][7 + (i - 1) * 4] = 2;
    }
    //room nodes
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            distances[7 + i * 4 + j][7 + i * 4 + j + 1] = distances[7 + i * 4 + j + 1][7 + i * 4 + j] = 1;
        }
    }

    for(size_t i = 0; i < 23; i++)
        for(size_t j = 0; j < i; j++)
            for (size_t k = 0; k < 23; k++)
            {
                if (distances[i][k] != 0 && distances[k][j] != 0 && (distances[i][j] == 0 || distances[i][j] > distances[i][k] + distances[k][j]))
                {
                    distances[i][j] = distances[j][i] = distances[i][k] + distances[k][j];
                }
             
            }

    return distances;
}

bool is_done(const Nodes& nodes)
{
    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            size_t index = 7 + i * 4 + j;
            if (nodes[index].content != char('A' + i))
                return false;
        }
    }
    return true;
}

bool room_has_only_homies_or_is_empty(const Nodes& nodes, char room_type)
{
    for (size_t i = 0; i < 4; i++)
    {
        const auto& node = nodes[7 + (room_type - 'A') * 4 + i];
        if (node.content != room_type && node.content != '.')
            return false;
    }
    return true;
}

bool is_path_clear(const Nodes& nodes, const Path_Bottlenecks& bottlenecks, size_t begin, size_t end)
{
    for (size_t e : bottlenecks[begin][end])
        if (nodes[e].content != '.')
            return false;
    return true;
}

inline void print_move(const Distances& dists, size_t i, size_t j, char amphipod, int32_t level)
{
    static const int32_t energy[4] = { 1, 10, 100, 1000 };
    
    std::string filler(level, ' ');
    std::string format_str_i = (i < 7 ? std::string("H{}") : char((i - 7) / 4 + 'A') + std::string("{}"));
    std::string format_str_j = (j < 7 ? std::string("H{}") : char((j - 7) / 4 + 'A') + std::string("{}"));
    outf(("{}Move {} from " + format_str_i + " to " + format_str_j + " cost = {}. \n").c_str(), filler, amphipod, (i < 7 ? i : (i - 7) % 4), (j < 7 ? j : (j - 7) % 4), dists[i][j] * energy[amphipod - 'A']);
}

std::vector<std::tuple<size_t, size_t, char>> moves;
void back_track(const Distances& distances, Nodes& nodes, const Path_Bottlenecks& bottlenecks, int32_t& min_energy, int32_t current_energy, int32_t level = 0)
{
    static const int32_t room_energy[4] = { 1, 10, 100, 1000 };
    if (current_energy >= min_energy)
    {
        return;
    }

    if (is_done(nodes))
    {
        if (min_energy > current_energy)
        {
            min_energy = current_energy;
            outf("\nMoves list to solve {}:\n", min_energy);
            for (auto [i, j, c] : moves)
            {
                print_move(distances, i, j, c, 0);
            }
        }
        return;
    }

    // go through all nodes
    for (size_t i = 0; i < nodes.size(); i++)
    {
        auto& node = nodes[i];
        if (node.content != '.')
        {
            bool room_of_current_amphipod_has_only_homies_or_is_empty = room_has_only_homies_or_is_empty(nodes, node.content);
            if (node.type == node.content && room_of_current_amphipod_has_only_homies_or_is_empty) // this amphipod is already home
            {
                // auto first = 7 + (node.type - 'A') * 4;
                // outf("\nNode content {} already in {} {} {} {}\n", node.content, nodes[first + 0].content, nodes[first + 1].content, nodes[first + 2].content, nodes[first + 3].content);
                continue;
            }

            if (room_of_current_amphipod_has_only_homies_or_is_empty)
            {
                for (size_t j = 7 + (node.content - 'A') * 4 + 3; j >= 7 + (node.content - 'A') * 4; j--)
                {
                    if (nodes[j].content == '.')
                    {
                        if (is_path_clear(nodes, bottlenecks, i, j)) // path is clear, go ahead
                        {
                            //print_move(i, j, node.content, level);
                            auto new_energy = current_energy + distances[i][j] * room_energy[node.content - 'A'];
                            moves.push_back({ i,j, node.content});
                            std::swap(node.content, nodes[j].content);
                            back_track(distances, nodes, bottlenecks, min_energy, new_energy, level + 1);
                            moves.pop_back();
                            std::swap(node.content, nodes[j].content);
                        }
                        break;
                    }
                }
            }
            if (node.type != '.') // we are in a room and we already checked if we can go into another room, so try hallways
            {
                for (size_t j = 0; j < 7; j++) // destination is a hallway node
                {
                    if (nodes[j].content == '.' // the destination has to be empty
                        && is_path_clear(nodes, bottlenecks, i, j) // path is clear, go ahead
                        )
                    {
                        
                        //print_move(i, j, node.content, level);
                        auto new_energy = current_energy + distances[i][j] * room_energy[node.content - 'A'];
                        moves.push_back({ i,j, node.content });
                        std::swap(node.content, nodes[j].content);
                        back_track(distances, nodes, bottlenecks, min_energy, new_energy, level + 1);
                        moves.pop_back();
                        std::swap(node.content, nodes[j].content);
                    }
                }
            }
        }
    }
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);
    
    Nodes nodes;
    // hallway nodes
    for (size_t i = 0; i < 7; i++)
    {
        nodes.push_back({ i, i, '.', '.'});
    }
    // room nodes
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            nodes.push_back({ j, 7 + i * 4 + j, '.', char('A' + i)});
        }
    }
    
    Distances distances = process_node_distances(nodes);
    Path_Bottlenecks bottlenecks = build_path_bottlenecks_data(nodes);

    //for (size_t i = 0; i < 23; i++)
    //{
    //    for (size_t j = i + 1; j < 23; j++)
    //    {
    //        outf("[{},{}] - ", i, j);
    //        for (auto k : bottlenecks[i][j])
    //        {
    //            outf("{} ", k);
    //        }
    //        outf("\n");
    //    }
    //}

    int32_t min_energy = 2'000'000'000;
    // my input
    // column A
    nodes[7].content = 'C'; 
    nodes[8].content = 'D';
    nodes[9].content = 'D';
    nodes[10].content = 'C';
    // column B
    nodes[11].content = 'B';
    nodes[12].content = 'C';
    nodes[13].content = 'B';
    nodes[14].content = 'D';
    // column C
    nodes[15].content = 'A';
    nodes[16].content = 'B';
    nodes[17].content = 'A';
    nodes[18].content = 'A';
    // column D
    nodes[19].content = 'D';
    nodes[20].content = 'A';
    nodes[21].content = 'C';
    nodes[22].content = 'B';

    // example
    // column A
    //nodes[7].content = 'B';
    //nodes[8].content = 'D';
    //nodes[9].content = 'D';
    //nodes[10].content = 'A';
    //// column B
    //nodes[11].content = 'C';
    //nodes[12].content = 'C';
    //nodes[13].content = 'B';
    //nodes[14].content = 'D';
    //// column C
    //nodes[15].content = 'B';
    //nodes[16].content = 'B';
    //nodes[17].content = 'A';
    //nodes[18].content = 'C';
    //// column D
    //nodes[19].content = 'D';
    //nodes[20].content = 'A';
    //nodes[21].content = 'C';
    //nodes[22].content = 'A';


    moves.reserve(1000);
    back_track(distances, nodes, bottlenecks, min_energy, 0);

    outf("{}", min_energy);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
