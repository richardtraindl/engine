
    #include "./board.hpp"


    cStep::cStep(uint8_t newowner, bool newrightshift, uint8_t newshiftcnt, uint8_t newstepcnt, uint64_t newborder){ 
        owner = newowner;
        rightshift = newrightshift;
        shiftcnt = newshiftcnt;
        stepcnt = newstepcnt;
        border = newborder;
    }

    cStep::cStep(){
    }


    cPin:: cPin(){
    }


    bool cPin::is_pinned(uint64_t pos){
        return (pins[mIDX_PIN] & pos) > 0;
    }


    cBoard::cBoard(){ 
    }

    cBoard::cBoard(const cBoard &board){
    } // copy constructor


    uint8_t cBoard::read(uint64_t pos){
        uint8_t piece = 0;
        uint8_t test = 0b10000000;
        for(int i = 0; i < 8; i++){
            if((field[i] & pos) > 0){
                piece = (piece | test);
            }
            test = (test >> 1);
        }
        return piece;
    }


    void cBoard::write(uint64_t pos, uint8_t piece){
        uint8_t test = 0b10000000;
        uint64_t negmask = (0xFFFFFFFFFFFFFFFF ^ pos);
        for(int i = 0; i < 8; i++){
            field[i] = (field[i] & negmask);
            if((piece & test) > 0){
                field[i] = (field[i] | pos);
            }
            test = (test >> 1);
        }
    }


    uint64_t cBoard::read_wk_pos(){
        return (field[mIDX_WHITE] & field[mIDX_KING]);
    }


    uint64_t cBoard::read_bk_pos(){
        return (field[mIDX_BLACK] & field[mIDX_KING]);
    }


    bool cBoard::is_square_blank(uint64_t pos){
        return (pos & field[mIDX_WHITE]) == 0 && (pos & field[mIDX_BLACK]) == 0;
    }


    bool cBoard::is_square_white_occupied(uint64_t pos){
        return (pos & field[mIDX_WHITE]) > 0;
    }


    bool cBoard::is_square_black_occupied(uint64_t pos){
        return (pos & field[mIDX_BLACK]) > 0;
    }


    bool cBoard::is_piece_blank(uint8_t piece){
        return (piece & mWHITE) == 0 && (piece & mBLACK) == 0;
    }


    bool cBoard::is_piece_white(uint8_t piece){
        return (piece & mWHITE) > 0;
    }


    bool cBoard::is_piece_black(uint8_t piece){
        return (piece & mBLACK) > 0;
    }


    bool cBoard::verify(){
        int wKg_cnt = 0;
        int bKg_cnt = 0;
        int wPw_cnt = 0;
        int bPw_cnt = 0;
        int wOfficer_cnt = 0;
        int bOfficer_cnt = 0;
        uint64_t pos = mPOS_A1; 

        while(pos > 0){
            uint8_t piece = read(pos);
            if(piece == mWKG){
                wKg_cnt += 1; 
                continue;
            }
            if(piece == mBKG){
                bKg_cnt += 1;
                continue;
            }
            if(piece == mWPW){
                wPw_cnt += 1;
                continue;
            }
            if(piece == mBPW){
                bPw_cnt += 1;
                continue; 
            }
            if(piece == mWRK || piece == mWBP || piece == mWKN || piece == mWQU){
                wOfficer_cnt += 1;
                continue;
            }
            if(piece == mBRK || piece == mBBP || piece == mBKN || piece == mBQU){
                bOfficer_cnt += 1;
                continue;
            }
            pos = (pos >> 1);
        }
        if(wKg_cnt != 1 || bKg_cnt != 1){
            return false;
        }
        if(wPw_cnt > 8 || bPw_cnt > 8){
            return false;
        }
        if(wPw_cnt + wOfficer_cnt > 15){
            return false;
        }
        if(bPw_cnt + bOfficer_cnt > 15){
            return false;
        }
        if((field[mIDX_WHITE] & field[mIDX_KING]) == 0 || 
            (field[mIDX_BLACK] & field[mIDX_KING]) == 0 ){
            return false;
        }
        // ToDo
        //if(abs(wKg / 8 - bKg / 8) < 2 && abs(wKg % 8 - bKg % 8) < 2){
        //    return false;
        //}
        return true;
    }


    void cBoard::eval_count_of_officers(int &wofficers, int &bofficers){
        wofficers = 0;
        bofficers = 0;
        for(uint64_t pos = mPOS_A1; pos != 0; pos = (pos >> 1)){
            int piece = read(pos);
            if(piece == mWRK || piece == mWBP || piece == mWKN || piece == mWQU){
                wofficers += 1;
                continue;
            }
            if(piece == mBRK || piece == mBBP || piece == mBKN || piece == mBQU){
                bofficers += 1;
                continue;
            }
        }
    }


    void cBoard::prnt(){
        string textcolor, backcolor, strpiece;
        uint64_t startpos = 0x0000000000000080;
        for(int y = 7; y >=0; --y){
            uint64_t pos = startpos;
            for(int x = 0; x < 8; ++x){
                uint8_t piece = read(pos);
                if(piece == mBLK){
                    strpiece = "   ";
                }
                else{
                    strpiece = reverse_lookup(PIECES, piece);
                }
                if(is_piece_white(piece)){
                    textcolor = mWHITE_TEXT + mBOLD_ON;
                }
                else{
                    textcolor = mBLACK_TEXT + mBOLD_ON;
                }
                if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
                    backcolor = mBLUE_BACK;
                }
                else{
                    backcolor = mGREEN_BACK;
                }
                // "\e#3" \e#4"
                cout << backcolor + textcolor + strpiece + mRESET_ALL;

                pos = (pos >> 1);
            }
            cout << endl;

            startpos = (startpos << 8);
        }
    }


    bool cBoard::compare(cBoard &newboard){
        for(int i = 0; i < 8; ++i){
            if(field[i] != newboard.field[i]){
                return false;
            }
        }
        return true;
    }


    void cBoard::gen_moves(list<cMove> &minutes){
        uint64_t pos = mPOS_A1;
        uint8_t color;
        const cStep *steps;
        bool tstcolor, tstmove;

        if((minutes.size() % 2) == 0){
            color = mWHITE;
        }
        else{
            color = mBLACK;
        }
        
        cPin *cpin = determine_pins(color);
        //for(uint8_t i = 0; i < 5; ++i){
        //    cout << "0x" << hex << setfill('0') << setw(64);
        //    cout << cpin->pins[i] << endl;
        //}

        while(pos > 0){
            uint8_t piece = read(pos);
            int maxidx;
            
            if((piece & color) > 0){
                if(piece == mWRK || piece == mBRK){ steps = rk_steps; maxidx = 4; }
                else if(piece == mWBP || piece == mBBP){ steps = bp_steps; maxidx = 4; }
                else if(piece == mWQU || piece == mBQU){ steps = qu_steps; maxidx = 8; }
                else if(piece == mWKG){ steps = wkg_steps_and_castl; maxidx = 10; }
                else if(piece == mBKG){ steps = bkg_steps_and_castl; maxidx = 10; }
                else if(piece == mWKN || piece == mBKN){ steps = kn_steps; maxidx = 8; }
                else if(piece == mWPW){ steps = wpw_steps; maxidx = 3; }
                else if(piece == mBPW){ steps = bpw_steps; maxidx = 3; }

                cout << "-----" << endl;
                cout << PIECES_STR[piece] << endl;

                for(int i = 0; i < maxidx; ++i){
                    uint64_t newpos = pos;

                    for(int k = 0; k < (steps + i)->stepcnt; ++k){
                        if((cpin->pins[mIDX_PIN] & pos) > 0){
                            if(piece == mWKN || piece == mBKN){
                                break;
                            }
                            else if((cpin->pins[mIDX_WST_EST] & pos) > 0){
                                if((steps + i)->shiftcnt != 1){
                                    break;
                                }
                            }
                            else if((cpin->pins[mIDX_STH_NTH] & pos) > 0){
                                if((steps + i)->shiftcnt != 8){
                                    break;
                                }
                            }
                            else if((cpin->pins[mIDX_STH_WST_NTH_EST] & pos) > 0){
                                if((steps + i)->shiftcnt != 9){
                                    break;
                                }
                            }
                            else if((cpin->pins[mIDX_STH_EST_NTH_WST] & pos) > 0){
                                if((steps + i)->shiftcnt != 7){
                                    break;
                                }
                            }
                        }

                        if((newpos & (steps + i)->border) > 0){
                            break;
                        }
                        if((steps + i)->rightshift){
                            newpos = (newpos >> (steps + i)->shiftcnt);
                        }
                        else{
                            newpos = (newpos << (steps + i)->shiftcnt);
                        }
                        if((color & mWHITE) > 0){
                            tstcolor = (field[mIDX_WHITE] & newpos) > 0;
                        }
                        else{
                            tstcolor = (field[mIDX_BLACK] & newpos) > 0;
                        }
                        if(tstcolor){
                            break;
                        }
                        else{
                            if(piece == mWPW){
                                tstmove = tst_wpw_move(pos, newpos, minutes);
                            }
                            else if(piece == mBPW){
                                tstmove = tst_bpw_move(pos, newpos, minutes);
                            }
                            else if(piece == mWKG || piece == mBKG){
                                tstmove = tst_kg_move(pos, newpos, minutes);
                            }
                            else{
                                tstmove = true;
                            }
                            if(tstmove){
                                cout << pos_to_coord(pos) << "-" << pos_to_coord(newpos) << endl;
                            }
                            if((field[mIDX_WHITE] & newpos) > 0 || 
                               (field[mIDX_BLACK] & newpos) > 0){
                                break;
                            }
                        }
                    }
                }
            }

            pos = (pos >> 1);
        }
    }


    bool cBoard::is_square_enemy_touched(uint64_t pos){
        const cStep *steps;
        uint8_t piece = read(pos);
        uint8_t enemy;
        uint8_t enemycolor;        
        if(is_piece_white(piece)){
            enemycolor = mBLACK;
            steps = steps_for_black_enemy_search;
        }
        else{
            enemycolor = mWHITE;
            steps = steps_for_white_enemy_search;
        }

        for(uint8_t i = 0; i < 27; ++i){
            cStep step = steps[i];

            uint64_t newpos = pos;

            for(int k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }

                if(is_square_blank(newpos)){
                    continue;
                }

                enemy = read(newpos);
                if((field[mIDX_WHITE] & newpos) > 0 && enemycolor == mWHITE){
                    if(step.owner == STEP_OWNER["rook"] &&
                       (enemy == mWRK || enemy == mWQU)){
                        return true;
                    }
                    else if(step.owner == STEP_OWNER["bishop"] &&
                       (enemy == mWBP || enemy == mWQU)){
                        return true;
                    }
                    else if(step.owner == STEP_OWNER["king"] && enemy == mWKG){
                        return true;
                    }
                    else if(step.owner == STEP_OWNER["knight"] && enemy == mWKN){
                        return true;
                    }
                    else if(step.owner == STEP_OWNER["wpawn"] && enemy == mWPW){
                        return true;
                    }
                    else{
                        break;
                    }
                }
                else if((field[mIDX_BLACK] & newpos) > 0 && enemycolor == mBLACK){
                    if(step.owner == STEP_OWNER["rook"] &&
                       (enemy == mBRK || enemy == mBQU)){
                        return true;
                    }
                    else if(step.owner == STEP_OWNER["bishop"] &&
                       (enemy == mBBP || enemy == mBQU)){
                        return true;
                    }
                    else if(step.owner == STEP_OWNER["king"] && enemy == mBKG){
                        return true;
                    }
                    else if(step.owner == STEP_OWNER["knight"] && enemy == mBKN){
                        return true;
                    }
                    else if(step.owner == STEP_OWNER["bpawn"] && enemy == mBPW){
                        return true;
                    }
                    else{
                        break;
                    }
                }
                else{
                    break;
                }
            }
        }
        return false;
    }


    bool cBoard::tst_en_passant(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        if(minutes.size() > 0){
            cMove move = minutes.back();

            if(is_square_white_occupied(pos)){
                uint64_t opp_pawn_src = (newpos >> 8);
                uint64_t opp_pawn_dst = (newpos << 8);
                return (move.src == opp_pawn_src && 
                        move.dst == opp_pawn_dst &&
                        read(opp_pawn_dst) == mBPW);
            }
            if(is_square_black_occupied(pos)){
                uint64_t opp_pawn_src = (newpos << 8);
                uint64_t opp_pawn_dst = (newpos >> 8);
                return (move.src == opp_pawn_src && 
                        move.dst == opp_pawn_dst &&
                        read(opp_pawn_dst) == mWPW);
            }
        }
        return false;
    }


    bool cBoard::tst_wpw_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        // check, if field after one step forward is blank
        if((pos >> 8) == newpos){
            return (field[mIDX_WHITE] & newpos) == 0 && 
                   (field[mIDX_BLACK] & newpos) == 0;
        }
        // check, if fields after two step forward are blank
        else if((pos >> 16) == newpos){
            return (field[mIDX_WHITE] & newpos) == 0 && 
                   (field[mIDX_BLACK] & newpos) == 0 && 
                   (field[mIDX_WHITE] & (pos >> 8)) == 0 && 
                   (field[mIDX_BLACK] & (pos >> 8)) == 0;
        }
        else if((pos >> 9) == newpos || (pos >> 7) == newpos){
            return (field[mIDX_BLACK] & newpos) > 0;
        }
        else{
            return tst_en_passant(pos, newpos, minutes);
        }
    }


    bool cBoard::tst_bpw_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        // check, if field after one step forward is blank
        if((pos << 8) == newpos){
            return (field[mIDX_WHITE] & newpos) == 0 && 
                   (field[mIDX_BLACK] & newpos) == 0;
        }
        // check, if fields after two step forward are blank
        else if((pos << 16) == newpos){
            return (field[mIDX_WHITE] & newpos) == 0 && 
                   (field[mIDX_BLACK] & newpos) == 0 && 
                   (field[mIDX_WHITE] & (pos << 8)) == 0 && 
                   (field[mIDX_BLACK] & (pos << 8)) == 0;
        }
        else if((pos << 9) == newpos || (pos << 7) == newpos){
            return (field[mIDX_WHITE] & newpos) > 0;
        }
        else{
            return tst_en_passant(pos, newpos, minutes);
        }
    }


    bool cBoard::tst_castling(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        uint64_t mask;
        uint8_t color_idx;
        
        if(is_square_white_occupied(pos)){
            color_idx = mIDX_WHITE;
            if((pos >> 2) == newpos){
                mask = 0x0900000000000000;
            }
            else{
                mask = 0x8800000000000000;
            }
        }
        else{
            color_idx = mIDX_BLACK;
            if((pos >> 2) == newpos){
                mask = 0x0000000000000009;
            }
            else{
                mask = 0x0000000000000088;
            }
        }

        for(list<cMove>::reverse_iterator it = minutes.rbegin(); it != minutes.rend(); ++it){
            if((it->prev_field[color_idx] & mask) != mask){
                return false;
            }
        }
        return true;
    }


    bool cBoard::tst_kg_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        uint8_t idx_color;

        if(is_square_white_occupied(pos)){
            idx_color = mIDX_WHITE;
        }
        else{
            idx_color = mIDX_BLACK;
        }

        if((pos >> 2) == newpos){
            if(is_square_blank((pos >> 1)) &&
               is_square_blank((pos >> 2)) &&
               (field[idx_color] & (pos >> 3)) > 0 &&
               (field[mIDX_ROOK] & (pos >> 3)) > 0){
                for(int i = 0; i < 3; ++i){
                    if(is_square_enemy_touched((pos >> i))){
                        return false;
                    }
                }
                return tst_castling(pos, newpos, minutes);
            }
            else{
                return false;
            }
        }
        if((pos << 2) == newpos){
            if(is_square_blank((pos << 1)) &&
               is_square_blank((pos << 2)) &&
               is_square_blank((pos << 3)) &&
               (field[idx_color] & (pos << 4)) > 0 &&
               (field[mIDX_ROOK] & (pos << 4)) > 0){
                for(int i = 0; i < 3; ++i){
                    if(is_square_enemy_touched((pos << i))){
                        return false;
                    }
                }
                return tst_castling(pos, newpos, minutes);
            }
            else{
                return false;
            }
        }
        else{
            return (is_square_enemy_touched(newpos) == false);
        }
    }


    cPin *cBoard::determine_pins(uint8_t color){
        uint64_t kg_pos;
        cPin *cpin = new cPin();

        if(color == mWHITE){
            kg_pos = read_wk_pos();
        }
        else{
            kg_pos = read_bk_pos();
        }

        for(uint8_t i = 0; i < 8; ++i){
            const cStep step = steps_for_pin_search[i];

            uint64_t newpos = kg_pos;
            uint64_t friend_pos = 0;

            for(int k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }

                if(is_square_blank(newpos)){
                    continue;
                }

                if((is_square_white_occupied(newpos) && color == mWHITE) ||
                   (is_square_black_occupied(newpos) && color == mBLACK)){
                    if(friend_pos > 0){
                        break;
                    }
                    else{
                        friend_pos = newpos;
                        continue;
                    }
                }

                // enemy found because of checks above
                uint8_t enemy = read(newpos);

                if(step.owner == STEP_OWNER["rook"] &&
                   (enemy == mWRK || enemy == mBRK || 
                    enemy == mWQU || enemy == mBQU) && 
                   friend_pos > 0){
                    cpin->pins[mIDX_PIN] = (cpin->pins[mIDX_PIN] | friend_pos);

                    if(step.shiftcnt == 1){
                        cpin->pins[mIDX_WST_EST] = (cpin->pins[mIDX_WST_EST] | friend_pos);
                    }
                    else{
                       cpin->pins[mIDX_STH_NTH] = (cpin->pins[mIDX_STH_NTH] | friend_pos);
                    }
                }
                else if(step.owner == STEP_OWNER["bishop"] &&
                        (enemy == mWBP || enemy == mBBP || 
                         enemy == mWQU || enemy == mBQU) && 
                        friend_pos > 0){
                    cpin->pins[mIDX_PIN] = (cpin->pins[mIDX_PIN] | friend_pos);

                    if(step.shiftcnt == 9){
                        cpin->pins[mIDX_STH_WST_NTH_EST] = (cpin->pins[mIDX_STH_WST_NTH_EST] | friend_pos);
                    }
                    else{
                       cpin->pins[mIDX_STH_EST_NTH_WST] = (cpin->pins[mIDX_STH_EST_NTH_WST] | friend_pos);
                    }
                }
                break;
            }
        }
        return cpin;
    }


    void cBoard::determine_checks(uint8_t color, uint64_t &fst_enemy, uint64_t &sec_enemy){
    }

