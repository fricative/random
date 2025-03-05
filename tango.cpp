#include <array>
#include <vector>
#include <cassert>
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;


struct coord{
    size_t row;
    size_t col;
    coord(){}
    coord(size_t r, size_t c) : row(r), col(c) {}
    bool operator==(const coord& other) const {
        return row == other.row && col == other.col;
    }
};

struct Game{
    array<array<int, 6>, 6> board;
    Game(array<array<int, 6>, 6> b): board{b}{};

    int at(struct coord c){return board[c.row][c.col];};
    void set(const coord& c, const int v){
        if(this->at(c) != 0 && this->at(c) != v) 
            throw runtime_error("trying to modify filled cell at " + to_string(c.row) + ", " + to_string(c.col) + " from " + to_string(this->at(c)) + " to " + to_string(v));
        if(this->at(c) == 0)
            cout << "SETTING CELL (" << to_string(c.row) << ", " << to_string(c.col) << ") TO " << v << endl;    
        board[c.row][c.col] = v;
    };
    
    bool solved(){
        for(int row=0; row<6; ++row){
            for(int col=0; col<6; ++col){
                if(board[row][col] == 0){
                    return false;
                }
            }
        }
        return true;
    };
};

class Constraint{
public:
    virtual bool infer(Game& game) = 0; // Make infer() a pure virtual function
    virtual ~Constraint() = default; // Ensure a virtual destructor
};

int negate_sign(Game& g, struct coord p){
    if(g.at(p)==0) 
        throw std::runtime_error("Cell is empty");
    return g.at(p) == 1 ? 2 : 1;
};

bool is_vertical(const struct coord& p1, const struct coord& p2){
    return p1.col == p2.col;
};

struct coord up(const struct coord& p, size_t offset=1){
    return {p.row - offset, p.col};
};

struct coord down(const struct coord& p, size_t offset=1){
    return {p.row + offset, p.col};
};

struct coord left(const struct coord& p, size_t offset=1){
    return {p.row, p.col - offset};
};

struct coord right(const struct coord& p, size_t offset=1){
    return {p.row, p.col + offset};
};

class ConstraintEqual: public Constraint{
public:
    ConstraintEqual(coord p1, coord p2): p1{p1}, p2{p2}{
        assert(p1.row == p2.row - 1 || p1.col == p2.col - 1);
    };

    bool infer(Game& game) override {

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
    };

    char get_symbol() const {
        return '=';
    }

    std::pair<int, int> get_print_loc(){
        unsigned int p1_row = p1.row << 1;
        unsigned int p1_col = p1.col << 1;
        return p1.row == p2.row ? std::make_pair(p1_row, p1_col + 1) : std::make_pair(p1_row + 1, p1_col);
    };
private:
    coord p1;
    coord p2;
};

class ConstraintNotEqual: public Constraint{
public:
    ConstraintNotEqual(coord p1, coord p2): p1{p1}, p2{p2}{};

    bool infer(Game& game) override {

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

    char get_symbol() const {
        return '!';
    }

    std::pair<int, int> get_print_loc(){
        unsigned int p1_row = p1.row << 1;
        unsigned int p1_col = p1.col << 1;
        return p1.row == p2.row ? std::make_pair(p1_row, p1_col + 1) : std::make_pair(p1_row + 1, p1_col);
    };
private:
    coord p1;
    coord p2;
};

class Constraint_AA_: public Constraint{
public:
    Constraint_AA_(struct coord p1, struct coord p2): p1{p1}, p2{p2}{
        if(p1.row == p2.row)
            assert(p1.col == p2.col - 1);
        else if(p1.col == p2.col)
            assert(p1.row == p2.row - 1);
    };

