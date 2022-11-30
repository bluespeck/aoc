import std.core;

using input_t = std::vector<int64_t>;

input_t
read_input()
{
    input_t input;
    
    while(std::cin)
    {
        int64_t spoken_number;
        if (std::cin >> spoken_number)
        {
            input.push_back(spoken_number);
        }
        std::cin.get();
    };
    
    return input;
}

void
solve(const input_t& input, int64_t count)
{
    std::unordered_map<int64_t, int64_t> turn_number_was_last_spoken;
    for (size_t i = 0; i < input.size() - 1; i++)
    {
        turn_number_was_last_spoken[input[i]] = i + 1;
    }

    int64_t current_turn = input.size() + 1;
    int64_t previous_spoken_number = input.back();
    while (current_turn <= count)
    {
        auto previous_turn = current_turn - 1;
        bool previous_spoken_number_was_new = !turn_number_was_last_spoken.contains(previous_spoken_number);

        int64_t number_spoken_this_turn = previous_spoken_number_was_new ? 0 : previous_turn - turn_number_was_last_spoken[previous_spoken_number];
        
        turn_number_was_last_spoken[previous_spoken_number] = previous_turn;
        previous_spoken_number = number_spoken_this_turn;
        current_turn++;
    }

    std::cout << previous_spoken_number << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve(input, 2020);
    solve(input, 30'000'000);

    return 0;
}