
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
                int8_t state = field.eval_score();
                //int8_t state = match.m_board.eval_field_state(x, y);

                cout << setfill(' ') << setw(3) << to_string(state);
            }
            cout << endl;
        }

    }


    void prnt_moves_score(cMatch &match){

        vector<cMove> moves;
        match.gen_moves(moves, match.next_color());

        cout << "score_moves" << endl;

        for(const cMove &move : moves){
            int32_t score = match.score_move(move);
            cout << move.format(true) << setfill(' ') << setw(5) << to_string(score) << endl;
        }

    }


    void prnt_eval_pin_state(const cMatch &match, const cPiece &piece){
    
        uint8_t state = match.m_board.eval_pin_state(piece.m_piece, piece.m_xpos, piece.m_ypos);

        cout << "state: " << to_string(state) << endl;

    }


    void prnt_does_move_touch_weak_piece(cMatch &match, const cMove &move){

        bool isweak = match.does_move_touch_vulnerable_piece(move);

        cout << "isweak: " << to_string(isweak) << endl;

    }


    void prnt_moves_touch_vulnerable_piece(cMatch &match){

        vector<cMove> moves;
        match.gen_moves(moves, match.next_color());

        uint8_t stage = match.eval_stage();
        cout << "stage: " << to_string(stage) << endl;

        for(const cMove &move : moves){
            match.do_move(move);

            bool isweak = match.does_move_touch_vulnerable_piece(move);

            cout << move.format(true) << setfill(' ') << setw(2) << to_string(isweak) << endl;

            match.undo_move();
        }

    }


    void prnt_moves_weak_state(cMatch &match){

        vector<cMove> moves;
        match.gen_moves(moves, match.next_color());

        for(const cMove &move : moves){
            bool isweak = match.does_move_touch_vulnerable_piece(move);
            cout << move.format(true) << setfill(' ') << setw(3) << to_string(isweak) << endl;
        }

    }


    void prnt_moves_support_supply(cMatch &match){

        vector<cMove> moves;
        match.gen_moves(moves, match.next_color());

        for(const cMove &move : moves){
            bool does_move_support_supply = match.does_move_support_supply(move);

            cout << "does_move_support_supply: " << to_string(does_move_support_supply) << endl;
        }

    }


    void prnt_test(cMatch &match){

        bool isinbounds = cBoard::is_inbounds(3, -1);
        cout << "isinbounds 3, -1: " << setfill(' ') << setw(3) << to_string(isinbounds) << endl;

        bool iswhitekingattacked = match.m_board.is_king_attacked(mBLACK);
        cout << "iswhitekingattacked " << setfill(' ') << setw(3) << to_string(iswhitekingattacked) << endl;

        bool isblackkingattacked = match.m_board.is_king_attacked(mWHITE);
        cout << "isblackkingattacked " << setfill(' ') << setw(3) << to_string(isblackkingattacked) << endl;

        uint8_t maxdiff = cBoard::max_diff(4, 4, 2, 0);
        cout << "maxdiff 4, 4, 2, 0: " << setfill(' ') << setw(3) << to_string(maxdiff) << endl;

        uint8_t difftomargin = cBoard::diff_to_margin(5, 5);
        cout << "difftomargin 5, 5: " << setfill(' ') << setw(3) << to_string(difftomargin) << endl;

        /*
        static bool is_margin_frame_pos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

        static bool is_margin_frame_ypos(uint8_t y1, uint8_t y2);

        static bool is_corner_pos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

        bool is_within_three_squares(uint8_t piece, uint8_t src_x, uint8_t src_y) const;

        bool is_passed_pawn(uint8_t piece, uint8_t src_x, uint8_t src_y) const;

        uint8_t search_dir_for_piece(uint8_t &dst_x, uint8_t &dst_y, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y, uint8_t maxcnt) const;

        void search_dir_for_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y) const;

        void search_all_dirs_for_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, uint8_t piece, uint8_t piece_x, uint8_t piece_y, uint8_t excl_dir) const;

        bool search_for_touching_piece(uint8_t src_x, uint8_t src_y, uint8_t color) const;

        void search_for_touching_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, uint8_t color, bool touching_only) const;

        void search_for_all_touching_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, uint8_t src_x, uint8_t src_y) const;

        static uint8_t eval_dir(uint8_t src_x1, uint8_t src_y1, uint8_t src_x2, uint8_t src_y2);
        
        uint8_t eval_pindir(uint8_t src_x, uint8_t src_y, uint8_t color) const;

        uint8_t eval_pin_state(uint8_t piece, uint8_t x, uint8_t y) const;

        int8_t eval_field_state(uint8_t x, uint8_t y) const;
        */
    }
