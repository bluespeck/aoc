#include "../outf.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <map>
#include <numeric>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <list>
#include <functional>

using grid_t = std::vector<std::string>;
struct path_element_t
{
    int32_t forward_dist;
    char dir;
};

struct face_transition_element_t
{
    int32_t face;
    int32_t facing;
};

enum dir_t
{
    right,
    down,
    left,
    up
};

struct input_t
{
    grid_t grid;
    std::vector<path_element_t> path;
    //std::array<std::array<face_transition_element_t, 4>, 6> face_transition_mapping;
    //std::array<std::array<int32_t, 2>, 6> upper_left_coords;
    int32_t face_size = -1;
};

input_t read()
{
    input_t input;
    std::string line;
    size_t max_line_size = 0;
    size_t min_face_size = 100;
    while (std::getline(std::cin, line))
    {
        if (!line.empty())
        {
            input.grid.push_back(line);
            if (max_line_size < line.size())
            {
                max_line_size = line.size();
            }
            size_t non_space_count = line.size() - std::count(line.begin(), line.end(), ' ');
            if (non_space_count < min_face_size)
            {
                min_face_size = non_space_count;
            }
        }
        else
        {
            std::getline(std::cin, line);
            
            for (size_t i = 0; i < line.size();)
            {
                if (line[i] >= 'A')
                {
                    input.path.emplace_back(0, line[i]);
                    ++i;
                }
                else
                {
                    std::string number_str = line.substr(i, line.find_first_of("RL", i) - i);
                    i += number_str.size();
                    input.path.emplace_back(stoll(number_str), 'F');
                }
            }
        }

    }

    for (auto& e : input.grid)
    {
        e.resize(max_line_size, ' ');
    }
    input.face_size = min_face_size;

    //
    //auto& fm = input.face_transition_mapping;
    //
    //auto init_1 = [&fm, &input]()
    //{
    //    input.face_size = 4;
    //
    //    fm[0][right] = { 5, left };
    //    fm[0][down] = { 1, down };
    //    fm[0][left] = { 2, down };
    //    fm[0][up] = { 3, down };
    //
    //    fm[1][right] = { 5, down };
    //    fm[1][down] = { 4, down };
    //    fm[1][left] = { 2, left };
    //    fm[1][up] = { 0, up };
    //
    //    fm[2][right] = { 1, right };
    //    fm[2][down] = { 3, down };
    //    fm[2][left] = { 3, left };
    //    fm[2][up] = { 0, right };
    //
    //    fm[3][right] = { 2, right };
    //    fm[3][down] = { 4, up };
    //    fm[3][left] = { 5, up };
    //    fm[3][up] = { 0, down };
    //
    //    fm[4][right] = { 5, right };
    //    fm[4][down] = { 3, up };
    //    fm[4][left] = { 2, up };
    //    fm[4][up] = { 1, up };
    //
    //    fm[5][right] = { 0, left };
    //    fm[5][down] = { 3, right };
    //    fm[5][left] = { 4, left };
    //    fm[5][up] = { 1, left };
    //
    //    input.upper_left_coords[0] = { 0 * input.face_size + 1, 2 * input.face_size + 1 };
    //    input.upper_left_coords[1] = { 1 * input.face_size + 1, 2 * input.face_size + 1 };
    //    input.upper_left_coords[2] = { 1 * input.face_size + 1, 1 * input.face_size + 1 };
    //    input.upper_left_coords[3] = { 1 * input.face_size + 1, 0 * input.face_size + 1 };
    //    input.upper_left_coords[4] = { 2 * input.face_size + 1, 2 * input.face_size + 1 };
    //    input.upper_left_coords[5] = { 2 * input.face_size + 1, 3 * input.face_size + 1 };
    //};
    //
    //auto init_2 = [&fm, &input]()
    //{
    //    input.face_size = 50;
    //
    //    fm[0][right] = { 1, right };
    //    fm[0][down] = { 2, down };
    //    fm[0][left] = { 4, right };
    //    fm[0][up] = { 5, right };
    //
    //    fm[1][right] = { 3, left };
    //    fm[1][down] = { 2, left };
    //    fm[1][left] = { 0, left };
    //    fm[1][up] = { 5, up };
    //
    //    fm[2][right] = { 1, up };
    //    fm[2][down] = { 3, down };
    //    fm[2][left] = { 4, down };
    //    fm[2][up] = { 0, up };
    //
    //    fm[3][right] = { 1, left };
    //    fm[3][down] = { 5, left };
    //    fm[3][left] = { 4, left };
    //    fm[3][up] = { 2, up };
    //
    //    fm[4][right] = { 3, right };
    //    fm[4][down] = { 5, down };
    //    fm[4][left] = { 0, right };
    //    fm[4][up] = { 2, right };
    //
    //    fm[5][right] = { 3, up };
    //    fm[5][down] = { 1, down };
    //    fm[5][left] = { 0, down };
    //    fm[5][up] = { 4, up };
    //
    //    input.upper_left_coords[0] = { 0 * input.face_size + 1, 1 * input.face_size + 1 };
    //    input.upper_left_coords[1] = { 0 * input.face_size + 1, 2 * input.face_size + 1 };
    //    input.upper_left_coords[2] = { 1 * input.face_size + 1, 1 * input.face_size + 1 };
    //    input.upper_left_coords[3] = { 2 * input.face_size + 1, 1 * input.face_size + 1 };
    //    input.upper_left_coords[4] = { 2 * input.face_size + 1, 0 * input.face_size + 1 };
    //    input.upper_left_coords[5] = { 3 * input.face_size + 1, 0 * input.face_size + 1 };
    //};

    //init_1();
    auto p =input;
    return p;
}

