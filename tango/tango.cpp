#include "tango.hpp"


bool ConstraintEqual::infer(Game& game) {
    cout << "INVOKING CONSTRAINT = AT (" << p1.row << ", " << p1.col << ") AND AT (" << p2.row << ", " << p2.col  << ")" << endl;
    bool fulfilled = false;
    
    if(game.at(p1) != 0){   
        game.set(p2, game.at(p1));
        fulfilled = true;
    } else if(game.at(p2) != 0){
        game.set(p1, game.at(p2));
        fulfilled = true;
    } 
    
    if(is_vertical(p1, p2)){

        auto has_up = p1.row > 0;
        auto has_down = p2.row < 5;
        if(has_up && game.at(up(p1)) != 0){
            auto opposite = negate_sign(game, up(p1));
            game.set(p1, opposite);
            game.set(p2, opposite);
            fulfilled = true;
        }
        if(has_down && game.at(down(p2)) != 0){
            auto opposite = negate_sign(game, down(p2));
            game.set(p1, opposite);
            game.set(p2, opposite);
            fulfilled = true;
        }
        if(p1.row == 0 && game.at(down(p1, 5)) != 0){
            auto opposite = negate_sign(game, down(p1, 5));
            game.set(p1, opposite);
            game.set(p2, opposite);
            fulfilled = true;
        }
        if(p2.row == 5 && game.at(up(p2, 5)) != 0){               
            auto opposite = negate_sign(game, up(p2, 5));
            game.set(p1, opposite);
            game.set(p2, opposite);
            fulfilled = true;
        }
        if(!fulfilled){
            unsigned int suns = 0, moons = 0;
            for(int i=0; i<5; ++i){
                switch(game.board[i][p1.col]){
                    case 1:
                        ++suns;
                        break;
                    case 2:
                        ++moons;
                        break;
                } 
            }
            if(suns > 1){
                game.set(p1, 2);
                game.set(p2, 2);
                fulfilled = true;
            } else if(moons > 1){
                game.set(p1, 1);
                game.set(p2, 1);                
                fulfilled = true;
            }     
        }      
    } else {
        auto has_left = p1.col > 0;
        auto has_right = p2.col < 4;
        if(has_left && game.at(left(p1)) != 0){
            auto opposite = negate_sign(game, left(p1));
            game.set(p1, opposite);
            game.set(p2, opposite);
            fulfilled = true;
        }
        if(has_right && game.at(right(p2)) != 0){
            auto opposite = negate_sign(game, right(p2));
            game.set(p1, opposite);
            game.set(p2, opposite);
            fulfilled = true;
        }
        if(p1.col == 0 && game.at(right(p1, 5)) != 0){
            auto opposite = negate_sign(game, right(p1, 5));
            game.set(p1, opposite);
            game.set(p2, opposite);
            fulfilled = true;
        }
        if(p2.col == 5 && game.at(left(p2, 5)) != 0){
            auto opposite = negate_sign(game, left(p2, 5));
            game.set(p1, opposite);
            game.set(p2, opposite);
            fulfilled = true;
        }
        if(!fulfilled){
            unsigned int suns = 0, moons = 0;
            for(int i=0; i<5; ++i){
                switch(game.board[p1.row][i]){
                    case 1:
                        ++suns;
                        break;
                    case 2:
                        ++moons;
                        break;
                }
            }
            if(suns > 1){
                game.set(p1, 2);
                game.set(p2, 2);
                fulfilled = true;
            } else if(moons > 1){
                game.set(p1, 1);
                game.set(p2, 1);
                fulfilled = true;
            }
        }
    }
    return fulfilled;
}


