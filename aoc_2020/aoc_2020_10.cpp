import std.core;

using input_t = std::vector<int64_t>;


input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        int64_t number;
        if(std::cin >> number)
            input.push_back(number);
    }

    return input;
}

void
solve_1(input_t& input)
{
    int64_t needed_joltage = 0;
    int64_t diff_ones = 0;
    int64_t diff_threes = 0;
    for (auto e : input)
    {
        if (e - needed_joltage == 1)
        {
            diff_ones++;
        }
        else if (e - needed_joltage == 3)
        {
            diff_threes++;
        }
        needed_joltage = e;
    }
    diff_threes++;
    std::cout << diff_ones * diff_threes << std::endl;
}

void
solve_2(input_t& input)
{
    input.insert(input.begin(), 0);

    std::vector<int64_t> num_arrangements;
    num_arrangements.resize(input.size(), 1);

    for (int64_t i = input.size() - 2; i >= 0; i--)
    {
        size_t sum = 0;
        for (int64_t j = i + 1; j <= i + 3 && j < input.size(); j++)
        {
            if (input[j] - input[i] <= 3)
                sum += num_arrangements[j];
            else
                break;
        }
        num_arrangements[i] = sum;
    }
    std::cout << num_arrangements[0] << std::endl;
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