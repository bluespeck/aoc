import std.core;
import std.regex;

struct passport_t
{
    std::vector<std::string> fields;
};
using input_t = std::vector<passport_t>;

input_t
read_input()
{
    input_t input;
    passport_t entry;

    std::regex field_rule{ "(byr|iyr|eyr|hgt|hcl|ecl|pid):\\S+" };

    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {

            if (line.size() == 0)
            {
                if (entry.fields.size() > 0)
                {
                    input.push_back(entry);
                    entry.fields.clear();
                }
            }
            else
            {
                std::copy(std::sregex_token_iterator(line.begin(), line.end(), field_rule),
                    std::sregex_token_iterator(), 
                    std::back_insert_iterator(entry.fields));
            }
        }
        else
        {
            if (entry.fields.size() > 0)
            {
                input.push_back(entry);
                entry.fields.clear();
            }
        }
    }

    return input;
}


void
solve_1(input_t& input)
{
    std::cout << std::ranges::count_if(input, [](const passport_t& entry) {
        return entry.fields.size() == 7;
        }) << std::endl;
}

void
solve_2(input_t& input)
{
    const std::regex field_value_rule[7] = {
        std::regex{ "^(19[2-9][0-9]|200[0-2])$" },
        std::regex{ "^(201[0-9]|2020)$" },
        std::regex{ "^(202[0-9]|2030)$" },
        std::regex{ "^((1[5-8][0-9]|19[0-3])cm)|((59|6[0-9]|7[0-6])in)$" },
        std::regex{ "^#[0-9a-f]{6}$" },
        std::regex{ "^(amb|blu|brn|gry|grn|hzl|oth)$" },
        std::regex{ "^\\d{9}$" }
    };

    std::cout << std::ranges::count_if(input, [&field_value_rule](const passport_t& entry) {
        static const std::string validation_fields[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };

        size_t valid_field_count = 0;
        for (size_t i = 0; i < 7; i++)
        {
            for (auto& field : entry.fields)
            {
                if (field.substr(0, 3) == validation_fields[i])
                {
                    std::smatch value_validation;
                    std::string field_value = field.substr(4);
                    if (std::regex_match(field_value, value_validation, field_value_rule[i]))
                    {
                        valid_field_count++;
                        break;
                    }
                }
            }
        }
        return valid_field_count == 7;
    }) << std::endl;
}   

int main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}