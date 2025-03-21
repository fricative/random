#include <stddef.h>
#include <array>
#include <deque>
#include <unordered_map>
#include <vector>

using coord = std::pair<size_t, size_t>;

struct Cell
{
    size_t row;
    size_t col;
    size_t color;
    bool is_set = false;
    bool has_queen = false;

    Cell() = default;

    Cell(size_t r, size_t c, size_t color) : row{r}, col{c}, color{color} {}
    
    // copy constructor
    Cell(const Cell &other) = default;
        
    // copy assignment
    Cell& operator=(const Cell &other) = default;
 

    bool operator==(const Cell &other) const noexcept
    {
        return row == other.row &&
               col == other.col &&
               color == other.color;
    }

    void set_color(size_t c) noexcept
    {
        color = c;
    }

    void unset() noexcept
    {
        is_set = false;
        has_queen = false;
    }

    void set_queen() noexcept
    {
        is_set = true;
        has_queen = true;
    }

    void set_unavailable() noexcept
    {
        is_set = true;
        has_queen = false;
    }
};


class Game
{
public:
    Game(std::vector<std::vector<Cell>> &b) : board{b}
    {
        for (size_t row = 0; row < b.size(); ++row)
        {
            for (size_t col = 0; col < b.size(); ++col)
            {
                lookup[board[row][col].color].emplace_back(row, col);
            }
        }
        done = std::vector<int>(lookup.size(), 0);
    }
    bool solve();
    void print_board();

private:
    std::vector<std::vector<Cell>> &board;
    std::deque<coord> steps;
    std::vector<int> done; // denote whether a color is fulfilled

    Cell& get_cell(const coord loc) noexcept;
    void rule_out_invalid_cells(const coord &recent_step);
    std::unordered_map<size_t, std::deque<coord>> lookup;
    size_t find_color_with_fewest_empty_cells();
    void backtrack(coord &stop_at);
    void place_queen(const coord cell);
    void set_unavailable(coord cell);
    bool game_finished();
};
