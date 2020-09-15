
    #include <iostream>
    #include <string>
    #include <list>
    #include "./play.hpp"
    #include "../helper.hpp"
    #include "../compute/calc.hpp"
    #include "../values.hpp"
    #include "../pieces/searchforpiece.hpp"
    #include "../pieces/touch.hpp"

    using namespace std;


    void play(cMatch &match, bool engine_is_white){
        char buffer[6];
        int length = 7;

        cout << "status: " << match.eval_status() << " score: " << match.score << " level: " << match.level <<  " is_endgame: " << match.is_endgame() << " movecnt: " << match.minutes.size() << endl;
        match.board.prnt();

        while(true){
            if((match.next_color() == COLORS["white"] && engine_is_white) || 
               (match.next_color() == COLORS["black"] && engine_is_white == false)){
                   cout << "start calc..." << endl;
                   list<cPrioMove> candidates;
                   calc_move(match, candidates);
                   if(candidates.size() > 0){
                       cPrioMove *priomove = &candidates.front();
                       match.do_move(priomove->src, priomove->dst, priomove->prompiece);
                       cout << priomove->format() << endl;
                       match.board.prnt();
                   }
                   else{
                       cout << "no move found!" << endl;
                   }
            }

            cout << "Input: " << endl;
            cin.getline(buffer, length);
            string input(buffer);

            if(input == "q" || input == "Q"){
                return;
            }
            
            if(input == "u" || input == "U"){
                match.undo_move();
                if((match.next_color() == COLORS["white"] && engine_is_white) || 
                   (match.next_color() == COLORS["black"] && engine_is_white == false)){
                    match.undo_move();
                }
                match.board.prnt();
                continue;
            }

            if(input.size() < 4){
                cout << "q(uit), u(ndo last move), e2e4 | e7e8q" << endl;
                continue;
            }

            if((match.next_color() == COLORS["white"] && engine_is_white == false) ||
               (match.next_color() == COLORS["black"] && engine_is_white)){
                int src = coord_to_index(input.substr(0, 2));
                int dst = coord_to_index(input.substr(2, 2));
                int prompiece = mBLK;
                if(input.size() > 4){
                    prompiece = PIECES[input.substr(4, 3)];
                }
                if(match.board.is_move_valid(src, dst, prompiece, match.minutes)){
                    match.do_move(src, dst, prompiece);
                    match.board.prnt();
                }
                else{
                    cout << "invalid move: " << dec << src << " " << dec << dst << " " << dec << prompiece << endl;
                    continue;
                }
            }
        }
    }
