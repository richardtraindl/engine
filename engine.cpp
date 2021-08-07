

    #include <iostream>
    #include <fstream>
    #include <string>
    #include <algorithm>
    #include "./match.hpp"
    #include "./move.hpp"
    #include "./endgame.hpp"


    using namespace std;


    void prnt_status(cMatch &match, uint8_t status){
        if(status == cMatch::STATUS_WINNER_WHITE){
            match.prnt_minutes();
            cout << "winner white!" << endl;
        }
        else if(status == cMatch::STATUS_WINNER_BLACK){
            match.prnt_minutes();
            cout << "winner black!" << endl;
        }
        else if(status == cMatch::STATUS_DRAW){
            match.prnt_minutes();
            cout << "draw!" << endl;
        }
    }


    bool import_minutes(cMatch &match, string path_and_filename){

        match.reset();

        ifstream file(path_and_filename);

        string content;
        
        uint8_t src_x, src_y, dst_x, dst_y;

        uint8_t prompiece = mBLK;

        while(file >> content) {

            cMove::coord_to_indices(src_x, src_y, content.substr(0, 2));

            cMove::coord_to_indices(dst_x, dst_y, content.substr(2, 2));

            if(match.is_move_valid(src_x, src_y, dst_x, dst_y, prompiece)){
                match.do_usr_move(src_x, src_y, dst_x, dst_y, prompiece);
            }
            else{
                cout << "import error!" << endl;
                return false;
            }
        }

        return true;

    }


    bool import_board(cMatch &match, string path_and_filename){

        match.reset();

        ifstream file(path_and_filename);

        string content;

        uint8_t y = 7;

        while(file >> content) {

            if(content.length() == 32){

                for(uint8_t x = 0; x < 8; ++x){

                    uint8_t piece = PIECES[content.substr((x * 4), 3)];

                    match.m_board.setfield(x, y, piece);

                }

                --y;

            }
            else{
                cout << "import error!" << endl;
                return false;
            }
        }

        // check kings and set vars
        bool wkg = false;
        bool bkg = false;

        for(uint8_t y = 0; y < 8; ++y){

            for(uint8_t x = 0; x < 8; ++x){

                uint8_t piece = match.m_board.getfield(x, y);

                if(piece == mWKG){
                    if(wkg){ 
                        cout << "import error: wkg!" << endl;
                        return false;
                    }
                    else{
                        wkg = true;
                        match.m_board.m_wKg_x = x;
                        match.m_board.m_wKg_y = y;
                    }
                }
                else if(piece == mBKG){
                    if(bkg){ 
                        cout << "import error: bkg!" << endl;
                        return false;
                    }
                    else{
                        bkg = true;
                        match.m_board.m_bKg_x = x;
                        match.m_board.m_bKg_y = y;
                    }
                }
            }
        }
                    
        if(wkg && bkg){
            match.m_board.m_wKg_has_moved_at = 1;
            match.m_board.m_bKg_has_moved_at = 1;
            match.m_board.m_wRkA_has_moved_at = 1;
            match.m_board.m_wRkH_has_moved_at = 1;
            match.m_board.m_bRkA_has_moved_at = 1;
            match.m_board.m_bRkH_has_moved_at = 1;

            return true;
        }
        else{
            cout << "import error: kg!" << endl;
            return false;
        }

    }


    void play(cMatch &match, uint8_t maxdepth, uint8_t engine_color){

        char buffer[12];
        int length = 13;

        uint8_t src_x, src_y, dst_x, dst_y;

        uint8_t prompiece = mBLK;

        cout << " movecnt: " << match.m_minutes.size() << endl;
        match.m_board.prnt();

        while(true){
            if((match.next_color() == mWHITE && engine_color == mWHITE) || 
               (match.next_color() == mBLACK && engine_color == mBLACK)){

                cout << "start calc..." << endl;

                vector<cMove> rmoves;

                int32_t rscore;

                match.calc_move(rscore, rmoves, maxdepth);

                if(rmoves.size() > 0){
                   cMove move = rmoves.front();
                   match.do_move(move);
                   cout << move.format(false) << endl;
                   match.m_board.prnt();
                }
                else{
                    uint8_t status = match.eval_status();
                    prnt_status(match, status);
                    cout << "game over - congratulation!" << endl;
                    return;
                }
            }
            else{
                uint8_t status = match.eval_status();
                if(status != cMatch::STATUS_OPEN){
                    prnt_status(match, status);
                    //return;
                }

                cout << "\n>";
                cin.getline(buffer, length);
                string input(buffer);
                input.erase(remove(input.begin(), input.end(), ' '), input.end());

                if(input == "h" || input == "H"){ 
                    cout << "\nmove formats: e2e4 | e7e8q | 0-0 | 0-0-0";
                    cout << "\ncommands: quit) | u(ndo) | w(hite engine) | b(lack engine) | pb(print board) | pm(print minutes) | im(import moves) | ib(import board)";
                    continue;
                }

                if(input == "quit" || input == "QUIT"){ 
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
                    match.m_board.prnt();
                    continue;
                }

                if(input == "pb" || input == "PB"){
                    match.m_board.prnt();
                    continue;
                }

                if(input == "pm" || input == "PM"){
                    match.prnt_minutes();
                    continue;
                }

                if(input.substr(0, 2) == "im" || input.substr(0, 2) == "IM"){
                    if(input.length() < 5){
                        cout << "format for import moves: 'im 001'" << endl;
                        continue;
                    }

                    string path_and_filename = "./matches/game" + input.substr(2) + ".txt";
                    if(import_minutes(match, path_and_filename)){

                        match.prnt_minutes();

                        match.m_board.prnt();
                    }

                    continue;

                }

                if(input == "w" || input == "W"){
                    engine_color = mWHITE;
                    continue;
                }

                if(input == "b" || input == "B"){
                    engine_color = mBLACK;
                    continue;
                }

                if(input.substr(0, 2) == "ib" || input.substr(0, 2) == "IB"){
                    if(input.length() < 5){
                        cout << "format for import board: 'ib 001'" << endl;
                        continue;
                    }

                    string path_and_filename = "./matches/board" + input.substr(2) + ".txt";
                    if(import_board(match, path_and_filename)){
                        match.m_board.prnt();
                    }

                    continue;

                }

                if(input.compare("test") == 0){
                    cout << "test start" << endl;
                    cMove move(5, 7, 2, 4, mBBP, mBLK, mBLK, 100);
                    //match.eval_prio(move);
                    bool flag = match.is_three_times_repetition(move);
                    //bool flag = match.does_move_touch_weak_piece(move);
                    cout << to_string(flag) << endl;
                    cout << "test end" << endl;
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
                    if(input.length() > 0){
                        cout << "invalid input" << endl;
                        continue;
                    }
                }

                if(match.is_move_valid(src_x, src_y, dst_x, dst_y, prompiece)){

                    match.do_usr_move(src_x, src_y, dst_x, dst_y, prompiece);

                    match.m_board.prnt();
                }
                else{
                    cout << "invalid move: " << input << endl;
                }
            }
        }
    }


    int main(void){
        cMatch match;

        uint8_t maxdepth = 5;

        uint8_t engine_color = mBLANK;
        
        play(match, maxdepth, engine_color);

        return 0;
    }
