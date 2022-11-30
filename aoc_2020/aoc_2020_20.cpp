import std.core;

using tile_t = std::vector<std::string>;
using tile_edges_t = std::vector<std::string>;
using tile_edge_configs_t = std::vector<tile_edges_t>;

struct tile_info_t
{
    int64_t id = id;
    tile_t tile;
    tile_edge_configs_t edge_configs;
};
using input_t = std::unordered_map<int64_t, tile_info_t>;

tile_edges_t
get_tile_edges(tile_t& tile)
{
    tile_edges_t edges;
    edges.push_back(tile.front());
    {
        std::string edge;
        for (int i = 0; i < tile.size(); i++)
        {
            edge += tile[i].back();
        }
        edges.push_back(edge);
    }
    {
        std::string edge = tile.back();
        std::ranges::reverse(edge);
        edges.push_back(edge);
    }
    {
        std::string edge;
        for (int i = tile.size() - 1; i >= 0; i--)
        {
            edge += tile[i].front();
        }
        edges.push_back(edge);
    }
    return edges;
}

void
rotate_90_right(tile_edges_t& edges)
{
    std::ranges::rotate(edges, edges.end() - 1);
}

void
flip_horrizontally(tile_edges_t& edges)
{
    for (auto& edge : edges)
    {
        std::ranges::reverse(edge);
    }
    std::swap(edges[1], edges[3]);
}

tile_edge_configs_t
get_all_configurations(tile_t& tile)
{
    tile_edge_configs_t configs;
    {
        tile_edges_t edges = get_tile_edges(tile);
        configs.push_back(edges);

        for (int i = 0; i < 3; i++)
        {
            rotate_90_right(edges);
            configs.push_back(edges);
        }

    }
    {
        tile_edges_t edges = get_tile_edges(tile);
        flip_horrizontally(edges);
        configs.push_back(edges);

        for (int i = 0; i < 3; i++)
        {
            rotate_90_right(edges);
            configs.push_back(edges);
        }
    }
    return configs;
}

bool
are_matching_configs(const tile_edge_configs_t& tc1, const tile_edge_configs_t& tc2)
{
    size_t count = 0;
    for (auto& c1 : tc1)
    {
        for (auto& c2 : tc2)
        {
            for (auto& e1 : c1)
            {
                for (auto& e2 : c2)
                {
                    std::string edge2 = e2;
                    std::ranges::reverse(edge2);
                    if (e1 == edge2)
                        return true;
                }
            }
        }
    }

    return false;
}


input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        std::string line; 
        std::getline(std::cin, line);
        
        tile_info_t tile_info;
        tile_info.id = std::stoll(line.substr(5, line.size() - 6));

        while (std::getline(std::cin, line))
        {
            if (line.size() == 0)
                break;
            tile_info.tile.push_back(line);
        }
        tile_info.edge_configs = get_all_configurations(tile_info.tile);
        input[tile_info.id] = tile_info;
    }

    return input;
}



std::vector<int64_t>
find_image_corners(input_t& input)
{
    std::vector<int64_t> corners;
    for (auto& [k1, val1] : input)
    {
        size_t count = 0;
        for (auto& [k2, val2] : input)
        {
            if (k2 != k1)
            {
                if (are_matching_configs(val1.edge_configs, val2.edge_configs))
                    count++;
            }
        }
        if (count == 2)
        {
            corners.push_back(k1);
        }
    } 

    return corners;
}

std::vector<int64_t>
find_image_edges(input_t& input)
{
    std::vector<int64_t> edges;
    for (auto& [k1, val1] : input)
    {
        size_t count = 0;
        for (auto& [k2, val2] : input)
        {
            if (k2 != k1)
            {
                if(are_matching_configs(val1.edge_configs, val2.edge_configs))
                {
                    count++;
                }
            }
        }
        if (count == 3)
        {
            edges.push_back(k1);
        }
    }
    std::cout << " #" << edges.size() << std::endl;
    return edges;
}

