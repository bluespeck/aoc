#include "../outf.h"

#include <iostream>
#include <format>
#include <list>
#include <vector>

void solve_1()
{
    const int32_t final_value = 2017;
    const int32_t step = 386;

    std::vector<int32_t> circular_buffer;

    size_t current_position = 0;
    int32_t current_value = 0;

    do
    {

        circular_buffer.insert(circular_buffer.begin() + current_position, current_value);
        current_position = (current_position + step) % circular_buffer.size() + 1;
    } while (current_value++ < final_value);

    for (int i = 0; i < circular_buffer.size(); i++)
    {
        if (circular_buffer[i] == final_value)
        {
            outf("{}\n", circular_buffer[(i + 1) % circular_buffer.size()]);
            break;
        }
    }

}

void solve_2()
{
    const int32_t final_value = 50'000'000;
    const int32_t step = 386;

    int32_t current_position = 0;
    int32_t current_value = 1;
    int32_t value_after_zero = 0;


    do
    {
        current_position = (current_position + step) % current_value + 1;
        if (current_position == 1)
        {
            value_after_zero = current_value;
        }
    } while (current_value++ < final_value);

    outf("{}\n", value_after_zero);
}

int main()
{
    solve_1();
    solve_2();

    return 0;
}