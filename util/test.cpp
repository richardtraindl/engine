


    #include <iostream>
    #include <fstream>
    #include <string>
    #include "../evaluator.hpp"
    #include "../match.hpp"
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


  void test_is_hotspot(cMatch &match){

      for(int8_t y = 7; y >= 0; --y){

          for(uint8_t x = 0; x <= 7; ++x){
            
              bool flag = cEvaluator::is_hotspot(match, x, y);

              cout << setw(5) << setfill(' ') <<  to_string(flag);

          }
          cout << endl;
          cout << endl;

      }

  }


  void test_eval_field_state(cMatch &match, const uint8_t src_x, const uint8_t src_y){

      uint8_t field_state = cEvaluator::eval_field_state(match, src_x, src_y);

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

