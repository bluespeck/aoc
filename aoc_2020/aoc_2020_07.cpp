import std.core;

struct bag_count_t
{
    std::string color;
    size_t count;
};

struct inner_bags_t
{
    std::vector< bag_count_t> inner_bags;
    size_t total_count;
};

using input_t = std::map<std::string, inner_bags_t>;

input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        std::string tint, base_color;
        std::string ignore;
        std::string bag_color;
        if (!(std::cin >> tint >> base_color >> ignore >> ignore))
            break;
        bag_color = tint + " " + base_color;
        input[bag_color].total_count = 0;
        while (ignore.back() != '.')
        {
            char c;
            c = std::cin.get();
            if (std::cin.peek() == 'n')
            {
                std::cin >> ignore >> ignore >> ignore;
                break; // no other bags
            }
            std::string inner_bag_color;
            size_t inner_bag_count = 0;
            if (!(std::cin >> inner_bag_count >> tint >> base_color >> ignore))
                break;
            inner_bag_color = tint + " " + base_color;
            input[bag_color].inner_bags.emplace_back(inner_bag_color, inner_bag_count);
        }
    }

    return input;
}

void
solve_1(input_t& input)
{
    std::vector<std::string> bags_with_gold_in_pass;
    bags_with_gold_in_pass.push_back("shiny gold");
    std::set<std::string> bags_with_gold;
    while (bags_with_gold_in_pass.size())
    {
        if (bags_with_gold_in_pass.size() > 1 || bags_with_gold_in_pass.back() != "shiny gold")
        {
            bags_with_gold.insert(bags_with_gold_in_pass.begin(), bags_with_gold_in_pass.end());
        }

        auto colors_to_search = bags_with_gold_in_pass;
        bags_with_gold_in_pass.clear();

        for (auto& [key, val] : input)
        {
            if (key == "shiny gold")
                continue;
            for (const auto& e : val.inner_bags)
            {
                if (std::ranges::any_of(colors_to_search,
                    [color = e.color](auto& str)
                    {
                        return color == str; 
                    }))
                {
                    bags_with_gold_in_pass.push_back(key);
                    break;
                }
            }
        }
    }
    std::cout << bags_with_gold.size() << std::endl;
}

size_t
count_inner_bags(input_t& input, std::string bag_color, size_t count = 0)
{
    if (!input.contains(bag_color))
    {
        return 1;
    }
    else
    {
        if (input[bag_color].total_count > 0)
            return input[bag_color].total_count;

        input[bag_color].total_count = 1 + std::accumulate(
            input[bag_color].inner_bags.begin(),
            input[bag_color].inner_bags.end(),
            0,
            [&input](size_t total, auto& e) {
                return total + e.count * count_inner_bags(input, e.color);
            });
        return input[bag_color].total_count;
    }
}

void
solve_2(input_t& input)
{
    std::cout << count_inner_bags(input, "shiny gold") - 1;
}   

int
main()
{
    input_t input = read_input();
    
    solve_1(input);
    solve_2(input);

    return 0;
}