constexpr std::array<char, 4> facing = { '>', 'v', '<', '^' };

constexpr std::array<int32_t, 4> di = { 0, 1, 0, -1 };
constexpr std::array<int32_t, 4> dj = { 1, 0, -1, 0 };


void move_forward(const grid_t& grid, int32_t& i, int32_t& j, int32_t facing)
{
    assert(grid[i][j] != ' ');
    auto ii = i + di[facing];
    auto jj = j + dj[facing];
    if (grid[ii][jj] == ' ')
    {
        auto iii = i;
        auto jjj = j;
        
        while (grid[iii][jjj] != ' ')
        {
            iii -= di[facing];
            jjj -= dj[facing];
        }
        ii = iii + di[facing];
        jj = jjj += dj[facing];
    }
    
    if (grid[ii][jj] != '#')
    {
        i = ii;
        j = jj;
    }
}

void solve_1(const input_t& input)
{
    auto grid = input.grid;
    add_padding(grid, ' ', 1);

    auto& path = input.path;

    int32_t i, j, facing;
    i = 1;
    j = grid[i].find_first_not_of(' ');
    facing = 0;



    for (auto& e : path)
    {
        if (e.dir == 'F')
        {
            for (int32_t k = 0; k < e.forward_dist; k++)
            {
                move_forward(grid, i, j, facing);
            }
        }
        else if (e.dir == 'R')
        {
            facing = (facing + 1) % 4;
        }
        else
        {
            assert(e.dir == 'L');
            facing = (facing - 1 + 4) % 4;
        }
    }

    outf("{}\n", 1000 * i + 4 * j + facing);
}


