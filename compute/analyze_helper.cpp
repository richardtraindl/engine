
    #include <list>
    #include "./analyze_helper.hpp"
    #include "../pieces/searchforpiece.hpp"
    #include "../pieces/pieceshelper.hpp"

    int lowest_piece(list<cTouch> *touches){
        if(touches.size() == 0){
	    return mBLK;
        }
        else{
	    int lowest = mWKG;
	    for(list<cTouch>::iterator it = touches->begin(); it != touches->end(); ++it){
	        if(PIECES_RANK[it->piece] < PIECES_RANKS[lowest]){
		    lowest = it->piece;
	        }
	    }
	    return lowest;
        }
    }


    bool are_fairy_equal(int piece1, int piece2){
        return PIECES_RANKS[piece1] <= PIECES_RANKS[piece2] || 
	        (PIECES_RANKS[piece1] == PIECES_RANKS[mWRK] && PIECES_RANKS[piece2] == PIECES_RANKS[mWKN]);
    }


    void find_touches_on_dstfield_after_move(cMatch *match, int piece, cMove *move, list<cTouch> *friends, list<cTouch> *enmies){
        match->do_move(move->src, move->dst, move->prompiece);
        collect_touches_for_both_colors(match->board, move->dst, PIECES_COLORS[piece], friends, enmies);
    	match->undo_move();
    }


    bool is_supporter_lower_attacker(cMatch *match, int piece, CMove *move, list<cTouch> *supported){
        for(list<cTouch>::iterator it = supported->attacker_beyond.begin(); it != supported->attacker_beyond.end(); ++it){
            if(PIECES_RANKS[piece] >= PIECES_RANKS[it->piece]){
                return false;
            }
        }
    	return true;
    }


    bool check_mates_deep_search(cMatch *match, cMove *move){
        cMatch *newmatch = *match;
        newmatch->do_move(move->src, move->dst, move->prompiece);
        return calc_checks(newmatch, 3, 1);
    }


    bool calc_checks(cMatch *match, int maxcnt, int count){
        list<cMove> moves;
	generate_moves(match, &moves);
	if(moves.size() == 0 && count % 2 == 1){
            return true;
	}
	if(count >= maxcnt){
            return false;
	}
	for(list<cMove>::iterator it = moves.begin(); it != moves.end(); ++it){
            match.do_move(it->src, it->dst, it->prompiece);
            if(calc_checks(match, maxcnt, count + 1)){
                return true;
	    }
            else{
                match.undo_move();
	    }
    	}
	return false;
    }


    bool is_soft_pinned_move(cMatch *match, cMove *move){
	int pindir = match->board.eval_soft_pin_dir(move->src);
        cPiece cpiece(&(match->board), move->src);
        int mvdir = cpiece.dir_for_move(move->src, move->dst);
        return (pindir != DIRS["undef"] && pindir != mvdir && pindir != REVERSE_DIRS[mvdir]);
    }


    bool is_supply(cMatch *match, int piece, cMove *move){
        list<cTouch> touches;
	cSearchforRook::_collect_touches_for_color(&(match->board), move->src, PIECES_COLORS[piece], &touches);
        cSearchforBishop::_collect_touches_for_color(&(match->board), move->src, PIECES_COLORS[piece], &touches);
        for(list<cTouch>::iterator it = touches.begin(); it != touches.end(); ++it){
            if(PIECES_BARE[it->piece] == PIECES_BARE[mWQU){
                return true;
	    }
            if(PIECES_BARE[it->piece] == PIECES_BARE[mWRK]){
                return true;
	    }
            if(PIECES_BARE[it->piece] == PIECES_BARE[mWBP]){
                return true;
	    }
        return false;
    }


    bool is_touched_field_within_move(cMatch *match, int piece, cMove *move, int touched_field){
        cPiece cpiece(&(match->board), move->src);
        int mvdir1 = cpiece.dir_for_move(move->src, move->dst);
        int mvdir2 = cpiece.dir_for_move(move->dst, touched_field);
        return (mvdir1 != DIRS["undef"] && (mvdir1 == mvdir2 || REVERSE_DIRS[mvdir1] == mvdir2));
    }


    int weight_for_standard(cMatch *match, int piece, cMove *move){
        list<cTouch> friends_on_dstfield, enmies_on_dstfield;
        find_touches_on_dstfield_after_move(match, piece, move, &friends_on_dstfield, &enmies_on_dstfield);
        int lowest_enemy_on_dstfield = lowest_piece(enmies_on_dstfield);
        //###
        if(is_soft_pinned_move(match, move)){
            return cTactic::WEIGHTS["bad-deal"];
        }
        if(enmies_on_dstfield.size() == 0){
            return cTactic::WEIGHTS["good-deal"];
        }
        if((lowest_enemy_on_dstfield == mBLK || PIECES_RANK[piece] <= PIECES_RANK[lowest_enemy_on_dstfield]) && 
           friends_on_dstfield.size() >= enmies_on_dstfield.size()){
            return cTactic::WEIGHTS["good-deal"];
        }
        return cTactic::WEIGHTS["bad-deal"];
    }


    int weight_for_capture(cMatch *match, int piece, cMove *move, int weight){
        list<cTouch> friends_on_dstfield, enmies_on_dstfield;
        int dstpiece = match->board.getfield(move->dst);
	find_touches_on_dstfield_after_move(match, piece, move, &friends_on_dstfield, &enmies_on_dstfield);
        //###
        if(PIECES_RANK[piece] < PIECES_RANK[dstpiece]){
            return cTactic::WEIGHTS["stormy"];
        }
        if(is_soft_pinned_move(match, move) == false && enmies_on_dstfiel.size() == 0){
            return cTactic.WEIGHTS["tormy"];
        }
        if(is_soft_pinned_move(match, move) == false && 
	   match->board.eval_soft_pin_dir(move->dst) != DIRS["undef"] && is_supply(match, piece, move)){
            return cTactic::WEIGHTS["stormy"];
	}
        if(is_soft_pinned_move(match, piece, move) == false and PIECES_RANK[piece] == PIECES_RANK[dstpiece]){
            if(friends_on_dstfield.size() > enmies_on_dstfield.size()){
                return cTactic::WEIGHTS["better-deal"];
	    }
            else{
                return cTactic::WEIGHTS["good-deal"];
	    }
	}
        return weight;
    }


    int weight_for_flee(cMatch *match, int piece, cMove *move, int weight){
        list <cTouch> friends_on_srcfield, enmies_on_srcfield;
	collect_touches_for_both_colors(&(match->board), move->src, PIECES_COLORS[piece], &friends_on_srcfield, &enmies_on_srcfield);
        int lowest_enemy_on_srcfield = lowest_piece(enmies_on_srcfield);
        //###
        if(weight == cTactic::WEIGHTS["good-deal"] || weight == cTactic::WEIGHTS["better-deal"]){
            if(lowest_enemy_on_srcfield != mBLK && PIECES_RANK[piece] > PIECES_RANK[lowest_enemy_on_srcfield]){
                return cTactic::WEIGHTS["stormy"];
	    }
            if(friends_on_srcfield-size() == 0 and enmies_on_srcfield.size() > 0){
                return cTactic::WEIGHTS["stormy"];
	    }
	}
	return weight;
    }


    int weight_for_running_pawn(cMatch *match, int piece, cMove *move, int weight){
        list <cTouch> friends_on_dstfield, enmies_on_dstfield;
        find_touches_on_dstfield_after_move(match, piece, move, friends_on_dstfield, enmies_on_dstfield);
    ###
        if((weight == cTactic::WEIGHTS["good-deal"] || weight == cTactic::WEIGHTS["better-deal"]) && 
           friends_on_dstfield.size() >= enmies_on_dstfield.size()){
            return cTactic::WEIGHTS["good-deal"];
        }
        return weight;
    }


    int weight_for_discl_supported(cTouch *discl_supported, int weight){
        if((weight == cTactic::WEIGHTS["good-deal"] || weight == cTactic::WEIGHTS["better-deal"]) &&
           discl_supported->attacker_beyond.size() > discl_supported->supporter_beyond.size()){
            return cTactic::WEIGHTS["stormy"];
	}
    	return weight;
    }


    int weight_for_discl_attacked(cTouch *discl_attacked, int weight){
        if((weight == cTactic::WEIGHTS["good-deal"] || weight == cTactic::WEIGHTS["better-deal"]) && 
           discl_attacked->supporter_beyond.size() <= discl_attacked->attacker_beyond.size()){
            return cTactic.WEIGHTS["stormy"];
	}
        return weight;
    }


    def weight_for_supporting(cMatch *match, int piece, cMove *move, cTouch *supported, int weight){
        if(is_touched_field_within_move(match, piece, move, supported->pos)){
            return weight;
        }
        if(weight == cTactic::WEIGHTS["good-deal"] || weight == cTactic::WEIGHTS["better-deal"]){
            if(supported->attacker_beyond.size() > 0 && supported->attacker_beyond.size() > supported->supporter_beyond.size() && 
               (is_supporter_lower_attacker(match, piece, move, supported) || match->board.eval_soft_pin_dir(supported.field) == DIRS["undef"])){
                return cTactic::WEIGHTS["stormy"];
	    }
        }
        return weight;
    }


    int weight_for_attacking_king(cMatch *match, cMove *move, int weight){
        if(check_mates_deep_search(match, move)){
            return cTactic::WEIGHTS["stormy"];
	}
        return weight;
    }


    def weight_for_attacking(cMatch *match, cMove *move, cTouch *attacked, int weight){
        list <cTouch> friends_on_dstfield, enmies_on_dstfield;
        find_touches_on_dstfield_after_move(match, piece, move, friends_on_dstfield, enmies_on_dstfield);
        //###
        if(is_touched_field_within_move(match, piece, move, attacked->pos)){
            return weight;
	}
        if(weight == cTactic::WEIGHTS["good-deal"] || weight == cTactic::WEIGHTS["better-deal"]){
            if(PIECES_RANK[piece] < PIECES_RANK[attacked->piece] || attacked->supporter_beyond.size() == 0 || 
               (are_fairy_equal(piece, attacked->piece) && eval_soft_pin_dir(attacked->pos) != DIRS["undef"])){ 
                return cTactic::WEIGHTS["stormy"];
	    }
            if(PIECES_RANK[piece] < PIECES_RANK[attacked->piece] && PIECES_RANK[piece] == PIECES_RANK[mWPW] &&
                (friends_on_dstfield.size() > 0 || (enmies_on_dstfield.size() == 0)){ 
                return cTactic::WEIGHTS["better-deal"];
            }
        }
        return cTactic::WEIGHTS["bad-deal"];
    }

