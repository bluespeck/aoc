import std.core;

using input_t = std::vector<std::string>;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {
            input.push_back(line);
        }
    }

    return input;
}

size_t
count_trees_on_slope(const input_t& input, size_t dx, size_t dy, size_t start_x = 0, size_t start_y = 0)
{
    size_t tree_count = 0;
    size_t x = start_x;
    size_t y = start_y;
    while (y < input.size())
    {
        x += dx;
        y += dy;
        if (y >= input.size())
        {
            break;
        }
        x = x % input[y].size();
        if (input[y][x] == '#')
            tree_count++;
    }

    return tree_count;
}

void
solve_1(input_t& input)
{   
    std::cout << count_trees_on_slope(input, 3, 1) << std::endl;
}

void
solve_2(input_t& input)
{
    std::pair<size_t, size_t> slope[5] = { {1,1}, {3,1}, {5,1}, {7,1}, {1,2} };
    
    size_t prod = 1;
    for (int i = 0; i < 5; i++)
    {
        prod *= count_trees_on_slope(input, slope[i].first, slope[i].second);
    }

    std::cout << prod << std::endl;
}   

int main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}