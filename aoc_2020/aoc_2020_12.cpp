import std.core;

struct instruction_t
{
    char name;
    int value;
};
using input_t = std::vector<instruction_t>;

input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        char c;
        int value;
        if (std::cin >> c >> value)
        {
            input.emplace_back( c, value );
        }
    }

    return input;
}

void
solve_1(const input_t& input)
{
    int x = 0;
    int y = 0;
    int facing = 0;
    std::map<int, char> directions = {
        {   0, 'E'},
        {  90, 'N'},
        { 180, 'W'},
        { 270, 'S'}
    };

    for (auto e : input)
    {
        if (facing < 0)
            facing += 360;
        switch (e.name == 'F' ? directions[facing % 360] : e.name)
        {
        case 'N':
            y += e.value;
            break;
        case 'S':                 
            y -= e.value;
            break;
        case 'E':
            x += e.value;
            break;
        case 'W':
            x -= e.value;
            break;
        case 'L':
            facing += e.value;
            break;
        case 'R':
            facing -= e.value;
            break;
        }
    }
    std::cout << std::abs(x) + std::abs(y) << std::endl;
}

void
solve_2(const input_t& input)
{
    int waipoint_x = 10;
    int waipoint_y = 1;
    int ship_x = 0;
    int ship_y = 0;
    std::map<int, int> sines = {
        {0, 0},
        {90, 1},
        {180, 0},
        {270, -1}
    };
    std::map<int, int> cosines = {
        {0, 1},
        {90, 0},
        {180, -1},
        {270, 0}
    };
    for (auto e : input)
    {
        switch (e.name)
        {
        case 'N':
            waipoint_y += e.value;
            break;
        case 'S':
            waipoint_y -= e.value;
            break;
        case 'E':
            waipoint_x += e.value;
            break;
        case 'W':
            waipoint_x -= e.value;
            break;
        case 'L':
        case 'R':
        {
            auto angle = e.name == 'L' ? e.value : (360 - e.value);
            int xx = waipoint_x;
            int yy = waipoint_y;
            waipoint_x = xx * cosines[angle] - yy *   sines[angle];
            waipoint_y = xx *   sines[angle] + yy * cosines[angle];
            break;
        }
        case 'F':
            ship_x += e.value * waipoint_x;
            ship_y += e.value * waipoint_y;
            break;
        }
    }
    std::cout << std::abs(ship_x) + std::abs(ship_y) << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}