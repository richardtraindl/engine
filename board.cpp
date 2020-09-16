
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
        pins[5] = { 0x0, 0x0, 0x0, 0x0, 0x0 };
    }


    bool cPin::is_pinned(uint64_t pos){
        return (pins[IDX_PIN] & pos) > 0;
    }


    cBoard::cBoard(){ 
    }

    cBoard::cBoard(const cBoard &board){
    } // copy constructor


    uint8_t cBoard::getfield(uint64_t pos){
        uint8_t piece = 0;
        uint8_t test = 0b10000000;
        for(int i = 0; i < 8; i++){
            if((fields[i] & pos) > 0){
                piece = (piece | test);
            }
            test = (test >> 1);
        }
        return piece;
    }


    void cBoard::setfield(uint64_t pos, uint8_t piece){
        uint8_t test = 0b10000000;
        uint64_t negmask = (0xFFFFFFFFFFFFFFFF ^ pos);
        for(int i = 0; i < 8; i++){
            fields[i] = (fields[i] & negmask);
            if((piece & test) > 0){
                fields[i] = (fields[i] | pos);
            }
            test = (test >> 1);
        }
    }


    uint64_t cBoard::getwk_pos(){
        return (fields[mWHITE] & fields[mKINGS]);
    }


    uint64_t cBoard::getbk_pos(){
        return (fields[mBLACK] & fields[mKINGS]);
    }


    bool cBoard::is_piece_white(uint8_t piece){
        return (piece & mWHITE) > 0;
    }


    bool cBoard::is_piece_black(uint8_t piece){
        return (piece & mBLACK) > 0;
    }


    bool cBoard::is_field_busy(uint64_t pos){
        return (pos & fields[mIDX_WHITE]) > 0 || (pos & fields[mIDX_BLACK]) > 0;
    }


    bool cBoard::is_field_white_busy(uint64_t pos){
        return (pos & fields[mIDX_WHITE]) > 0;
    }


    bool cBoard::is_field_black_busy(uint64_t pos){
        return (pos & fields[mIDX_BLACK]) > 0;
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
            uint8_t piece = getfield(pos);
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
        if((fields[mIDX_WHITE] & fields[mIDX_KING]) == 0 || 
            (fields[mIDX_BLACK] & fields[mIDX_KING]) == 0 ){
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
            int piece = getfield(pos);
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
                uint8_t piece = getfield(pos);
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
            if(fields[i] != newboard.fields[i]){
                return false;
            }
        }
        return true;
    }

    bool cBoard::is_field_enemy_touched(uint64_t pos){
        list<const cStep *> steps;
        uint8_t piece = getfield(pos);
        uint8_t enemycolor;        
        if(is_piece_white(piece)){
            enemycolor = mBLACK;
        }
        else{
            enemycolor = mWHITE;
        }

        steps.push_back(rk_steps);
        steps.push_back(bp_steps);
        steps.push_back(kg_steps_generic);
        steps.push_back(kn_steps);
        if(enemycolor == mWHITE){
            steps.push_back(wpw_steps_attack);
        }
        else{
            steps.push_back(bpw_steps_attack);
        }

        for(const cStep *step : steps){
            uint64_t newpos = pos;

            for(int k = 0; k < step->stepcnt; ++k){
                if((newpos & step->border) > 0){
                    break;
                }
                if(step->rightshift){
                    newpos = (newpos >> step->shiftcnt);
                }
                else{
                    newpos = (newpos << step->shiftcnt);
                }
                if(is_field_busy(newpos) == false){
                    continue;
                }
                if((fields[mIDX_WHITE] & newpos) > 0 && enemycolor == mWHITE){
                    if(step->owner == STEP_OWNER["rook"] &&
                       (piece == mWRK || piece == mWQU)){
                        return true;
                    }
                    else if(step->owner == STEP_OWNER["bishop"] &&
                       (piece == mWBP || piece == mWQU)){
                        return true;
                    }
                    else if(step->owner == STEP_OWNER["king"] && piece == mWKG){
                        return true;
                    }
                    else if(step->owner == STEP_OWNER["knight"] && piece == mWKN){
                        return true;
                    }
                    else if(step->owner == STEP_OWNER["wpawn"] && piece == mWPW){
                        return true;
                    }
                    else{
                        break;
                    }
                }
                else if((fields[mIDX_BLACK] & newpos) > 0 && enemycolor == mBLACK){
                    if(step->owner == STEP_OWNER["rook"] &&
                       (piece == mBRK || piece == mBQU)){
                        return true;
                    }
                    else if(step->owner == STEP_OWNER["bishop"] &&
                       (piece == mBBP || piece == mBQU)){
                        return true;
                    }
                    else if(step->owner == STEP_OWNER["king"] && piece == mBKG){
                        return true;
                    }
                    else if(step->owner == STEP_OWNER["knight"] && piece == mBKN){
                        return true;
                    }
                    else if(step->owner == STEP_OWNER["bpawn"] && piece == mBPW){
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

            if(is_field_white_busy(pos)){
                uint64_t opp_pawn_src = (newpos >> 8);
                uint64_t opp_pawn_dst = (newpos << 8);
                return (move.src == opp_pawn_src && 
                        move.dst == opp_pawn_dst &&
                        getfield(opp_pawn_dst) == mBPW);
            }
            if(is_field_black_busy(pos)){
                uint64_t opp_pawn_src = (newpos << 8);
                uint64_t opp_pawn_dst = (newpos >> 8);
                return (move.src == opp_pawn_src && 
                        move.dst == opp_pawn_dst &&
                        getfield(opp_pawn_dst) == mWPW);
            }
        }
        return false;
    }


    bool cBoard::tst_wpw_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        // check, if field after one step forward is blank
        if((pos >> 8) == newpos){
            return (fields[mIDX_WHITE] & newpos) == 0 && 
                   (fields[mIDX_BLACK] & newpos) == 0;
        }
        // check, if fields after two step forward are blank
        else if((pos >> 16) == newpos){
            return (fields[mIDX_WHITE] & newpos) == 0 && 
                   (fields[mIDX_BLACK] & newpos) == 0 && 
                   (fields[mIDX_WHITE] & (pos >> 8)) == 0 && 
                   (fields[mIDX_BLACK] & (pos >> 8)) == 0;
        }
        else if((pos >> 9) == newpos || (pos >> 7) == newpos){
            return (fields[mIDX_BLACK] & newpos) > 0;
        }
        else{
            return tst_en_passant(pos, newpos, minutes);
        }
    }


    bool cBoard::tst_bpw_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        // check, if field after one step forward is blank
        if((pos << 8) == newpos){
            return (fields[mIDX_WHITE] & newpos) == 0 && 
                   (fields[mIDX_BLACK] & newpos) == 0;
        }
        // check, if fields after two step forward are blank
        else if((pos << 16) == newpos){
            return (fields[mIDX_WHITE] & newpos) == 0 && 
                   (fields[mIDX_BLACK] & newpos) == 0 && 
                   (fields[mIDX_WHITE] & (pos << 8)) == 0 && 
                   (fields[mIDX_BLACK] & (pos << 8)) == 0;
        }
        else if((pos << 9) == newpos || (pos << 7) == newpos){
            return (fields[mIDX_WHITE] & newpos) > 0;
        }
        else{
            return tst_en_passant(pos, newpos, minutes);
        }
    }


    bool cBoard::tst_castling(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        uint64_t mask;
        uint8_t color_idx;
        
        if(is_field_white_busy(pos)){
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
            if((it->prev_fields[color_idx] & mask) != mask){
                return false;
            }
        }
        return true;
    }


    bool cBoard::tst_kg_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        uint8_t idx_color;

        if(is_field_white_busy(pos)){
            idx_color = mIDX_WHITE;
        }
        else{
            idx_color = mIDX_BLACK;
        }

        if((pos >> 2) == newpos){
            if(is_field_busy((pos >> 1)) == false &&
               is_field_busy((pos >> 2)) == false &&
               (fields[idx_color] & (pos >> 3)) > 0 &&
               (fields[mIDX_ROOK] & (pos >> 3)) > 0){
                for(int i = 0; i < 3; ++i){
                    if(is_field_enemy_touched((pos >> i))){
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
            if(is_field_busy((pos << 1)) == false &&
               is_field_busy((pos << 2)) == false &&
               is_field_busy((pos << 3)) == false &&
               (fields[idx_color] & (pos << 4)) > 0 &&
               (fields[mIDX_ROOK] & (pos << 4)) > 0){
                for(int i = 0; i < 3; ++i){
                    if(is_field_enemy_touched((pos << i))){
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
            return (is_field_enemy_touched(newpos) == false);
        }
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

        while(pos > 0){
            uint8_t piece = getfield(pos);
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
                            tstcolor = (fields[mIDX_WHITE] & newpos) > 0;
                        }
                        else{
                            tstcolor = (fields[mIDX_BLACK] & newpos) > 0;
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
                            if((fields[mIDX_WHITE] & newpos) > 0 || 
                               (fields[mIDX_BLACK] & newpos) > 0){
                                break;
                            }
                        }
                    }
                }
            }

            pos = (pos >> 1);
        }
    }