void
solve_1( input_t& input)
{
    int64_t prod = 1;
    for (auto corner : find_image_corners(input))
        prod *= corner;

    std::cout << prod << std::endl;
}

using id_config_pair_t = std::pair<int64_t, size_t>;
using grid_t = std::vector<std::vector<id_config_pair_t>>;



using adjacency_map_t = std::unordered_map<int64_t, std::vector<int64_t>>;

adjacency_map_t
find_image_adjacencies(input_t& input)
{
    adjacency_map_t adjacencies;
    for (auto& [k1, val1] : input)
    {
        size_t count = 0;
        for (auto& [k2, val2] : input)
        {
            if (k2 != k1)
            {
                if(are_matching_configs(val1.edge_configs, val2.edge_configs))
                {
                    adjacencies[k1].push_back(k2);
                }
            }
        }
        
    }
    
    return adjacencies;
}

bool
fits(const input_t& input, grid_t& grid, const tile_edges_t& tile_edges, int64_t i, int64_t j)
{
    int8_t di[4] = { -1, 0, 1,  0 }; // up, right, down, left as configs
    int8_t dj[4] = { 0, 1, 0, -1 };

    bool all_edges_match = true;

    for (int k = 0; k < 4; k++)
    {
        auto ii = di[k] + i;
        auto jj = di[k] + j;

        if (ii < 0 || ii >= grid.size() || jj < 0 || jj >= grid[ii].size() || grid[ii][jj].first == -1)
        {
            continue;
        }
        else
        {
            int64_t neighbor_id = grid[ii][jj].first;
            size_t neighbor_config_index = (grid[ii][jj].second + 2) % 4;
            const std::string& edge1 = tile_edges[k];
            const std::string& edge2 = input.at(neighbor_id).edge_configs[grid[ii][jj].second][neighbor_config_index];
            bool edges_match = true;
            for (int l = 0; l < edge1.size(); l++)
            {
                if (edge1[l] != edge2[edge2.size() - 1 - l])
                {
                    edges_match = false;
                    break;
                }
            }
            all_edges_match = all_edges_match && edges_match;
        }
    }

    return all_edges_match;
}

void
fill_grid(
    const input_t& input,
    grid_t& grid,
    const adjacency_map_t& adjacency_map,
    std::vector<int64_t>& corner_tiles,
    std::vector<int64_t>& edge_tiles,
    std::vector<int64_t>& inner_tiles)
{
    std::vector<std::pair<int, int>> indices_to_fill;
    for (int i = 0; i < grid.size(); i++)
    {
        indices_to_fill.emplace_back(0, i);
    }
    for (int i = 1; i < grid.size(); i++)
    {
        indices_to_fill.emplace_back(i, grid[0].size() - 1);
    }
    for (int i = 1; i < grid[grid.size() - 1].size(); i++)
    {
        indices_to_fill.emplace_back(grid.size() - 1, grid[grid.size() - 1].size() - 1 - i);
    }
    for (int i = 1; i < grid.size() - 1; i++)
    {
        indices_to_fill.emplace_back(grid.size() - 1 - i, 0);
    }

    for (int i = 1; i < grid.size() - 1; i++)
    {
        for (int j = 1; j < grid[i].size() - 1; j++)
        {
            indices_to_fill.emplace_back(i, j);
        }
    }

    std::swap(corner_tiles[0], corner_tiles[2]);
    for (auto& [i, j] : indices_to_fill)
    {
        std::vector<int64_t>* lookup_container;
        if ((i == 0 || i == grid.size() - 1) && (j == 0 || j == grid[i].size() - 1))
        {
            lookup_container = &corner_tiles;
        }
        else if (i == 0 || i == grid.size() - 1 || j == 0 || j == grid[i].size() - 1)
        {
            lookup_container = &edge_tiles;
        }
        else
        {
            lookup_container = &inner_tiles;
        }
        for (int l = 0; l < lookup_container->size(); l++)
        {
            bool good_neighbor = true;
            int64_t tile_id = (*lookup_container)[l];
            int8_t di[4] = { -1, 0, 1,  0 }; // up, right, down, left as configs
            int8_t dj[4] = { 0, 1, 0, -1 };
            for (int k = 0; k < 4; k++)
            {
                auto ii = di[k] + i;
                auto jj = dj[k] + j;

                if (ii < 0 || ii >= grid.size() || jj < 0 || jj >= grid[ii].size() || grid[ii][jj].first == -1)
                {
                    continue;
                }
                else
                {
                    std::vector<int64_t> neighbors1 = adjacency_map.at(grid[ii][jj].first);
                    std::vector<int64_t> neighbors2 = adjacency_map.at(tile_id);

                    if (std::ranges::find(neighbors1, tile_id) == neighbors1.end()
                        || std::ranges::find(neighbors2, grid[ii][jj].first) == neighbors2.end())
                    {
                        good_neighbor = false;
                        break;
                    }
                }
            }
            if (good_neighbor)
            {
                lookup_container->erase(lookup_container->begin() + l);
                grid[i][j].first = tile_id;
                break;
            }
        }
    }
    
}


