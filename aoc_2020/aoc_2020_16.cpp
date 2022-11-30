import std.core;

struct interval_t
{
    int64_t start;
    int64_t end;
};

using validator_t = std::unordered_map<std::string, std::vector<interval_t>>;
using ticket_numbers_t = std::vector<int64_t>;

struct ticket_info_t
{
    validator_t validators;
    ticket_numbers_t my_ticket;
    std::vector<ticket_numbers_t> nearby_tickets;
};

using input_t = ticket_info_t;

input_t
read_input()
{
    input_t input;
    bool validators_read = false;
    bool my_ticket_read = false;

    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {
            if (line == "")
            {
                if (validators_read == false)
                {
                    validators_read = true;
                }
                else if (my_ticket_read == false)
                {
                    my_ticket_read = true;
                }
            }
            else if (!validators_read && !my_ticket_read)
            {
                std::string field_name = line.substr(0, std::ranges::find(line, ':') - line.begin());
                
                line = line.substr(field_name.size() + 1);
                interval_t interval;
                std::stringstream ss(line);
                ss >> interval.start;
                ss.ignore(1);
                ss >> interval.end;
                input.validators[field_name].push_back(interval);
                ss.ignore(3);
                ss >> interval.start;
                ss.ignore(1);
                ss >> interval.end;
                input.validators[field_name].push_back(interval);
            }
            else if (validators_read && !my_ticket_read)
            {
                if (line[0] == 'y')
                    continue;
                std::stringstream ss(line);
                ticket_numbers_t ticket_numbers;
                while (ss)
                {
                    int64_t num;
                    if (ss >> num)
                        ticket_numbers.push_back(num);
                    ss.ignore(1);
                }
                input.my_ticket = ticket_numbers;
            }
            else
            {
                if (line[0] == 'n')
                    continue;
                
                std::stringstream ss(line);
                ticket_numbers_t ticket_numbers;
                while (ss)
                {
                    int64_t num;
                    if (ss >> num)
                        ticket_numbers.push_back(num);
                    ss.ignore(1);
                }
                input.nearby_tickets.push_back(ticket_numbers);
            }
        }
    }
    
    return input;
}

void
solve_1(const input_t& input)
{
    int64_t sum_invalids = 0;
    for (auto& e : input.nearby_tickets)
    {
        for (auto num : e)
        {
            bool found_valid = false;
            for (auto& [key, val] : input.validators)
            {
                for (const auto& interval : val)
                {
                    if (num >= interval.start && num <= interval.end)
                    {
                        found_valid = true;
                        break;
                    }
                }
            }
            if (!found_valid)
            {
                sum_invalids += num;
            }
        }
    }

    std::cout << sum_invalids << std::endl;
}

bool is_valid_for_intervals(int64_t num, std::vector<interval_t> intervals)
{
    for (const auto& interval : intervals)
    {
        if (num >= interval.start && num <= interval.end)
        {
            return true;
        }
    }
    return false;
}

void
solve_2(input_t& input)
{
    {  // remove invalid tickets
        std::vector<size_t> invalid_ticket_indices;
        {
            int64_t ticket_index = 0;
            for (auto& e : input.nearby_tickets)
            {
                bool found_invalid_ticket = false;
                for (auto num : e)
                {
                    bool found_valid = false;
                    for (auto& [key, val] : input.validators)
                    {
                        found_valid = is_valid_for_intervals(num, val);
                    }
                    if (!found_valid)
                    {
                        found_invalid_ticket = true;
                    }
                }
                if (found_invalid_ticket)
                {
                    invalid_ticket_indices.push_back(ticket_index);
                }
                ticket_index++;
            }
        }

        std::ranges::reverse(invalid_ticket_indices);
        for (auto e : invalid_ticket_indices)
            input.nearby_tickets.erase(std::begin(input.nearby_tickets) + e);
    }
    input.nearby_tickets.push_back(input.my_ticket);
    auto& tickets = input.nearby_tickets;
    std::vector<std::set<std::string>> invalid_fields_per_field_index(tickets[0].size());
    for (size_t field_index = 0; field_index < tickets[0].size(); field_index++)
    {
        for (size_t i = 0; i < tickets.size(); i++)
        {
            for (auto& [key, val] : input.validators)
            {
                if (!is_valid_for_intervals(tickets[i][field_index], val))
                {
                    invalid_fields_per_field_index[field_index].insert(key);
                }
            }
        }
    }

    std::vector<std::pair<std::string, size_t>> field_indices;
    std::set<std::string> found_field_names;
    for(int64_t current_num_invalids = input.validators.size() - 1; current_num_invalids >=0; current_num_invalids--)
    {
        int ticket_field_index = 0;
        for (auto& e : invalid_fields_per_field_index)
        {
            if (e.size() == current_num_invalids)
            {
                for (auto& [key, value] : input.validators)
                {
                    if (e.find(key) == e.end() && found_field_names.find(key) == found_field_names.end())
                    {
                        field_indices.emplace_back(key, ticket_field_index);
                        found_field_names.insert(key);
                        e.erase(key);
                        break;
                    }
                }
            }
            ticket_field_index++;
        }
    }

    int64_t prod = 1;
    for (const auto& e : field_indices)
    {
        if(e.first.substr(0, 9) == "departure")
            prod *= input.my_ticket[e.second];
    }
    std::cout << prod << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}