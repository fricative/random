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
    bool operator!=(const coord& other) const {
        return !(*this == other);
    }
};

struct Game{
    array<array<int, 6>, 6> board;
    Game(array<array<int, 6>, 6> b): board{b}{};

    int at(struct coord c) {return board[c.row][c.col];};
    
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

inline int negate_sign(Game& game, coord pos) {
    if(game.at(pos)==0) 
        throw std::runtime_error("Cell is empty");
    return game.at(pos) == 1 ? 2 : 1;
}

inline bool is_vertical(const coord& p1, const coord& p2) noexcept {
    return p1.col == p2.col;
}

inline coord up(const coord& p, size_t offset=1) noexcept {
    return {p.row - offset, p.col};
}

inline coord down(const coord& p, size_t offset=1) noexcept {
    return {p.row + offset, p.col};
}

inline coord left(const coord& p, size_t offset=1) noexcept {
    return {p.row, p.col - offset};
}

inline coord right(const coord& p, size_t offset=1) noexcept {
    return {p.row, p.col + offset};
}

class Constraint{
    public:
        virtual bool infer(Game& game) = 0; // Make infer() a pure virtual function
        virtual ~Constraint() = default; // Ensure a virtual destructor
};   

class ConstraintEqual: public Constraint{
public:
    ConstraintEqual(coord p1, coord p2): p1{p1}, p2{p2}{
        assert(p1.row == p2.row - 1 || p1.col == p2.col - 1);
    };

    bool infer(Game& game) override;

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

    bool infer(Game& game) override; 

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

inline void print_game(const Game& g, vector<Constraint*> constraints){
    for(int i=0; i<11; i++){
        for(int j=0; j<11; j++){
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
}

class Tango {
public:
    void play(Game& game, vector<Constraint*>& constraints);
};