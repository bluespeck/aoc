#include "../outf.h"

#include <array>
#include <cassert>
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

using input_t = std::string;
input_t read()
{
    input_t input;
    std::getline(std::cin, input);

    return input;
}

using rock_t = std::vector<std::string>;

const std::array<rock_t, 5> rocks =
{
    std::vector{
        std::string{"####"}
    },
    std::vector{
        std::string{".#."},
        std::string{"###"},
        std::string{".#."}
    },
    std::vector{
        std::string{"..#"},
        std::string{"..#"},
        std::string{"###"}
    },
    std::vector{
        std::string{"#"},
        std::string{"#"},
        std::string{"#"},
        std::string{"#"}
    },
    std::vector{
        std::string{"##"},
        std::string{"##"}
    }
};

using chamber_t = std::vector<std::string>;
bool can_go_left(const chamber_t& chamber, const rock_t& rock, size_t i_bottom, size_t j_left)
{
    for (size_t i = 0; i < rock.size(); i++)
    {
        size_t jj = rock[i].find_first_of('#');
        if (chamber[(rock.size() - 1 - i) + i_bottom][j_left + jj - 1] != '.')
        {
            return false;
        }
    }
    return true;
}

bool can_go_right(const chamber_t& chamber, const rock_t& rock, size_t i_bottom, size_t j_left)
{
    for (size_t i = 0; i < rock.size(); i++)
    {
        size_t jj = rock[i].find_last_of('#');
        if (chamber[(rock.size() - 1 - i) + i_bottom][j_left + jj + 1] != '.')
        {
            return false;
        }
    }
    return true;
}

bool can_go_down(const chamber_t& chamber, const rock_t& rock, size_t i_bottom, size_t j_left)
{
    for (size_t j = 0; j < rock.back().size(); j++)
    {
        size_t ii = 0;
        // find last of '#' vertically in the rock
        for (size_t rock_i = rock.size(); rock_i > 0; rock_i--)
        {
            if (rock[rock_i - 1][j] == '#')
            {
                ii = rock_i - 1;
                break;
            }
        }

        if (chamber[i_bottom + (rock.size() - 1 - ii) - 1][j_left + j] != '.')
        {
            return false;
        }
    }
    return true;
}

void settle_rock(chamber_t& chamber,const rock_t& rock,size_t i_bottom,size_t j_left, char rock_piece = '#')
{
    for (size_t i = 0; i < rock.size(); i++)
    {
        for (size_t j = 0; j < rock[i].size(); j++)
        {
            if (rock[i][j] == '#')
            {
                assert(chamber[i_bottom + (rock.size() - 1 - i)][j_left + j] == '.');
                chamber[i_bottom + (rock.size() - 1 - i)][j_left + j] = rock_piece;
            }
        }
    }
}

size_t find_tallest_rock_tip_height(const chamber_t& chamber, size_t offset)
{
    for (size_t i = offset + 1; i < chamber.size(); i++)
    {
        if (chamber[i].find_first_of('#') == std::string::npos)
        {
            return i - 1;
        }
    }
    return 0;
}

void print_chamber(const chamber_t& chamber)
{
    for (size_t i = 0; i < chamber.size(); i++)
    {
        outf("{}\n", chamber[chamber.size() - 1 - i]);
    }
    outf("\n");
}

void print_falling_rock(const chamber_t& chamber, const rock_t& rock, size_t i_bottom, size_t j_left)
{
    if (!show_diagnostic_messages)
        return;
    auto chamber_copy = chamber;
    settle_rock(chamber_copy, rock, i_bottom, j_left, '@');
    for (size_t i = chamber_copy.size(); i >= i_bottom; i--)
    {
        outf("{}\n", chamber_copy[i - 1]);
    }
    outf("\n");
}

bool started_repeating(const chamber_t& chamber, size_t rock_height, size_t pattern_size, size_t& start_height, size_t& period)
{
    if (rock_height <= pattern_size * 2)
        return false;
    size_t upper_limit = rock_height - pattern_size * 2;
    for (size_t i = 1; i < upper_limit; i++)
    {
        bool found = true;
        for (size_t k = 0; k < pattern_size; k++)
        {
            if (chamber[i + k] != chamber[rock_height - pattern_size + k])
                found = false;
        }
        if (found)
        {
            period = upper_limit - i + pattern_size;
            start_height = rock_height - pattern_size;
            return true;
        }
    }
    return false;
}

bool find_pattern(const chamber_t& chamber, size_t rock_height, const std::vector<std::string>& pattern)
{
    for (size_t k = 0; k < pattern.size(); k++)
    {
        if (chamber[rock_height - pattern.size() + k] != pattern[k])
            return false;
    }

    return true;
}

