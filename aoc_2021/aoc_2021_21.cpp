#include <cassert>
#include <string>
#include <vector>
#include "outf.h"

struct Input
{
    uint32_t pos1;
    uint32_t pos2;
};

uint32_t roll_die()
{
    static uint32_t die_current_value = 99;
    die_current_value = (die_current_value + 1) % 100;
    return die_current_value + 1;
}

Input read_input()
{
    Input input;
    
    input.pos1 = 1;
    input.pos2 = 5;
    return input;
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
   
    uint64_t score1 = 0;
    uint64_t score2 = 0;
    auto [pos1, pos2] = input;
    pos1--;
    pos2--;

    uint64_t roll_count = 0;
    while (true)
    {
        {
            uint64_t roll = roll_die() + roll_die() + roll_die();
            roll_count += 3;
            //outf("P1 @ {} ", pos1 + 1);
            pos1 = (pos1  + roll) % 10;
            score1 += pos1 + 1;
            //outf("rolls {} moves {}\n",  roll, pos1 + 1);
            if (score1 >= 1000)
            {
                outf("{}", score2 * roll_count);
                return;
            }
        }
        {
            uint64_t roll = roll_die() + roll_die() + roll_die();
            roll_count += 3;
            //outf("P2 @ {} ", pos2 + 1);

            pos2 = (pos2 + roll) % 10;
            score2 += pos2 + 1;
            
            //outf("rolls {} moves {}\n", roll, pos2 + 1);
            
            if (score2 >= 1000)
            {
                outf("{}", score1 * roll_count);
                return;
            }
        }
    }
    

}

std::pair<int64_t, int64_t> solve_2_rec(uint32_t player, uint32_t score1, int32_t pos1, uint32_t score2, int32_t pos2)
{
    static const uint32_t roll_sum_count[10] = { 0, 0, 0, 1, 3, 6, 7, 6, 3, 1 };
    
    if (score1 >= 21)
        return { 1, 0 };
    else if (score2 >= 21)
        return { 0, 1 };
    
    std::pair<int64_t, int64_t> win_counts = { 0, 0 };
    
    for (uint32_t sum = 3; sum <= 9; sum++)
    {
        auto new_pos = ((player == 0 ? pos1 : pos2) + sum) % 10;
        auto new_score = (player == 0 ? score1 : score2) + new_pos + 1;
        auto result = player == 0 ? solve_2_rec((player + 1) % 2, new_score, new_pos, score2, pos2) : solve_2_rec((player + 1) % 2, score1, pos1, new_score, new_pos);
        win_counts.first += roll_sum_count[sum] * result.first;
        win_counts.second += roll_sum_count[sum] * result.second;
    }

    return win_counts;
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);
    auto [pos1, pos2] = input;
    pos1--;
    pos2--;

    auto result = solve_2_rec(0, 0, pos1, 0, pos2);
    outf("{}", (result.first > result.second ? result.first : result.second) );
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
