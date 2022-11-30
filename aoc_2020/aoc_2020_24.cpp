import std.core;

using input_t = std::vector<std::string>;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {
            input.push_back(line);
        }

    };

    return input;
}

struct tile_t
{
    bool white_up = true;
    float x;
    int y;
};

void
solve_1(const input_t& input)
{
    std::vector<tile_t> tiles;
    for (auto& e : input)
    {
        float x = 0;
        int y = 0;
        for (size_t i = 0; i < e.size(); i++)
        {
            if (e[i] == 'w')
                x = x - 1.f;
            else if (e[i] == 'e')
                x = x + 1.f;
            else {
                if (e[i] == 'n')
                    y ++;
                else
                    y --;
                if (e[++i] == 'w')
                    x -= 0.5f;
                else
                    x += 0.5f;
            }
        }
        auto it = std::ranges::find_if(tiles, [x, y](auto& tile) {
            return std::abs(tile.x - x) < 0.005f && tile.y == y;
            });
        if (it == tiles.end())
            tiles.emplace_back(false, x, y);
        else
            it->white_up = !it->white_up;
    }
    size_t count = 0;
    for (auto& t : tiles)
    {
        if (!t.white_up)
            count++;
    }
    std::cout << count << std::endl;
}

auto
find_tile(std::vector<tile_t>& tiles, float x, int y)
{
    auto it = std::ranges::find_if(tiles, [x, y](auto& tile) {
        return std::abs(tile.x - x) < 0.005f && tile.y == y;
        });
    return it;
}

size_t
count_neighboring_black_tiles(std::vector<tile_t>& tiles, tile_t& tile)
{
    static const float   dx[] = { -1.f, 1.f,    -0.5f,  -0.5f,  0.5f,   0.5f };
    static const int     dy[] = { 0,    0,      1,      -1,     1,      -1 };

    // check neighbors of black tile
    
    size_t count = 0;
    for (int k = 0; k < 6; k++)
    {
        float xx = tile.x + dx[k];
        int yy = tile.y + dy[k];
        auto it = find_tile(tiles, xx, yy);
        if (it != tiles.cend())
        {
            count++;
        }
    }
    
    return count;
}

void
solve_2(const input_t& input)
{
    std::vector<tile_t> tiles;
    for (auto& e : input)
    {
        float x = 0;
        int y = 0;
        for (size_t i = 0; i < e.size(); i++)
        {
            if (e[i] == 'w')
                x = x - 1.f;
            else if (e[i] == 'e')
                x = x + 1.f;
            else {
                if (e[i] == 'n')
                    y++;
                else
                    y--;
                if (e[++i] == 'w')
                    x -= 0.5f;
                else
                    x += 0.5f;
            }
        }
        auto it = find_tile(tiles, x, y);
        if (it == tiles.cend())
            tiles.emplace_back(false, x, y);
        else
            it->white_up = !it->white_up;
    }
    
    for (int day = 0; day < 100; day++)
    {
        std::vector<tile_t> daily_tiles;
        for (auto& e : tiles) {
            if (!e.white_up)
                daily_tiles.push_back(e);
        }
        tiles.clear();

        for (auto& t : daily_tiles)
        {
            // check neighbors of black tile
            {
                size_t count = count_neighboring_black_tiles(daily_tiles, t);
                if (count == 1 || count == 2)
                {
                    // 0 or > 2 is not added in back, with the meaning that it's flipped
                    tiles.push_back(t);
                }
            }

            // process white tiles around black tile
            {
                const float   dx[] = { -1.f, 1.f,    -0.5f,  -0.5f,  0.5f,   0.5f };
                const int     dy[] = { 0,    0,      1,      -1,     1,      -1 };
            
                for (int k = 0; k < 6; k++)
                {
                    float xx = t.x + dx[k];
                    int yy = t.y + dy[k];
                    auto it_old = find_tile(daily_tiles, xx, yy);
                    auto it_new = find_tile(tiles, xx, yy);
                    if (it_old == daily_tiles.cend() && it_new == tiles.cend())
                    {
                        tile_t tile;
                        tile.x = xx;
                        tile.y = yy;
                        size_t count = count_neighboring_black_tiles(daily_tiles, tile);
                        if (count == 2)
                        {
                            tile.white_up = false;
                            tiles.push_back(tile);
                        }
                    }
                }
            }
        }

        std::cout << "day " << day + 1 << ": " << tiles.size() << "\n";
    }
    std::cout << tiles.size() << "\n";
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}