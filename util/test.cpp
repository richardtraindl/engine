


    #include <iostream>
    #include <fstream>
    #include <string>
    #include <vector>
    #include "../evaluator.hpp"
    #include "../generator.hpp"
    #include "../generator2.hpp"
    #include "../match.hpp"
    #include "../daemon.hpp"
    #include "../move.hpp"
    #include "./test.hpp"



    //*****************************************
    void do_usr_move(cMatch &match, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece){

        uint8_t srcpiece = match.m_board.getfield(src_x, src_y);

        uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

        cMove move(src_x, src_y, dst_x, dst_y, srcpiece, dstpiece, prompiece, cMove::P_MEDIUM);

        match.do_move(move);

    }
    //*****************************************



    //*****************************************
    bool import_minutes(cMatch &match, string filename){

        string path_and_filename = "./matches/" + filename + ".txt";

        ifstream file(path_and_filename);

        if(file.fail()){
            cout << "file error!" << endl;
            return false;
        }

        string content;

        uint8_t src_x, src_y, dst_x, dst_y;
        uint8_t prompiece = mBLK;

        while(file >> content) {

            cMove::coord_to_indices(src_x, src_y, content.substr(0, 2));

            cMove::coord_to_indices(dst_x, dst_y, content.substr(2, 2));
            
            if(match.is_move_valid(src_x, src_y, dst_x, dst_y, prompiece)){
                do_usr_move(match, src_x, src_y, dst_x, dst_y, prompiece);
            }
            else{
                cout << "import error!" << content << endl;
                return false;
            }

        }

        return true;

    }


  void test_eval_piece_state(cMatch &match){

      for(int8_t y = 7; y >= 0; --y){

          for(uint8_t x = 0; x <= 7; ++x){

              uint8_t piece = match.m_board.getfield(x, (uint8_t)y);

              if(piece == mBLK){
                  cout << setw(5) << setfill(' ') << "0";
              }
              else{
                  int8_t piece_state;

                  piece_state = cEvaluator::eval_piece_state(match, piece, x, y);

                  cout << setw(5) << setfill(' ') <<  to_string(piece_state);
              }

          }
          cout << endl;
          cout << endl;

      }

  }


  void test_eval_field_state(cMatch &match, const uint8_t src_x, const uint8_t src_y){

      uint8_t field_state = cEvaluator::eval_field_state(match, mBLK, src_x, src_y);

      switch(field_state){
          case mF_CLEAR: cout << "mF_CLEAR" << endl; break;

          case mF_HAZY: cout << "mF_HAZY" << endl; break;

          case mF_WGT: cout << "mF_WGT" << endl; break;

          case mF_WDOM: cout << "mF_WDOM" << endl; break;

          case mF_BGT: cout << "mF_BGT" << endl; break;

          case mF_BDOM: cout << "mF_BDOM" << endl; break;

          default: cout << "???" << endl;
    }

  }


  void test_gen2(cMatch &match){

    cGenerator2 gen2(match);

    cMove *moveptr;

    while(true){

        moveptr = gen2.gen_move();

        if(moveptr != nullptr){
            cout << moveptr->format(true) << endl;

            delete moveptr;
        }
        else{
            return;
        }

    }

  }


  void test_does_move_escape_soft_pin(cMatch &match, const cMove &move){

      bool flag = match.m_board.is_piece_behind_soft_pinned(4, 6);
      cout << flag << endl;

      flag = cEvaluator::does_move_escape_soft_pin(match, move);
      
      cout << flag << endl;

  }


  void test_is_continue(cMatch &match, const uint8_t depth){

      vector<cMove> moves;

      cGenerator gen(match);
      gen.gen_moves(moves, match.next_color());

      //match.sort(moves.begin(), moves.end(), match.sortByPrio);

      cDaemon daemon(match);

      for(const cMove &move : moves){

          cout << "started for: " + move.format(true) << endl;

          bool flag = daemon.is_continue(match, move, depth, 1);

          cout << flag << endl;

          cout << "************" << endl;

        }

  }


  void test_is_continue_sequence(cMatch &match){

        uint8_t src_x, src_y, dst_x, dst_y, depth;

        vector<cMove> sequence;
        
        cMove *moveptr;

        cMove::coord_to_indices(src_x, src_y, "h1");
        cMove::coord_to_indices(dst_x, dst_y, "d1");
        moveptr = new cMove(src_x, src_y, dst_x, dst_y, mWRK, mBLK, mBLK, cMove::P_MEDIUM);
        cEvaluator::priorize_move(match, *moveptr);
        sequence.push_back(*moveptr);

        match.do_move(*moveptr);

        cMove::coord_to_indices(src_x, src_y, "e7");
        cMove::coord_to_indices(dst_x, dst_y, "e6");
        moveptr = new cMove(src_x, src_y, dst_x, dst_y, mBQU, mBLK, mBLK, cMove::P_MEDIUM);
        cEvaluator::priorize_move(match, *moveptr);
        sequence.push_back(*moveptr);

        match.do_move(*moveptr);

        cMove::coord_to_indices(src_x, src_y, "b5");
        cMove::coord_to_indices(dst_x, dst_y, "d7");
        moveptr = new cMove(src_x, src_y, dst_x, dst_y, mWBP, mBRK, mBLK, cMove::P_MEDIUM);
        cEvaluator::priorize_move(match, *moveptr);
        sequence.push_back(*moveptr);

        match.do_move(*moveptr);

        cMove::coord_to_indices(src_x, src_y, "f6");
        cMove::coord_to_indices(dst_x, dst_y, "d7");
        moveptr = new cMove(src_x, src_y, dst_x, dst_y, mBKN, mWBP, mBLK, cMove::P_MEDIUM);
        cEvaluator::priorize_move(match, *moveptr);
        sequence.push_back(*moveptr);

        match.do_move(*moveptr);

        cMove::coord_to_indices(src_x, src_y, "b3");
        cMove::coord_to_indices(dst_x, dst_y, "b8");
        moveptr = new cMove(src_x, src_y, dst_x, dst_y, mWQU, mBLK, mBLK, cMove::P_MEDIUM);
        cEvaluator::priorize_move(match, *moveptr);
        sequence.push_back(*moveptr);

        match.do_move(*moveptr);

        cMove::coord_to_indices(src_x, src_y, "d7");
        cMove::coord_to_indices(dst_x, dst_y, "b8");
        moveptr = new cMove(src_x, src_y, dst_x, dst_y, mBKN, mWQU, mBLK, cMove::P_MEDIUM);
        cEvaluator::priorize_move(match, *moveptr);
        sequence.push_back(*moveptr);

        match.do_move(*moveptr);

        cMove::coord_to_indices(src_x, src_y, "d1");
        cMove::coord_to_indices(dst_x, dst_y, "d8");
        moveptr = new cMove(src_x, src_y, dst_x, dst_y, mWRK, mBLK, mBLK, cMove::P_MEDIUM);
        cEvaluator::priorize_move(match, *moveptr);
        sequence.push_back(*moveptr);

        match.do_move(*moveptr);

        /*cMove::coord_to_indices(src_x, src_y, "f6");
        cMove::coord_to_indices(dst_x, dst_y, "d7");
        sequence.push_back(cMove(src_x, src_y, dst_x, dst_y, mBKN, mWBP, mBLK, cMove::P_MEDIUM));

        cMove::coord_to_indices(src_x, src_y, "b3");
        cMove::coord_to_indices(dst_x, dst_y, "b8");
        sequence.push_back(cMove(src_x, src_y, dst_x, dst_y, mWQU, mBLK, mBLK, cMove::P_MEDIUM));

        cMove::coord_to_indices(src_x, src_y, "d7");
        cMove::coord_to_indices(dst_x, dst_y, "b8");
        sequence.push_back(cMove(src_x, src_y, dst_x, dst_y, mBKN, mWQU, mBLK, cMove::P_MEDIUM));

        cMove::coord_to_indices(src_x, src_y, "d1");
        cMove::coord_to_indices(dst_x, dst_y, "d8"); 
        sequence.push_back(cMove(src_x, src_y, dst_x, dst_y, mWRK, mBLK, mBLK, cMove::P_MEDIUM));*/

        depth = 1;

        cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;

        for(const cMove &move : sequence){

            cDaemon daemon(match);

            bool flag = daemon.is_continue(match, move, depth, 1);

            cout << move.format(true) << " : " << flag << endl;
            cout << "-----------------------------" << endl;

            match.do_move(move);

            depth++;

        }

    }


    void test_does_move_touch_soft_pinned(cMatch &match, cMove &move){

        bool flag = cEvaluator::does_move_touch_soft_pinned(match, move);
        cout << "does_move_touch_soft_pinned: " << flag << endl;

    }


    void test_does_move_sac_for_supply(cMatch &match, cMove &move){

        bool flag = cEvaluator::does_move_sac_for_supply(match, move);
        cout << move.format(true) << " " << flag << endl;

    }

