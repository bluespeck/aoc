import std.core;

struct food_t
{
    std::set<std::string> ingredients;
    std::set<std::string> allergens;
};

using input_t = std::vector<food_t>;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);

        food_t food;
        std::istringstream ss(line);
        while (ss)
        {
            std::string word;
            if ((ss >> word))
            {
                if (word.size() && word[0] == '(')
                {
                    while (ss >> word)
                    {
                        word.pop_back();
                        food.allergens.insert(word);
                    }
                }
                else
                {
                    food.ingredients.insert(word);
                }
            }
        }
        input.push_back(food);
    };

    return input;
}

void
solve(const input_t& input)
{
    std::unordered_map<std::string, std::set<std::string>> allergen_ingredients;
    for (int k = 0; k < input.size(); k++)
    {
        auto& e = input[k];
        for (auto& i : e.allergens)
        {
            if (!allergen_ingredients.contains(i))
            {
                allergen_ingredients[i] = e.ingredients;
            }
            else
            {
                std::set<std::string> res;
                std::ranges::set_intersection(
                    allergen_ingredients[i],
                    e.ingredients,
                    std::inserter(res, res.end()));
                allergen_ingredients[i] = res;
            }
        }
    }

    bool resolved = false;
    do
    {
        resolved = true;
        for (auto& [k, val] : allergen_ingredients)
        {
            if (val.size() == 1)
            {
                for (auto& [ko, valo] : allergen_ingredients)
                {
                    if (k != ko && valo.size() > 1)
                    {
                        std::set<std::string> res;
                        std::ranges::set_difference(
                            valo,
                            val,
                            std::inserter(res, res.end()));
                        valo = res;
                        resolved = false;
                    }
                }
            }
        }
    } while (!resolved);
    std::set<std::string> allergenic_ingredient_set;
    for (auto& [k, val] : allergen_ingredients)
    {
        allergenic_ingredient_set.insert(val.begin(), val.end());
    }

    size_t count = 0;
    for (auto& e : input)
    {
        count += std::ranges::count_if(e.ingredients, [](std::string& ingredient)
            {
                return !allergenic_ingredient_set.contains(ingredient);
            });
    }
    std::cout << "\noutput 1: " << count << std::endl;

    std::vector<std::string> allergens;
    std::vector<std::string> allergenic_ingredients_vec;
    for (auto& [k, val] : allergen_ingredients)
    {
        allergenic_ingredients_vec.push_back(*val.begin());
        allergens.push_back(k);
    }

    bool sorted = false;
    while (!sorted)
    {
        sorted = true;
        for (size_t i = 0; i < allergens.size() - 1; i++)
        {
            if (allergens[i] >= allergens[i + 1])
            {
                std::swap(allergens[i], allergens[i + 1]);
                std::swap(allergenic_ingredients_vec[i], allergenic_ingredients_vec[i + 1]);
                sorted = false;
            }
        }
    }

    std::ostringstream ss;
    for (auto& e : allergenic_ingredients_vec)
    {
        ss << e << ",";
    }

    auto output_2 = ss.str();
    output_2.pop_back();
    std::cout << "output 2: " << output_2 << std::endl;

}

int
main()
{
    input_t input = read_input();

    solve(input);

    return 0;
}