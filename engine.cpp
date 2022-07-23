

    #include <iostream>
    #include <fstream>
    #include <string>
    #include <algorithm>
    #include "./match.hpp"
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

        uint8_t engine_color = mWHITE;

        import_minutes(match, "morphy");
        
        if(match.m_board.getfield(match.m_board.m_wKg_x, match.m_board.m_wKg_y) != mWKG || match.m_board.getfield(match.m_board.m_bKg_x, match.m_board.m_bKg_y) != mBKG){
            cout << "DAMAGE" << endl;
            return 0;
        }
        else{
            cout << "OK" << endl;
        }

        match.m_board.prnt();
        
        //bool touched = match.m_board.is_field_touched(6, 6, mWHITE);
        //cout << touched << endl;
        
        //vector<cPiece> wpieces, bpieces;
        //match.m_board.search_all_dirs_for_touching_pieces(wpieces, bpieces, 6, 6, match.get_last_move(), false);
        //cout << to_string(wpieces.size()) << " " << to_string(bpieces.size()) << endl;

        //bool is_softpin = match.m_board.is_soft_pinned(5, 5);
        //cout << "is_softpin: " << is_softpin << endl;
        
        //bool valid = match.is_move_valid(4, 7, 3, 7, mBLK);
        //cout << "valid: " << valid << endl;

        //test_eval_piece_state(match);
      
        //test_search_all_dirs_for_touching_pieces(match);

        //cMove move(4, 0, 2, 0, mWKG, mBLK, mBLK, 0);
        //cMove move2(4, 0, 6, 0, mWKG, mBLK, mBLK, 0);
        
        //int32_t score = cEvaluator::score_move(match.m_board, move);
        //cout << "score: " << score << endl;
        
        //int32_t score2 = cEvaluator::score_board(match.m_board, 1, 1);
        //cout << "score2: " << score2 << endl;

        //test_eval_field_state(match, 3, 6);

        //test_eval_piece_state(match);
        
        //test_is_hotspot(match);

        play(match, engine_color);

        return 0;

    }

