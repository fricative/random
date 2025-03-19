#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "tango.hpp"

using namespace std;


int main(int argc, char* argv[]) {
    
    if(argc != 1){
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

    int row = 0;
    string fileContent, line;
    vector<Constraint*> constraints;
    Game game {array<std::array<int, 6>, 6> {}};

    while (getline(infile, line)) {
        
        // every game has 11 rows in txt file
        if(row == 11){
            
            row = 0;
            continue;
        }
        
        auto is_vertical_constraint_row = row % 2 == 1;
        if(is_vertical_constraint_row){
            for(int i = 0; i < line.size(); ++i){
                if (line[i] == ' ')
                    continue;
                else {
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
            }  
        } else {
            for(int i = 0; i < line.size(); ++i){
                if (line[i] == ' ')
                    continue;
                
                auto actual_row = row >> 1;
                auto col = i >> 1;

                if (i % 2 == 1){
                    if (line[i] == '=')
                        constraints.push_back(
                            new ConstraintEqual(coord(actual_row, col), coord(actual_row, col)));
                    else 
                        constraints.push_back(
                            new ConstraintNotEqual(coord(actual_row, col), coord(actual_row, col)));
                } else {
                    auto value = line[i] - '0';
                    tango.game.set(row, col, value);
                }
            }
        }
       
        row++;
    }
    
    
    infile.close();

    // auto rounds = {
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,1,1,0,0},
    //                 std::array{0,0,1,2,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //             }
    //         ),
    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(0,2), coord(1,2)),
    //             new ConstraintEqual(coord(2,0), coord(2,1)),
    //             new ConstraintEqual(coord(2,4), coord(3,4)),

    //             new ConstraintNotEqual(coord(1,2), coord(1,3)),
    //             new ConstraintNotEqual(coord(2,1), coord(3,1)),
    //             new ConstraintNotEqual(coord(3,4), coord(3,5)),
    //             new ConstraintNotEqual(coord(4,2), coord(4,3)),
    //             new ConstraintNotEqual(coord(4,3), coord(5,3))
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,0,0,0,0,2},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,2,0,0,0},
    //                 std::array{0,0,0,2,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{2,0,0,0,0,0},
    //             }
    //         ),    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(0,4), coord(1,4)),
    //             new ConstraintEqual(coord(4,1), coord(5,1)),
    //             new ConstraintEqual(coord(4,5), coord(5,5)),
    //             new ConstraintEqual(coord(5,3), coord(5,4)),

    //             new ConstraintNotEqual(coord(0,0), coord(0,1)),
    //             new ConstraintNotEqual(coord(0,1), coord(0,2)),
    //             new ConstraintNotEqual(coord(0,0), coord(1,0)),
    //             new ConstraintNotEqual(coord(1,0), coord(2,0)),
    //             new ConstraintNotEqual(coord(3,5), coord(4,5)),
    //             new ConstraintNotEqual(coord(4,0), coord(4,1)),
    //             new ConstraintNotEqual(coord(1,4), coord(1,5)),
    //             new ConstraintNotEqual(coord(5,4), coord(5,5)),
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,1,0,0,1,0},
    //                 std::array{0,0,0,1,0,0},
    //                 std::array{1,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,1},
    //                 std::array{0,0,1,0,0,0},
    //                 std::array{0,1,0,0,1,0},
    //             }
    //         ),
    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(1,2), coord(2,2)),
    //             new ConstraintEqual(coord(3,4), coord(4,4)),

    //             new ConstraintNotEqual(coord(1,1), coord(2,1)),
    //             new ConstraintNotEqual(coord(3,3), coord(4,3)),
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,2,2,1,2,0},
    //                 std::array{0,0,1,0,0,0},
    //                 std::array{0,0,2,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //             }
    //         ),
            
    //         vector<Constraint*> {        
    //             new ConstraintEqual(coord(2,3), coord(3,3)),
    //             new ConstraintEqual(coord(4,1), coord(4,2)),
    //             new ConstraintEqual(coord(4,3), coord(4,4)),
    //             new ConstraintNotEqual(coord(3,3), coord(4,3)),
    //             new ConstraintNotEqual(coord(4,2), coord(4,3))
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,2,2,1,2,0},
    //                 std::array{0,0,1,0,0,0},
    //                 std::array{0,0,2,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //             }
    //         ),  
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(2,3), coord(3,3)),
    //             new ConstraintEqual(coord(4,1), coord(4,2)),
    //             new ConstraintEqual(coord(4,3), coord(4,4)),
    //             new ConstraintNotEqual(coord(3,3), coord(4,3)),
    //             new ConstraintNotEqual(coord(4,2), coord(4,3))
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //                 std::array{
    //                     std::array{0,0,0,0,0,0},
    //                     std::array{0,1,0,0,0,0},
    //                     std::array{2,1,0,0,0,0},
    //                     std::array{0,2,0,0,0,0},
    //                     std::array{1,2,1,0,0,0},
    //                     std::array{0,0,0,0,0,0},
    //                 }
    //             ),
    //         vector<Constraint*>  {        
    //             new ConstraintEqual(coord(2,3), coord(2,4)),
    //             new ConstraintEqual(coord(2,4), coord(3,4)),
    //             new ConstraintEqual(coord(4,4), coord(4,5)),
    //             new ConstraintNotEqual(coord(1,4), coord(2,4)),
    //             new ConstraintNotEqual(coord(3,4), coord(4,4)),
    //             new ConstraintNotEqual(coord(4,3), coord(4,4))
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,2,0,1,2,0},
    //                 std::array{0,1,0,0,0,0},
    //                 std::array{0,0,0,0,1,0},
    //                 std::array{0,2,1,0,1,0},
    //                 std::array{0,0,0,0,0,0},
    //             }
    //         ),    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(0,2), coord(0,3)),
    //             new ConstraintEqual(coord(3,5), coord(4,5)),
    //             new ConstraintEqual(coord(5,0), coord(5,1)),

    //             new ConstraintNotEqual(coord(0,4), coord(0,5)),
    //             new ConstraintNotEqual(coord(1,0), coord(2,0)),
    //             new ConstraintNotEqual(coord(5,2), coord(5,3)),
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,0,0,0,0,2},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{1,0,0,0,0,0},
    //             }
    //         ),    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(2,0), coord(2,1)),
    //             new ConstraintEqual(coord(4,2), coord(4,3)),
    //             new ConstraintEqual(coord(5,1), coord(5,2)),

    //             new ConstraintNotEqual(coord(0,3), coord(0,4)),
    //             new ConstraintNotEqual(coord(0,3), coord(1,3)),
    //             new ConstraintNotEqual(coord(1,1), coord(1,2)),
    //             new ConstraintNotEqual(coord(1,2), coord(1,3)),
    //             new ConstraintNotEqual(coord(1,5), coord(2,5)),
    //             new ConstraintNotEqual(coord(2,0), coord(3,0)),
    //             new ConstraintNotEqual(coord(2,5), coord(3,5)),
    //             new ConstraintNotEqual(coord(3,4), coord(3,5)),
    //             new ConstraintNotEqual(coord(3,0), coord(4,0)),
    //             new ConstraintNotEqual(coord(4,0), coord(5,0)),
    //             new ConstraintNotEqual(coord(4,2), coord(5,2)),
    //             new ConstraintNotEqual(coord(4,3), coord(4,4)),
    //             new ConstraintNotEqual(coord(5,0), coord(5,1)),
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{2,0,1,0,2,0},
    //                 std::array{0,2,0,1,0,0},
    //                 std::array{1,0,2,0,0,0},
    //                 std::array{0,1,0,0,0,0},
    //                 std::array{2,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //             }
    //         ),    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(1,5), coord(2,5)),
    //             new ConstraintEqual(coord(5,1), coord(5,2)),

    //             new ConstraintNotEqual(coord(2,4), coord(3,4)),
    //             new ConstraintNotEqual(coord(3,5), coord(4,5)),
    //             new ConstraintNotEqual(coord(4,2), coord(4,3)),
    //             new ConstraintNotEqual(coord(5,3), coord(5,4)),
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,2,2,0,0,0},
    //                 std::array{2,2,0,0,0,0},
    //                 std::array{2,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //             }
    //         ),    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(2,3), coord(2,4)),
    //             new ConstraintEqual(coord(2,2), coord(3,2)),
    //             new ConstraintEqual(coord(4,2), coord(5,2)),
    //             new ConstraintEqual(coord(4,3), coord(4,4)),

    //             new ConstraintNotEqual(coord(2,2), coord(2,3)),
    //             new ConstraintNotEqual(coord(2,4), coord(2,5)),
    //             new ConstraintNotEqual(coord(3,2), coord(4,2)),
    //             new ConstraintNotEqual(coord(4,4), coord(4,5)),
    //             new ConstraintNotEqual(coord(4,5), coord(5,5)),
    //             new ConstraintNotEqual(coord(5,2), coord(5,3)),
    //             new ConstraintNotEqual(coord(5,3), coord(5,4)),
    //             new ConstraintNotEqual(coord(5,4), coord(5,5)),
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{2,1,0,0,0,0},
    //                 std::array{0,2,0,0,0,0},
    //                 std::array{0,0,0,2,1,1},
    //                 std::array{0,0,0,1,0,2},
    //                 std::array{0,0,0,2,1,1},
    //                 std::array{0,0,0,0,0,0},
    //             }
    //         ),    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(0,4), coord(1,4)),

    //             new ConstraintNotEqual(coord(0,3), coord(0,4)),
    //             new ConstraintNotEqual(coord(2,0), coord(2,1)),
    //             new ConstraintNotEqual(coord(2,1), coord(2,2)),
    //             new ConstraintNotEqual(coord(2,0), coord(3,0)),
    //             new ConstraintNotEqual(coord(2,2), coord(3,2)),
    //             new ConstraintNotEqual(coord(3,0), coord(4,0)),
    //             new ConstraintNotEqual(coord(3,2), coord(4,2)),
    //             new ConstraintNotEqual(coord(4,0), coord(4,1)),   
    //             new ConstraintNotEqual(coord(4,1), coord(4,2)),   
    //         }
    //     ),
    //     make_pair(
    //         Game(
    //             std::array{
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,2,1,1,2,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //                 std::array{0,0,0,0,0,0},
    //             }
    //         ),    
    //         vector<Constraint*>{        
    //             new ConstraintEqual(coord(4,1), coord(4,2)),
    //             new ConstraintEqual(coord(4,3), coord(4,4)),
    //             new ConstraintEqual(coord(5,2), coord(5,3)),
    //             new ConstraintEqual(coord(2,1), coord(3,1)),

    //             new ConstraintNotEqual(coord(2,2), coord(3,2)),
    //             new ConstraintNotEqual(coord(2,3), coord(3,3)),
    //             new ConstraintNotEqual(coord(2,4), coord(3,4)),
    //         }
    //     ),
    // };
    
    // for(auto round: rounds) 
    //     tango.play(round.first, round.second);
    
    return 0;
}
