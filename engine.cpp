

    #include <iostream>
    #include <fstream>
    #include <string>
    #include <algorithm>
    #include "./match.hpp"
    #include "./daemon.hpp"
    #include "./util/test.hpp"    


    using namespace std;


  void prnt_minutes(const cMatch &match){

      for(const cMove &move : match.m_minutes){

          cout << move.format(false) << endl;

      }

  }


  void prnt_status(const cMatch &match, const uint8_t status){
      if(status == cMatch::STATUS_WINNER_WHITE){
          prnt_minutes(match);
          cout << "winner white!" << endl;
      }
      else if(status == cMatch::STATUS_WINNER_BLACK){
          prnt_minutes(match);
          cout << "winner black!" << endl;
      }
      else if(status == cMatch::STATUS_DRAW){
          prnt_minutes(match);
          cout << "draw!" << endl;
      }
  }


  void play(cMatch &match, uint8_t engine_color){

        char buffer[31];
        int length = 32;

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

                match.calc_move(rscore, rmoves);

                if(rmoves.size() > 0){
                   cMove move = rmoves.front();

                   match.do_move(move);

                   cout << move.format(false) << endl;

                   cout << "m_score: " << match.m_score << endl;

                   match.m_board.prnt();
                }
                else{
                    uint8_t status = match.eval_status();

                    cout << "status " << status;

                    cout << "game over - congratulation!" << endl;

                    return;
                }
            }
            else{
                uint8_t status = match.eval_status();

                if(status != cMatch::STATUS_OPEN){
                    cout << "status " << status;
                }

                cout << "\n>";
                cin.getline(buffer, length);
                string input(buffer);
                input.erase(remove(input.begin(), input.end(), ' '), input.end());

                if(input == "h" || input == "H"){ 
                    cout << "\nmove formats: e2e4 | e7e8q | 0-0 | 0-0-0";
                    cout << "\ncommands: u(ndo) | w(hite engine) | b(lack engine) | pb(print board) | pm(print minutes)";
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
                    prnt_minutes(match);

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

                    do_usr_move(match, src_x, src_y, dst_x, dst_y, prompiece);

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

        uint8_t engine_color = mBLANK;

        import_minutes(match, "morphy");
        
        if(match.m_board.getfield(match.m_board.m_wKg_x, match.m_board.m_wKg_y) != mWKG || match.m_board.getfield(match.m_board.m_bKg_x, match.m_board.m_bKg_y) != mBKG){
            cout << "DAMAGE" << endl;
            return 0;
        }
        else{
            cout << "OK" << endl;
        }

        match.m_board.prnt();
        
        cout << "score: " << to_string(match.m_score) << endl;

        //test_is_continue_sequence(match);
        
        //uint8_t src_x, src_y, dst_x, dst_y;
        //cMove::coord_to_indices(src_x, src_y, "h1");
        //cMove::coord_to_indices(dst_x, dst_y, "d1");
        //cMove move(src_x, src_y, dst_x, dst_y, mWRK, mBKN, mBLK, cMove::P_MEDIUM);

        //test_does_move_sac_for_supply(match, move);
        
        //test_does_move_touch_soft_pinned(match, move);

        play(match, engine_color);

        return 0;

    }
