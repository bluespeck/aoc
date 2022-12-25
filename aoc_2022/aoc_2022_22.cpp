#include "../outf.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <vector>
#include <functional>

using grid_t = std::vector<std::string>;
struct path_element_t
{
    int32_t forward_dist;
    char dir;
};

enum dir_t
{
    right,
    down,
    left,
    up
};

using path_t = std::vector<path_element_t>;

struct input_t
{
    grid_t grid;
    path_t path;
    size_t face_size = 0;
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
                    input.path.emplace_back(stol(number_str), 'F');
                }
            }
        }

    }

    for (auto& e : input.grid)
    {
        e.resize(max_line_size, ' ');
    }
    input.face_size = min_face_size;

    auto p =input;
    return p;
}

constexpr std::array<char, 4> facings = { '>', 'v', '<', '^' };

constexpr std::array<int32_t, 4> di = { 0, 1, 0, -1 };
constexpr std::array<int32_t, 4> dj = { 1, 0, -1, 0 };


void move_forward(const grid_t& grid, size_t& i, size_t& j, size_t facing)
{
    assert(grid[i][j] != ' ');
    size_t ii = i + di[facing];
    size_t jj = j + dj[facing];
    if (grid[ii][jj] == ' ')
    {
        size_t iii = i;
        size_t jjj = j;
        
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

    size_t i, j, facing;
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

using trace_point_t = std::pair<std::pair<size_t, size_t>, size_t>; // i, j, facing
using face_t = std::vector<std::vector<char>>;
struct vector_t
{
    int32_t x, y, z;
};

bool operator==(const vector_t& a, const vector_t& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
bool operator!=(const vector_t& a, const vector_t& b)
{
    return !(a == b);
}

struct orientation_t
{
    vector_t ox, oy, oz;
};

bool operator==(const orientation_t& a, const orientation_t& b)
{
    return a.ox == b.ox && a.oy == b.oy && a.oz == b.oz;
}
bool operator!=(const orientation_t& a, const orientation_t& b)
{
    return !(a == b);
}

struct face_3d_t
{
    face_t face;
    orientation_t orientation;
};

struct cube_t
{
    std::array<face_3d_t, 6> faces;
    std::array<orientation_t, 6> orientation_when_stamped;
    std::array<std::pair<size_t, size_t>, 6> face_to_grid_tile_map;
    size_t front = 4;
    size_t back = 5;
    size_t left = 2;
    size_t right = 3;
    size_t bottom = 1;
    size_t top = 0;
    size_t start_face = 6;
};

void rotate_face_left(face_t& face)
{
    size_t face_size = face.size();
    face_t new_face = face;
    for (size_t i = 0; i < face_size; i++)
    {
        for (size_t j = 0; j < face_size; j++)
        {
            auto& new_face_value = new_face[face_size - 1 - j][i];
            if (face[i][j] == '.' || face[i][j] == '#')
            {
                new_face_value = face[i][j];
            }
            else
            {
                switch (face[i][j])
                {
                case '>':
                    new_face_value = '^';
                    break;
                case 'v':
                    new_face_value = '>';
                    break;
                case '<':
                    new_face_value = 'v';
                    break;
                case '^':
                    new_face_value = '<';
                    break;
                }
            }
        }
    }
    face = new_face;
}

void rotate_face_right(face_t& face)
{
    size_t face_size = face.size();
    face_t new_face = face;
    for (size_t i = 0; i < face_size; i++)
    {
        for (size_t j = 0; j < face_size; j++)
        {
            auto& new_face_value = new_face[j][face_size - 1 - i];
            if (face[i][j] == '.' || face[i][j] == '#')
            {
                new_face_value = face[i][j];
            }
            else
            {
                switch (face[i][j])
                {
                case '>':
                    new_face_value = 'v';
                    break;
                case 'v':
                    new_face_value = '<';
                    break;
                case '<':
                    new_face_value = '^';
                    break;
                case '^':
                    new_face_value = '>';
                    break;
                }
            }
        }
    }
    face = new_face;
}

void roll_vector_left(vector_t& vec)
{
    vec = { -vec.y, vec.x, vec.z };
}

void roll_vector_right(vector_t& vec)
{
    vec = { vec.y, -vec.x, vec.z };
}

void roll_vector_up(vector_t& vec)
{
    vec = { vec.x, -vec.z, vec.y };
}

void roll_vector_down(vector_t& vec)
{
    vec = { vec.x, vec.z, -vec.y };
}

void spin_vector_left(vector_t& vec)
{
    vec = { -vec.z, vec.y, vec.x };
}

void spin_vector_right(vector_t& vec)
{
    vec = { vec.z, vec.y, -vec.x };
}

void roll_orientation_left(orientation_t& orientation)
{
    roll_vector_left(orientation.ox);
    roll_vector_left(orientation.oy);
    roll_vector_left(orientation.oz);
}

void roll_orientation_right(orientation_t& orientation)
{
    roll_vector_right(orientation.ox);
    roll_vector_right(orientation.oy);
    roll_vector_right(orientation.oz);
}

void roll_orientation_up(orientation_t& orientation)
{
    roll_vector_up(orientation.ox);
    roll_vector_up(orientation.oy);
    roll_vector_up(orientation.oz);
}

void roll_orientation_down(orientation_t& orientation)
{
    roll_vector_down(orientation.ox);
    roll_vector_down(orientation.oy);
    roll_vector_down(orientation.oz);
}

void spin_orientation_left(orientation_t& orientation)
{
    spin_vector_left(orientation.ox);
    spin_vector_left(orientation.oy);
    spin_vector_left(orientation.oz);
}

void spin_orientation_right(orientation_t& orientation)
{
    spin_vector_right(orientation.ox);
    spin_vector_right(orientation.oy);
    spin_vector_right(orientation.oz);
}


void roll_cube_left(cube_t& cube)
{
    auto temp = cube.bottom;
    cube.bottom = cube.left;
    cube.left = cube.top;
    cube.top = cube.right;
    cube.right = temp;
    
    for (auto& e : cube.faces)
    {
        roll_orientation_left(e.orientation);
    }
}

void roll_cube_right(cube_t& cube)
{
    auto temp = cube.bottom;
    cube.bottom = cube.right;
    cube.right = cube.top;
    cube.top = cube.left;
    cube.left = temp;

    for (auto& e : cube.faces)
    {
        roll_orientation_right(e.orientation);
    }
}

void roll_cube_up(cube_t& cube)
{
    auto temp = cube.bottom;
    cube.bottom = cube.back;
    cube.back = cube.top;
    cube.top = cube.front;
    cube.front = temp;

    for (auto& e : cube.faces)
    {
        roll_orientation_up(e.orientation);
    }
}

void roll_cube_down(cube_t& cube)
{
    auto temp = cube.bottom;
    cube.bottom = cube.front;
    cube.front = cube.top;
    cube.top = cube.back;
    cube.back = temp;

    for (auto& e : cube.faces)
    {
        roll_orientation_down(e.orientation);
    }
}

void spin_cube_left(cube_t& cube)
{
    auto temp = cube.back;
    cube.back = cube.right;
    cube.right = cube.front;
    cube.front = cube.left;
    cube.left = temp;

    for (auto& e : cube.faces)
    {
        spin_orientation_left(e.orientation);
    }
}

void spin_cube_right(cube_t& cube)
{
    auto temp = cube.back;
    cube.back = cube.left;
    cube.left = cube.front;
    cube.front = cube.right;
    cube.right = temp;

    for (auto& e : cube.faces)
    {
        spin_orientation_right(e.orientation);
    }
}

face_t get_aligned_top_cube_face(const cube_t& cube)
{
    face_t face = cube.faces[cube.top].face;
    auto orientation = cube.orientation_when_stamped[cube.top];

    size_t steps = 0;
    while (orientation != cube.faces[cube.top].orientation)
    {
        spin_orientation_left(orientation);
        rotate_face_left(face);
        steps++;
        assert(steps < 4);
    }
    return face;
}

trace_point_t get_aligned_cube_top_face_coords(const cube_t& cube, size_t i, size_t j, size_t facing = 0)
{
    const auto face_size = cube.faces[cube.top].face.size();
    auto orientation = cube.faces[cube.top].orientation;

    size_t steps = 0;
    while (orientation != cube.orientation_when_stamped[cube.top])
    {
        spin_orientation_right(orientation);
        auto ii = j;
        auto jj = face_size - 1 - i;
        i = ii;
        j = jj;
        facing = (facing + 1 + 4) % 4;
        steps++;
        assert(steps < 4);
    }
    
    trace_point_t point;
    
    point.first.first = i;
    point.first.second = j;
    point.second = facing;

    return point;
}

char read_aligned_cube_top_face(cube_t& cube, size_t i, size_t j)
{
    auto point = get_aligned_cube_top_face_coords(cube, i, j);
    return cube.faces[cube.top].face[point.first.first][point.first.second];
}

void write_to_aligned_cube_top_face(cube_t& cube, size_t i, size_t j, size_t facing)
{
    auto point = get_aligned_cube_top_face_coords(cube, i, j, facing);
    cube.faces[cube.top].face[point.first.first][point.first.second] = facings[point.second];
}

void transfer_grid_face_to_cube_face(face_t& cube_face, const grid_t& grid, size_t grid_i, size_t grid_j, size_t face_size)
{
    assert(grid[grid_i][grid_j] != ' ');
    for (size_t i = 0; i < face_size; i++)
    {
        for (size_t j = 0; j < face_size; j++)
        {
            cube_face[i][j] = grid[grid_i + i][grid_j + j];
        }
    }
}

void transfer_cube_face_to_grid(const face_t& cube_face, grid_t& grid, size_t grid_i, size_t grid_j, size_t face_size)
{
    for (size_t i = 0; i < face_size; i++)
    {
        for (size_t j = 0; j < face_size; j++)
        {
            grid[grid_i + i][grid_j + j] = cube_face[i][j];
        }
    }
}

void print_face(face_t& face)
{
    for (size_t i = 0; i < face.size(); i++)
    {
        for (size_t j = 0; j < face.size(); j++)
        {
            outf("{}", face[i][j]);
        }
        outf("\n");
    }
    outf("\n");
}

void print_cube(cube_t& cube)
{
    print_face(cube.faces[cube.back].face);
    print_face(cube.faces[cube.top].face);
    print_face(cube.faces[cube.right].face);
    print_face(cube.faces[cube.bottom].face);
    print_face(cube.faces[cube.left].face);
    print_face(cube.faces[cube.front].face);
}

void print_4_faces_side_by_side(face_t& f1, face_t& f2, face_t& f3, face_t& f4)
{
    for (size_t i = 0; i < f1.size(); i++)
    {
        for (size_t j = 0; j < f1.size(); j++)
        {
            outf("{}", f1[i][j]);
        }
        outf(" ");
        for (size_t j = 0; j < f1.size(); j++)
        {
            outf("{}", f2[i][j]);
        }
        outf(" ");
        for (size_t j = 0; j < f1.size(); j++)
        {
            outf("{}", f3[i][j]);
        }
        outf(" ");
        for (size_t j = 0; j < f1.size(); j++)
        {
            outf("{}", f4[i][j]);
        }
        outf("\n");
    }
    outf("\n");
}

void print_cube_unwrapping(cube_t& cube)
{
    auto width = cube.faces[cube.top].face.size() + 1;

    auto top = get_aligned_top_cube_face(cube);
    roll_cube_right(cube);
    auto left = get_aligned_top_cube_face(cube);
    roll_cube_right(cube);
    auto bottom = get_aligned_top_cube_face(cube);
    roll_cube_right(cube);
    auto right = get_aligned_top_cube_face(cube);
    roll_cube_right(cube);
    roll_cube_up(cube);
    auto front = get_aligned_top_cube_face(cube);
    roll_cube_up(cube);
    roll_cube_up(cube);
    auto back = get_aligned_top_cube_face(cube);
    roll_cube_up(cube);

    outf("{:<{}}\n", "back", width);
    print_face(back);

    outf("{:<{}}{:<{}}{:<{}}{:<{}}\n", "top", width, "rght", width, "bot", width, "left", width);
    print_4_faces_side_by_side(top, right, bottom, left);

    outf("{:<{}}\n", "frnt", width);
    print_face(front);
}

void copy_grid_to_cube_recursive(cube_t& cube, grid_t& grid, size_t face_i, size_t face_j)
{
    static std::array<std::function<void(cube_t&)>, 4> descent_rolls = { roll_cube_left, roll_cube_up, roll_cube_right, roll_cube_down };
    static std::array<std::function<void(cube_t&)>, 4> ascent_rolls = { roll_cube_right, roll_cube_down, roll_cube_left, roll_cube_up };
    
    const auto face_size = cube.faces[cube.top].face.size();
    const auto num_faces_per_row = grid[0].size() / face_size;
    const auto num_face_rows = grid.size() / face_size;

    
    transfer_grid_face_to_cube_face(cube.faces[cube.top].face, grid, face_i * face_size, face_j * face_size, face_size);
    cube.orientation_when_stamped[cube.top] = cube.faces[cube.top].orientation;
    cube.face_to_grid_tile_map[cube.top] = { face_i, face_j };
    grid[face_i * face_size][face_j * face_size] = ' ';
    
    for (size_t k = 0; k < di.size(); k++)
    {
        int64_t ii = face_i + di[k];
        int64_t jj = face_j + dj[k];
        if (ii >= 0 && ii < static_cast<int64_t>(num_face_rows) && jj >= 0 
            && jj < static_cast<int64_t>(num_faces_per_row) && grid[ii * face_size][jj * face_size] != ' ')
        {
            descent_rolls[k](cube);
            copy_grid_to_cube_recursive(cube, grid, ii, jj);
            ascent_rolls[k](cube);
        }
    }
}

void copy_grid_to_cube(cube_t& cube, grid_t& grid, size_t face_i, size_t face_j)
{
    auto grid_copy = grid;
    copy_grid_to_cube_recursive(cube, grid_copy, face_i, face_j);
}

void copy_cube_to_grid_recursive(cube_t& cube, grid_t& guide_grid, grid_t& grid, size_t face_i, size_t face_j)
{
    static std::array<std::function<void(cube_t&)>, 4> descent_rolls = { roll_cube_left, roll_cube_up, roll_cube_right, roll_cube_down };
    static std::array<std::function<void(cube_t&)>, 4> ascent_rolls = { roll_cube_right, roll_cube_down, roll_cube_left, roll_cube_up };

    const auto face_size = cube.faces[cube.top].face.size();
    const auto num_faces_per_row = guide_grid[0].size() / face_size;
    const auto num_face_rows = guide_grid.size() / face_size;

    transfer_cube_face_to_grid(get_aligned_top_cube_face(cube), grid, face_i * face_size, face_j * face_size, face_size);
    guide_grid[face_i * face_size][face_j * face_size] = ' ';

    for (size_t k = 0; k < di.size(); k++)
    {
        int64_t ii = face_i + di[k];
        int64_t jj = face_j + dj[k];
        if (ii >= 0 && ii < static_cast<int64_t>(num_face_rows) && jj >= 0
            && jj < static_cast<int64_t>(num_faces_per_row) && guide_grid[ii * face_size][jj * face_size] != ' ')
        {
            descent_rolls[k](cube);
            copy_cube_to_grid_recursive(cube, guide_grid, grid, ii, jj);
            ascent_rolls[k](cube);
        }
    }
}

void copy_cube_to_grid(cube_t& cube, grid_t& grid, size_t face_i, size_t face_j)
{
    auto guide_grid = grid;
    copy_cube_to_grid_recursive(cube, guide_grid, grid, face_i, face_j);
}

void move_forward_on_cube(cube_t& cube, size_t& i, size_t& j, size_t facing)
{
    const size_t face_size = cube.faces[0].face.size();
    int64_t ii = i + di[facing];
    int64_t jj = j + dj[facing];
    if (ii < 0 || ii >= static_cast<int64_t>(face_size) || jj < 0 || jj >= static_cast<int64_t>(face_size))
    {
        ii = i;
        jj = j;
        switch (facing)
        {
        case right:
            jj = 0;
            //outf("Roll left\n");
            roll_cube_left(cube);
            break;
        case down:
            ii = 0;
            //outf("Roll up\n");
            roll_cube_up(cube);
            break;
        case left:
            jj = face_size - 1;
            //outf("Roll right\n");
            roll_cube_right(cube);
            break;
        case up:
            ii = face_size - 1;
            //outf("Roll down\n");
            roll_cube_down(cube);
            break;
        }
        //print_cube_unwrapping(cube);
        if (read_aligned_cube_top_face(cube, ii, jj) == '#')
        {
            //outf("Rollback\n");
            switch (facing)
            {
            case right:
                roll_cube_right(cube);
                break;
            case down:
                roll_cube_down(cube);
                break;
            case left:
                roll_cube_left(cube);
                break;
            case up:
                roll_cube_up(cube);
                break;
            }
            
            //print_cube_unwrapping(cube);
        }
        else
        {
            i = ii;
            j = jj;
            write_to_aligned_cube_top_face(cube, i, j, facing);
        }
    }
    else if(read_aligned_cube_top_face(cube, ii, jj) != '#')
    {
        i = ii;
        j = jj;
        write_to_aligned_cube_top_face(cube, i, j, facing);
    }
}

void print_grid(const grid_t& grid)
{
    for (auto& row : grid)
    {
        outf("{}\n", row);
    }
    outf("\n");
}

void align_cube_to_initial_position(cube_t& cube)
{
    face_t face = cube.faces[cube.top].face;
    orientation_t orientation = cube.orientation_when_stamped[cube.top];

    size_t steps = 0;
    
    while (orientation != cube.faces[cube.top].orientation)
    {
        spin_cube_left(cube);
        steps++;
        assert(steps < 4);
    }
}

void print_cube_as_grid(cube_t& cube, const grid_t& grid)
{
    auto grid_copy = grid;
    copy_cube_to_grid(cube, grid_copy, cube.face_to_grid_tile_map[cube.top].first, cube.face_to_grid_tile_map[cube.top].second);
    print_grid(grid_copy);
}


trace_point_t trace_path_on_cube(const path_t& path, cube_t&cube, const grid_t& grid)
{
    size_t face_size = cube.faces[cube.start_face].face.size();
    size_t i, j, facing;
    i = 0;
    assert(cube.start_face == cube.top);
    for (j = 0; j < cube.faces[cube.top].face[i].size(); j++)
    {
        if (cube.faces[cube.top].face[i][j] != '#')
        {
            break;
        }
    }
    facing = 0;
    cube.faces[cube.top].face[i][j] = facings[facing];

    for (auto& e : path)
    {
        if (e.dir == 'F')
        {
            for (int32_t k = 0; k < e.forward_dist; k++)
            {
                move_forward_on_cube(cube, i, j, facing);
            }
        }
        else if (e.dir == 'R')
        {
            facing = (facing + 1) % 4;
            write_to_aligned_cube_top_face(cube, i, j, facing);
        }
        else
        {
            assert(e.dir == 'L');
            facing = (facing - 1 + 4) % 4;
            write_to_aligned_cube_top_face(cube, i, j, facing);
        }
    }
    
    return { {i, j}, facing };
}

cube_t create_cube(size_t face_size)
{
    cube_t cube;
    for (auto& e : cube.faces)
    {
        e.face.resize(face_size);
        for (auto& row : e.face)
        {
            row.resize(face_size, ' ');
        }
    }

    orientation_t orientation = { {1,0,0}, { 0, 1, 0 }, {0, 1, 0} };
    cube.faces[cube.top].orientation = orientation;

    roll_orientation_right(orientation);
    cube.faces[cube.right].orientation = orientation;

    roll_orientation_right(orientation);
    cube.faces[cube.bottom].orientation = orientation;

    roll_orientation_right(orientation);
    cube.faces[cube.left].orientation = orientation;

    roll_orientation_right(orientation);

    roll_orientation_down(orientation);
    cube.faces[cube.front].orientation = orientation;
    
    roll_orientation_up(orientation);

    roll_orientation_up(orientation);
    cube.faces[cube.back].orientation = orientation;

    auto p = cube;
    return p;
}

void solve_2_1(const input_t& input)
{
    auto grid = input.grid;
    auto cube = create_cube(input.face_size);

    size_t num_faces_per_row = input.grid[0].size() / input.face_size;
    size_t num_face_rows = input.grid.size() / input.face_size;
    for (size_t i = 0; i < num_face_rows; i++)
    {
        bool starting_tile_found = false;
        for (size_t j = 0; j < num_faces_per_row; j++)
        {
            if (input.grid[i * input.face_size][j * input.face_size] != ' ')
            {
                cube.start_face = cube.top;
                copy_grid_to_cube(cube, grid, i, j);
                assert(cube.start_face == cube.top);
                starting_tile_found = true;
                break;
            }
        }
        if (starting_tile_found)
        {
            break;
        }
    }
    auto trace_endpoint = trace_path_on_cube(input.path, cube, grid);
    auto trace_endpoint_aligned = get_aligned_cube_top_face_coords(cube, trace_endpoint.first.first, trace_endpoint.first.second, trace_endpoint.second);
    {
        align_cube_to_initial_position(cube);
        print_cube_unwrapping(cube);
        print_cube_as_grid(cube, grid);
    }
    outf("{}\n", 1000 * (cube.face_to_grid_tile_map[cube.top].first * input.face_size + trace_endpoint_aligned.first.first + 1) + 4 * (cube.face_to_grid_tile_map[cube.top].second * input.face_size + trace_endpoint_aligned.first.second + 1) + trace_endpoint_aligned.second);
}

int main()
{
    input_t input = read();
    //solve_1(input);
    solve_2_1(input);

    return 0;
}