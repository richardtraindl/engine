
    #include <iostream>
    #include "./analyze_helper.hpp"
    #include "./calc.hpp"
    #include "../pieces/piece.hpp"
    #include "../pieces/searchforpiece.hpp"
    #include "../values.hpp"


    cAnalyzeField::cAnalyzeField(cBoard &newboard, int newcolor, int newpos){
        board = &newboard;
        color = newcolor;
        pos = newpos;
        piece = board->getfield(pos);
        collect_touches_for_both_colors(*board, pos, color, friends_on_field, enmies_on_field);
        lowest_friend = lowest_piece(friends_on_field);
        lowest_enemy = lowest_piece(enmies_on_field);
        _is_field_ok();
        _is_clean();
        _is_supported();
        _is_attacked();
        _is_suppsize_eq_attsize();
        _is_suppsize_lt_attsize();
        _is_suppsize_gt_attsize();
        _is_soft_pinned();
    }

    void cAnalyzeField::_is_field_ok(){
        if(enmies_on_field.size() == 0){
            is_field_ok = true;
            return;
        }
        if(friends_on_field.size() >= enmies_on_field.size() && 
           PIECES_RANKS[piece] <= PIECES_RANKS[lowest_enemy]){
            is_field_ok = true;
            return;
        }
        is_field_ok = false;
    }


    void cAnalyzeField::_is_clean(){
        is_clean = enmies_on_field.size() == 0;
    }

    void cAnalyzeField::_is_supported(){
        is_supported = friends_on_field.size() > 0;
    }

    void cAnalyzeField::_is_attacked(){
        is_attacked = enmies_on_field.size() > 0;
    }

    void cAnalyzeField::_is_suppsize_eq_attsize(){
        is_suppsize_eq_attsize = friends_on_field.size() == enmies_on_field.size();
    }
    
    void cAnalyzeField::_is_suppsize_lt_attsize(){
        is_suppsize_lt_attsize = friends_on_field.size() < enmies_on_field.size();
    }

    void cAnalyzeField::_is_suppsize_gt_attsize(){
        is_suppsize_gt_attsize = friends_on_field.size() > enmies_on_field.size();
    }

    void cAnalyzeField::_is_soft_pinned(){
        is_soft_pinned = board->eval_soft_pin_dir(pos) != DIRS["undef"];
    }

    int cAnalyzeField::lowest_piece(list<cTouch> touches){
        if(touches.size() == 0){
            return mBLK;
        }
        else{
            int lowest = mWKG;
            for(cTouch touch : touches){
                if(PIECES_RANKS[touch.piece] < PIECES_RANKS[lowest]){
                    lowest = touch.piece;
                }
            }
            return lowest;
        }
    }


    bool are_fairy_equal(int piece1, int piece2){
        return PIECES_RANKS[piece1] <= PIECES_RANKS[piece2] || 
           (PIECES_RANKS[piece1] == PIECES_RANKS[mWRK] && PIECES_RANKS[piece2] == PIECES_RANKS[mWKN]);
    }


    bool is_move_out_of_soft_pin(cMatch &match, int piece ,cPrioMove &priomove){
        int pindir = match.board.eval_soft_pin_dir(priomove.src);
        int mvdir = cPiece::dir_for_move(piece, priomove.src, priomove.dst);
        return (pindir != DIRS["undef"] && pindir != mvdir && pindir != REVERSE_DIRS[mvdir]);
    }


    bool is_supply(cMatch &match, int piece, cPrioMove &priomove){
        list<cTouch> touches;
        collect_long_distance_touches_for_color(match.board, priomove.src, PIECES_COLORS[piece], touches);
        for(cTouch touch : touches){
            if(PIECES_BARES[touch.piece] == PIECES_BARES[mWQU]){
                return true;
            }
            if(PIECES_BARES[touch.piece] == PIECES_BARES[mWRK]){
                return true;
            }
            if(PIECES_BARES[touch.piece] == PIECES_BARES[mWBP]){
                return true;
            }
        }
        return false;
    }


    bool are_move_dirs_equal(int piece1, int src1, int dst1, int piece2, int src2, int dst2){
        int mvdir1 = cPiece::dir_for_move(piece1, src1, dst1);
        int mvdir2 = cPiece::dir_for_move(piece2, src2, dst2);
        return (mvdir1 != DIRS["undef"] && (mvdir1 == mvdir2 || REVERSE_DIRS[mvdir1] == mvdir2));
    }


    int search_for_checkmate(cMatch &match){
        cout << "s" << endl;
        return _search_for_checkmate(match, 1, 4);
    }

    int _search_for_checkmate(cMatch &match, int count, int maxcnt){
        list<cMove *> moves;
        generate_moves(match, moves);
        if(moves.size() == 0){
            if(match.next_color() == COLORS["white"]){
                uint64_t wkg = match.board.fields[0] & match.board.fields[2];
                if(is_field_touched(match.board, wkg, COLORS["black"], EVAL_MODES["ignore-pins"])){
                    return COLORS["white"];
                }
            }
            else{
                uint64_t bkg = match.board.fields[1] & match.board.fields[2];
                if(is_field_touched(match.board, bkg, COLORS["white"], EVAL_MODES["ignore-pins"])){
                    return COLORS["black"];
                }
            }
            return COLORS["undef"];
        }
        if(count >= maxcnt){
            return COLORS["undef"];
        }
        for(cMove *move : moves){
            match.do_move(move->src, move->dst, move->prompiece);
            int color = _search_for_checkmate(match, count + 1, maxcnt);
            match.undo_move();
            if(color != COLORS["undef"]){ 
                return color; 
            }
        }
        return COLORS["undef"];
    }


    bool find_excluded(list<cExclude*> &excludes, int pos, int touched_pos, int mvdir, int domain){
        for(cExclude *excluded : excludes){
            if(excluded->pos == pos && 
               excluded->touched_pos == touched_pos && 
               excluded->mvdir == mvdir &&
               excluded->domain == domain){
                return true;
            }
        }
        return false;
    }

    int weight_for_standard(cMatch &match, int piece, cPrioMove &priomove, cAnalyzeField &analyzedst){
        if(is_move_out_of_soft_pin(match, piece, priomove)){
            return cTactic::WEIGHTS["bad-deal"];
        }
        if(analyzedst.is_field_ok){
            return cTactic::WEIGHTS["good-deal"];
        }
        return cTactic::WEIGHTS["bad-deal"];
    }


    int weight_for_capture(cMatch &match, int piece,  int dstpiece, cPrioMove &priomove, cAnalyzeField &analyzedst){
        bool out_of_softpin = is_move_out_of_soft_pin(match, piece, priomove);
        if(PIECES_RANKS[piece] < PIECES_RANKS[dstpiece]){
            return cTactic::WEIGHTS["stormy"];
        }
        if(out_of_softpin == false && analyzedst.is_field_ok){
            return cTactic::WEIGHTS["stormy"];
        }
        if(out_of_softpin == false && is_supply(match, piece, priomove)){
            return cTactic::WEIGHTS["stormy"];
        }
        if(out_of_softpin == false && analyzedst.is_field_ok){
            if(are_fairy_equal(piece, dstpiece)){
                return cTactic::WEIGHTS["better-deal"];
            }
            else{
                return cTactic::WEIGHTS["good-deal"];
            }
        }
        return cTactic::WEIGHTS["bad-deal"];
    }


    int weight_for_flee(cMatch &match, int piece, cPrioMove &priomove, cAnalyzeField &analyzesrc, cAnalyzeField &analyzedst){
        if(is_move_out_of_soft_pin(match, piece, priomove) == false &&
           analyzedst.is_field_ok){
            if(analyzesrc.is_attacked && 
               PIECES_RANKS[piece] < PIECES_RANKS[analyzesrc.lowest_enemy]){
                return cTactic::WEIGHTS["stormy"];
            }
            if(analyzesrc.is_suppsize_lt_attsize){
                return cTactic::WEIGHTS["stormy"];
            }
            return cTactic::WEIGHTS["good-deal"];
        }
        return cTactic::WEIGHTS["bad-deal"];
    }


    int weight_for_discl_support(cMatch &match, int piece, cPrioMove &priomove, cTouch &discl_supported, cAnalyzeField &analyzedst){
        if(is_move_out_of_soft_pin(match, piece, priomove) == false &&
           analyzedst.is_field_ok){
            if(discl_supported.attacker_beyond.size() > discl_supported.supporter_beyond.size()){
                return cTactic::WEIGHTS["stormy"];
            }
            else{
                return cTactic::WEIGHTS["good-deal"];
            }
        }
        return cTactic::WEIGHTS["bad-deal"];
    }


    int weight_for_discl_attack(cMatch &match, int piece, cPrioMove &priomove, cTouch &discl_attacked, cAnalyzeField &analyzedst){
        if(is_move_out_of_soft_pin(match, piece, priomove) == false &&
           analyzedst.is_field_ok){
            if(discl_attacked.supporter_beyond.size() <= discl_attacked.attacker_beyond.size()){
                return cTactic::WEIGHTS["stormy"];
            }
            else{
                return cTactic::WEIGHTS["good-deal"];
            }
        }
        return cTactic::WEIGHTS["bad-deal"];
    }


    int weight_for_support(cMatch &match, int piece, cPrioMove &priomove, cTouch &supported, cAnalyzeField &analyzedst){
        if(is_move_out_of_soft_pin(match, piece, priomove) == false &&
           analyzedst.is_field_ok){
            if(cPiece::dir_for_move(piece, priomove.src, supported.pos) ==
               cPiece::dir_for_move(piece, priomove.dst, supported.pos)){
                return cTactic::WEIGHTS["downgraded"];
            }
            int lowest_attacker = cAnalyzeField::lowest_piece(supported.attacker_beyond);
            if(supported.attacker_beyond.size() > 0 && 
               ((supported.attacker_beyond.size() >= supported.supporter_beyond.size() &&
                 PIECES_RANKS[piece] > PIECES_RANKS[lowest_attacker]) || 
                 match.board.eval_soft_pin_dir(supported.pos) != DIRS["undef"])){
                return cTactic::WEIGHTS["stormy"];
            }
            else{
                return cTactic::WEIGHTS["good-deal"];
            }
        }
        return cTactic::WEIGHTS["bad-deal"];
    }


    int weight_for_attack(cMatch &match, int piece, cPrioMove &priomove, cTouch &attacked, cAnalyzeField &analyzedst){
        if(is_move_out_of_soft_pin(match, piece, priomove) == false &&
           analyzedst.is_field_ok){
            if(cPiece::dir_for_move(piece, priomove.src, attacked.pos) ==
               cPiece::dir_for_move(piece, priomove.dst, attacked.pos)){
                return cTactic::WEIGHTS["downgraded"];
            }
            if(are_fairy_equal(piece, attacked.piece)){
                if(match.board.eval_soft_pin_dir(attacked.pos) != DIRS["undef"]){
                    return cTactic::WEIGHTS["stormy"];
                }
                else{
                    return cTactic::WEIGHTS["good-deal"];
                }
            }
            if(PIECES_RANKS[piece] < PIECES_RANKS[attacked.piece]){
                return cTactic::WEIGHTS["better-deal"];
            }
            if(attacked.supporter_beyond.size() == 0){
                return cTactic::WEIGHTS["better-deal"];
            }
        }
        return cTactic::WEIGHTS["bad-deal"];
    }


    int weight_for_attack_on_king(cMatch &match, int piece, cPrioMove &priomove, cTouch &attacked, cAnalyzeField &analyzedst){
        cPiece cking(match.board, priomove.src);
        if(analyzedst.is_field_ok){
            if(cPiece::dir_for_move(piece, priomove.src, attacked.pos) ==
               cPiece::dir_for_move(piece, priomove.dst, attacked.pos)){
                return cTactic::WEIGHTS["downgraded"];
            }
            if(cking.is_king_safe() == false){
                return cTactic::WEIGHTS["stormy"];
            }
            else{
                return cTactic::WEIGHTS["good-deal"];
            }
        }
        else{
            return cTactic::WEIGHTS["bad-deal"];
        }
    }
