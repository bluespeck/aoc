import std.core;

using input_t = std::vector<int>;

input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {
            int total = 0;
            for (char c: line)
            {
                if (c == 'B' || c == 'R')
                {
                    total += 1;
                }
                total <<= 1;
            }
            total >>= 1;    // 10 minutes late because of this off by 1 :(
            input.push_back(total);
        }
    }

    return input;
}

void
solve_1(input_t& input)
{
    std::cout << input.back() << std::endl;
}

void
solve_2(input_t& input)
{
    for(int i = 0; i < input.size() - 1; i++)
    {
        if (input[i + 1] - input[i] == 2)
        {
            std::cout << input[i] << std::endl;
            break;
        }
    }
}   

int
main()
{
    input_t input = read_input();
    std::ranges::sort(input);
    solve_1(input);
    solve_2(input);

    return 0;
}