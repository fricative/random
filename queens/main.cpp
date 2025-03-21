#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include "queens.hpp"


int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << "./queens games.txt" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "Error: " << "Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    size_t line_no = 0;
    auto board = generate_blank_board();
    std::vector<std::array<std::array<Cell, 9>, 9>> games;

    while (std::getline(infile, line)) {
        if(line[0] == '='){
            games.push_back(board);
            board = generate_blank_board();
            line_no = 0;
            continue;
        }
        for(size_t col = 0; col < 9; ++col){
            board[line_no][col].set_color(line[col] - '0');            
        }
        line_no++;
    }

    for(auto& game: games){
        Game g{game};
        bool success = g.solve();
        std::cout << (success ? "successfully solved:\n" : "failed to solve:\n") << std::endl;
        g.print_board();
    }
    return 0;
}