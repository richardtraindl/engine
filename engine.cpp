

    #include <iostream>
    #include <fstream>
    #include <string>
    #include "./match.hpp"
    #include "./move.hpp"


    using namespace std;


    void play(cMatch &match, uint8_t maxdepth, uint8_t engine_color, bool with_threads){

        char buffer[6];
        int length = 7;

        uint8_t src_x, src_y, dst_x, dst_y;
        uint8_t prompiece = mBLK;

        cout << " movecnt: " << match.minutes.size() << endl;
        match.board.prnt();

        while(true){
            if((match.next_color() == mWHITE && engine_color == mWHITE) || 
               (match.next_color() == mBLACK && engine_color == mBLACK)){
                   cout << "start calc..." << endl;
                   vector<cMove> candidates;
                   int32_t calc_score;
                   match.calc_move(calc_score, candidates, maxdepth, with_threads);

                   if(candidates.size() > 0){
                       cMove move = candidates.front();
                       match.do_move(move);
                       cout << move.format() << endl;
                       match.board.prnt();
                   }
                   else{
                        cout << "game over - congratulation!" << endl;
                        return;
                   }
            }
            else{
                uint8_t status = match.eval_status();
                    
                if(status == match.STATUS_WINNER_WHITE){
                    match.prnt_minutes();
                    cout << "winner white" << " --- good bye!\n" << endl;
                    return;
                }
                else if(status == match.STATUS_WINNER_BLACK){
                    match.prnt_minutes();
                    cout << "winner black" << " --- good bye!\n" << endl;
                    return;
                }
                else if(status == match.STATUS_DRAW){
                    match.prnt_minutes();
                    cout << "draw" << " --- good bye!\n" << endl;
                    return;
                }

                cout << "\nyour input please... q(uit) | u(ndo) r(ead) | e2e4 | e7e8q | 0-0: ";
                cin.getline(buffer, length);
                string input(buffer);

                if(input == "q" || input == "Q"){ 
                    cout << "good bye!" << endl; 
                    return; 
                }

                if(input == "u" || input == "U"){
                    if(engine_color == mWHITE || engine_color == mBLACK){
                        match.undo_move(); // undo last engine move
                        match.undo_move(); // undo last human move
                    }
                    else{
                        match.undo_move(); // undo last human move
                    }
                    match.board.prnt();
                    continue;
                }

                if(input == "p" || input == "P"){
                    match.prnt_minutes();
                    continue;
                }

                if(input == "r" || input == "R"){
                    match.reset();

                    ifstream file("game004.txt");
                    string content;

                    while(file >> content) {
                        cout << content << ' ';
                        cMove::coord_to_indices(src_x, src_y, content.substr(0, 2));
                        cMove::coord_to_indices(dst_x, dst_y, content.substr(2, 2));
                        if(match.is_move_valid(src_x, src_y, dst_x, dst_y, prompiece)){
                            match.do_usr_move(src_x, src_y, dst_x, dst_y, prompiece);
                        }
                        else{
                            cout << "wrong import move!" << endl;
                        }
                    }
                    match.prnt_minutes();
                    match.board.prnt();
                    continue;
                }

                if(input.size() == 3 && input.compare(0, 3, "0-0") == 0){
                    src_x = 4;
                    dst_x = 6;
                    if(match.next_color() == mWHITE){
                        src_y = 0;
                        dst_y = 0;
                    }
                    else{
                        src_y = 7;
                        dst_y = 7;
                    }
                }
                else if(input.size() == 5 && input.compare(0, 5, "0-0-0") == 0){
                    src_x = 4;
                    dst_x = 2;
                    if(match.next_color() == mWHITE){
                        src_y = 0;
                        dst_y = 0;
                    }
                    else{
                        src_y = 7;
                        dst_y = 7;
                    }
                }
                else if(input.size() == 4 || input.size() == 5){
                    cMove::coord_to_indices(src_x, src_y, input.substr(0, 2));
                    cMove::coord_to_indices(dst_x, dst_y, input.substr(2, 2));

                    if(input.size() > 4){
                        if(match.next_color() == mWHITE){
                            if(input.compare(4, 1, "q") == 0){ prompiece = mWQU; }
                            else if(input.compare(4, 1, "r") == 0){ prompiece = mWRK; }
                            else if(input.compare(4, 1, "b") == 0){ prompiece = mWBP; }
                            else if(input.compare(4, 1, "k") == 0){ prompiece = mWKN; }
                        }
                        else{
                            if(input.compare(4, 1, "q") == 0){ prompiece = mBQU; }
                            else if(input.compare(4, 1, "r") == 0){ prompiece = mBRK; }
                            else if(input.compare(4, 1, "b") == 0){ prompiece = mBBP; }
                            else if(input.compare(4, 1, "k") == 0){ prompiece = mBKN; }
                        }
                    }
                }
                else{
                    cout << "invalid move: " << input << endl;
                    continue;
                }

                if(match.is_move_valid(src_x, src_y, dst_x, dst_y, prompiece)){
                    match.do_usr_move(src_x, src_y, dst_x, dst_y, prompiece);
                    match.board.prnt();
                }
            }
        }
    }


    int main(void){
        cMatch match;
        
        /*uint8_t src_x, src_y, dst_x, dst_y;

        cMove::coord_to_indices(src_x, src_y, "c2");
        cMove::coord_to_indices(dst_x, dst_y, "c4");
        match.do_usr_move(src_x, src_y, dst_x, dst_y, mBLK);

        cMove::coord_to_indices(src_x, src_y, "c7");
        cMove::coord_to_indices(dst_x, dst_y, "c5");
        match.do_usr_move(src_x, src_y, dst_x, dst_y, mBLK);

        cout << "eval_board: " << to_string(match.eval_board()) << endl; */

        uint8_t maxdepth = 5;

        uint8_t engine_color = mBLACK;

        bool with_threads = true;

        play(match, maxdepth, engine_color, with_threads); 

        return 0;
    }
