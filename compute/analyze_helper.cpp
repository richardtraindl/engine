	
  
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

			
				bool check_mates_deep_search(cMatch *match, cMove *move){
		cMatch *newmatch = *match;
		newmatch->do_move(move->src, move->dst, move->prompiece);
		return calc_checks(newmatch, 3, 1);


	def calc_checks(cMatch *match, int maxcnt, int count){
		moves = generate_moves(match, None, None, False, False)
		if(len(moves) == 0 and count % 2 == 1){
			return True
		if(count >= maxcnt){
			return False
		for move in moves:
			match.do_move(move.src, move.dst, move.prompiece)
			if(calc_checks(match, maxcnt, count + 1)){
				return True
			else{
				match.undo_move()
		return False


def is_soft_pinned_move(match, piece, move){
    flag, pindir = match.is_soft_pin(move.src)
    cpiece = match.obj_for_piece(piece, move.src)
    mvdir = cpiece.dir_for_move(move.src, move.dst)
    return (flag and pindir != mvdir and pindir != REVERSE_DIRS[mvdir])


def is_supply(match, piece, move){
    supplies = cSearchForRook.list_field_touches(match, move.src, match.color_of_piece(piece))
    supplies.extend(cSearchForBishop.list_field_touches(match, move.src, match.color_of_piece(piece)))
    for supply in supplies:
        if(PIECES_BARE[supply.piece] == PIECES_BARE[PIECES['wQu']]){
            return True
        elif(PIECES_BARE[supply.piece] == PIECES_BARE[PIECES['wRk']]){
            return True
        elif(PIECES_BARE[supply.piece] == PIECES_BARE[PIECES['wBp']]){
            return True
    return False


def is_touched_field_within_move(match, piece, move, touched_field){
    cpiece = match.obj_for_piece(piece, move.src)
    mvdir1 = cpiece.dir_for_move(move.src, move.dst)
    mvdir2 = cpiece.dir_for_move(move.dst, touched_field)
    return (mvdir1 != DIRS['undef'] and (mvdir1 == mvdir2 or REVERSE_DIRS[mvdir1] == mvdir2))


def weight_for_standard(match, piece, move){
    friends_on_dstfield, enmies_on_dstfield = find_touches_on_dstfield_after_move(match, piece, move)
    lowest_enemy_on_dstfield = lowest_piece(enmies_on_dstfield)
    ###
    if(is_soft_pinned_move(match, piece, move)){
        return cTactic.WEIGHTS['bad-deal']
    elif(len(enmies_on_dstfield) == 0){
        return cTactic.WEIGHTS['good-deal']
    elif((lowest_enemy_on_dstfield is None or PIECES_RANK[piece] <= PIECES_RANK[lowest_enemy_on_dstfield]) and
         len(friends_on_dstfield) >= len(enmies_on_dstfield)){
        return cTactic.WEIGHTS['good-deal']
    else{
        return cTactic.WEIGHTS['bad-deal']


def weight_for_capture(match, piece, move, weight){
    dstpiece = match.board.getfield(move.dst)
    friends_on_dstfield, enmies_on_dstfield = find_touches_on_dstfield_after_move(match, piece, move)
    ###
    if(PIECES_RANK[piece] < PIECES_RANK[dstpiece]){
        return cTactic.WEIGHTS['stormy']
    elif(is_soft_pinned_move(match, piece, move) == False and len(enmies_on_dstfield) == 0){
        return cTactic.WEIGHTS['stormy']
    elif(is_soft_pinned_move(match, piece, move) == False and 
         match.is_soft_pin((move.dst))[0] and is_supply(match, piece, move)){
        return cTactic.WEIGHTS['stormy']
    elif(is_soft_pinned_move(match, piece, move) == False and 
         PIECES_RANK[piece] == PIECES_RANK[dstpiece]){
        if(len(friends_on_dstfield) > len(enmies_on_dstfield)){
            return cTactic.WEIGHTS['better-deal']
        else{
            return cTactic.WEIGHTS['good-deal']
    else{
        return weight


def weight_for_flee(match, piece, move, weight){
    friends_on_srcfield, enmies_on_srcfield = list_all_field_touches(match, move.src, match.color_of_piece(piece))
    lowest_enemy_on_srcfield = lowest_piece(enmies_on_srcfield)
    ###
    if(weight == cTactic.WEIGHTS['good-deal'] or 
       weight == cTactic.WEIGHTS['better-deal']){
        if(lowest_enemy_on_srcfield is not None and 
           PIECES_RANK[piece] > PIECES_RANK[lowest_enemy_on_srcfield]){
            return cTactic.WEIGHTS['stormy']
        elif(len(friends_on_srcfield) == 0 and len(enmies_on_srcfield) > 0){
            return cTactic.WEIGHTS['stormy']
    return weight


def weight_for_running_pawn(match, piece, move, weight){
    friends_on_dstfield, enmies_on_dstfield = find_touches_on_dstfield_after_move(match, piece, move)
    ###
    if((weight == cTactic.WEIGHTS['good-deal'] or 
        weight == cTactic.WEIGHTS['better-deal']) and 
        len(friends_on_dstfield) >= len(enmies_on_dstfield)){
        return cTactic.WEIGHTS['good-deal']
    else{
        return weight


def weight_for_discl_supported(discl_supported, weight){
    if((weight == cTactic.WEIGHTS['good-deal'] or 
        weight == cTactic.WEIGHTS['better-deal']) and 
        len(discl_supported.attacker_beyond) > len(discl_supported.supporter_beyond)){
        return cTactic.WEIGHTS['stormy']
    return weight


def weight_for_discl_attacked(discl_attacked, weight){
    if((weight == cTactic.WEIGHTS['good-deal'] or 
        weight == cTactic.WEIGHTS['better-deal']) and 
        len(discl_attacked.supporter_beyond) <= len(discl_attacked.attacker_beyond)){
        return cTactic.WEIGHTS['stormy']
    else{
        return weight


def weight_for_supporting(match, piece, move, supported, weight){
    if(is_touched_field_within_move(match, piece, move, supported.field)){
        return weight
    if(weight == cTactic.WEIGHTS['good-deal'] or 
       weight == cTactic.WEIGHTS['better-deal']){
        if(len(supported.attacker_beyond) > 0 and
           len(supported.attacker_beyond) > len(supported.supporter_beyond) and
           (is_supporter_lower_attacker(match, piece, move, supported) or
            match.is_soft_pin(supported.field)[0])){
            return cTactic.WEIGHTS['stormy']
    return weight


def weight_for_attacking_king(match, move, weight){
    if(check_mates_deep_search(match, move)){
        return cTactic.WEIGHTS['stormy']
    else{
        return weight


def weight_for_attacking(match, piece, move, attacked, weight){
    friends_on_dstfield, enmies_on_dstfield = find_touches_on_dstfield_after_move(match, piece, move)
    ###
    if(is_touched_field_within_move(match, piece, move, attacked.field)){
        return weight
    if(weight == cTactic.WEIGHTS['good-deal'] or 
       weight == cTactic.WEIGHTS['better-deal']){
        if(PIECES_RANK[piece] < PIECES_RANK[attacked.piece] or 
           len(attacked.supporter_beyond) == 0 or 
           (are_fairy_equal(piece, attacked.piece) and match.is_soft_pin(attacked.field)[0])){ 
            return cTactic.WEIGHTS['stormy']
        elif(PIECES_RANK[piece] < PIECES_RANK[attacked.piece] and 
             PIECES_RANK[piece] == PIECES_RANK[PIECES['wPw']] and
             (len(friends_on_dstfield) > 0 or len(enmies_on_dstfield) == 0)){ 
            return cTactic.WEIGHTS['better-deal']
    return cTactic.WEIGHTS['bad-deal']
