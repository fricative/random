#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "tango.hpp"

using namespace std;


int main(int argc, char* argv[]) {
    
    if(argc != 2){
        cout << "Usage: ./tango game.txt" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream infile(filename);
    if(!infile.is_open()){
        cout << "Could not open file " << filename << endl;
        return 1;
    }

    cout << "Reading puzzle data from: " << filename << endl;

    vector<pair<Game, vector<Constraint*>>> games;

    int row = 0;
    constexpr int line_char_count = 11;
    string fileContent, line;
    vector<Constraint*> constraints;
    Game game {array<std::array<int, 6>, 6> {}};

    while (getline(infile, line)) {

        // every game has 11 rows in txt file
        if(row == 11){
            games.push_back(make_pair(game, constraints));
            game = Game {array<std::array<int, 6>, 6> {}};
            constraints.clear();
            row = 0;
            continue;
        }
        
        auto is_vertical_constraint_row = row % 2 == 1;
        if(is_vertical_constraint_row){
            for(int i = 0; i < line_char_count; ++i){

                if (line[i] == ' ')
                    continue;

                auto col = i >> 1;
                auto upper_row = (row - 1) >> 1;
                auto lower_row = (row + 1) >> 1;
                if (line[i] == '=')
                    constraints.push_back(
                        new ConstraintEqual(coord(upper_row, col), coord(lower_row, col)));
                else 
                    constraints.push_back(
                        new ConstraintNotEqual(coord(upper_row, col), coord(lower_row, col))); 
            }  
        } else {
            for(int i = 0; i < line_char_count; ++i){

                if (line[i] == ' ')
                    continue;
                
                auto actual_row = row >> 1;
                auto left_col = (i - 1) >> 1;
                auto col = i >> 1;
                auto right_col = (i + 1) >> 1;

                if (i % 2 == 1){
                    if (line[i] == '=')
                        constraints.push_back(
                            new ConstraintEqual(coord(actual_row, left_col), coord(actual_row, right_col)));
                    else 
                        constraints.push_back(
                            new ConstraintNotEqual(coord(actual_row, left_col), coord(actual_row, right_col)));
                } else {
                    auto value = line[i] - '0';
                    game.set(coord(actual_row, col), value);
                }
            }
        }       
        row++;
    }    
    infile.close();

    Tango tango;
    for(auto game: games) 
        tango.play(game.first, game.second);
    
    return 0;
}
