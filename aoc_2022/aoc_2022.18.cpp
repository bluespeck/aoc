#include "../outf.h"

#include <array>
#include <string>
#include <vector>

constexpr bool show_diagnostic_messages = false;

template <typename ...Args>
void doutf(const std::_Fmt_string<Args...> format_str, Args&&... args)
{
    if (show_diagnostic_messages)
    {
        outf(format_str, std::forward<Args>(args)...);
    }
}

struct point_t
{
    int64_t x, y, z;
};

using input_t = std::vector<point_t>;
input_t read()
{
    input_t input;
    std::string line;
    while (std::getline(std::cin, line))
    {
        auto parts = split_string(line, ",");
        input.emplace_back(std::stoll(parts[0]), std::stoll(parts[1]), std::stoll(parts[2]));
    }

    input_t p = input;
    return p;
}

static constexpr size_t grid_size = 25;
using grid_t = std::array<std::array<std::array<char, grid_size>, grid_size>, grid_size>;

bool has_common_face(point_t p1, point_t p2)
{
    return p1.x == p2.x && p1.y == p2.y && std::abs(p1.z - p2.z) == 1
        || p1.x == p2.x && p1.z == p2.z && std::abs(p1.y - p2.y) == 1
        || p1.y == p2.y && p1.z == p2.z && std::abs(p1.x - p2.x) == 1;
}

void solve_1(const input_t& input)
{
    
    int64_t area = 0;
    std::vector<point_t> joined_points;



    for (auto& e : input)
    {
        size_t count = 0;
        for (auto& j : joined_points)
        {
            if (has_common_face(e, j))
            {
                count++;
            }
        }
        //assert(count <= 3);
        area += 6 - 2 * count;
        joined_points.push_back(e);
    }
    

    outf("{}\n", area);
}

using bounds_array_t = std::array<std::array<std::vector<size_t>, grid_size>, grid_size>;


void solve_2_1(const input_t& input)
{
    grid_t grid;
    for (auto& x : grid)
    {
        for (auto& y : x)
        {
            y.fill('.');
        }
    }
    
    for (auto& i : input)
    {
        grid[i.x][i.y][i.z] = '*';
    }

    bounds_array_t z_bounds;
    bounds_array_t y_bounds;
    bounds_array_t x_bounds;


    for (size_t x = 0; x < grid_size; x++)
    {
        for(size_t y = 0; y < grid_size; y++)
        {
            for(size_t z = 0; z < grid_size; z++)
            {
                if (z == 0 && grid[x][y][z] == '*' || grid[x][y][z] != grid[x][y][z - 1])
                {
                    z_bounds[x][y].push_back(z);
                    break;
                }
            }
        }
    }

    for (size_t x = 0; x < grid_size; x++)
    {
        for (size_t z = 0; z < grid_size; z++)
        {
            for (size_t y = 0; y < grid_size; y++)
            {
                if (y == 0 && grid[x][y][z] == '*' || grid[x][y][z] != grid[x][y-1][z])
                {
                    y_bounds[x][z].push_back(y);
                }
            }
        }
    }
    for (size_t y = 0; y < grid_size; y++)
    {
        for (size_t z = 0; z < grid_size; z++)
        {
            for (size_t x = 0; x < grid_size; x++)
            {
                if (y == 0 && grid[x][y][z] == '*' || grid[x][y][z] != grid[x - 1][y][z])
                {
                    x_bounds[y][z].push_back(x);
                }
            }
            
        }
    }
    
    int64_t area = 0;
    {
        std::vector<point_t> joined_points;
        for (auto& e : input)
        {
            size_t count = 0;
            for (auto& j : joined_points)
            {
                if (has_common_face(e, j))
                {
                    count++;
                }
            }
            //assert(count <= 3);
            area += 6 - 2 * count;
            joined_points.push_back(e);
        }
    }
    
    auto is_inside = [&x_bounds, &y_bounds, &z_bounds, &grid](size_t x, size_t y, size_t z)
    {
        bool is_inside_x = false;
        for (size_t i = 0; i < x_bounds[y][z].size() - 1; i++)
        {
            if (x_bounds[y][z][i] <= x && x_bounds[y][z][i + 1] > x)
            {
                is_inside_x = i % 2 == 0;
            }
        }

        bool is_inside_y;
        for (size_t j = 0; j < y_bounds[x][z].size() - 1; j++)
        {
        }

        bool is_inside_z;
        for (size_t k = 0; k < y_bounds[x][y].size() - 1; k++)
        {
        }

    };

    std::vector<point_t> inside_cubes;
    for (size_t x = 0; x < grid_size; x++)
    {
        for (size_t y = 0; y < grid_size; y++)
        {
            for (size_t z = 0; z < grid_size; z++)
            {
                if (grid[x][y][z] == '.'
                    //&& x > x_bounds[y][z].first && x < x_bounds[y][z].second
                    //&& y > y_bounds[x][z].first && y < y_bounds[x][z].second
                    //&& z > z_bounds[x][y].first && z < z_bounds[x][y].second
                    )
                {
                    inside_cubes.emplace_back(x, y, z);
                }
            }
        }
    }

    int64_t inside_area = 0;
    {
        std::vector<point_t> joined_points;
        for (auto& e : inside_cubes)
        {
            size_t count = 0;
            for (auto& j : joined_points)
            {
                if (has_common_face(e, j))
                {
                    count++;
                }
            }
            //assert(count <= 3);
            inside_area += 6 - 2 * count;
            joined_points.push_back(e);
        }
    }


    outf("{}", area - inside_area);
}

