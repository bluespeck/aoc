#include <iostream>
#include <string>
#include "md5.h" // https://github.com/CommanderBubble/MD5/blob/master/src/md5.h

using input_t = std::string;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if(line.size())
            input = line;
    };

    return input;
}

void
solve_1(const input_t& input)
{
    for (uint64_t i = 1; ; i++)
    {
        auto str = input + std::to_string(i);
        md5::md5_t md5;
        md5.process(str.c_str(), str.size());
        md5.finish();
        char md5_str[MD5_STRING_SIZE];
        md5.get_string(md5_str);
        
        if (md5_str[0] != '0')
            continue;
        if (md5_str[1] != '0')
            continue;  
        if (md5_str[2] != '0')
            continue;
        if (md5_str[3] != '0')
            continue;
        if (md5_str[4] != '0')
            continue;
        
        for (auto c : md5_str)
        {
            std::cout << c;
        }
        std::cout << std::endl;
        std::cout << i << std::endl;
        break;
    }
}

void
solve_2(const input_t& input)
{
    for (uint64_t i = 1; ; i++)
    {
        auto str = input + std::to_string(i);
        md5::md5_t md5;
        md5.process(str.c_str(), str.size());
        md5.finish();
        char md5_str[MD5_STRING_SIZE];
        md5.get_string(md5_str);

        if (md5_str[0] != '0')
            continue;
        if (md5_str[1] != '0')
            continue;
        if (md5_str[2] != '0')
            continue;
        if (md5_str[3] != '0')
            continue;
        if (md5_str[4] != '0')
            continue;
        if (md5_str[5] != '0')
            continue;

        for (auto c : md5_str)
        {
            std::cout << c;
        }
        std::cout << std::endl;
        std::cout << i << std::endl;
        break;
    }
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}