
    #include <iostream>
    #include <string>
    #include "./generator.hpp"
    #include "./helper.hpp"
    #include "./calc.hpp"
    #include "./match.hpp"
    #include "./move.hpp"
    #include "./values.hpp"
    #include "./bitmaps/mates.hpp"

    using namespace std;


    int main(void){
        cMatch match;
        
        for(uint8_t i = 0; i < 8; ++i){
            match.board.fields[i] = white_backrank_mates[i];
        }
        match.board.prnt();

        for(uint8_t i = 0; i < 8; ++i){
            match.board.fields[i] = black_backrank_mates[i];
        }
        match.board.prnt();

        cout << "********************************" << endl;

        match.import("./data/morphy.txt");

        match.board.prnt();

        cout << "score: " << to_string(match.score) << endl;

        list<cGMove *> rcandidates;
        calc_move(match, rcandidates);

        int input;
        for(list<cGMove *>::iterator it = rcandidates.begin(); it != rcandidates.end(); ++it){
            cout << pos_to_coord((*it)->src) << " " << pos_to_coord((*it)->dst) << " " << PIECES_STR[(*it)->prompiece] << endl;
            cin >> input;
            match.do_move((*it)->src, (*it)->dst, (*it)->prompiece);
            match.board.prnt();
        }

        return 0;
    }
