#include "../outf.h"

#include <algorithm>
#include <utility>
#include <string>
#include <vector>

using input_t = std::vector<std::string>;
input_t read()
{
    input_t input;
    std::string line;
	while(std::getline(std::cin, line))
	{
        input.push_back(line);
	}

    return input;
}

using path_t = std::vector<std::pair<size_t, size_t>>;


void print_grid_path(const input_t& grid, const path_t& path)
{
    input_t my_grid = grid;
    for (size_t i = 0; i < path.size(); i++)
    {
        my_grid[path[i].first][path[i].second] = 'A' + i;
    }

    for (size_t ii = 0; ii < grid.size(); ii++)
    {
        outf("{}\n", my_grid[ii]);
    }
    outf("\n\n");
}


path_t shortest_path;
size_t fi;
size_t fj;

void recurse_bad(input_t& grid, path_t& path,size_t ii, size_t jj)
{
    static constexpr int di[4] = {0,1,0,-1};
    static constexpr int dj[4] = {1,0,-1,0};

    

    if (fi == ii && fj == jj)
    {
        if (shortest_path.empty() || shortest_path.size() > path.size())
        {
            print_grid_path(grid, path);
            shortest_path = path;
        }
        return;
    }

    for (size_t k = 0; k < 4; k++)
    {
        size_t nii = ii + di[k];
        size_t njj = jj + dj[k];
        if ((shortest_path.empty() || path.size() < shortest_path.size())
            && std::find(path.begin(), path.end(), std::pair{ nii,njj }) == path.end()
            && (grid[ii][jj] == grid[nii][njj] ||
            (grid[ii][jj] == grid[nii][njj] - 1))
            )
        {
            path.push_back({ nii, njj });
            recurse_bad(grid, path, nii, njj);
            path.pop_back();
        }
    }
}

using dists_t = std::vector<std::vector<int32_t>>;

void flood_fill_distances(input_t& grid, dists_t& dists, size_t ii, size_t jj)
{
    static constexpr int di[4] = { 0,1,0,-1 };
    static constexpr int dj[4] = { 1,0,-1,0 };

    for (size_t k = 0; k < 4; k++)
    {
        size_t nii = ii + di[k];
        size_t njj = jj + dj[k];
        if (grid[nii][njj] <= grid[ii][jj] + 1 &&
            dists[nii][njj] > dists[ii][jj] + 1)
        {
            dists[nii][njj] = dists[ii][jj] + 1;
            flood_fill_distances(grid, dists, nii, njj);
        }
    }
}

void solve_1(const input_t& input)
{
    input_t grid = input;
    add_padding(grid, 'z' + 3, 1);
    
    size_t si, sj;
    
    for (size_t ii = 0; ii < grid.size(); ii++)
    {
        for (size_t jj = 0; jj < grid[ii].size(); jj++)
        {
            if (grid[ii][jj] == 'S')
            {
                si = ii;
                sj = jj;
            }
            else if (grid[ii][jj] == 'E')
            {
                fi = ii;
                fj = jj;
            }
        }
    }

    grid[si][sj] = 'a';
    grid[fi][fj] = 'z';
    dists_t dists;
    dists.resize(grid.size());
    for (auto& e : dists)
    {
        e.resize(grid[0].size(), grid.size() * grid[0].size());
    }
    dists[si][sj] = 0;

    flood_fill_distances(grid, dists, si, sj);
    outf("Shortest path lentgh is {}\n", dists[fi][fj]);

}

void solve_2(const input_t& input)
{
    input_t grid = input;
    add_padding(grid, 'z' + 3, 1);

    size_t si, sj;

    for (size_t ii = 0; ii < grid.size(); ii++)
    {
        for (size_t jj = 0; jj < grid[ii].size(); jj++)
        {
            if (grid[ii][jj] == 'S')
            {
                si = ii;
                sj = jj;
            }
            else if (grid[ii][jj] == 'E')
            {
                fi = ii;
                fj = jj;
            }
        }
    }

    grid[si][sj] = 'a';
    grid[fi][fj] = 'z';
    size_t shortest_path_length = grid.size() * grid[0].size();

    for (size_t ii = 0; ii < grid.size(); ii++)
    {
        for (size_t jj = 0; jj < grid[ii].size(); jj++)
        {
            if (grid[ii][jj] == 'a')
            {
                si = ii;
                sj = jj;
                dists_t dists;
                dists.resize(grid.size());
                for (auto& e : dists)
                {
                    e.resize(grid[0].size(), grid.size() * grid[0].size());
                }
                dists[si][sj] = 0;

                flood_fill_distances(grid, dists, si, sj);
                if (dists[fi][fj] < shortest_path_length)
                {
                    shortest_path_length = dists[fi][fj];
                }
            }
        }
    }
    outf("Shortest path lentgh from any starting position with elevation 'a' is {}\n", shortest_path_length);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}