void solve_1(const input_t& input)
{
    constexpr size_t rock_limit = 2022;

    const std::string empty_chamber_row = "|.......|";
    chamber_t chamber;
    chamber.reserve(rock_limit);
    chamber.resize(4 + 3 + 1);
    for (auto& e : chamber)
    {
        e = empty_chamber_row;
    }
    chamber[0] = "+-------+";

    constexpr size_t chamber_width = 7;
    size_t tallest_rock_height = 0;
    size_t current_rock_index = 0;
    size_t i_bottom = tallest_rock_height + 4;
    size_t j_left = 1 + 2;
    size_t rock_count = 0;
    
    doutf("A new rock begins falling\n");
    print_falling_rock(chamber, rocks[current_rock_index], i_bottom, j_left);

    do
    {
        for (char dir : input)
        {
            if (dir == '<' && can_go_left(chamber, rocks[current_rock_index], i_bottom, j_left))
            {
                j_left--;
                doutf("Jet of gas pushes rock left\n");
                print_falling_rock(chamber, rocks[current_rock_index], i_bottom, j_left);
            }
            else if (dir == '>' && can_go_right(chamber, rocks[current_rock_index], i_bottom, j_left))
            {
                j_left++;
                doutf("Jet of gas pushes rock left\n");
                print_falling_rock(chamber, rocks[current_rock_index], i_bottom, j_left);
            }

            if (can_go_down(chamber, rocks[current_rock_index], i_bottom, j_left))
            {
                i_bottom--;
                doutf("Rock falls 1 unit\n");
                print_falling_rock(chamber, rocks[current_rock_index], i_bottom, j_left);
            }
            else
            {
                settle_rock(chamber, rocks[current_rock_index], i_bottom, j_left);
                doutf("Resting\n");
                size_t new_tallest_rock_height = find_tallest_rock_tip_height(chamber, tallest_rock_height);
                tallest_rock_height = new_tallest_rock_height;
                chamber.resize(tallest_rock_height + 4 + 3 + 1, empty_chamber_row);
                current_rock_index = (current_rock_index + 1) % rocks.size();
                i_bottom = tallest_rock_height + 4;
                j_left = 1 + 2;
                rock_count++;
                
                if (rock_count == rock_limit)
                    break;

                doutf("A new rock begins falling\n");
                print_falling_rock(chamber, rocks[current_rock_index], i_bottom, j_left);
            }
        }

    }
    while (rock_count < rock_limit);

    //print_chamber(chamber);
    
    outf("The tower height after {} rocks have stopped is {} m.\n", rock_limit, tallest_rock_height);
}

void solve_2(const input_t& input)
{
    static constexpr size_t rock_count_limit = 10'000;
    const std::string empty_chamber_row = "|.......|";
    chamber_t chamber;
    chamber.reserve(rock_count_limit);
    chamber.resize(4 + 3 + 1);
    for (auto& e : chamber)
    {
        e = empty_chamber_row;
    }
    chamber[0] = "+-------+";

    constexpr size_t chamber_width = 7;
    size_t tallest_rock_height = 0;
    size_t current_rock_index = 0;
    size_t i_bottom = tallest_rock_height + 4;
    size_t j_left = 1 + 2;
    size_t rock_count = 0;

    doutf("A new rock begins falling\n");
    print_falling_rock(chamber, rocks[current_rock_index], i_bottom, j_left);
    std::vector<size_t> pile_heights_relative_to_pattern_start;
    pile_heights_relative_to_pattern_start.push_back(0);

    size_t rock_count_when_pattern_started_repeating = 0;
    size_t rock_count_pattern_repetition_period = 0;
    size_t height_period = 0;
    size_t start_height = 0;
    size_t pattern_height = 24;
    std::vector<std::string> pattern;
    do
    {
        for (char dir : input)
        {
            if (dir == '<' && can_go_left(chamber, rocks[current_rock_index], i_bottom, j_left))
            {
                j_left--;
            }
            else if (dir == '>' && can_go_right(chamber, rocks[current_rock_index], i_bottom, j_left))
            {
                j_left++;
            }

            if (can_go_down(chamber, rocks[current_rock_index], i_bottom, j_left))
            {
                i_bottom--;
            }
            else
            {
                settle_rock(chamber, rocks[current_rock_index], i_bottom, j_left);
                tallest_rock_height = find_tallest_rock_tip_height(chamber, tallest_rock_height);

                chamber.resize(tallest_rock_height + 4 + 3 + 1, empty_chamber_row);
                current_rock_index = (current_rock_index + 1) % rocks.size();

                i_bottom = tallest_rock_height + 4;
                j_left = 1 + 2;
                rock_count++;
                if (rock_count_when_pattern_started_repeating == 0)
                {
                    if (started_repeating(chamber, tallest_rock_height, pattern_height, start_height, height_period))
                    {
                        rock_count_when_pattern_started_repeating = rock_count;
                        doutf("Started repeating after {} rocks, at height {}, with period {}.\n", rock_count, start_height, height_period);
                        for (size_t i = start_height; i < start_height + pattern_height; i++)
                        {
                            pattern.push_back(chamber[i]);
                        }
                    }
                }
                else
                {
                    if (find_pattern(chamber, tallest_rock_height, pattern))
                    {
                        rock_count_pattern_repetition_period = rock_count - rock_count_when_pattern_started_repeating;
                        size_t rock_falling_pattern_repetitions = (1'000'000'000'000 - rock_count_when_pattern_started_repeating) / rock_count_pattern_repetition_period;

                        size_t remaining_rocks_at_the_end = (1'000'000'000'000 - rock_count_when_pattern_started_repeating) % rock_count_pattern_repetition_period;

                        size_t total_height = tallest_rock_height - height_period + height_period * rock_falling_pattern_repetitions;

                        total_height += pile_heights_relative_to_pattern_start[remaining_rocks_at_the_end];

                        outf("The tower height after 1'000'000'000'000 rocks have stopped is {} m.\n", total_height);
                        return;
                    }
                    else
                    {
                        pile_heights_relative_to_pattern_start.push_back((tallest_rock_height - start_height - pattern_height) % height_period);
                    }

                }
            }
        }

    } while (rock_count < rock_count_limit);
}


int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}