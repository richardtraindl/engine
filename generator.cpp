
    #include "./generator.hpp"


    cGenerator::cGenerator(cMatch *newmatch){
        match = newmatch;
    }


    void cGenerator::gen_moves(list<cGMove> &moves){
        uint64_t pos = mPOS_A1;
        uint8_t color;
        const cStep *steps;

        if((match->minutes.size() % 2) == 0){
            color = mWHITE;
        }
        else{
            color = mBLACK;
        }
        
        cPin *cpin = match->board.determine_pins(color);

        list <cLink *> attackers;
        match->board.determine_checks(color, attackers);
        if(attackers.size() > 1){
            gen_kg_moves(color, moves);
            return;
        }
        else if(attackers.size() == 1){
            gen_kg_moves(color, moves);
            gen_kg_support_moves(attackers, moves);
            return;
        }

        while(pos > 0){
            uint8_t piece = match->board.read(pos);
            int maxidx;
            
            if((piece & color) > 0){
                if(piece == mWRK || piece == mBRK){ steps = cBoard::rk_steps; maxidx = 4; }
                else if(piece == mWBP || piece == mBBP){ steps = cBoard::bp_steps; maxidx = 4; }
                else if(piece == mWQU || piece == mBQU){ steps = cBoard::qu_steps; maxidx = 8; }
                else if(piece == mWKG){ steps = cBoard::wkg_steps_and_castl; maxidx = 10; }
                else if(piece == mBKG){ steps = cBoard::bkg_steps_and_castl; maxidx = 10; }
                else if(piece == mWKN || piece == mBKN){ steps = cBoard::kn_steps; maxidx = 8; }
                else if(piece == mWPW){ steps = cBoard::wpw_steps; maxidx = 3; }
                else if(piece == mBPW){ steps = cBoard::bpw_steps; maxidx = 3; }

                //cout << "-----" << endl;
                //cout << PIECES_STR[piece] << endl;

                for(int i = 0; i < maxidx; ++i){
                    cStep step = steps[i];
                    uint64_t newpos = pos;

                    for(int k = 0; k < step.stepcnt; ++k){
                        if(cpin->is_pinned(pos, step.dir)){
                            break;
                        }

                        if((newpos & step.border) > 0){
                            break;
                        }

                        if(step.rightshift){
                            newpos = (newpos >> step.shiftcnt);
                        }
                        else{
                            newpos = (newpos << step.shiftcnt);
                        }

                        if(cBoard::is_piece_white(piece) && (match->board.field[mIDX_WHITE] & newpos) > 0){
                            break;
                        }
                        else if(cBoard::is_piece_black(piece) && (match->board.field[mIDX_BLACK] & newpos) > 0){
                            break;
                        }
                        else{
                            if(piece == mWPW){
                                if(match->board.tst_wpw_move(pos, newpos)){
                                    add_pw_moves(pos, newpos, moves);
                                }
                                else if(match->board.tst_en_passant_move(pos, newpos, match->minutes)){
                                    cGMove move(pos, newpos, mBLK);
                                    score_interesting(move);
                                    move.presort -= 5; // score e.p. capture
                                    moves.push_back(move);
                                }
                            }
                            else if(piece == mBPW){
                                if(match->board.tst_bpw_move(pos, newpos)){
                                    add_pw_moves(pos, newpos, moves);
                                }
                                else if(match->board.tst_en_passant_move(pos, newpos, match->minutes)){
                                    cGMove move(pos, newpos, mBLK);
                                    score_interesting(move);
                                    move.presort -= 5; // score e.p. capture
                                    moves.push_back(move);
                                }
                            }
                            else if(piece == mWKG || piece == mBKG){
                                if(match->board.tst_kg_move(pos, newpos, match->minutes)){
                                    cGMove move(pos, newpos, mBLK);
                                    score_interesting(move);
                                    if(is_castling(move)){ move.presort -= 5; }
                                    moves.push_back(move);
                                }
                            }
                            else{
                                cGMove move(pos, newpos, mBLK);
                                score_interesting(move);
                                moves.push_back(move);
                            }
                            if((match->board.field[mIDX_WHITE] & newpos) > 0 || 
                               (match->board.field[mIDX_BLACK] & newpos) > 0){
                                break;
                            }
                        }
                    }
                }
            }

            pos = (pos >> 1);
        }
    }


    void cGenerator::gen_kg_moves(uint8_t color, list <cGMove> &moves){
        uint64_t kg_pos;
        uint8_t king, enemycolor;
        bool tstsquare;

        if(color == mWHITE){
            kg_pos = match->board.read_wkg_pos();
            enemycolor = mBLACK;
        }
        else{
            kg_pos = match->board.read_bkg_pos();
            enemycolor = mWHITE;
        }
        king = match->board.read(kg_pos);

        for(int i = 0; i < 8; ++i){
            uint64_t newpos = kg_pos;
            cStep step = cBoard::kg_steps_generic[i];

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

                if(cBoard::is_piece_white(king) && (match->board.field[mIDX_WHITE] & newpos) > 0){
                    break;
                }
                else if(cBoard::is_piece_black(king) && (match->board.field[mIDX_BLACK] & newpos) > 0){
                    break;
                }
                else{
                    match->board.write(kg_pos, mBLK);
                    tstsquare = match->board.is_square_enemy_touched(enemycolor, newpos);
                    match->board.write(kg_pos, king);

                    if(tstsquare == false){
                        cGMove move(kg_pos, newpos, mBLK);
                        score_interesting(move);
                        move.presort -= 5; // score for fleeing check
                        moves.push_back(move);
                    }
                }
            }
        }
    }


    void cGenerator::gen_kg_support_moves(list <cLink *> &attackers, list <cGMove> &moves){
        if(attackers.size() != 1){
            return;
        }

        uint64_t kg_pos, enemy_pos;
        uint8_t king, color;

        cLink *clink = attackers.front();
        kg_pos = clink->posA;
        king = clink->pieceA;
        enemy_pos = clink->posB;
        const cStep *steps;

        if(cBoard::is_piece_white(king)){
            color = mWHITE;
            steps = cBoard::steps_for_wkg_support_search;
        }
        else{
            color = mBLACK;
            steps = cBoard::steps_for_bkg_support_search;
        }

        cPin *cpin = match->board.determine_pins(color);

        uint64_t pos = enemy_pos;
        cStep outerstep;
        if(clink->pieceA == mWKG || clink->pieceA == mBKG){
            uint8_t reversed_dir = cBoard::reverse_dir(clink->dirAB);
            outerstep = cBoard::step_for_dir(reversed_dir);
        }
        else{
            outerstep = cBoard::step_for_dir(clink->dirAB);
        }

        while(pos != kg_pos){
            for(uint8_t i = 0; i < 19; ++i){
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

                    if(match->board.is_square_blank(newpos)){
                        continue;
                    }

                    uint8_t piece = match->board.read(newpos);

                    if((match->board.field[mIDX_WHITE] & newpos) > 0 && color == mWHITE){
                        if(cpin->is_pinned(pos, step.dir)){
                            break;
                        }
                        if(step.owner == STEP_OWNER["wpawn"] && piece == mWPW){
                            if(match->board.tst_wpw_move(newpos, pos)){
                                add_pw_moves(newpos, pos, moves);
                            }
                            break;
                        }
                        if((step.owner == STEP_OWNER["rook"] && (piece == mWRK || piece == mWQU)) ||
                           (step.owner == STEP_OWNER["bishop"] && (piece == mWBP || piece == mWQU)) ||
                           (step.owner == STEP_OWNER["knight"] && piece == mWKN)){
                            cGMove move(newpos, pos, mBLK);
                            score_interesting(move);
                            move.presort -= 5; // score for defending check
                            moves.push_back(move);
                            break;
                        }
                    }
                    else if((match->board.field[mIDX_BLACK] & newpos) > 0 && color == mBLACK){
                        if(cpin->is_pinned(pos, step.dir)){
                            break;
                        }
                        if(step.owner == STEP_OWNER["bpawn"] && piece == mBPW){
                            if(match->board.tst_bpw_move(newpos, pos)){
                                add_pw_moves(newpos, pos, moves);
                            }
                            break;
                        }
                        if((step.owner == STEP_OWNER["rook"] && (piece == mBRK || piece == mBQU)) ||
                           (step.owner == STEP_OWNER["bishop"] && (piece == mBBP || piece == mBQU)) ||
                           (step.owner == STEP_OWNER["knight"] && piece == mBKN)){
                            cGMove move(newpos, pos, mBLK);
                            score_interesting(move);
                            move.presort -= 5; // score for defending check
                            moves.push_back(move);
                            break;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            if((outerstep.border & pos) > 0){
                return;
            }

            if(outerstep.rightshift){
                pos = (pos >> outerstep.shiftcnt);
            }
            else{
                pos = (pos << outerstep.shiftcnt);
            }
        }
    }
  

    void cGenerator::add_pw_moves(uint64_t src, uint64_t dst, list <cGMove> &moves){
        if((dst & 0xFF00000000000000) > 0){
            uint8_t pieces[] = { mWQU, mWRK, mWBP, mWKN };

            for(uint8_t i = 0; i < 4; ++i){
                cGMove move(src, dst, pieces[i]);
                score_interesting(move);
                move.presort -= 20; // score for promotion
                moves.push_back(move);
            }
        }
        else if((dst & 0x00000000000000FF) > 0){
            uint8_t pieces[] = { mBQU, mBRK, mBBP, mBKN };

            for(uint8_t i = 0; i < 4; ++i){
                cGMove move(src, dst, pieces[i]);
                score_interesting(move);
                move.presort -= 20; // score for promotion
                moves.push_back(move);
            }
        }
        else{
                cGMove move(src, dst, mBLK);
                score_interesting(move);
                moves.push_back(move);
        }
    }


    bool cGenerator::is_castling(cGMove &move){
        return false;
    }


    void cGenerator::score_interesting(cGMove &move){
        set_score_capture(move); 

        if(is_forking(move)){
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(cGMove::PRESORT_HIGH, move.presort);
            }
        }

        if(is_defending_fork(move)){
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(cGMove::PRESORT_HIGH, move.presort);
            }
        }

        if(is_fleeing(move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(cGMove::PRESORT_HIGH, move.presort);
            };
        }

        if(is_unpin(move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(cGMove::PRESORT_HIGH, move.presort);
            }
        }

        if(is_blocking(move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(cGMove::PRESORT_HIGH, move.presort);
            }
        }

        if(is_running_pawn(move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(cGMove::PRESORT_HIGH, move.presort);
            }
        }

        if(is_tactical_draw(move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(cGMove::PRESORT_HIGH, move.presort);
            }
        }
    }


    void cGenerator::set_score_capture(cGMove &move){
        uint8_t src_piece = match->board.read(move.src);
        uint8_t dst_piece = match->board.read(move.dst);
         
        if(dst_piece == mBLK){ 
            move.presort = cGMove::PRESORT_MEDIUM; 
            return; 
        }

        if(PIECES_RANKS[src_piece] < PIECES_RANKS[dst_piece]){ 
            uint8_t diff = PIECES_RANKS[PIECES["wBp"]] - PIECES_RANKS[PIECES["wPw"]];

            if(diff + PIECES_RANKS[src_piece] < PIECES_RANKS[dst_piece]){ 
                move.presort = cGMove::PRESORT_VERY_HIGH; 
            }
            else{
                move.presort = cGMove::PRESORT_HIGH; 
            }
            return; 
        }

        if(PIECES_RANKS[src_piece] == PIECES_RANKS[dst_piece]){ 
            move.presort = cGMove::PRESORT_MEDIUM; 
            return; 
        }

        if(PIECES_RANKS[src_piece] > PIECES_RANKS[dst_piece]){ 
            move.presort = cGMove::PRESORT_LOW; 
            return; 
        }
    }


    bool cGenerator::is_forking(cGMove &move){
        return false;
    }


    bool cGenerator::is_defending_fork(cGMove &move){
        return false;
    }


    bool cGenerator::is_fleeing(cGMove &move){
        return false;
    }


    bool cGenerator::is_unpin(cGMove &move){
        return false;
    }


    bool cGenerator::is_blocking(cGMove &move){
        return false;
    }


    bool cGenerator::is_running_pawn(cGMove &move){
        return false;
    }


    bool cGenerator::is_tactical_draw(cGMove &move){
        return false;
    }