    bool infer(Game& game) override {

        cout << "INVOKING CONSTRAINT _AA_ AT (" << p1.row << ", " << p1.col << ") AND AT (" << p2.row << ", " << p2.col  << ")" << endl;
        if (game.at(p1)==0 || game.at(p2)==0) 
            return false;

        if (game.at(p1)!=game.at(p2))
            return true;

        if(!is_vertical(p1, p2)){
            if (p1.col > 0 && game.at(left(p1))==0)
                game.set(left(p1), negate_sign(game, p1));
            if (p2.col < 5 && game.at(right(p2))==0)
                game.set(right(p2), negate_sign(game, p2));
        } else {
            if (p1.row > 0 && game.at(up(p1))==0)
                game.set(up(p1), negate_sign(game, p1));
            if (p2.row < 5 && game.at(down(p2))==0)
                game.set(down(p2), negate_sign(game, p2));
        }
        return true;
    };
private:
    coord p1;
    coord p2;
};

class ConstraintA_A: public Constraint{
public:
    ConstraintA_A(struct coord p1, struct coord p2): p1{p1}, p2{p2}{
        if(p1.row == p2.row)
            assert(p1.col == p2.col - 2);
        else if(p1.col == p2.col)
            assert(p1.row == p2.row - 2);
    };

    bool infer(Game& game) override {
        if (game.at(p1)==0 || game.at(p2)==0)
            return false;
        
        if (game.at(p1)!=game.at(p2))
            return true;

        cout << "INVOKING CONSTRAINT A_A AT (" << p1.row << ", " << p1.col << ") AND AT (" << p2.row << ", " << p2.col  << ")" << endl;
        
        if(!is_vertical(p1, p2)) 
            game.set(right(p1), negate_sign(game, p1));
        else 
            game.set(down(p1), negate_sign(game, p1));
        return true;
    };
private:
    struct coord p1;
    struct coord p2;
};

class ConstraintA__A: public Constraint{
public:
    ConstraintA__A(struct coord p1, struct coord p2): p1{p1}, p2{p2}{
        if(p1.row == p2.row)
            assert(p1.col == 0 && p2.col == 5);
        else if(p1.col == p2.col)
            assert(p1.row == 0 && p2.row == 5);
    };
    
    bool infer(Game& game) override {
        if(game.at(p1)==0 || game.at(p2)==0)
            return false;
        if(game.at(p1)!=game.at(p2))
            return true;
        
        cout << "INVOKING CONSTRAINT A__A AT (" << p1.row << ", " << p1.col << ") AND AT (" << p2.row << ", " << p2.col  << ")" << endl;

        auto opposite = negate_sign(game, p1);
        if(p1.row == p2.row){
            game.set(right(p1), opposite);
            game.set(left(p2), opposite);
        } else {
            game.set(down(p1), opposite);
            game.set(up(p2), opposite);
        }
        return true;
    };
private:
    struct coord p1;
    struct coord p2;
};

class ConstraintAA__: public Constraint{
public:
    ConstraintAA__(struct coord p1, struct coord p2): p1{p1}, p2{p2}{
        if(p1.row == p2.row)
            assert(p1.col + 1 == p2.col && (p1.col == 0 || p2.col == 5));
        if(p1.col == p2.col)
            assert(p1.row + 1 == p2.row && (p1.row == 0 || p2.row == 5));
    }

    bool infer(Game& game) override {
        struct coord opposite;
        if(is_vertical(p1, p2)){
            if(p1.row == 0){
                opposite = coord(5, p1.col);
            } else {
                opposite = coord(0, p1.col);
            }
        } else {
            if(p1.col == 0){
                opposite = coord(p1.row, 5);
            } else {
                opposite = coord(p1.row, 0);
            }
        }

        auto triplet = array<coord, 3>{p1, p2, opposite};
        auto fill_count = std::count_if(triplet.begin(), triplet.end(), 
            [&](const auto& c){return game.at(c)!=0;});
        
        if(fill_count == 3) 
            return true;
        
        if(fill_count < 2)
            return false;

        array<coord, 2> filled;
        size_t fill_idx = 0;
        for(auto& c: triplet){
            if(game.at(c) != 0){
                filled[fill_idx] = c;
                fill_idx++;
            }
        }
        if(game.at(filled[0]) != game.at(filled[1]))
            return true;

        cout << "INVOKING CONSTRAINT AA__ AT (" << p1.row << ", " << p1.col << ") AND AT (" << p2.row << ", " << p2.col  << ")" << endl;
        for(auto& c: triplet){
            if(game.at(c) == 0){
                game.set(c, negate_sign(game, filled[0]));
            }
        }
        return true;
    }

private:
    coord p1;
    coord p2;
};

class ConstraintMax3: public Constraint{
public:
    ConstraintMax3(char direction, size_t index): direction{direction}, index{index}{};

