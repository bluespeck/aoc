#include <cstdint>
#include <iostream>
#include <format>
#include <array>

int main()
{

    std::array<std::array<uint32_t, 201>, 201> grid;

    for (auto& row : grid)
    {
        row.fill(0);
    }

    size_t x = 100;
    size_t y = 100;
    uint32_t current_value = 1;
    grid[y][x] = current_value;

    auto sum_of_neighbors = [&grid](auto x, auto y) {
        uint32_t sum = 0;
        for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
                sum += grid[y + dy][x + dx];
        return sum;
    };

    // right up left down right -- five steps, each consisting of the given substeps
    int32_t ruldr[5] = { 1, 1, 2, 2, 2 };
    int32_t ruldr_delta[5] = { 0, 2, 2, 2, 2 };
    int32_t ruldr_dx[5] = { 1, 0, -1, 0, 1 };
    int32_t ruldr_dy[5] = { 0, 1, 0, -1, 0 };
    size_t step = 0;

    size_t min_x = x, min_y = y, max_x = x, max_y = y;

    while (current_value < 325489)
    {
        for (size_t sub_step = 0; sub_step < ruldr[step]; sub_step++)
        {
            x += ruldr_dx[step];
            y += ruldr_dy[step];
            if (x > max_x)
                max_x = x;
            else if (x < min_x)
                min_x = x;
            if (y > max_y)
                max_y = y;
            else if (y < min_y)
                min_y = y;
            current_value = sum_of_neighbors(x, y);
            grid[y][x] = current_value;
            if (current_value > 325489)
                break;
        }
        step = (step + 1) % 5;
        if (step == 0)
        {
            for (int i = 0; i < 5; i++)
                ruldr[i] += ruldr_delta[i];
        }
    }

    std::cout << "The grid:\n";
    for (size_t i = max_y; i >= min_y; i--)
    {
        for (size_t j = min_x; j <= max_x; j++)
            std::cout << std::format("{:7}", grid[i][j]);
        std::cout << "\n";
    }

    std::cout << std::format("\nThe output: {} \n", current_value);

    return 0;
}