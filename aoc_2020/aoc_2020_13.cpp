import std.core;

struct time_stuff_t
{
    int64_t time_stamp;
    std::vector<int64_t> bus_ids;
};
using input_t = time_stuff_t;

input_t
read_input()
{
    input_t input;
    std::cin >> input.time_stamp;
    while (std::cin)
    {
        while (std::cin.peek() == 'x' || std::cin.peek() == ',')
        {
            if (std::cin.get() == 'x')
            {
                input.bus_ids.push_back(-1);
            }
        }
        int64_t bus_id;
        if(std::cin >> bus_id)
            input.bus_ids.push_back(bus_id);
    }

    return input;
}

void
solve_1(const input_t& input)
{
    std::vector<int64_t> filtered_bus_ids;
    std::ranges::copy_if(input.bus_ids, std::back_inserter(filtered_bus_ids), [](int64_t e) { return e != -1; });
    
    auto e = *std::ranges::min_element(
        filtered_bus_ids
        , [time_stamp = input.time_stamp](auto e1, auto e2)
            {
                return time_stamp / e1 * e1 + e1 - time_stamp < time_stamp / e2 * e2 + e2 - time_stamp;
            }
        );
    std::cout << e * (input.time_stamp / e * e + e - input.time_stamp) << std::endl;
   
}
// https://github.com/bluespeck/aoc_2019/blob/master/aoc_2019_22.cpp
// and thus using the awesome explanation by mcpower 
// on https://www.reddit.com/r/adventofcode/comments/ee0rqi/2019_day_22_solutions/fbnkaju/
int64_t euclidian_remainder(int64_t num, int64_t modulus)
{
    num = num % modulus;
    return (num + modulus) % modulus;
}

int64_t modular_mul(int64_t a, int64_t b, int64_t modulus)
{
    int64_t result = 0;
    int64_t signA = a < 0 ? -1 : 1;
    int64_t signB = b < 0 ? -1 : 1;
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;

    while (a != 0)
    {
        if (a & 1)
            result = (result + b) % modulus;
        a >>= 1;
        b = (b << 1) % modulus;
    }

    result *= signA * signB;
    result = euclidian_remainder(result, modulus);
    return result;
}

int64_t modular_pow(int64_t base, int64_t exponent, int64_t modulus)
{
    int64_t result = 1;
    base = euclidian_remainder(base, modulus);

    while (exponent > 0)
    {
        if (exponent & 1)
        {
            result = modular_mul(result, base, modulus); // prevents overflow
        }

        exponent >>= 1;
        base = modular_mul(base, base, modulus);
    }
    return result;
}

// https://rosettacode.org/wiki/Chinese_remainder_theorem
int64_t chinese_remainder(std::vector<int64_t> n, std::vector<int64_t> a) {
    int64_t prod = std::reduce(n.begin(), n.end(), 1LL, 
        [](int64_t a, int64_t b) 
        {
            return a * b;
        });

    int64_t sm = 0;
    for (size_t i = 0; i < n.size(); i++) {
        int64_t p = prod / n[i];
        sm += modular_mul(modular_mul(a[i], modular_pow(p, n[i] -2, n[i]), prod), p, prod);
    }

    return prod - sm % prod;
}


void
solve_2(const input_t& input)
{
    std::vector<int64_t> filtered_bus_ids;
    std::vector<int64_t> offsets;
    for (int64_t i = 0; i < input.bus_ids.size(); i++)
    {
        if (input.bus_ids[i] != -1)
        {
            offsets.push_back(i);
            filtered_bus_ids.push_back(input.bus_ids[i]);
        }
    }

    std::cout << chinese_remainder(filtered_bus_ids, offsets) << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}