    bool infer(Game& game) override {
        size_t suns = 0, moons = 0;
        if(direction == 'r'){
            for(size_t i=0; i<6; i++){
                if(game.board[index][i]==1)
                    suns++;
                if(game.board[index][i]==2)
                    moons++;
            }
            if(suns == 3){
                for(size_t i=0; i<6; i++){
                    if(game.board[index][i]==0)
                        game.board[index][i] = 2;
                }
                cout << "INVOKING CONSTRAINT MAX3 AT ROW " << index << endl;
                return true;
            } else if (moons == 3){
                for(size_t i=0; i<6; i++){
                    if(game.board[index][i]==0)
                        game.board[index][i] = 1;
                }
                cout << "INVOKING CONSTRAINT MAX3 AT ROW " << index << endl;
                return true;
            }
        } else if(direction == 'c'){
            for(size_t i=0; i<6; i++){
                if(game.board[i][index]==1)
                    suns++;
                if(game.board[i][index]==2)
                    moons++;
            }
            if(suns == 3){
                for(size_t i=0; i<6; i++){
                    if(game.board[i][index]==0)
                        game.board[i][index] = 2;
                }
                cout << "INVOKING CONSTRAINT MAX3 AT COL " << index << endl;
                return true;
            } else if (moons == 3){
                for(size_t i=0; i<6; i++){
                    if(game.board[i][index]==0)
                        game.board[i][index] = 1;
                }
                cout << "INVOKING CONSTRAINT MAX3 AT COL " << index << endl;
                return true;
            }
        }
        return false;
    };
private:
    char direction;
    size_t index;
};

void print_game(const Game& g, vector<Constraint*> constraints){
    for(size_t i=0; i<11; i++){
        for(size_t j=0; j<11; j++){
            auto showed = false;
            for(const auto& c: constraints){
                if(auto ce = dynamic_cast<ConstraintEqual*>(c)){
                    auto loc = ce->get_print_loc();
                    if(loc.first == i && loc.second == j){
                        cout << ce->get_symbol();
                        showed = true;
                        break;
                    }
                } 
                if(auto ce = dynamic_cast<ConstraintNotEqual*>(c)){
                    auto loc = ce->get_print_loc();
                    if(loc.first == i && loc.second == j){
                        cout << ce->get_symbol();
                        showed = true;
                        break;
                    }
                }
            }
            if (showed)
                continue;
            else{
                if(i % 2 == 1 || j % 2 == 1){
                    cout << " ";
                } else {
                    cout << g.board[i/2][j/2];
                }
            }
        }
        cout << endl;
    }
    cout << endl;
};


class Tango {
public:
    Tango() {}
    void play(Game& game, vector<Constraint*>& constraints) {

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
    
    ~Tango() {
    }
};


int main() {
    auto tango = Tango{};

    auto rounds = {
        make_pair(
            Game(
                std::array{
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,1,1,0,0},
                    std::array{0,0,1,2,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                }
            ),
    
            vector<Constraint*>{        
                new ConstraintEqual(coord(0,2), coord(1,2)),
                new ConstraintEqual(coord(2,0), coord(2,1)),
                new ConstraintEqual(coord(2,4), coord(3,4)),

                new ConstraintNotEqual(coord(1,2), coord(1,3)),
                new ConstraintNotEqual(coord(2,1), coord(3,1)),
                new ConstraintNotEqual(coord(3,4), coord(3,5)),
                new ConstraintNotEqual(coord(4,2), coord(4,3)),
                new ConstraintNotEqual(coord(4,3), coord(5,3))
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{0,0,0,0,0,2},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,2,0,0,0},
                    std::array{0,0,0,2,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{2,0,0,0,0,0},
                }
            ),    
            vector<Constraint*>{        
                new ConstraintEqual(coord(0,4), coord(1,4)),
                new ConstraintEqual(coord(4,1), coord(5,1)),
                new ConstraintEqual(coord(4,5), coord(5,5)),
                new ConstraintEqual(coord(5,3), coord(5,4)),

                new ConstraintNotEqual(coord(0,0), coord(0,1)),
                new ConstraintNotEqual(coord(0,1), coord(0,2)),
                new ConstraintNotEqual(coord(0,0), coord(1,0)),
                new ConstraintNotEqual(coord(1,0), coord(2,0)),
                new ConstraintNotEqual(coord(3,5), coord(4,5)),
                new ConstraintNotEqual(coord(4,0), coord(4,1)),
                new ConstraintNotEqual(coord(1,4), coord(1,5)),
                new ConstraintNotEqual(coord(5,4), coord(5,5)),
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{0,1,0,0,1,0},
                    std::array{0,0,0,1,0,0},
                    std::array{1,0,0,0,0,0},
                    std::array{0,0,0,0,0,1},
                    std::array{0,0,1,0,0,0},
                    std::array{0,1,0,0,1,0},
                }
            ),
    
            vector<Constraint*>{        
                new ConstraintEqual(coord(1,2), coord(2,2)),
                new ConstraintEqual(coord(3,4), coord(4,4)),

                new ConstraintNotEqual(coord(1,1), coord(2,1)),
                new ConstraintNotEqual(coord(3,3), coord(4,3)),
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{0,0,0,0,0,0},
                    std::array{0,2,2,1,2,0},
                    std::array{0,0,1,0,0,0},
                    std::array{0,0,2,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                }
            ),
            
            vector<Constraint*> {        
                new ConstraintEqual(coord(2,3), coord(3,3)),
                new ConstraintEqual(coord(4,1), coord(4,2)),
                new ConstraintEqual(coord(4,3), coord(4,4)),
                new ConstraintNotEqual(coord(3,3), coord(4,3)),
                new ConstraintNotEqual(coord(4,2), coord(4,3))
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{0,0,0,0,0,0},
                    std::array{0,2,2,1,2,0},
                    std::array{0,0,1,0,0,0},
                    std::array{0,0,2,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                }
            ),  
            vector<Constraint*>{        
                new ConstraintEqual(coord(2,3), coord(3,3)),
                new ConstraintEqual(coord(4,1), coord(4,2)),
                new ConstraintEqual(coord(4,3), coord(4,4)),
                new ConstraintNotEqual(coord(3,3), coord(4,3)),
                new ConstraintNotEqual(coord(4,2), coord(4,3))
            }
        ),
        make_pair(
            Game(
                    std::array{
                        std::array{0,0,0,0,0,0},
                        std::array{0,1,0,0,0,0},
                        std::array{2,1,0,0,0,0},
                        std::array{0,2,0,0,0,0},
                        std::array{1,2,1,0,0,0},
                        std::array{0,0,0,0,0,0},
                    }
                ),
            vector<Constraint*>  {        
                new ConstraintEqual(coord(2,3), coord(2,4)),
                new ConstraintEqual(coord(2,4), coord(3,4)),
                new ConstraintEqual(coord(4,4), coord(4,5)),
                new ConstraintNotEqual(coord(1,4), coord(2,4)),
                new ConstraintNotEqual(coord(3,4), coord(4,4)),
                new ConstraintNotEqual(coord(4,3), coord(4,4))
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{0,0,0,0,0,0},
                    std::array{0,2,0,1,2,0},
                    std::array{0,1,0,0,0,0},
                    std::array{0,0,0,0,1,0},
                    std::array{0,2,1,0,1,0},
                    std::array{0,0,0,0,0,0},
                }
            ),    
            vector<Constraint*>{        
                new ConstraintEqual(coord(0,2), coord(0,3)),
                new ConstraintEqual(coord(3,5), coord(4,5)),
                new ConstraintEqual(coord(5,0), coord(5,1)),

                new ConstraintNotEqual(coord(0,4), coord(0,5)),
                new ConstraintNotEqual(coord(1,0), coord(2,0)),
                new ConstraintNotEqual(coord(5,2), coord(5,3)),
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{0,0,0,0,0,2},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{1,0,0,0,0,0},
                }
            ),    
            vector<Constraint*>{        
                new ConstraintEqual(coord(2,0), coord(2,1)),
                new ConstraintEqual(coord(4,2), coord(4,3)),
                new ConstraintEqual(coord(5,1), coord(5,2)),

                new ConstraintNotEqual(coord(0,3), coord(0,4)),
                new ConstraintNotEqual(coord(0,3), coord(1,3)),
                new ConstraintNotEqual(coord(1,1), coord(1,2)),
                new ConstraintNotEqual(coord(1,2), coord(1,3)),
                new ConstraintNotEqual(coord(1,5), coord(2,5)),
                new ConstraintNotEqual(coord(2,0), coord(3,0)),
                new ConstraintNotEqual(coord(2,5), coord(3,5)),
                new ConstraintNotEqual(coord(3,4), coord(3,5)),
                new ConstraintNotEqual(coord(3,0), coord(4,0)),
                new ConstraintNotEqual(coord(4,0), coord(5,0)),
                new ConstraintNotEqual(coord(4,2), coord(5,2)),
                new ConstraintNotEqual(coord(4,3), coord(4,4)),
                new ConstraintNotEqual(coord(5,0), coord(5,1)),
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{2,0,1,0,2,0},
                    std::array{0,2,0,1,0,0},
                    std::array{1,0,2,0,0,0},
                    std::array{0,1,0,0,0,0},
                    std::array{2,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                }
            ),    
            vector<Constraint*>{        
                new ConstraintEqual(coord(1,5), coord(2,5)),
                new ConstraintEqual(coord(5,1), coord(5,2)),

                new ConstraintNotEqual(coord(2,4), coord(3,4)),
                new ConstraintNotEqual(coord(3,5), coord(4,5)),
                new ConstraintNotEqual(coord(4,2), coord(4,3)),
                new ConstraintNotEqual(coord(5,3), coord(5,4)),
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{0,2,2,0,0,0},
                    std::array{2,2,0,0,0,0},
                    std::array{2,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                }
            ),    
            vector<Constraint*>{        
                new ConstraintEqual(coord(2,3), coord(2,4)),
                new ConstraintEqual(coord(2,2), coord(3,2)),
                new ConstraintEqual(coord(4,2), coord(5,2)),
                new ConstraintEqual(coord(4,3), coord(4,4)),

                new ConstraintNotEqual(coord(2,2), coord(2,3)),
                new ConstraintNotEqual(coord(2,4), coord(2,5)),
                new ConstraintNotEqual(coord(3,2), coord(4,2)),
                new ConstraintNotEqual(coord(4,4), coord(4,5)),
                new ConstraintNotEqual(coord(4,5), coord(5,5)),
                new ConstraintNotEqual(coord(5,2), coord(5,3)),
                new ConstraintNotEqual(coord(5,3), coord(5,4)),
                new ConstraintNotEqual(coord(5,4), coord(5,5)),
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{2,1,0,0,0,0},
                    std::array{0,2,0,0,0,0},
                    std::array{0,0,0,2,1,1},
                    std::array{0,0,0,1,0,2},
                    std::array{0,0,0,2,1,1},
                    std::array{0,0,0,0,0,0},
                }
            ),    
            vector<Constraint*>{        
                new ConstraintEqual(coord(0,4), coord(1,4)),

                new ConstraintNotEqual(coord(0,3), coord(0,4)),
                new ConstraintNotEqual(coord(2,0), coord(2,1)),
                new ConstraintNotEqual(coord(2,1), coord(2,2)),
                new ConstraintNotEqual(coord(2,0), coord(3,0)),
                new ConstraintNotEqual(coord(2,2), coord(3,2)),
                new ConstraintNotEqual(coord(3,0), coord(4,0)),
                new ConstraintNotEqual(coord(3,2), coord(4,2)),
                new ConstraintNotEqual(coord(4,0), coord(4,1)),   
                new ConstraintNotEqual(coord(4,1), coord(4,2)),   
            }
        ),
        make_pair(
            Game(
                std::array{
                    std::array{0,0,0,0,0,0},
                    std::array{0,2,1,1,2,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                    std::array{0,0,0,0,0,0},
                }
            ),    
            vector<Constraint*>{        
                new ConstraintEqual(coord(4,1), coord(4,2)),
                new ConstraintEqual(coord(4,3), coord(4,4)),
                new ConstraintEqual(coord(5,2), coord(5,3)),
                new ConstraintEqual(coord(2,1), coord(3,1)),

                new ConstraintNotEqual(coord(2,2), coord(3,2)),
                new ConstraintNotEqual(coord(2,3), coord(3,3)),
                new ConstraintNotEqual(coord(2,4), coord(3,4)),
            }
        ),
    };
    
    for(auto round: rounds) 
        tango.play(round.first, round.second);
    
    return 0;
}

 
