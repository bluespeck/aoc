import std.core;

using input_t = std::vector<int64_t>;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        int64_t key;
        if (std::cin >> key)
        {
            input.push_back(key);
        }
    };

    return input;
}

int64_t
transform(int64_t num, int64_t loop_size)
{
    int64_t val = 1;
    for (size_t i = 0; i < loop_size; i++)
    {
        val *= num;
        val %= 20201227;
    }
    return val;
}

int64_t
find_loop_size(int64_t num, int64_t public_key)
{
    int64_t val = 1;
    int64_t loop_size = 0;
    while (public_key != val)
    {
        loop_size++;
        val *= num;
        val %= 20201227;
    }
    return loop_size;
}


void
solve(const input_t& input)
{
    int64_t card_loop_size = find_loop_size(7, input[0]);
    int64_t door_loop_size = find_loop_size(7, input[1]);

    std::cout << transform(input[1], card_loop_size) << std::endl;
    std::cout << transform(input[0], door_loop_size) << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve(input);

    return 0;
}