size_t
get_matching_config(const tile_info_t& ti1, const tile_info_t& ti2, int k, size_t t2_config)
{   
    bool t2_is_flipped = t2_config >= 4;

    for (int i1 = 0; i1 < ti1.edge_configs.size(); i1++)
    {
        auto& e1 = ti1.edge_configs[i1][k];
        int i2 = (k + 2) % 4;
        std::string edge2 = ti2.edge_configs[t2_config][i2];
        std::ranges::reverse(edge2);
        if (e1 == edge2)
        {
            return i1;
        }
    }

    return 999;
}

void
flip_tiles(
    const input_t& input,
    grid_t& grid,
    const adjacency_map_t& adjacency_map)
{
    grid[0][0].second = get_matching_config(input.at(grid[0][0].first), input.at(grid[0][1].first), 1, 7);
    // std::cout << grid[0][0].second << " ";

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid.size(); j++)
        {
            const auto& tile_info = input.at(grid[i][j].first);
            int8_t di[4] = { -1, 0, 1,  0 }; // up, right, down, left as configs
            int8_t dj[4] = { 0, 1, 0, -1 };
            for (int k = 0; k < 4; k++)
            {
                auto ii = di[k] + i;
                auto jj = dj[k] + j;
                if (ii < 0 || ii >= grid.size() || jj < 0 || jj >= grid[ii].size() || grid[ii][jj].second == 999)
                {
                    continue;
                }
                else
                {
                    const auto& neighbor_tile_info = input.at(grid[ii][jj].first);
                    auto matching_config = get_matching_config(tile_info, neighbor_tile_info, k, grid[ii][jj].second);
                    //if (matching_config != 999)
                    //{
                    //    std::cout << matching_config << ((k == 3) ? "-":" ");
                    //}
                    grid[i][j].second = matching_config;
                }
            }
        }
        std::cout << "\n";
    }
}

void set_config(std::vector<std::string>& tile, size_t config)
{
    auto result = tile;
    switch (config)
    {
    case 3:
    case 2:
    case 1:
    case 0:
        for (int k = 0; k < config; k++)
        {
            for (int i = 0; i < result.size(); i++)
            {
                for (int j = 0; j < result.size(); j++)
                {
                    result[i][j] = tile[tile.size() - 1 - j][i];
                }
            }
            tile = result;
        }
        break;
    case 7:
    case 6:
    case 5:
    case 4:
        for (int i = 0; i < result.size(); i++)
        {
            std::ranges::reverse(result[i]);
        }
        tile = result;
        for (int k = 0; k < config - 4; k++)
        {
            for (int i = 0; i < result.size(); i++)
            {
                for (int j = 0; j < result.size(); j++)
                {
                    result[i][j] = tile[tile.size() - 1 - j][i];
                }
            }
            tile = result;
        }
        break;  
    }
}

