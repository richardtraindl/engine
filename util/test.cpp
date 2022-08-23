


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
    void do_usr_move(cMatch &match, const uint8_t src_x, const uint8_t src_y, const uint8_t dst_x, const uint8_t dst_y, const uint8_t prompiece){

        uint8_t srcpiece = match.m_board.getfield(src_x, src_y);

        uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

        cMove move(src_x, src_y, dst_x, dst_y, srcpiece, dstpiece, prompiece, cMove::P_BOTTOM);

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

            cBoard::coord_to_indices(src_x, src_y, content.substr(0, 2));

            cBoard::coord_to_indices(dst_x, dst_y, content.substr(2, 2));
            
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
    //*****************************************



    //*****************************************
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
    //*****************************************



    //*****************************************
    void test_eval_field_state(cMatch &match, const uint8_t src_x, const uint8_t src_y){

        cPiece *wlowest, *blowest;
        uint8_t field_state = cEvaluator::eval_field_state(wlowest, blowest, match, mBLK, src_x, src_y);

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
    //*****************************************



    //*****************************************
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
    //*****************************************



    //*****************************************
    void test_gen2_king(cMatch &match, const uint8_t color){

      cGenerator2 gen2(match);
      
      /*if(color == mWHITE){
          gen2.m_gen_x = match.m_board.m_wKg_x;
          gen2.m_gen_y = match.m_board.m_wKg_y;
      }
      else{
          gen2.m_gen_x = match.m_board.m_bKg_x;
          gen2.m_gen_y = match.m_board.m_bKg_y;
      }*/

      cMove *moveptr;

      for(uint8_t i = 0; i < 20; ++i){

          moveptr = gen2.gen_move();

          if(moveptr == nullptr){
              return;
          }
          else{
              cout << moveptr->format(true) << endl;

              delete moveptr;
          }

      }

    }
    //*****************************************



    //*****************************************
    void test_does_move_escape_soft_pin(cMatch &match, const cMove &move){

        bool flag = match.m_board.is_piece_behind_soft_pinned(4, 6);
        cout << flag << endl;

        flag = cEvaluator::does_move_escape_soft_pin(match, move);
        
        cout << flag << endl;

    }
    //*****************************************



    //*****************************************
    void test_is_continue(cMatch &match, const uint8_t depth){

        vector<cMove> moves;

        cGenerator gen(match);
        gen.gen_moves(moves, match.next_color());

        sort(moves.begin(), moves.end(), cMatch::sortByPrio);

        cDaemon daemon(match);

        for(const cMove &move : moves){

            cout << "started for: " + move.format(true) << endl;

            bool flag = daemon.is_continue(match, move, depth);

            cout << flag << endl;

            cout << "************" << endl;

          }

    }
    //*****************************************



    //*****************************************
    void test_does_move_touch_weak_piece(cMatch &match, const cMove &move){

        bool flag = cEvaluator::does_move_touch_weak_piece(match, move);

        cout << move.format(false) << endl;
        cout << "test_does_move_touch_weak_piece: " << flag << endl;

    }
    //*****************************************



    //*****************************************
    void test_is_piece_weak(cMatch &match){

        for(int8_t y = 7; y >= 0; --y){

            for(uint8_t x = 0; x <= 7; ++x){

                uint8_t piece = match.m_board.getfield(x, (uint8_t)y);
                
                if(piece == mBLK){
                    cout << "-";
                }
                else{
                    bool flag = cEvaluator::is_piece_weak(match, cPiece(piece, x, y));
                    cout << flag;
                }

            }
            
            cout << endl;

        }
        
    }
    //*****************************************



    //*****************************************
    void test_find_mate(cMatch &match, const cMove &move, const uint8_t maxdepth){

          bool mate = cEvaluator::find_mate(match, move, maxdepth);

          cout << "mate: " << mate << endl;

    }
    //*****************************************



    //*****************************************
    void test_pins(cMatch &match, const uint8_t src_x, const uint8_t src_y){

        bool pin = match.m_board.is_soft_pinned(src_x, src_y);

        cout << "pin: " << pin << endl;

    }
    //*****************************************
