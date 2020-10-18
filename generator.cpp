
    #include "./generator.hpp"


    void gen_moves(cMatch &match, list<cGMove *> &moves){
        uint64_t pos = mPOS_A1;
        uint8_t color = match.next_color();
        const cStep *steps;

        list <cLink *> attackers;
        match.board.determine_checks(color, attackers);

        if(attackers.size() > 1){
            gen_kg_moves(color, match, moves);
            return;
        }
        else if(attackers.size() == 1){
            gen_kg_moves(color, match, moves);
            gen_kg_support_moves(match, attackers, moves);
            return;
        }

        cPin *cpin = match.board.determine_pins(color);

        while(pos > 0){
            uint8_t piece = match.board.read(pos);
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

                for(int i = 0; i < maxidx; ++i){
                    cStep step = steps[i];
                    uint64_t newpos = pos;

                    for(int k = 0; k < step.stepcnt; ++k){
                        if((newpos & step.border) > 0){
                            break;
                        }

                        if(cpin->is_dir_pinned(pos, step.dir)){
                            break;
                        }

                        if(step.rightshift){
                            newpos = (newpos >> step.shiftcnt);
                        }
                        else{
                            newpos = (newpos << step.shiftcnt);
                        }

                        if(cBoard::is_piece_white(piece) && match.board.is_square_white_occupied(newpos)){
                            break;
                        }
                        else if(cBoard::is_piece_black(piece) && match.board.is_square_black_occupied(newpos)){
                            break;
                        }
                        else{
                            if(piece == mWPW){
                                if(match.board.tst_wpw_move(pos, newpos)){
                                    add_pw_moves(match, pos, newpos, moves);
                                }
                                else if(match.board.tst_en_passant_move(pos, newpos, match.minutes)){
                                    cGMove *move = new cGMove(pos, newpos, mBLK);
                                    score_move_presort(match, *move);
                                    moves.push_back(move);
                                }
                            }
                            else if(piece == mBPW){
                                if(match.board.tst_bpw_move(pos, newpos)){
                                    add_pw_moves(match, pos, newpos, moves);
                                }
                                else if(match.board.tst_en_passant_move(pos, newpos, match.minutes)){
                                    cGMove *move = new cGMove(pos, newpos, mBLK);
                                    score_move_presort(match, *move);
                                    move->presort -= 5; // score e.p. capture
                                    moves.push_back(move);
                                }
                            }
                            else if(piece == mWKG || piece == mBKG){
                                if(match.board.tst_kg_move(pos, newpos, match.minutes)){
                                    cGMove *move = new cGMove(pos, newpos, mBLK);
                                    score_move_presort(match, *move);
                                    if(is_move_castling(*move)){ move->presort -= 10; }
                                    moves.push_back(move);
                                }
                            }
                            else{
                                cGMove *move = new cGMove(pos, newpos, mBLK);
                                score_move_presort(match, *move);
                                moves.push_back(move);
                            }
                            if((match.board.field[mIDX_WHITE] & newpos) > 0 || 
                               (match.board.field[mIDX_BLACK] & newpos) > 0){
                                break;
                            }
                        }
                    }
                }
            }

            pos = (pos >> 1);
        }
        delete cpin;
    }


    void gen_kg_moves(uint8_t color, cMatch &match, list <cGMove *> &moves){
        uint64_t kg_pos;
        uint8_t king, enemycolor;
        bool tstsquare;

        if(color == mWHITE){
            kg_pos = match.board.read_wkg_pos();
            enemycolor = mBLACK;
        }
        else{
            kg_pos = match.board.read_bkg_pos();
            enemycolor = mWHITE;
        }
        king = match.board.read(kg_pos);

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

                if(cBoard::is_piece_white(king) && (match.board.field[mIDX_WHITE] & newpos) > 0){
                    break;
                }
                else if(cBoard::is_piece_black(king) && (match.board.field[mIDX_BLACK] & newpos) > 0){
                    break;
                }
                else{
                    match.board.write(kg_pos, mBLK);
                    tstsquare = match.board.is_square_enemy_touched(enemycolor, newpos);
                    match.board.write(kg_pos, king);

                    if(tstsquare == false){
                        cGMove *move = new cGMove(kg_pos, newpos, mBLK);
                        score_move_presort(match, *move);
                        move->presort -= 5; // score for fleeing check
                        moves.push_back(move);
                    }
                }
            }
        }
    }


    void gen_kg_support_moves(cMatch &match, list <cLink *> &attackers, list <cGMove *> &moves){
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

        cPin *cpin = match.board.determine_pins(color);

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

                    if(newpos == kg_pos){
                        break;
                    }

                    if(match.board.is_square_blank(newpos)){
                        continue;
                    }

                    uint8_t piece = match.board.read(newpos);

                    if(match.board.is_square_white_occupied(newpos) && color == mWHITE){
                        if(cpin->is_dir_pinned(pos, step.dir)){
                            break;
                        }
                        if(step.owner == STEP_OWNER["wpawn"] && piece == mWPW){
                            if(match.board.tst_wpw_move(newpos, pos)){
                                add_pw_moves(match, newpos, pos, moves);
                            }
                            break;
                        }
                        if((step.owner == STEP_OWNER["rook"] && (piece == mWRK || piece == mWQU)) ||
                           (step.owner == STEP_OWNER["bishop"] && (piece == mWBP || piece == mWQU)) ||
                           (step.owner == STEP_OWNER["knight"] && piece == mWKN)){
                            cGMove *move = new cGMove(newpos, pos, mBLK);
                            score_move_presort(match, *move);
                            move->presort -= 5; // score for defending check
                            moves.push_back(move);
                            break;
                        }
                    }
                    else if(match.board.is_square_black_occupied(newpos) && color == mBLACK){
                        if(cpin->is_dir_pinned(pos, step.dir)){
                            break;
                        }
                        if(step.owner == STEP_OWNER["bpawn"] && piece == mBPW){
                            if(match.board.tst_bpw_move(newpos, pos)){
                                add_pw_moves(match, newpos, pos, moves);
                            }
                            break;
                        }
                        if((step.owner == STEP_OWNER["rook"] && (piece == mBRK || piece == mBQU)) ||
                           (step.owner == STEP_OWNER["bishop"] && (piece == mBBP || piece == mBQU)) ||
                           (step.owner == STEP_OWNER["knight"] && piece == mBKN)){
                            cGMove *move = new cGMove(newpos, pos, mBLK);
                            score_move_presort(match, *move);
                            move->presort -= 5; // score for defending check
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
                delete cpin;
                return;
            }

            if(outerstep.rightshift){
                pos = (pos >> outerstep.shiftcnt);
            }
            else{
                pos = (pos << outerstep.shiftcnt);
            }
        }

        delete cpin;
    }
  

    void add_pw_moves(cMatch &match, uint64_t src, uint64_t dst, list <cGMove *> &moves){
        if((dst & 0xFF00000000000000) > 0){
            uint8_t pieces[] = { mWQU, mWRK, mWBP, mWKN };

            for(uint8_t i = 0; i < 4; ++i){
                cGMove *move = new cGMove(src, dst, pieces[i]);
                score_move_presort(match, *move);
                move->presort -= 20; // score for promotion
                moves.push_back(move);
            }
        }
        else if((dst & 0x00000000000000FF) > 0){
            uint8_t pieces[] = { mBQU, mBRK, mBBP, mBKN };

            for(uint8_t i = 0; i < 4; ++i){
                cGMove *move = new cGMove(src, dst, pieces[i]);
                score_move_presort(match, *move);
                move->presort -= 20; // score for promotion
                moves.push_back(move);
            }
        }
        else{
                cGMove *move = new cGMove(src, dst, mBLK);
                score_move_presort(match, *move);
                moves.push_back(move);
        }
    }


    void score_move_presort(cMatch &match, cGMove &move){
        list<uint64_t> white_touches_on_dst, black_touches_on_dst;
        uint8_t dst_support = determine_level_for_move_dstfield(match, move, white_touches_on_dst, black_touches_on_dst);

        set_score_for_capture_move(match, move, dst_support);

        if(dst_support == SUPPORT_LEVELS["weak"]){
            move.presort = min(move.presort, cGMove::PRESORT_LOW);
        }
        else{
            uint8_t level_for_supp_att = determine_level_for_move_supportings_or_attackings(match, move);
            set_score_for_supporting_attacking_move(move, level_for_supp_att);
        }

        if(does_move_clear_for_supply(match, move)){
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 15; // -5 for double -10 for supply
            }
            else{
                move.presort = min(move.presort, (uint8_t)(cGMove::PRESORT_HIGH - 10));
            }
        }

        if(is_move_forking(match, move)){
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(move.presort, cGMove::PRESORT_HIGH);
            }
        }

        if(is_move_defending_fork(match, move)){
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(move.presort, cGMove::PRESORT_HIGH);
            }
        }

        if(is_move_fleeing(match, move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(move.presort, cGMove::PRESORT_HIGH);
            };
        }

        if(is_move_unpin(match, move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(move.presort, cGMove::PRESORT_HIGH);
            }
        }

        if(is_move_blocking(match, move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(move.presort, cGMove::PRESORT_HIGH);
            }
        }

        if(is_move_running_pawn(match, move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(move.presort, cGMove::PRESORT_HIGH);
            }
        }

        if(is_move_tactical_draw(match, move)){ 
            if(move.presort < cGMove::PRESORT_HIGH){ 
                move.presort -= 5; 
            }
            else{
                move.presort = min(move.presort, cGMove::PRESORT_HIGH);
            }
        }
    }


    bool does_capture_eliminate_supporter_attacker(cMatch &match, cGMove &move){
        //uint8_t srcpiece = match.board.read(move.src);
        uint8_t dstpiece = match.board.read(move.dst);
        const cStep *steps;
        uint8_t maxidx;

        if(dstpiece == mWRK || dstpiece == mBRK){ steps = cBoard::rk_steps; maxidx = 4; }
        else if(dstpiece == mWBP || dstpiece == mBBP){ steps = cBoard::bp_steps; maxidx = 4; }
        else if(dstpiece == mWQU || dstpiece == mBQU){ steps = cBoard::qu_steps; maxidx = 8; }
        else if(dstpiece == mWKG || dstpiece == mBKG){ steps = cBoard::kg_steps_generic; maxidx = 8; }
        else if(dstpiece == mWKN || dstpiece == mBKN){ steps = cBoard::kn_steps; maxidx = 8; }
        else if(dstpiece == mWPW){ steps = cBoard::wpw_steps_attack; maxidx = 2; }
        else if(dstpiece == mBPW){ steps = cBoard::bpw_steps_attack; maxidx = 2; }
        else{ return false; }

        for(uint8_t i = 0; i < maxidx; ++i){
            cStep step = steps[i];

            uint64_t newpos = move.dst;

            list<uint64_t> white_pos, black_pos;

            for(uint8_t k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }
                
                if(newpos == move.src){
                    break;
                }

                if(match.board.is_square_blank(newpos)){
                    continue;
                }

                uint8_t touched_piece = match.board.read(newpos);

                cPin *cpin = match.board.determine_pins(cBoard::color_of_piece(touched_piece));
                if(cpin->is_pinned(newpos) > 0){
                    return true;
                }

                match.board.determine_touches_on_square(newpos, white_pos, black_pos);

                if(cBoard::color_of_piece(dstpiece) == cBoard::color_of_piece(touched_piece)){
                    if(touched_piece == mWKG || touched_piece == mBKG){
                        break;
                    }
                    if(cBoard::is_piece_white(touched_piece)){
                        if(black_pos.size() > 0 &&
                           white_pos.size() >= black_pos.size()){
                            return true;
                        }
                    }
                    else{
                        if(white_pos.size() > 0 && 
                           black_pos.size() >= white_pos.size()){
                            return true;
                        }
                    }
                    break;
                }
                else{
                    if(PIECES_RANKS[dstpiece] < PIECES_RANKS[touched_piece]){
                        return true;
                    }
                    if(cBoard::is_piece_white(touched_piece)){
                        if(white_pos.size() <= black_pos.size()){
                            return true;
                        }
                    }
                    else{
                        if(black_pos.size() <= white_pos.size()){
                            return true;
                        }
                    }
                    break;
                }
            }
        }

        return false;
    }


    void set_score_for_capture_move(cMatch &match, cGMove &move, uint8_t dst_support){
        uint8_t src_piece = match.board.read(move.src);
        uint8_t dst_piece = match.board.read(move.dst);

        if(cBoard::is_move_capture(move.src, src_piece, move.dst, dst_piece) == false){
            return;
        }

        bool exchange = false;
        uint64_t captured_pos = 0;
        uint8_t cnt = 0;
        for(list<cMove>::reverse_iterator it = match.minutes.rbegin(); it != match.minutes.rend(); ++it){
            if(it->type == MOVE_TYPE["capture"] || it->type == MOVE_TYPE["en-passant"]){
                if(cnt == 0){
                    captured_pos = it->dst;
                }
                exchange = true;
                break;
            }
            ++cnt;
            if(cnt >= 2){ break; }
        }

        int8_t adjust = 0;
        if(exchange){
            if(captured_pos == move.dst){
                move.presort = min((int)move.presort, (int)cGMove::PRESORT_STORMY);
                return;
            }
            adjust = cGMove::PRESORT_STEP;
        }

        // en passant
        if(dst_piece == mBLK){ 
            move.presort = min(move.presort, (uint8_t)(cGMove::PRESORT_HIGH - adjust));
            return;
        }

        // capture of higher peace is always fine!
        if(PIECES_RANKS[src_piece] < PIECES_RANKS[dst_piece]){
            move.presort = min(move.presort, cGMove::PRESORT_STORMY);
            return;
        }
        
        if(does_capture_eliminate_supporter_attacker(match, move)){
            move.presort = min(move.presort, cGMove::PRESORT_STORMY);
            return;
        }

        if(PIECES_RANKS[src_piece] == PIECES_RANKS[dst_piece]){
            if(dst_support == SUPPORT_LEVELS["good"] || dst_support == SUPPORT_LEVELS["none"]){
                move.presort = min(move.presort, cGMove::PRESORT_STORMY);
                return;
            }
            else if(dst_support == SUPPORT_LEVELS["equal"]){
                move.presort = min(move.presort, (uint8_t)(cGMove::PRESORT_HIGH - adjust));
                return;
            }
            else{ // dst_support == SUPPORT_LEVELS["low"]
                move.presort = min(move.presort, (uint8_t)(cGMove::PRESORT_MEDIUM - adjust));
                return;
            }
        }
        else{ // PIECES_RANKS[src_piece] > PIECES_RANKS[dst_piece]
            if(PIECES_RANKS[src_piece] <= PIECES_RANKS[dst_piece] + PIECES_RANKS[PIECES["wPw"]] &&
               (dst_support == SUPPORT_LEVELS["good"] || dst_support == SUPPORT_LEVELS["none"])){
                move.presort = min(move.presort, (uint8_t)(cGMove::PRESORT_MEDIUM - adjust));
                return;
            }
            else{
                move.presort = min(move.presort, (uint8_t)(cGMove::PRESORT_LOW - adjust));
                return;
            }
        }
    }


    void set_score_for_supporting_attacking_move(cGMove &move, uint8_t level_for_supp_att){
        if(level_for_supp_att == 3){
            move.presort = min(move.presort, (uint8_t)(cGMove::PRESORT_HIGH - 10));
            return;
        }
        else if(level_for_supp_att == 2){
            move.presort = min(move.presort, (uint8_t)(cGMove::PRESORT_MEDIUM - 10));
            return;
        }
    }


    bool is_move_castling(cGMove &move){
        return ((move.src >> 2) == move.dst) || ((move.src << 2) == move.dst);
    }


    uint8_t search_lowest(cMatch &match, list<uint64_t> &touches){
        uint8_t lowest = mWKG;

        for(uint64_t pos : touches){
            uint8_t piece = match.board.read(pos);
            if(PIECES_RANKS[piece] < PIECES_RANKS[lowest]){
                lowest = piece;
            }
        }
        return lowest;
    }

    uint8_t determine_level_for_move_dstfield(cMatch &match, cGMove &move, list<uint64_t> &white_touches_on_dst, list<uint64_t> &black_touches_on_dst){
        uint8_t piece = match.board.read(move.src);

        match.board.write(move.src, mBLK);
        match.board.determine_touches_on_square(move.dst, white_touches_on_dst, black_touches_on_dst);
        match.board.write(move.src, piece);

        uint8_t lowest;
        if(cBoard::color_of_piece(piece) == mWHITE){
            lowest = search_lowest(match, black_touches_on_dst);
        }
        else{
            lowest = search_lowest(match, white_touches_on_dst);
        }
        if(PIECES_RANKS[piece] > PIECES_RANKS[lowest]){
            return SUPPORT_LEVELS["weak"];
        }

        if(white_touches_on_dst.size() < black_touches_on_dst.size()){
            if(cBoard::is_piece_white(piece)){
                return SUPPORT_LEVELS["weak"];
            }
            else{
                return SUPPORT_LEVELS["good"];
            }
        }
        else if(white_touches_on_dst.size() > black_touches_on_dst.size()){
            if(cBoard::is_piece_white(piece)){
                return SUPPORT_LEVELS["good"];
            }
            else{
                return SUPPORT_LEVELS["weak"];
            }
        }
        else{
            if((white_touches_on_dst.size() == 0 && cBoard::is_piece_white(piece)) ||
               (black_touches_on_dst.size() == 0 && cBoard::is_piece_black(piece))){
                return SUPPORT_LEVELS["none"];
            }
            else{
                return SUPPORT_LEVELS["equal"];
            }
        }
    }


    uint8_t determine_level_for_move_supportings_or_attackings(cMatch &match, cGMove &move){
        uint8_t srcpiece = match.board.read(move.src);
        uint8_t dstpiece = match.board.read(move.dst);
        const cStep *steps;
        uint8_t maxidx;
        uint8_t mvdir = mUNDEF;
        uint8_t level = 0; // none == 0, bad == 1, medium == 2, good == 3

        if(srcpiece == mWRK || srcpiece == mBRK){ 
            steps = cBoard::rk_steps; 
            maxidx = 4; 
            if(dstpiece == mBLK){ mvdir = cBoard::dir_for_move(move.src, move.dst); }
        }
        else if(srcpiece == mWBP || srcpiece == mBBP){ 
            steps = cBoard::bp_steps; 
            maxidx = 4; 
            if(dstpiece == mBLK){ mvdir = cBoard::dir_for_move(move.src, move.dst); }
        }
        else if(srcpiece == mWQU || srcpiece == mBQU){ 
            steps = cBoard::qu_steps; 
            maxidx = 8; 
            if(dstpiece == mBLK){ mvdir = cBoard::dir_for_move(move.src, move.dst); }
        }
        else if(srcpiece == mWKG || srcpiece == mBKG){ steps = cBoard::kg_steps_generic; maxidx = 8; }
        else if(srcpiece == mWKN || srcpiece == mBKN){ steps = cBoard::kn_steps; maxidx = 8; }
        else if(srcpiece == mWPW){ steps = cBoard::wpw_steps_attack; maxidx = 2; }
        else if(srcpiece == mBPW){ steps = cBoard::bpw_steps_attack; maxidx = 2; }
        else{ return false; }

        for(uint8_t i = 0; i < maxidx; ++i){
            cStep step = steps[i];
            
            if(step.dir == mvdir || step.dir == cBoard::reverse_dir(mvdir)){
                continue;
            }

            uint64_t newpos = move.dst;

            list<uint64_t> white_pos, black_pos;

            for(uint8_t k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }
                
                if(newpos == move.src){
                    break;
                }

                if(match.board.is_square_blank(newpos)){
                    continue;
                }

                uint8_t touched_piece = match.board.read(newpos);

                match.board.write(move.src, mBLK);
                match.board.determine_touches_on_square(newpos, white_pos, black_pos);
                match.board.write(move.src, srcpiece);

                cPin *cpin = match.board.determine_pins(cBoard::color_of_piece(touched_piece));
                if(cpin->is_pinned(newpos) > 0){
                    return 3;
                }

                if(cBoard::is_piece_white(srcpiece)){
                    if(cBoard::color_of_piece(touched_piece) == mBLACK){
                        if(PIECES_RANKS[srcpiece] < PIECES_RANKS[touched_piece]){
                            return 3;
                        }
                        else if(white_pos.size() >= black_pos.size()){
                            level = max((int)level, 2);
                            break;
                        }
                        else{
                            level = max((int)level, 1);
                            break;
                        }
                    }
                    else{
                        if(srcpiece == mWKG || srcpiece == mBKG){
                            break;
                        }
                        else if(white_pos.size() < black_pos.size()){
                            return 3;
                        }
                        else if(black_pos.size() > 0){
                            level = max((int)level, 2);
                            break;
                        }
                        else{
                            level = max((int)level, 1);
                            break;
                        }
                    }
                }
                else{
                    if(cBoard::color_of_piece(touched_piece) == mWHITE){
                        if(PIECES_RANKS[srcpiece] < PIECES_RANKS[touched_piece]){
                            return 3;
                        }
                        else if(black_pos.size() >= white_pos.size()){
                            level = max((int)level, 2);
                            break;
                        }
                        else{
                            level = max((int)level, 1);
                            break;
                        }
                    }
                    else{
                        if(srcpiece == mWKG || srcpiece == mBKG){
                            break;
                        }
                        else if(black_pos.size() < white_pos.size()){
                            return 3;
                        }
                        else if(white_pos.size() > 0){
                            level = max((int)level, 2);
                            break;
                        }
                        else{
                            level = max((int)level, 1);
                            break;
                        }
                    }
                }
            }
        }
        if(srcpiece != mWKG && srcpiece != mBKG){
            return level;
        }

        if((move.src >> 2) == move.dst){
            cGMove rkmove((move.dst >> 1), (move.dst << 1), mBLK);
            return determine_level_for_move_supportings_or_attackings(match, rkmove);
        }

        if((move.src << 2) == move.dst){
            cGMove rkmove((move.dst << 2), (move.dst >> 1), mBLK);
            return determine_level_for_move_supportings_or_attackings(match, rkmove);
        }

        return level;
    }


    bool has_piece_double_attack_in_dir(cMatch &match, uint64_t pos, uint8_t dir){
        uint8_t srcpiece = match.board.read(pos);

        const cStep step = cBoard::step_for_dir(dir);

        bool first = false;

        uint64_t newpos = pos;

        for(uint8_t k = 0; k < step.stepcnt; ++k){
            if((newpos & step.border) > 0){
                break;
            }

            if(step.rightshift){
                newpos = (newpos >> step.shiftcnt);
            }
            else{
                newpos = (newpos << step.shiftcnt);
            }

            if(match.board.is_square_blank(newpos)){
                continue;
            }

            uint8_t found_piece = match.board.read(newpos);
            if(cBoard::color_of_piece(srcpiece) == cBoard::color_of_piece(found_piece)){
                return false;
            }
            else{
                if(first){ 
                    return true; 
                }
                else{
                    first = true;
                }
            }
        }

        return false;
    }


    bool does_move_clear_for_supply(cMatch &match, cGMove &move){
        if(match.board.read(move.dst) == mBLK){
            return false;
        }

        uint8_t srcpiece = match.board.read(move.src);

        if(srcpiece == mWPW || srcpiece == mBPW ||
           srcpiece == mWKG || srcpiece == mBKG ||
           srcpiece == mWKN || srcpiece == mBKN){ 
            return false; 
        }

        uint8_t dir = cBoard::dir_for_move(move.src, move.dst);
        if(has_piece_double_attack_in_dir(match, move.src, dir) == false){
            return false;
        }

        for(uint8_t i = 0; i < 8; ++i){
            cStep step = cBoard::qu_steps[i];

            uint64_t newpos = move.src;

            for(uint8_t k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }

                if(newpos == move.dst){
                    break;
                }

                if(match.board.is_square_blank(newpos)){
                    continue;
                }

                uint8_t found_piece = match.board.read(newpos);
                if(srcpiece == found_piece){
                    return true;
                }
                else if(cBoard::color_of_piece(srcpiece) == cBoard::color_of_piece(found_piece) &&
                        (found_piece == mWQU || found_piece == mBQU)){
                    return true;
                }
                else{
                    break;
                }
            }
        }

        return false;
    }


    bool is_move_forking(cMatch &match, cGMove &move){
        // ???
        return false;
    }


    bool is_move_defending_fork(cMatch &match, cGMove &move){
        // ???
        return false;
    }


    bool is_move_fleeing(cMatch &match, cGMove &move){
        // ???
        return false;
    }


    bool is_move_unpin(cMatch &match, cGMove &move){
        // ???
        return false;
    }


    bool is_move_blocking(cMatch &match, cGMove &move){
        // ???
        return false;
    }


    bool is_move_running_pawn(cMatch &match, cGMove &move){
        // ???
        return false;
    }


    bool is_move_tactical_draw(cMatch &match, cGMove &move){
        // ???
        return false;
    }
