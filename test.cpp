
 #include "./test.hpp"


    void play100(cMatch &match, uint8_t engine_color){
        
        time_t time_start = time(0);

        uint8_t wkgcnt = 0;
        uint8_t bkgcnt = 0;
        uint8_t bpcnt = 0;
        uint8_t kncnt = 0;

        for(uint8_t i = 0; i < 100; ++i){
            
            cout << "\n*****************" << endl;
            cout << "count: " << to_string(i + 1) << endl;
            cout << "*****************" << endl;

            if(i % 5 == 0){
                match.m_board.prnt();

                sleep(5);
            }

            vector<cMove> rmoves;

            int32_t rscore;

            //match.gen_moves(rmoves, match.next_color());
            match.calc_move(rscore, rmoves);

            if(rmoves.size() > 0){
                //uint8_t idx = rand() % rmoves.size();
                //cMove move = rmoves.at(idx);

                cMove move = rmoves.front();

                match.do_move(move);

                switch(move.m_srcpiece){
                    case mWKG: wkgcnt++; break;
                    case mBKG: bkgcnt++; break;
                    case mWKN: kncnt++; break;
                    case mBKN: kncnt++; break;
                    case mWBP: bpcnt++; break;
                    case mBBP: bpcnt++; break;
                }
            }
            else{
                match.m_board.prnt();

                cout << "count: " << to_string(i) << endl;

                uint8_t status = match.eval_status();
                match.prnt_status(status);

                cout << "wkg: " + to_string(wkgcnt) + " bkg:" + to_string(bkgcnt) + " bp: " + to_string(bpcnt) + " kn: " + to_string(kncnt) << endl;

                cout << "game over! " << to_string(i) << endl;

                cMatch::prnt_fmttime("\ncalc-time: ", time(0) - time_start);

                return;
            }
        }

        cout << "regular finish " << endl;

        cMatch::prnt_fmttime("\ncalc-time: ", time(0) - time_start);

    }


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
