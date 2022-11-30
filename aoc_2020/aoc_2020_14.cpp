import std.core;

struct mem_t
{
    std::string mask;
    std::vector<std::pair<int64_t, int64_t>> memory_writes;

};

using input_t = std::vector<mem_t>;

input_t
read_input()
{
    input_t input;

    mem_t mem;
    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {
            if (line[3] == '[')
            {
                auto address_end = line.find(']');
                int64_t address = std::stoi(line.substr(4, address_end - 4));
                int64_t value = std::stoi(line.substr(address_end + 3));
                mem.memory_writes.emplace_back(address, value);
            }                                                            
            else
            {
                if(mem.memory_writes.size())
                    input.push_back(mem);
                mem = {};
                mem.mask = line.substr(7);
            }

        }
    }
    if (mem.memory_writes.size())
        input.push_back(mem);
    return input;
}

int64_t
apply_mask(int64_t value, std::string mask)
{
    int64_t dest = value;

    for(size_t i = 0; i < mask.size(); i++)
    { 
        switch (mask[i])
        {
        case '0':
        {
            int64_t m = ~(1LL << (35 - i));
            dest &= m;
            break;
        }
        case '1':
        {
            int64_t m = 1LL << (35 - i);
            dest |= m;
        }
        }
    }

    return dest;
}

void
solve_1(const input_t& input)
{
    std::unordered_map<int64_t, int64_t> mem_map;
    for (auto e : input)
    {
        for (auto mem : e.memory_writes)
        {
            mem_map[mem.first] = apply_mask(mem.second, e.mask);
        }
    }

    size_t sum = 0;
    for (auto e : mem_map)
    {
        if (e.second != 0)
            sum += e.second;
    }

    std::cout << sum << std::endl;
}

std::string
decode(int64_t value, std::string mask)
{
    std::string dest;
    int64_t value_temp = value;
    while (value_temp)
    {
        dest += (value_temp % 2) + '0';
        value_temp /= 2;
    }
    while (dest.size() < 36)
    {
        dest.push_back('0');
    }
    std::ranges::reverse(dest);

    for (size_t i = 0; i < mask.size(); i++)
    {
        switch (mask[i])
        {
        case '1':
        {
            dest[i] = '1';
            break;
        }
        case 'X':
        {
            dest[i] = 'X';
        }
        }
    }

    return dest;
}

std::vector<int64_t>
expand_addresses(std::string floating_value)
{
    std::vector<int64_t> addresses;
    
    size_t x_bits_count = std::ranges::count(floating_value, 'X');
    if (x_bits_count == 0)
    {
        addresses.push_back(std::stoll(floating_value, nullptr, 2));
        return addresses;
    }

    int64_t x_bits = (1LL << x_bits_count) - 1;
    while (x_bits >= 0)
    {
        std::string value = floating_value;
        size_t x_bit_index = 0;
        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == 'X')
            {
                value[i] = ((x_bits & (1LL << (x_bits_count - x_bit_index - 1))) != 0) ? '1' : '0';
                x_bit_index++;
            }
        }
        addresses.push_back(std::stoll(value, nullptr, 2));
        x_bits--;
    }
    return addresses;
}

void
solve_2(const input_t& input)
{
    std::unordered_map<int64_t, int64_t> mem_map;
    for (auto e : input)
    {
        for (auto mem : e.memory_writes)
        {
            auto addresses = expand_addresses(decode(mem.first, e.mask));
            for (auto addr : addresses)
            {
                mem_map[addr] = mem.second;
            }
        }
    }

    size_t sum = 0;
    for (auto e : mem_map)
    {
        if (e.second != 0)
            sum += e.second;
    }

    std::cout << sum << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}