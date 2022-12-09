#include "../outf.h"

#include <algorithm>
#include <string>
#include <vector>

struct rope_head_move_t
{
    char direction;
    int distance;
};

using input_t = std::vector<rope_head_move_t>;
input_t read()
{
    input_t input;

    std::string line;
    while (std::getline(std::cin, line))
    {
        input.emplace_back(line[0], stoi(line.substr(2)));
    }

    return input;
}

using grid_t = std::vector<std::string>;

struct position_t
{
    int i, j;
};

void print_grid(const grid_t& grid, position_t head, position_t tail, position_t start)
{
    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid.size(); j++)
        {
            if (i == head.i && j == head.j)
            {
                outf("H");
            }
            else if (i == tail.i && j == tail.j)
            {
                outf("T");
            }
            else if (i == start.i && j == start.j)
            {
                outf("s");
            }
            else
            {
                outf("{}", grid[i][j]);
            }
        }
        outf("\n");
    }
    outf("\n");
}

void solve_1(const input_t& input)
{
    grid_t grid;
    grid.resize(5);
    for (auto& e : grid)
    {
        e = std::string(5, '.');
    }

    position_t start{ 2,2 };
    position_t head = start;
    position_t tail = start;
    std::vector<position_t> positions_visited_by_the_tail;
    positions_visited_by_the_tail.push_back(tail);

    for (auto e : input)
    {
        if (head.j <= e.distance || head.j >= grid.size() - e.distance
            || head.i <= e.distance || head.i >= grid.size() - e.distance)
        {
            add_padding(grid, '.', e.distance);
            head.i += e.distance;
            head.j += e.distance;
            tail.i += e.distance;
            tail.j += e.distance;
            start.i += e.distance;
            start.j += e.distance;
            for (auto& pos : positions_visited_by_the_tail)
            {
                pos.i += e.distance;
                pos.j += e.distance;
            }
        }

        switch (e.direction)
        {
        case 'L':
        {
            for (int step = 0; step < e.distance; step++)
            {
                head.j--;
                if (tail.j - 1 > head.j)
                {
                    if (std::find_if(positions_visited_by_the_tail.begin(), positions_visited_by_the_tail.end(), [tail](auto pos)
                        {
                            return tail.i == pos.i && tail.j == pos.j;
                        }) == positions_visited_by_the_tail.end())
                    {
                        positions_visited_by_the_tail.push_back(tail);
                    }
                    grid[tail.i][tail.j] = '#';
                    tail.j--;
                    if (tail.i != head.i)
                    {
                        tail.i = head.i;
                    }
                }
            }
            break;
        }
        case 'R':
        {
            for (int step = 0; step < e.distance; step++)
            {
                head.j++;
                if (tail.j + 1 < head.j)
                {
                    if (std::find_if(positions_visited_by_the_tail.begin(), positions_visited_by_the_tail.end(), [tail](auto pos)
                        {
                            return tail.i == pos.i && tail.j == pos.j;
                        }) == positions_visited_by_the_tail.end())
                    {
                        positions_visited_by_the_tail.push_back(tail);
                    }
                    grid[tail.i][tail.j] = '#';
                    tail.j++;
                    if (tail.i != head.i)
                    {
                        tail.i = head.i;
                    }
                }
            }
            break;
        }
        case 'U':
        {
            for (int step = 0; step < e.distance; step++)
            {
                head.i--;
                if (tail.i - 1 > head.i)
                {
                    if (std::find_if(positions_visited_by_the_tail.begin(), positions_visited_by_the_tail.end(), [tail](auto pos)
                        {
                            return tail.i == pos.i && tail.j == pos.j;
                        }) == positions_visited_by_the_tail.end())
                    {
                        positions_visited_by_the_tail.push_back(tail);
                    }
                    grid[tail.i][tail.j] = '#';
                    tail.i--;
                    if (tail.j != head.j)
                    {
                        tail.j = head.j;
                    }
                }
            }
            break;
        }
        case 'D':
        {
            for (int step = 0; step < e.distance; step++)
            {
                head.i++;
                if (tail.i + 1 < head.i)
                {
                    if (std::find_if(positions_visited_by_the_tail.begin(), positions_visited_by_the_tail.end(), [tail](auto pos)
                        {
                            return tail.i == pos.i && tail.j == pos.j;
                        }) == positions_visited_by_the_tail.end())
                    {
                        positions_visited_by_the_tail.push_back(tail);
                    }
                    grid[tail.i][tail.j] = '#';
                    tail.i++;
                    if (tail.j != head.j)
                    {
                        tail.j = head.j;
                    }
                }
            }
            break;
        }
        }

        //print_grid(grid, head, tail, start);
    }
    if (std::find_if(positions_visited_by_the_tail.begin(), positions_visited_by_the_tail.end(), [tail](auto pos)
        {
            return tail.i == pos.i && tail.j == pos.j;
        }) == positions_visited_by_the_tail.end())
    {
        positions_visited_by_the_tail.push_back(tail);
    }

    outf("{}\n\n", positions_visited_by_the_tail.size());
}

