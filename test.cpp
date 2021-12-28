
 #include "./test.hpp"


    void prnt_eval_field_states(const cMatch &match){

        for(int8_t y = 7; y >=0; --y){
            for(uint8_t x = 0; x < 8; ++x){

                cField field(&match.m_board, x, y);
                int8_t state = field.eval_advant_score();
                //int8_t state = match.m_board.eval_field_state(x, y);

                cout << setfill(' ') << setw(3) << to_string(state);
            }
            cout << endl;
        }

    }
