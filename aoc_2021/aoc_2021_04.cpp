#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "outf.h"


using Board = std::array<std::array<int64_t, 5>, 5>;

struct Bingo {
    std::vector<int64_t> numbers;
    std::vector<Board> boards;

};

using Input = Bingo;

Input read_input()
{
    Input input;
    
    std::string line;
    std::getline(std::cin, line);
    
    auto number_strs = split_string(line, ",");
    for (auto e : number_strs)
    {
        input.numbers.push_back(std::stoull(e));
    }

    std::getline(std::cin, line);
    while (std::cin)
    {
        if (std::cin)
        {
            Board board;
            for (size_t i = 0; i < 5; i++)
                for (size_t j = 0; j < 5; j++)
                {
                    std::cin >> board[i][j];
                }
            input.boards.push_back(board);
            std::getline(std::cin, line);
        }
    }
    return input;
}

int64_t sum_unmarked_numbers(const Board& board)
{
    int64_t sum = 0;
    for (size_t l = 0; l < 5; l++)
    {
        for (size_t m = 0; m < 5; m++)
        {
            if (board[l][m] > 0)
                sum += board[l][m];
        }
    }
    return sum;
}

std::optional<std::pair<size_t, size_t>> find_number_position_on_board(const Board& board, int64_t number)
{
    for (size_t i = 0; i < 5; i++)
        for (size_t j = 0; j < 5; j++)
            if (board[i][j] == number)
                return std::pair{ i, j };
    return std::nullopt;
}

bool has_bingo_row(const Board& board, size_t row_index)
{
    for(auto e: board[row_index])
        if (e != -1)
            return false;
    return true;
}

bool has_bingo_column(const Board& board, size_t column_index)
{
    for (size_t row_index = 0; row_index < 5; row_index++)
        if (board[row_index][column_index] != -1)
            return false;
    return true;
}

void solve_1(const Input _input)
{
    outf("\nPart {}: ", 1);

    auto input = _input;
    for (auto e : input.numbers)
    {
        for (auto& board : input.boards)
        {
            auto pos = find_number_position_on_board(board, e);
            if(pos)
            {
                auto [i, j] = pos.value();

                board[i][j] = -1;

                if(has_bingo_row(board, i) || has_bingo_column(board, j))
                {
                    outf("{}", sum_unmarked_numbers(board) * e);
                    return;
                }
            }
        }
    }
}

void solve_2(const Input _input)
{
    outf("\nPart {}: ", 2);

    auto input = _input;
    std::vector<bool> has_bingo(input.numbers.size(), false);
    Board last_winner;
    int64_t last_number = 0;

    for (auto e : input.numbers)
    {
        for (size_t ii = 0; ii < input.boards.size(); ii++)
        {
            if (has_bingo[ii])
                continue;

            auto& board = input.boards[ii];

            if (auto pos = find_number_position_on_board(board, e))
            {
                auto [i, j] = pos.value();

                board[i][j] = -1;

                if (has_bingo_row(board, i) || has_bingo_column(board, j))
                {
                    last_winner = board;
                    last_number = e;
                    has_bingo[ii] = true;
                }
            }
        }
    }
    outf("{}", sum_unmarked_numbers(last_winner) * last_number);
}

int main()
{
    auto input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}
