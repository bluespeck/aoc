import std.core;
import std.regex;

using rule_branch_t = std::vector<int64_t>;
struct rule_t
{
    int64_t id;
    bool is_terminal = false;
    std::string sequence;
    std::vector<rule_branch_t> branches;
};

struct input_t
{
    std::unordered_map<int64_t, rule_t> rules;
    std::vector<std::string> messages;
};


input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        if (std::isdigit(std::cin.peek()))
        {
            rule_t rule;
            std::cin >> rule.id;
            std::cin.ignore(2); // ": "
            if(std::cin.peek() == '"')
            {
                std::cin.ignore();
                rule.is_terminal = true;
                rule.sequence += std::cin.get();
            }
            else
            {
                int64_t id;
                rule_branch_t branch;
                while (std::cin.peek() != '|' && std::cin >> id)
                {
                    branch.push_back(id);
                    if (std::cin.peek() == '\n')
                        break;
                    std::cin.ignore();
                }
                rule.branches.push_back(branch);
                branch.clear();
                std::string pipe;
                if (std::cin.peek() == '|')
                {
                    std::cin.ignore();
                    while (std::cin.peek() != '\n' && std::cin >> id)
                    {
                        branch.push_back(id);
                        if (std::cin.peek() == '\n')
                            break;
                    }
                    rule.branches.push_back(branch);
                }
            }
            std::cin.ignore(256, '\n');
            input.rules[rule.id] = rule;
        }
        else
        {
            std::string line;
            while (std::getline(std::cin, line))
            {
                input.messages.push_back(line);
            }
        }
    }

    return input;
}

using rule_cache_t = std::unordered_map<int64_t, std::string>;

std::string expand(input_t& input, rule_cache_t& cache, int64_t rule_id, bool special_rules = false)
{
    if (cache.contains(rule_id))
        return cache[rule_id];

    auto& rule = input.rules[rule_id];
    if (rule.is_terminal)
    {
        cache[rule_id] = rule.sequence;
        return rule.sequence;
    }
    else
    {
        std::string expansion = "";
        if (special_rules && rule_id == 8)
        {
            expansion = expand(input, cache, 42, special_rules) + "+";
        }
        else if (special_rules && rule_id == 11)
        {
            std::string r42 = expand(input, cache, 42, special_rules);
            std::string r31 = expand(input, cache, 31, special_rules);

            for (int i = 1; i <= 5; i++)
            {
                std::string count = std::to_string(i);
                expansion += r42 + "{" + count + "}"
                    + r31 + "{" + count + "}|";
            }
            expansion.pop_back();
        }
        else
        {
            for (auto& r : rule.branches)
            {
                for (auto& id : r)
                {
                    expansion += expand(input, cache, id, special_rules);
                }
                expansion += '|';
            }
            expansion.pop_back();
        }
        expansion = '(' + expansion + ')';
        cache[rule_id] = expansion;
        return expansion;
    }
}

void
solve_1( input_t& input)
{
    rule_cache_t cache;
    std::regex r(expand(input, cache, 0));
    std::cout << std::ranges::count_if(input.messages, [&r](std::string& str)
        {
            return std::regex_match(str.begin(), str.end(), r);
        }) << std::endl;
}

void
solve_2(input_t& input)
{
    bool special_rules = true;

    rule_cache_t cache;
    std::string regex_str = expand(input, cache, 0, special_rules);
    std::regex r(regex_str);

    std::cout << std::ranges::count_if(input.messages, [&r](std::string& str)
        {
            return std::regex_match(str.begin(), str.end(), r);
        }) << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}