
    #include <cstdint>
    #include "./piece_ext2.hpp"
    #include "./searchforpiece.hpp"
    #include "../values.hpp"

    bool is_move_valid_from_ext(cPiece *cpiece, int dst, int prompiece, list<cMove> &minutes){
        if(cpiece->piece == mWPW){
            return is_move_valid_for_whitepawn(cpiece, dst, prompiece, minutes);
        }
        if(cpiece->piece == mBPW){
            return is_move_valid_for_blackpawn(cpiece, dst, prompiece, minutes);
        }
        if(cpiece->piece == mWKN || cpiece->piece == mBKN){
            return is_move_valid_for_knight(cpiece, dst, prompiece);
        }
        if(cpiece->piece == mWKG || cpiece->piece == mBKG){
            return is_move_valid_for_king(cpiece, dst, prompiece);
        }
        else{
            return is_move_valid_for_long_distance_piece(cpiece, dst, prompiece);
        }
    }

    // White Pawn
    bool is_move_valid_for_whitepawn(cPiece *cpiece, int dst, int prompiece, list<cMove> &minutes){
        // check move step
        bool flag = false;
        for(auto &step : cpiece->get_mv_steps()){
            if(cpiece->pos + step == dst && cBoard::is_inbounds(cpiece->pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){ return false; }

        // check double step from second rank
        if(dst - cpiece->pos == 16 && cpiece->pos > 15){ return false; }

        int move_dir = cPiece::dir_for_move(mWPW, cpiece->pos, dst);
        if(move_dir == DIRS["undef"]){ return false; }

        int pin_dir = cpiece->board->eval_pin_dir(cpiece->pos);
        int dstpiece = cpiece->board->getfield(dst);

        // check pins
        if(pin_dir != DIRS["undef"]){
            if(move_dir == DIRS["nth"]){
                if(pin_dir != DIRS["nth"] && pin_dir != DIRS["sth"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
            if(move_dir == DIRS["nth-wst"]){
                if(pin_dir != DIRS["nth-wst"] && pin_dir != DIRS["sth-est"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
            if(move_dir == DIRS["nth-est"]){
                if(pin_dir != DIRS["nth-est"] && pin_dir != DIRS["sth-wst"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
        }

        // check fields
        if(move_dir == DIRS["nth"]){ 
            if(dstpiece != mBLK){ return false; }
            if(dst - cpiece->pos == 16){
                int midpiece = cpiece->board->getfield(cpiece->pos + 8);
                if(midpiece != mBLK){ return false; }
            }
        }
        if(move_dir == DIRS["nth-est"] || move_dir == DIRS["nth-wst"]){
            if(PIECES_COLORS[dstpiece] != COLORS["black"]){
                return is_ep_move_ok_for_whitepawn(cpiece, dst, minutes);
            }
        }

        // check promotion
        if((dst / 8) == 7 && prompiece != mWQU && prompiece != mWRK && 
           prompiece != mWBP && prompiece != mWKN){ return false; }

        if((dst / 8) < 7 && prompiece != mBLK){ return false; }

        return true;
    }

    bool is_ep_move_ok_for_whitepawn(cPiece *piece, int dst, list<cMove> &minutes){
        cMove lastmove;
        if(minutes.empty()){ 
            return false; 
        }
        else{
            lastmove = minutes.back();
        }

        int dstpiece = piece->board->getfield(dst);
        int enemy = piece->board->getfield(lastmove.dst);
        if(dstpiece == mBLK && enemy == mBPW){
            if((lastmove.src / 8) - (lastmove.dst / 8) == 2 && 
               (lastmove.dst / 8) == (piece->pos / 8) && 
               (lastmove.dst % 8) == (dst % 8) && 
               (lastmove.dst / 8) - (dst / 8) == -1){
                return true;
            }
        }
        return false;
    }
    // White Pawn end


    // Black Pawn
    bool is_move_valid_for_blackpawn(cPiece *cpiece, int dst, int prompiece, list<cMove> &minutes){
        // check move step
        bool flag = false;
        for(auto &step : cpiece->get_mv_steps()){
            if(cpiece->pos + step == dst && cBoard::is_inbounds(cpiece->pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){ return false; }

        // check double step from seventh rank
        if(cpiece->pos - dst == 16 && cpiece->pos < 48){ return false; }

        int move_dir = cPiece::dir_for_move(mBPW, cpiece->pos, dst);
        if(move_dir == DIRS["undef"]){ return false; }

        int pin_dir = cpiece->board->eval_pin_dir(cpiece->pos);
        int dstpiece = cpiece->board->getfield(dst);

        // check pins
        if(pin_dir != DIRS["undef"]){
            if(move_dir == DIRS["sth"]){
                if(pin_dir != DIRS["nth"] && pin_dir != DIRS["sth"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
            if(move_dir == DIRS["sth-wst"]){
                if(pin_dir != DIRS["nth-est"] && pin_dir != DIRS["sth-wst"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
            if(move_dir == DIRS["sth-est"]){
                if(pin_dir != DIRS["nth-wst"] && pin_dir != DIRS["sth-est"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
        }

        // check fields
        if(move_dir == DIRS["sth"]){
            if(dstpiece != mBLK){
                return false;
            }
            if(cpiece->pos - dst == 16){
                int midpiece = cpiece->board->getfield(cpiece->pos - 8);
                if(midpiece != mBLK){
                    return false;
                }
            }
        }
        if(move_dir == DIRS["sth-est"] || move_dir == DIRS["sth-wst"]){
            if(PIECES_COLORS[dstpiece] != COLORS["white"]){
                return is_ep_move_ok_for_blackpawn(cpiece, dst, minutes);
            }
        }

        // check promotion
        if((dst / 8) == 0 && prompiece != mBQU && prompiece != mBRK && 
           prompiece != mBBP && prompiece != mBKN){ return false; }

        if((dst / 8) > 0 && prompiece != mBLK){ return false; }

        return true;
    }

    bool is_ep_move_ok_for_blackpawn(cPiece *piece, int dst, list<cMove> &minutes){
        cMove lastmove;
        if(minutes.empty()){
            return false;
        }
        else{
            lastmove = minutes.back();
        }
        int dstpiece = piece->board->getfield(dst);
        int enemy = piece->board->getfield(lastmove.dst);
        if(dstpiece == mBLK && enemy == mWPW){
            if((lastmove.src / 8) - (lastmove.dst / 8) == -2 && 
               (lastmove.dst / 8) == (piece->pos / 8) && 
               (lastmove.dst % 8) == (dst % 8) && 
               (lastmove.dst / 8) - (dst / 8) == 1){
                return true;
            }
        }
        return false;
    }
    // Black Pawn end

    
    // Knight
    bool is_move_valid_for_knight(cPiece *cpiece, int dst, int prompiece){
        // check move step
        bool flag = false;
        for(auto &step : cpiece->get_mv_steps()){
            if(cpiece->pos + step == dst && cBoard::is_inbounds(cpiece->pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){ return false; }

        int pin_dir = cpiece->board->eval_pin_dir(cpiece->pos);
        if(pin_dir != DIRS["undef"]){ return false; }

        int dstpiece = cpiece->board->getfield(dst);
        if(PIECES_COLORS[dstpiece] == cpiece->color){ return false; }

        return true;
    }
    // Knight end


    // King
    bool is_move_valid_for_king(cPiece *cpiece, int dst, int prompiece){
        // check move step
        bool flag = false;
        for(auto &step : cpiece->get_mv_steps()){
            if(cpiece->pos + step == dst && cBoard::is_inbounds(cpiece->pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){ return false; }

        if(is_short_castling_ok(cpiece, dst)){ return true; }

        if(is_long_castling_ok(cpiece, dst)){ return true; }
        
        if(abs(cpiece->pos - dst) == 2){ return false; }

        int captured = cpiece->board->getfield(dst);
        cpiece->board->setfield(cpiece->pos, mBLK);
        cpiece->board->setfield(dst, cpiece->piece);
        bool isattacked = is_field_touched(*(cpiece->board), dst, REVERSED_COLORS[cpiece->color], EVAL_MODES["ignore-pins"]);
        cpiece->board->setfield(cpiece->pos, cpiece->piece);
        cpiece->board->setfield(dst, captured);
        if(isattacked){ return false; }

        int dstpiece = cpiece->board->getfield(dst);
        if(PIECES_COLORS[dstpiece] == cpiece->color){ return false; }

        return true;
    }

    bool is_short_castling_ok(cPiece *cpiece, int dst){
        uint_fast64_t shorttest, shortmask;
        if(cpiece->pos % 8 != 4 || cpiece->pos - dst != -2){ return false; }

        if(cpiece->color == COLORS["white"]){
            if(cpiece->board->wKg_first_move_on != -1 || cpiece->board->wRkH_first_move_on != -1){
                return false;
            }
            shorttest  = 0x0000600400000000;
            shortmask  = 0x0000FFFF00000000;
            uint_fast64_t fields = cpiece->board->fields[0] & shortmask;
            if(fields != shorttest){ return false; }
        }
        else{
            if(cpiece->board->bKg_first_move_on != -1 || cpiece->board->bRkH_first_move_on != -1){
                return false;
            }
            shorttest  = 0x000000000000E00C;
            shortmask  = 0x000000000000FFFF;
            uint_fast64_t fields = cpiece->board->fields[3] & shortmask;
            if(fields != shorttest){ return false; }
        }            

        cpiece->board->setfield(cpiece->pos, mBLK);
        for(int i = 0; i < 3; ++i){
            int dst2 = cpiece->pos + i;
            bool isattacked = is_field_touched(*(cpiece->board), dst2, REVERSED_COLORS[cpiece->color], EVAL_MODES["ignore-pins"]);
            if(isattacked){
                cpiece->board->setfield(cpiece->pos, cpiece->piece);
                return false;
            }
        }
        cpiece->board->setfield(cpiece->pos, cpiece->piece);
        return true;
    }

    bool is_long_castling_ok(cPiece *cpiece, int dst){
        uint_fast64_t longtest, longmask;
        if(cpiece->pos % 8 != 4 || cpiece->pos - dst != 2){ return false; }

        if(cpiece->color == COLORS["white"]){
            if(cpiece->board->wKg_first_move_on != -1 || cpiece->board->wRkA_first_move_on != -1){
                return false;
            }
            longtest  = 0x4000600000000000;
            longmask  = 0xFFFFF00000000000;
            uint_fast64_t fields = cpiece->board->fields[0] & longmask;
            if(fields != longtest){ return false; }
        }
        else{
            if(cpiece->board->bKg_first_move_on != -1 || cpiece->board->bRkA_first_move_on != -1){
                return false;
            }
            longtest  = 0x00000000C000E000;
            longmask  = 0x00000000FFFFF000;
            uint_fast64_t fields = cpiece->board->fields[3] & longmask;
            if(fields != longtest){ return false; }
        }

        cpiece->board->setfield(cpiece->pos, mBLK);
        for(int i = 0; i < 3; ++i){
            int dst2 = cpiece->pos - i;
            bool isattacked = is_field_touched(*(cpiece->board), dst2, REVERSED_COLORS[cpiece->color], EVAL_MODES["ignore-pins"]);
            if(isattacked){
                cpiece->board->setfield(cpiece->pos, cpiece->piece);
                return false;
            }
        }
        cpiece->board->setfield(cpiece->pos, cpiece->piece);
        return true;
    }
    // King end


    // Rook, Bishop, Queen
    bool is_move_valid_for_long_distance_piece(cPiece *cpiece, int dst, int prompiece){
        int dir = cPiece::dir_for_move(cpiece->piece, cpiece->pos, dst);
        if(dir == DIRS["undef"]){ return false; }
        
        // check pins
        int pin_dir = cpiece->board->eval_pin_dir(cpiece->pos);
        for(auto &piecedir : cpiece->get_dirs_ary()){
            if(piecedir == DIRS["undef"]){ break; }
            if(dir == piecedir){
                if(pin_dir != piecedir && 
                   pin_dir != REVERSE_DIRS[piecedir] && 
                   pin_dir != DIRS["undef"]){
                    return false;
                }
            }
        }

        int step = cPiece::step_for_dir(cpiece->piece, dir);
        if(step == 0){ return false; }
        int newpos = cpiece->pos + step;
        while(cpiece->board->is_inbounds(cpiece->pos, newpos, 0)){
            int newpiece = cpiece->board->getfield(newpos);
            if(newpos == dst){
                if(PIECES_COLORS[newpiece] == cpiece->color){
                    return false;
                }
                else{
                    return true;
                }
            }
            if(newpiece != mBLK){
                return false;
            }
            newpos += step;
        }
        return false;
    }
    // Rook, Bishop, Queen end


    bool is_trapped_from_ext(cPiece *cpiece){
        if(cpiece->piece == mWPW || cpiece->piece == mBPW || 
           cpiece->piece == mWKN || cpiece->piece == mBKN ||
           cpiece->piece == mWKG || cpiece->piece == mBKG){
            return false; // these piece cannot be trapped
        }
        else{
            if(is_field_touched(*(cpiece->board), cpiece->pos, REVERSED_COLORS[cpiece->color], EVAL_MODES["only-pins-to-king"])){
                return false;
            }
            for(int step : cpiece->get_steps()){
                int pos1 = cpiece->pos + step;
                if(cpiece->board->is_inbounds(cpiece->pos, pos1, step)){
                    int dstpiece = cpiece->board->getfield(pos1);
                    if(PIECES_COLORS[dstpiece] == cpiece->color){
                        continue;
                    }
                    else{
                        if(dstpiece != mBLK && PIECES_RANKS[cpiece->piece] <= PIECES_RANKS[dstpiece]){
                            return false;
                        }
                        list<cTouch> frdlytouches, enmytouches;
                        collect_touches_for_both_colors(*(cpiece->board), pos1, cpiece->color, frdlytouches, enmytouches);
                        bool enmy_is_lower = false;
                        for(const cTouch enmy : enmytouches){
                            if(PIECES_RANKS[enmy.piece] < PIECES_RANKS[cpiece->piece]){
                                enmy_is_lower = true;
                                break;
                            }
                        }
                        if(frdlytouches.size() >= enmytouches.size() && enmy_is_lower == false){
                            return false;
                        }
                    }
                }
            }
            return true;
        }
    }


    bool is_move_stuck_from_ext(cPiece *cpiece, int dst){
        if(cpiece->piece == mWKG || cpiece->piece == mBKG){
            return false; //king cannot stuck
        }
        else{
            int mv_dir = cPiece::dir_for_move(cpiece->piece, cpiece->pos, dst);
            int pin_dir = cpiece->board->eval_pin_dir(cpiece->pos);
            if(pin_dir == DIRS["undef"] || mv_dir == pin_dir || REVERSE_DIRS[mv_dir] == pin_dir){
                return false;
            }
            else{
                return true;
            }
        }
    }
