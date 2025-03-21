#include "queens.hpp"
#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>
#include <ranges>

std::array<std::array<Cell, 9>, 9> generate_blank_board()
{
    std::array<std::array<Cell, 9>, 9> board;
    for (size_t row = 0; row < 9; ++row)
    {
        for (size_t col = 0; col < 9; ++col)
        {
            board[row][col] = Cell{row, col, 0};
        }
    }
    return board;
}

size_t Game::find_color_with_fewest_empty_cells()
{
    size_t min = 100;
    size_t color = 1;
    for (auto &[key, value] : lookup)
    {
        if (!value.empty() && value.size() < min)
        {
            min = value.size();
            color = key;
        }
    }
    return color;
}

void Game::print_board()
{
    for (size_t row = 0; row < 9; ++row)
    {
        for (size_t col = 0; col < 9; ++col)
        {   
            auto cell = get_cell({row, col});
            if(!cell.is_set || !cell.has_queen)
                std::cout << cell.color;
            else
                std::cout << "*";   
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Game::game_finished()
{
    return std::all_of(done.begin(), done.end(), [](int i) { return i == 1; });
}

bool Game::solve()
{
    if (game_finished())
        return true;

    auto guess_color = find_color_with_fewest_empty_cells();
    unsigned int attempts = 0;
    unsigned int max_attempts = lookup[guess_color].size();

    auto cells_to_try = lookup[guess_color];
    for (auto &guess_cell : cells_to_try)
    {   
        place_queen(guess_cell);
        if (solve())
            return true;
        backtrack(guess_cell);
        if (++attempts == max_attempts)
            break;
    }
    return false;
}

void Game::backtrack(coord &stop_at)
{
    while (true)
    {
        auto [row, col] = steps.back();     
        auto& cell = get_cell({row, col});   
        if (cell.has_queen)
            done[board[row][col].color - 1] = 0;
        
        board[row][col].unset();
        steps.pop_back();
        lookup[board[row][col].color].emplace_back(row, col);

        if (stop_at == coord{row, col})
            break;
    }
}

void Game::set_unavailable(coord loc)
{
    auto [row, col] = loc;

    if (get_cell(loc).is_set)
        return;

    get_cell(loc).set_unavailable();
    steps.push_back(loc);
    std::erase(lookup[board[row][col].color], loc);
}

void Game::rule_out_invalid_cells(const coord &loc)
{   
    auto cell = get_cell(loc);
    auto recent_row = cell.row;
    auto recent_col = cell.col;
    auto recent_color = cell.color;

    // rule out same color cells
    std::ranges::for_each(lookup[recent_color], [this](coord c)
                          { set_unavailable(c); });

    // rule out adjacent diagonal cells
    if (recent_row > 0 && recent_col > 0)
        set_unavailable({recent_row - 1, recent_col - 1});
    if (recent_row > 0 && recent_col < 8)
        set_unavailable({recent_row - 1, recent_col + 1});
    if (recent_row < 8 && recent_col > 0)
        set_unavailable({recent_row + 1, recent_col - 1});
    if (recent_row < 8 && recent_col < 8)
        set_unavailable({recent_row + 1, recent_col + 1});

    // rule out cells in the same row
    for (size_t col = 0; col < 9; ++col)
    {
        if (col != recent_col)
            set_unavailable({recent_row, col});
    }

    // rule out cells in the same column
    for (size_t row = 0; row < 9; ++row)
    {
        if (row != recent_row)
            set_unavailable({row, recent_col});
    }
}

void Game::place_queen(const coord loc)
{
    auto& cell = get_cell(loc);
    cell.set_queen();
    done[cell.color - 1] = 1;
    steps.push_back(loc);
    std::erase(lookup[cell.color], loc);
    rule_out_invalid_cells(loc);
}

Cell &Game::get_cell(const coord loc) noexcept
{
    return board[loc.first][loc.second];
}