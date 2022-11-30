#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "outf.h"

using Input = std::string;
std::unordered_map<char, std::string> digits_to_value{
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'A', "1010"},
    {'B', "1011"},
    {'C', "1100"},
    {'D', "1101"},
    {'E', "1110"},
    {'F', "1111"}};

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            for(auto e: line)
            input += digits_to_value[e];
        }
    }
    return input;
}

struct Packet
{
    int64_t version = 0;
    int64_t type = 0;
    int64_t value = 0;
    int64_t bit_length = 0;
    std::vector<std::unique_ptr<Packet>> subpackets;
};

Packet decode_packet(std::string input)
{
    int64_t bit_index = 0;
    Packet packet;
    auto& version = packet.version;
    auto& type = packet.type;
    version = std::stoll(input.substr(bit_index, 3), nullptr, 2);
    bit_index += 3;
    type = std::stoll(input.substr(bit_index, 3), nullptr, 2);
    bit_index += 3;
    if(type == 4)
    {
        std::string number_acc = "";
        while (input[bit_index] == '1')
        {
            number_acc += input.substr(bit_index + 1, 4);
            bit_index += 5;
        }
        number_acc += input.substr(bit_index + 1, 4);
        bit_index += 5;
        //while (bit_index % 4)
        //    bit_index++;

        packet.value = std::stoll(number_acc, nullptr, 2);
    }
    else
    {
        int64_t length_length = (input[bit_index] == '0') ? 15 : 11;
        bit_index++;
        int64_t length = std::stoll(input.substr(bit_index, length_length), nullptr, 2);
        bit_index += length_length;
        auto& subpackets = packet.subpackets;
        if (length_length == 11)
        {
            for (int i = 0; i < length; i++)
            {
                subpackets.push_back(std::make_unique<Packet>(decode_packet(input.substr(bit_index))));
                bit_index += subpackets.back()->bit_length;
            }
        }
        else
        {
            while(length > 0) {
                subpackets.push_back(std::make_unique<Packet>(decode_packet(input.substr(bit_index))));
                bit_index += subpackets.back()->bit_length;
                length -= subpackets.back()->bit_length;
            }
            
        }
    }
    packet.bit_length = bit_index;
    return packet;
}

void compute_hierarchy_version_sum(const Packet& packet, int64_t & sum )
{
    sum += packet.version;
    for (auto& e : packet.subpackets)
    {
        compute_hierarchy_version_sum(*e, sum);
    }
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    int64_t sum = 0;

    Packet packet = decode_packet(input);
    compute_hierarchy_version_sum(packet, sum);
    outf("{}", sum);

}

int64_t compute_hierarchy_ops(const Packet& packet)
{
    if (packet.type == 4)
        return packet.value;

    int64_t result = 0;
    switch(packet.type)
    {
    case 0: // sum  
    {
        for (auto& e : packet.subpackets)
            result += compute_hierarchy_ops(*e);
        break;
    }
    case 1: // prod
    {
        result = 1;
        for (auto& e : packet.subpackets)
            result *= compute_hierarchy_ops(*e);
        break;
    }
    case 2: // min
    {
        result = compute_hierarchy_ops(*packet.subpackets[0]);
        for (size_t i = 1; i < packet.subpackets.size(); i++)
        {
            auto partial_result = compute_hierarchy_ops(*packet.subpackets[i]);
            if (result > partial_result)
                result = partial_result;
        }
        break;
    }
    case 3: // max
    {
        result = compute_hierarchy_ops(*packet.subpackets[0]);
        for (size_t i = 1; i < packet.subpackets.size(); i++)
        {
            auto partial_result = compute_hierarchy_ops(*packet.subpackets[i]);
            if (result < partial_result)
                result = partial_result;
        }
        break;
    }
    case 5: // gt
    {
        assert(packet.subpackets.size() == 2);
        result = (compute_hierarchy_ops(*packet.subpackets[0]) > compute_hierarchy_ops(*packet.subpackets[1])) ? 1 : 0;
        break;
    }
    case 6: // lt
    {
        assert(packet.subpackets.size() == 2);
        result = (compute_hierarchy_ops(*packet.subpackets[0]) < compute_hierarchy_ops(*packet.subpackets[1])) ? 1 : 0;
        break;
    }
    case 7: // eq
    {
        assert(packet.subpackets.size() == 2);
        result = (compute_hierarchy_ops(*packet.subpackets[0]) == compute_hierarchy_ops(*packet.subpackets[1])) ? 1 : 0;
        break;
    }
    }
    return result;
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);

    Packet packet = decode_packet(input);
    auto result = compute_hierarchy_ops(packet);
    outf("{}", result);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);
   
    return 0;
}
