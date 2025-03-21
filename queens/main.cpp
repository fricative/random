#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <memory>
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
    std::vector<std::vector<Cell>> board;
    std::vector<std::vector<std::vector<Cell>>> games;
    std::vector<Cell> row;
    while (std::getline(infile, line)) {
        if(line[0] == '='){
            games.push_back(board);
            board.clear();
            line_no = 0;
            continue;
        }
        row.clear();
        for(size_t col = 0; col < line.size(); ++col){
            if(line[col] < '1' || line[col] > '9') 
                break;
            row.emplace_back(line_no, col, line[col] - '0');            
        }
        board.push_back(row);
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