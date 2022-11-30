import std.core;

using input_t = std::vector<std::string>;

enum class problem_index_t
{
    one,
    two
};

const std::map<problem_index_t, int> acceptable_occupied_seats = {
    { problem_index_t::one, 4 },
    { problem_index_t::two, 5 }
};

input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        std::string line;
        if(std::getline(std::cin, line))
            input.push_back(line);
    }

    return input;
}

size_t
count_seat_neighboring_occupancy(problem_index_t problem_index, const input_t& input, int x, int y)
{
    static const int dx[] = { -1, -1, 1,  1,  0, -1, 0, 1 };
    static const int dy[] = { -1,  1, -1, 1, -1,  0, 1, 0 };

    auto in_bounds = [&input](int x, int y)
        {
            return  y >= 0 && y < input.size()
                && x >= 0 && x < input[0].size();
        };

    size_t occupied_adjacent_count = 0;

    for (int i = 0; i < 8; i++)
    {
        int yy = y + dy[i];
        int xx = x + dx[i];
        if (in_bounds(xx, yy))
        {
            if (problem_index == problem_index_t::two)
            {
                while (in_bounds(xx, yy) && input[yy][xx] == '.')
                {
                    xx += dx[i];
                    yy += dy[i];
                }
                if (!in_bounds(xx, yy))
                {
                    xx -= dx[i];
                    yy -= dy[i];
                }
            }

            if (input[yy][xx] == '#')
            {
                occupied_adjacent_count++;
            }
        }
    }

    return occupied_adjacent_count;
}

inline size_t
count_occupied_seats(const input_t& input)
{
    return std::accumulate(
        input.cbegin(),
        input.cend(),
        0,
        [](size_t acc, auto& e)
        {
            return acc + std::ranges::count(e, '#');
        });
}

bool
apply_pass(problem_index_t problem_index, input_t& input)
{
    bool changed = false;
    input_t output = input;
    for (int y = 0; y < input.size(); y++)
    {
        for (int x = 0; x < input[0].size(); x++)
        {
            const auto current_seat_state = input[y][x];
            auto& new_seat_state = output[y][x];
            if (current_seat_state == 'L')
            {
                if (count_seat_neighboring_occupancy(problem_index, input, x, y) == 0)
                {
                    new_seat_state = '#';
                    changed = true;
                }
            }
            else if (current_seat_state == '#')
            {
                if (count_seat_neighboring_occupancy(problem_index, input, x, y)
                    >= acceptable_occupied_seats.at(problem_index))
                {
                    new_seat_state = 'L';
                    changed = true;
                }
            }
            else
            {
                new_seat_state = current_seat_state;
            }
        }
    }
    input = output;

    return changed;
}

void
solve_1(const input_t& input)
{
    input_t grid = input;

    do
    {
    } while (apply_pass(problem_index_t::one, grid));

    std::cout << count_occupied_seats(grid) << std::endl;
}

void
solve_2(const input_t& input)
{
    input_t grid = input;

    do
    {
    } while (apply_pass(problem_index_t::two, grid));

    std::cout << count_occupied_seats(grid) << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}