bool ConstraintNotEqual::infer(Game& game) {
    cout << "INVOKING CONSTRAINT != AT (" << p1.row << ", " << p1.col << ") AND AT (" << p2.row << ", " << p2.col  << ")" << endl;

    if(game.at(p1)!=0){
        game.set(p2, negate_sign(game, p1));
        return true;
    } 
    if(game.at(p2)!=0){
        game.set(p1, negate_sign(game, p2));
        return true;
    }
    
    auto suns = 0, moons = 0;
    if(is_vertical(p1, p2)){
        for(int i=0; i<6; ++i)
            switch(game.board[i][p1.col]){
                case 1:
                    ++suns;
                    break;
                case 2:
                    ++moons;
                    break;
            }
        if(suns == 2 || moons == 2){
            auto opposite = moons == 2 ? 1 : 2;
            for(int i=0; i<6; i++){
                auto c = coord(i, p1.col);
                if(game.at(c) == 0 && c != p1 && c != p2){
                    game.set(c, opposite);                    
                }
            }
        }
    } else {
        for(int i=0; i<6; ++i)
            switch(game.board[p1.row][i]){
                case 1:
                    ++suns;
                    break;
                case 2:
                    ++moons;
                    break;
            }
        if(suns == 2 || moons == 2){
            auto opposite = moons == 2 ? 1 : 2;
            for(int i=0; i<6; i++){
                auto c = coord(p1.row, i);
                if(game.at(c) == 0 && c != p1 && c != p2){
                    game.set(c, opposite);
                }
            }
        }
    }
    return false;
}


void Tango::play(Game& game, vector<Constraint*>& constraints) {
    
    vector<Constraint*> display_constraints = constraints;

    for(size_t i = 0; i < 6; ++i) {
        constraints.push_back(new ConstraintMax3('r', i));
        constraints.push_back(new ConstraintMax3('c', i));
    }


    for(size_t i = 0; i < 6; ++i){
        constraints.push_back(new ConstraintA__A(coord(i, 0), coord(i, 5)));
        constraints.push_back(new ConstraintA__A(coord(0, i), coord(5, i)));
    }


    for(size_t row = 0; row < 6; ++row) {
        for(size_t col = 0; col < 5; ++col) {
            constraints.push_back(new Constraint_AA_(coord(row, col), coord(row, 1+col)));
        }
    }
    for(size_t col = 0; col < 6; ++col){
        for(size_t row = 0; row < 5; ++row){
            constraints.push_back(new Constraint_AA_(coord(row, col), coord(1+row, col)));
        }
    }
    

    for(size_t row = 0; row < 6; ++row) {
        for(size_t col = 0; col < 4; ++col) {
            constraints.push_back(new ConstraintA_A(coord(row, col), coord(row, 2+col)));
        }
    }
    for(size_t col = 0; col < 6; ++col){
        for(size_t row = 0; row < 4; ++row){
            constraints.push_back(new ConstraintA_A(coord(row, col), coord(2+row, col)));
        }
    }

    for(size_t i = 0; i < 6; ++i){
        constraints.push_back(new ConstraintAA__(coord(i, 0), coord(i, 1)));
        constraints.push_back(new ConstraintAA__(coord(i, 4), coord(i, 5)));
        constraints.push_back(new ConstraintAA__(coord(0, i), coord(1, i)));
        constraints.push_back(new ConstraintAA__(coord(4, i), coord(5, i)));
    }
        
    cout << "game: " << endl;
    print_game(game, display_constraints);

    vector<Constraint*> fulfilled_constraints;
    
    while(true) {

        vector<Constraint*> new_constraints;
        for(const auto& c : constraints) {
            auto fulfilled = c->infer(game);
            if(!fulfilled) {
                new_constraints.push_back(c);
            } else {
                fulfilled_constraints.push_back(c);
            }
        }

        if(new_constraints.empty()) {
            cout << "solved:" << endl;
            print_game(game, display_constraints);
            cout << "==================================" << endl;
            break;
        }

        // shouldn't stuck if all possible deduction constraints are exhausted, but just in case.
        if(new_constraints.size() == constraints.size()) {  
            cout << "stuck at:\n" << endl;  
            print_game(game, display_constraints);
            break;
        } 
        constraints = new_constraints;
    }

    for(const auto& c : fulfilled_constraints) 
        delete c;
}