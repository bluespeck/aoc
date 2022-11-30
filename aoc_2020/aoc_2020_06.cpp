import std.core;

struct group_t
{
    std::array<int, 26> answers;
    std::vector<std::string> person_questions;
};
using input_t = std::vector<group_t>;

input_t
read_input()
{
    input_t input;
    group_t group;
    group.answers.fill(0);
    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {
            if (line.size() == 0)
            {
                input.push_back(group);
                group.answers.fill(0);
                group.person_questions.clear();
            }
            else
            {
                group.person_questions.push_back(line);
                for (auto c : line)
                {
                    group.answers[c - 'a'] ++;
                }
            }
        }
    }

    input.push_back(group);

    return input;
}

void
solve_1(input_t& input)
{
    std::cout << std::accumulate(input.begin(), input.end(), 0, [](int total, const group_t& group)
        {
            return total + std::ranges::count_if(group.answers, [](int value) { return value > 0; });
        }
    ) << std::endl;
}

void
solve_2(input_t& input)
{
    std::cout << std::accumulate(input.begin(), input.end(), 0, [](int total, const group_t& group)
        {
            return total + std::ranges::count_if(group.answers, [group_size = group.person_questions.size()](int value) { return value == group_size; });
        }
    ) << std::endl;
}   

int
main()
{
    input_t input = read_input();
    
    solve_1(input);
    solve_2(input);

    return 0;
}