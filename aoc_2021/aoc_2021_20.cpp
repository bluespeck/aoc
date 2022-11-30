#include <iostream>
#include <string>
#include <vector>
#include "outf.h"

struct Image
{
    std::string algorithm;
    std::vector<std::string> image;
};
using Input = Image;

Input read_input()
{
    Input input;
    std::getline(std::cin, input.algorithm);
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            input.image.push_back(line);
        }
    }
    return input;
}

uint32_t count_lit(const std::vector<std::string> &image, int32_t border_size)
{
    int32_t count = 0;
    for (int32_t j = border_size; j < image.size() - border_size; j++)
    {
        for (int32_t i = border_size; i < image[j].size() - border_size; i++)
        {
            if (image[j][i] == '#')
                count++;
        }
    }
    return count;
}

void print_full_image(const std::vector<std::string>& image)
{
    outf("Image:\n");
    for (int32_t j = 0; j < image.size(); j++)
    {
        outf("{}\n", image[j]);
    }
    outf("\n");
}


void solve_1_2(const Input& input)
{
    const int32_t num_steps = 50;

    const int32_t di[] = { -1, 0 ,1, -1, 0, 1, -1, 0, 1 };
    const int32_t dj[] = { -1, -1, -1, 0, 0 ,0, 1, 1, 1 };

    auto image = input.image;
    pad_vec_str(image, '.', 2 * num_steps + 1);
    

    for (int step = 0; step < num_steps; step++)
    {
        auto output = image;
        for (int32_t j = 1; j < image.size() - 1; j++)
        {
            for (int32_t i = 1; i < image[j].size() - 1; i++)
            {
                std::string number;
                for (int k = 0; k < 9; k++)
                {
                    auto ii = i + di[k];
                    auto jj = j + dj[k];
                    number += (image[jj][ii] == '.' ? '0' : '1');
                }
                uint32_t index = std::stoul(number, nullptr, 2);
                output[j][i] = input.algorithm[index];
            }
        }
        image = output;
        if (step == 1)
        {
            outf("Part 1: {}\n", count_lit(image, num_steps));
        }

    }

    //outf("Final image:\n");
    //print_full_image(image);

    outf("Part 2: {}\n", count_lit(image, num_steps));
}

int main()
{
    auto input = read_input();
    solve_1_2(input);

    return 0;
}