std::vector<std::string>
extract_image(const input_t& input, grid_t grid, bool with_config)
{
    std::vector<std::string> image;

    for (auto& row : grid)
    {
        auto current_index = image.size();
        for (int i = 0; i < input.at(row[0].first).tile.size() - 2; i++)
        {
            image.push_back("");
        }
        
        for (auto& cell : row)
        {
            auto tile = input.at(cell.first).tile;
            if (with_config)
            {
                set_config(tile, cell.second);
            }
            tile.pop_back();
            tile.erase(tile.begin());
            for (int i = 0; i < tile.size(); i++)
            {
                image[current_index + i] += tile[i].substr(1, tile[i].size() - 2);
            }
        }
    }

    for (auto& e : image)
    {
        std::cout << e << "\n";
    }

    return image;
}

size_t
get_sea_roughness(std::vector<std::string>& image, const std::vector<std::string>& sea_monster)
{
    auto image_copy = image;
    for (size_t i = 0; i < image.size() - sea_monster.size(); i++)
    {
        for (size_t j = 0; j < image.size() - sea_monster[0].size(); j++)
        {
            bool found = true;
            for (size_t k = 0; k < sea_monster.size() && found == true; k++)
            {
                for (size_t l = 0; l < sea_monster[k].size(); l++)
                {
                    if (sea_monster[k][l] == '#' && image[i + k][j + l] != '#')
                    {
                        found = false;
                        break;
                    }
                }
            }
            if (found)
            {
                for (size_t k = 0; k < sea_monster.size(); k++)
                {
                    for (size_t l = 0; l < sea_monster[k].size(); l++)
                    {
                        if (sea_monster[k][l] == '#' && image[i + k][j + l] == '#')
                        {
                            image_copy[i + k][j + l] = 'O';
                        }
                    }
                }
                
            }
        }
    }

    size_t count = 0;
    for (size_t i = 0; i < image_copy.size(); i++)
    {
        for (size_t j = 0; j < image_copy.size() ; j++)
        {
            if (image_copy[i][j] == '#')
                count++;
        }
    }

    return count;
}


void
solve_2(input_t& input)
{
    std::vector<int64_t> corner_tiles, edge_tiles, inner_tiles;

    auto adjacency_map = find_image_adjacencies(input);
    for (auto& [k,val]:adjacency_map)
    {
        std::cout << k << ": ";
        for (auto& e : val)
        {
            std::cout << e << " ";
        }

        std::cout << '\n';

        if (val.size() == 2)
            corner_tiles.push_back(k);
        else if (val.size() == 3)
            edge_tiles.push_back(k);
        else
            inner_tiles.push_back(k);
    }

    std::cout << "corners: ";
    for (auto corner : corner_tiles)
        std::cout << corner << " ";
    std::cout << "\n edges: ";
    for (auto edge : edge_tiles)
        std::cout << edge << " ";

    grid_t grid;
    const size_t grid_side_size = sqrt(input.size());
    grid.resize(grid_side_size);
    for (auto& row : grid)
    {
        row.resize(grid_side_size, std::pair{ -1, 999 });
    }

    fill_grid(input, grid, adjacency_map, corner_tiles, edge_tiles, inner_tiles);
    //std::cout << "\n grid:\n";
    //for (auto& row : grid)
    //{
    //    for (auto& cell : row)
    //    {
    //        std::cout << std::setw(4) << cell.first << " ";
    //    }
    //    std::cout << '\n';
    //}

    //std::cout << "\n";
    //extract_image(input, grid, false);

    flip_tiles(input, grid, adjacency_map);
    //std::cout << "\n";
    auto image = extract_image(input, grid, true);

    std::vector<std::string> sea_monster =
    {
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    };

    std::cout << "\n\n";
    auto image_copy = image;
    for (int k = 0; k < 8; k++)
    {
        image = image_copy;
        set_config(image, k);
        auto count = get_sea_roughness(image, sea_monster);
        if (count < 2853)
        {
            //for (auto& e : image)
            //{
            //    std::cout << e << "\n";
            //}

            std::cout << count << "\n";
        }
        std::cout << "\n";
    }                                                                                         \
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}