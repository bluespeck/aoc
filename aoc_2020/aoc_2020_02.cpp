import std.core;

struct entry
{
    int64_t min, max;
    char letter;
    std::string password;
};

using input_t = std::vector<entry>;

input_t read_input()
{
    input_t input;

    while (std::cin)
    {
        entry e;
        char ignore;
        if (std::cin >> e.min >> e.max >> e.letter >> ignore >> e.password)
        {
            e.max = -e.max;
            input.push_back(e);
        }
    }

    return input;
}

void solve_1(input_t& input)
{
    int valid = std::ranges::count_if(input, [](const entry& e)
        {
            auto num_letters_found = std::ranges::count(e.password, e.letter);
            return e.min <= num_letters_found && e.max >= num_letters_found;
        });
    std::cout << valid << "\n";
}

void solve_2(input_t& input)
{
    int valid = std::ranges::count_if(input, [](const entry& e)
        {
            return e.password[e.min - 1] == e.letter
                && e.password[e.max - 1] != e.letter
                || e.password[e.min - 1] != e.letter
                && e.password[e.max - 1] == e.letter;
        });
    
    std::cout << valid << "\n";
}   

int main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}