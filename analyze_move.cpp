

    #include "./analyze_move.hpp"


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