using rope_t = std::vector<position_t>;
void print_grid(const grid_t& grid, const rope_t& rope, position_t start)
{
    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid.size(); j++)
        {
            bool found_rope = false;
            for (auto k = 0; k < rope.size(); k++)
            {
                if (i == rope[k].i && j == rope[k].j)
                {
                    found_rope = true;
                    if (k == 0)
                    {
                        outf("H");
                    }
                    else
                    {
                        outf("{}", k);
                    }
                    break;
                }
            }
            if (!found_rope)
            {
                if (i == start.i && j == start.j)
                {
                    outf("s");
                }
                else
                {
                    outf("{}", grid[i][j]);
                }
            }
        }
        outf("\n");
    }
    outf("\n");
}

void update_tail_position(grid_t& grid, std::vector<position_t>& positions_visited_by_the_tail, const position_t& tail)
{
    if (std::find_if(positions_visited_by_the_tail.begin(), positions_visited_by_the_tail.end(), 
        [tail](auto pos) {
            return tail.i == pos.i && tail.j == pos.j;
        }
    ) == positions_visited_by_the_tail.end())
    {
        positions_visited_by_the_tail.push_back(tail);
    }

    grid[tail.i][tail.j] = '#';
}

void adjust_knots(rope_t& rope)
{
    for (size_t k = 1; k < rope.size(); k++)
    {
        int dist_i = rope[k - 1].i - rope[k].i;
        int dist_j = rope[k - 1].j - rope[k].j;

        if (dist_i == 0)
        {
            if (dist_j > 1)
                rope[k].j++;
            else if (dist_j < -1)
                rope[k].j--;
        }
        else if (dist_j == 0)
        {
            if (dist_i > 1)
                rope[k].i++;
            else if (dist_i < -1)
                rope[k].i--;
        }
        else if (std::abs(dist_i) + std::abs(dist_j) > 2)
        {
            rope[k].i += dist_i / std::abs(dist_i);
            rope[k].j += dist_j / std::abs(dist_j);
        }
    }
}

void solve_2(const input_t& input, size_t num_knots)
{
    grid_t grid;
    grid.resize(5);
    for (auto& e : grid)
    {
        e = std::string(5, '.');
    }

    position_t start{ 2,2 };
    rope_t rope;
    rope.resize(num_knots,start);

    std::vector<position_t> positions_visited_by_the_tail;
    positions_visited_by_the_tail.push_back(rope.back());

    //print_grid(grid, rope, start);

    for (auto e : input)
    {
        if (e.direction == 'L' && rope[0].j <= e.distance || e.direction == 'R' && rope[0].j >= grid.size() - e.distance
            || e.direction == 'U' && rope[0].i <= e.distance || e.direction == 'D' && rope[0].i >= grid.size() - e.distance)
        {
            add_padding(grid, '.', e.distance);
            for (auto& knot : rope)
            {
                knot.i += e.distance;
                knot.j += e.distance;
            }

            start.i += e.distance;
            start.j += e.distance;

            for (auto& pos : positions_visited_by_the_tail)
            {
                pos.i += e.distance;
                pos.j += e.distance;
            }
        }
        
        //outf("{} {}\n", e.direction, e.distance);
        int di = 0;
        int dj = 0;
        switch (e.direction)
        {
        case 'L':
        {
            di = 0;
            dj = -1;
            break;
        }
        case 'R':
        {
            di = 0;
            dj = 1;
            break;
        }
        case 'U':
        {
            di = -1;
            dj = 0;
            break;
        }
        case 'D':
        {
            di = 1;
            dj = 0;
            break;
        }
        }

        for (int step = 0; step < e.distance; step++)
        {
            update_tail_position(grid, positions_visited_by_the_tail, rope.back());
            rope[0].i += di;
            rope[0].j += dj;
            adjust_knots(rope);
        }
    }
    update_tail_position(grid, positions_visited_by_the_tail, rope.back());

    print_grid(grid, rope, start);
    outf("{}\n", positions_visited_by_the_tail.size());
}

void solve_1_v2(const input_t& input)
{
    solve_2(input, 2);
}

int main()
{
    input_t input = read();
    solve_1_v2(input);
    solve_2(input, 10);

    return 0;
}