/*
int32_t get_face(int32_t i, int32_t j, const input_t& input)
{
    int32_t index = 0;
    for (auto& e : input.upper_left_coords)
    {
        if (i >= e[0] && i < e[0] + input.face_size &&
            j >= e[1] && j < e[1] + input.face_size)
        {
            return index;
        }
        index++;
    }
    return -1;
}

void rotate_coords_right(int32_t& i, int32_t& j, int32_t face_size)
{
    const int32_t radius = face_size / 2;
    i -= radius;
    j -= radius;
    i = j;
    j = -i;
    i += radius;
    j += radius;
}

void transform_coords(int32_t& i, int32_t& j, const face_transition_element_t& from, const face_transition_element_t& to, const input_t& input)
{
    i = (i - 1) % input.face_size;
    j = (j - 1) % input.face_size;
    if (to.facing == from.facing)
    {
        if (to.facing == right)
        {
            j = input.face_size - 1;
        }
        else if (to.facing == up)
        {
            i = input.face_size - 1;
        }
        else if (to.facing == down)
        {
            i = 0;
        }
        else if (to.facing == left)
        {
            j = 0;
        }
    }
    else if (to.facing != (from.facing + 2) % 4)
    {
        if (to.facing == right)
        {
        }
    }
    i += input.upper_left_coords[to.face][0];
    j += input.upper_left_coords[to.face][1];
}

void move_forward_on_cube(const grid_t& grid, int32_t& i, int32_t& j, int32_t& facing, const input_t& input)
{
    assert(grid[i][j] != ' ');
    auto current_face = get_face(i, j, input);
    
    auto ii = i + di[facing];
    auto jj = j + dj[facing];
    auto ccurrent_face = get_face(ii, jj, input);
    auto ffacing = facing;
    if (current_face != ccurrent_face)
    {
        ii = i;
        jj = j;
        transform_coords(ii, jj, { current_face, facing }, input.face_transition_mapping[current_face][facing], input);
        ffacing = input.face_transition_mapping[current_face][facing].facing;
        ccurrent_face = input.face_transition_mapping[current_face][facing].face;
        assert(grid[ii][jj] != ' ');
    }
    
    if (grid[ii][jj] != '#')
    {
        i = ii;
        j = jj;
        facing = ffacing;
    }
}

void solve_2(const input_t& input)
{
    auto grid = input.grid;
    add_padding(grid, ' ', 1);

    auto& path = input.path;

    int32_t i, j, facing;
    i = 1;
    j = grid[i].find_first_not_of(' ');
    facing = 0;



    for (auto& e : path)
    {
        if (e.dir == 'F')
        {
            for (int32_t k = 0; k < e.forward_dist; k++)
            {
                move_forward_on_cube(grid, i, j, facing, input);
            }
        }
        else if (e.dir == 'R')
        {
            facing = (facing + 1) % 4;
        }
        else
        {
            assert(e.dir == 'L');
            facing = (facing - 1 + 4) % 4;
        }
    }

    outf("{}\n", 1000 * i + 4 * j + facing);
}
*/

using face_t = std::vector<std::vector<char>>;

struct cube_t
{
    std::array<face_t, 6> faces;
    size_t front = 0;
    size_t back = 2;
    size_t left = 3;
    size_t right = 1;
    size_t bottom = 4;
    size_t top = 5;
};

void roll_cube_left(cube_t& cube)
{
    auto temp = cube.bottom;
    cube.bottom = cube.left;
    cube.left = cube.top;
    cube.top = cube.right;
    cube.right = temp;
}

void roll_cube_right(cube_t& cube)
{
    auto temp = cube.bottom;
    cube.bottom = cube.right;
    cube.right = cube.top;
    cube.top = cube.left;
    cube.left = temp;
}

void roll_cube_up(cube_t& cube)
{
    auto temp = cube.bottom;
    cube.bottom = cube.back;
    cube.back = cube.top;
    cube.top = cube.front;
    cube.front = temp;
}

void roll_cube_down(cube_t& cube)
{
    auto temp = cube.bottom;
    cube.bottom = cube.front;
    cube.front = cube.top;
    cube.top = cube.back;
    cube.back = temp;
}

void solve_2_1(const input_t& input)
{
    cube_t cube;
    for (auto& e : cube.faces)
    {
        e.resize(input.face_size);
        for (auto& row : e)
        {
            row.resize(input.face_size, ' ');
        }
    }


}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2_1(input);

    return 0;
}