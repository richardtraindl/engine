	
  
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
			   (PIECES_RANKS[piece1] == PIECES_RANKS[mWRK] && PIECES_RANKS[piece2] == PIECES_RANKS[mWKN])


	void find_touches_on_dstfield_after_move(cMatch *match, int piece, cMove *move, list<cTouch> *friends, list<cTouch> *enmies){
		match->do_move(move->src, move->dst, move->prompiece);
		collect_touches_for_both_colors(match->board, move->dst, PIECES_COLORS[piece], friends, enmies);
		match->undo_move();


	bool is_supporter_lower_attacker(cMatch *match, int piece, CMove *move, list<cTouch> *supported){
		for(list<cTouch>::iterator it =  supported->attacker_beyond.begin(); it != supported->attacker_beyond.end(); ++it){
			if(PIECES_RANKS[piece] >= PIECES_RANKS[it->piece]){
				return false;
			}
		}
		return true;
	}