void flood_fill(grid_t& grid, int64_t x, int64_t y, int64_t z)
{
    static const std::array<int,6> dx = { 0, 0,  0,  0, 1,   -1 };
    static const std::array<int,6> dy = { 0, 0,  1, -1, 0,   0 };
    static const std::array<int,6> dz = { 1, -1, 0,  0, 0,   0 };

    if (grid[x][y][z] == '.')
    {
        grid[x][y][z] = '*';
        for (int k = 0; k < 6; k++)
        {
            auto xx = x + dx[k];
            auto yy = y + dy[k];
            auto zz = z + dz[k];
            if (xx >= 0 && xx < grid_size && yy >= 0 && yy < grid_size && zz >= 0 && zz < grid_size)
            {
                flood_fill(grid, xx, yy, zz);
            }
        }
    }
}
void solve_2_2(const input_t& input)
{
    grid_t grid;
    for (auto& x : grid)
    {
        for (auto& y : x)
        {
            y.fill('.');
        }
    }

    for (auto& i : input)
    {
        grid[i.x][i.y][i.z] = '*';
    }

    flood_fill(grid, grid_size - 1, grid_size - 1, grid_size - 1);

    std::vector<point_t> inside_cubes;
    for (size_t x = 0; x < grid_size; x++)
    {
        for (size_t y = 0; y < grid_size; y++)
        {
            for (size_t z = 0; z < grid_size; z++)
            {
                if (grid[x][y][z] == '.')
                {
                    inside_cubes.emplace_back(x, y, z);
                }
            }
        }
    }

    int64_t area = 0;
    {
        std::vector<point_t> joined_points;
        for (auto& e : input)
        {
            size_t count = 0;
            for (auto& j : joined_points)
            {
                if (has_common_face(e, j))
                {
                    count++;
                }
            }
            //assert(count <= 3);
            area += 6 - 2 * count;
            joined_points.push_back(e);
        }
    }

    int64_t inside_area = 0;
    {
        std::vector<point_t> joined_points;

        for (auto& e : inside_cubes)
        {
            size_t count = 0;
            for (auto& j : joined_points)
            {
                if (has_common_face(e, j))
                {
                    count++;
                }
            }
            //assert(count <= 3);
            inside_area += 6 - 2 * count;
            joined_points.push_back(e);
        }
    }
    outf("{}\n", area - inside_area);
}
int main()
{
    input_t input = read();
    solve_1(input);
    solve_2_2(input);

    return 0;
}