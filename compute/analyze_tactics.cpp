
    #include "./analyze_tactics.hpp"
    #include "../pieces/searchforpiece.hpp"
    #include "../values.hpp"


    cDirTouch::cDirTouch(int _piece, int _pos, int _dir){
            piece = _piece;
            pos = _pos;
            dir = _dir;
    }
    
/*
def search_dir(match, field, direction, excl){
    dirtouches = []
    src = field
    step = cQueen.step_for_dir(direction)
    if(step is None){
        return dirtouches
    for i in range(7){
        dst = match.board.search(src, step, cQueen.MAXCNT)
        if(dst is not None and dst != excl){
            piece = match.board.getfield(dst)
            dirtouches.append(cDirTouch(piece, direction, dst))
            src = dst
        else:
            break
    return dirtouches


def search_lines_of_pin(match, color, field, excl){
    pinlines = []
    piece = match.board.getfield(field)
    oppcolor = REVERSED_COLORS[color]
 
    for i in range(len(cQueen.DIRS_ARY)){
        dirtouches = search_dir(match, field, cQueen.DIRS_ARY[i], excl)
        if(len(dirtouches) < 2){
            continue

        if(match.color_of_piece(dirtouches[0].piece) == color and 
           match.color_of_piece(dirtouches[1].piece) == oppcolor and
           PIECES_RANK[piece] > PIECES_RANK[dirtouches[0].piece] and 
           PIECES_RANK[piece] > PIECES_RANK[dirtouches[1].piece]){

            if(dirtouches[1].piece == PIECES['wQu'] or dirtouches[1].piece == PIECES['bQu']){
                pinlines.append([dirtouches[0], dirtouches[1]])

            elif(i < 4 and (dirtouches[1].piece == PIECES['wRk'] or dirtouches[1].piece == PIECES['bRk'])){ 
                pinlines.append([dirtouches[0], dirtouches[1]])

            elif(i >= 4 and (dirtouches[1].piece == PIECES['wBp'] or dirtouches[1].piece == PIECES['bBp'])){
                pinlines.append([dirtouches[0], dirtouches[1]])

    return pinlines
*/

    bool is_supported_running_pawn(cMatch &match, cTouch &supported){
        if(match.is_endgame() == false){
            return false;
        }
        cPiece cpiece(&(match.board), supported.pos);
        return cpiece.is_running_pawn();
    }


    bool castles(cMatch &match, int piece, cPrioMove &priomove){
        if(piece == mWKG || piece == mBKG){
            if(abs(priomove.src - priomove.dst) == 2){
                return true;
            }
        }
        return false;
    }


    bool promotes(cPrioMove &priomove){
        if(priomove.prompiece != mBLK){
            return true;
        }
        else{
            return false;
        }
    }

    bool captures(cMatch &match, int piece, cPrioMove &priomove){
        int dstpiece = match.board.getfield(priomove.dst);
        if(dstpiece != mBLK){
            return true;
        }
        if((piece == mWPW || piece == mBPW) && (priomove.src % 8) != (priomove.dst %8)){
            return true;
        }
        else{
            return false;
        }
    }

/*
def forks(piece, from_dstfield_attacked){
    if(len(from_dstfield_attacked) < 2){
        return false;
    count = 0
    for attacked in from_dstfield_attacked:
        if(len(attacked.supporter_beyond) == 0 or
           PIECES_RANK[attacked.piece] >= PIECES_RANK[piece]){
            count += 1
    return count >= 2


def defends_fork(match, piece, move, dstpiece){
    attacked = []
    supported = []
    if(dstpiece == PIECES['blk']){
        if(piece == PIECES['wPw'] or piece == PIECES['bPw']){
            cpawn = match.obj_for_piece(piece, move.src)
            if(cpawn.is_ep_move_ok(move.dst)){
                forking = move.src
                forking_piece = match.board.getfield(forking)
            else:
                return false;
        else:
            return false;
    else:
        forking = move.dst
        forking_piece = dstpiece

    cforking_piece = match.obj_for_piece(forking_piece, forking)
    if(cforking_piece is not None){
        cforking_piece.find_attacks_and_supports(attacked, supported)
        return forks(forking_piece, attacked)
    else:
        return false;


def threatens_fork(match, piece, move){
    attacked = []
    supported = []
    is_fork_threat = false;
    ###
    newmatch = copy.deepcopy(match)
    newmatch.do_move(move.src, move.dst, move.prompiece)
    cpiece = newmatch.obj_for_piece(piece, move.dst)
    first_move_dir = cpiece.dir_for_move(move.src, move.dst)
    if(cpiece is not None){
        moves = cpiece.generate_moves(false;, None, false;, None)
        for mv in moves:
            cnewpiece = newmatch.obj_for_piece(piece, mv.dst)
            second_move_dir = cnewpiece.dir_for_move(mv.src, mv.dst)
            if(first_move_dir == second_move_dir or 
               REVERSE_DIRS[first_move_dir] == second_move_dir){
                continue
            newdstpiece = newmatch.board.getfield(mv.dst)
            newmatch.board.setfield(mv.src, PIECES['blk'])
            newmatch.board.setfield(mv.dst, piece)
            if(cnewpiece is not None){
                attacked.clear()
                supported.clear()
                cnewpiece.find_attacks_and_supports(attacked, supported)
                if(forks(piece, attacked)){
                    is_fork_threat = true;
                    newmatch.board.setfield(mv.src, piece)
                    newmatch.board.setfield(mv.dst, newdstpiece)
                    break
            newmatch.board.setfield(mv.src, piece)
            newmatch.board.setfield(mv.dst, newdstpiece)
    ###
    return is_fork_threat
*/

    bool flees(cMatch &match, int piece, cMove &move){
        list<cTouch> frdlytouches_new, enmytouches_new;
        list<cTouch> frdlytouches_old, enmytouches_old;
        int lower_enmy_cnt_old = 0
        int lower_enmy_cnt_new = 0
        int piece = match.board.getfield(move.src);
        if(piece == mWKG || piece == mBKG){
            return false;
        }
        collect_touches_for_both_colors(match.board, move.src, PIECES_COLORS[piece], frdlytouches_old, enmytouches_old);

        match.do_move(move.src, move.dst, move.prompiece)
        collect_touches_for_both_colors(match.board, move.dst, PIECES_COLORS[piece], frdlytouches_new, enmytouches_new);
        match.undo_move()

        if(enmytouches_old.size() > 0 && frdlytouches_old.size() < frdlytouches_new.size()){
            return true;
        }
        if(enmytouches_old.size() > enmytouches_new.size()){
            return true;
        }
        for(auto &enmy : enmytouches_old){
            if(PIECES_RANK[enmy.piece] < PIECES_RANK[piece]){
                lower_enmy_cnt_old += 1;
            }
        }
        for(auto &enmy : enmytouches_new){
            if(PIECES_RANK[enmy.piece] < PIECES_RANK[piece]){
                lower_enmy_cnt_new += 1;
            }
        }
        if(lower_enmy_cnt_old > lower_enmy_cnt_new){
            return true;
        }
        else{
            return false;
        }
    }


    void find_touches_after_move(cMatch &match, cMove &move, list<cTouch> &supported, list<cTouch> &attacked){
        match.do_move(move.src, move.dst, move.prompiece)
        cPiece cpiece(&(match.board), (move.dst);
        cpiece.find_attacks_and_supports(supported, attacked);
        match.undo_move();
    }


    void find_rook_touches_after_castling(cMatch &match, cMove &move, cPiece int &rook, list<cTouch> &attacked, list<cTouch> &supported){
        match.do_move(move.src, move.dst, move.prompiece);
        if((move.src % 8) + 2 == move.dst % 8){
            rook = match.board.getfield(move.dst - 1);
            cPiece cpiece(match.board, move.dst - 1);
            cpiece.find_attacks_and_supports(attacked, supported);
        }
        else{
            rook = match.board.getfield(move.dst + 1);
            cPiece cpiece(match.board, move.dst + 1);
            crook.find_attacks_and_supports(attacked, supported);
        }
        match.undo_move()
    }

    /*
    bool does_unpin(cMatch &match, int piece, cMove &move){
        color = match.color_of_piece(piece);
        //pinlines_before = search_lines_of_pin(match, color, move.src, move.dst)
        match.do_move(move.src, move.dst, move.prompiece);
        //pinlines_after = search_lines_of_pin(match, color, move.dst, None)
        match.undo_move()
        if(pinlines_after.size() < pinlines_before.size()){
            return true;
        }
        for(auto &pbefore : pinlines_before){
            bool identical = false;
            for(auto &pafter : pinlines_after){
                if(pbefore.field == pafter.field){
                    identical = true;
                }
            }
            if(identical == false){
                return true;
            }
        }
        return false;
    }
    */


    bool defends_check(cMatch &match){
        if(match.next_color() == COLORS["white"]){
            return is_field_touched(match.board, match.board.wKg, COLORS["black"], EVAL_MODES["ignore-pins"]);
        }
        else{
            return is_field_touched(match.board, match.board.bKg, COLORS["white"], EVAL_MODES["ignore-pins"]);
        }
    }

/*
def find_disclosures(match, move){
    discl_supported = []
    discl_attacked = []
    piece = match.board.getfield(move.src)
    cpiece = match.obj_for_piece(piece, move.src)
    mv_dir = cpiece.dir_for_move(move.src, move.dst)
    targets = [[cRook.STEPS[0], PIECES_BARE[PIECES['wRk']]],
               [cRook.STEPS[2], PIECES_BARE[PIECES['wRk']]],
               [cBishop.STEPS[0], PIECES_BARE[PIECES['wBp']]],
               [cBishop.STEPS[2], PIECES_BARE[PIECES['wBp']]]]
    ###
    match.do_move(move.src, move.dst, move.prompiece)
    for target in targets:
        fst_field, snd_field = match.board.search_bi_dirs(move.src, target[0], 6)
        if(fst_field is None or snd_field is None){
            continue
        if(fst_field == move.dst or snd_field  == move.dst){
            continue
        fst_piece = match.board.getfield(fst_field)
        cfirst = match.obj_for_piece(fst_piece, fst_field)
        """first_dir = cfirst.dir_for_move(target[0], move.src)
        if(first_dir == mv_dir or REVERSE_DIRS[first_dir] == mv_dir){
            continue"""
        snd_piece = match.board.getfield(snd_field)
        csecond = match.obj_for_piece(snd_piece, snd_field)
        if(PIECES_BARE[cfirst.piece] == target[1] or
           PIECES_BARE[cfirst.piece] == PIECES_BARE[PIECES['wQu']]){
            if(cpiece.color == cfirst.color){
                if(cfirst.color == csecond.color){
                    discl_supported.append(cTouch(csecond.piece, csecond.pos))
                else:
                    discl_attacked.append(cTouch(csecond.piece, csecond.pos))
        if(PIECES_BARE[csecond.piece] == target[1] or
           PIECES_BARE[csecond.piece] == PIECES_BARE[PIECES['wQu']]){
            if(cpiece.color == csecond.color){
                if(csecond.color == cfirst.color){
                    discl_supported.append(cTouch(cfirst.piece, cfirst.pos))
                else:
                    discl_attacked.append(cTouch(cfirst.piece, cfirst.pos))    
    match.undo_move()
    ###
    match.board.setfield(move.src, PIECES['blk'])
    for item in discl_attacked:
        list_field_touches_beyond(match, item, match.color_of_piece(item.piece))
    for item in discl_supported:
        list_field_touches_beyond(match, item, match.color_of_piece(item.piece))
    match.board.setfield(move.src, piece)
    ###
    return discl_supported, discl_attacked


def blocks(match, piece, move){
    STEPS = [8, 1, 9, -7]
    color = match.color_of_piece(piece)
    #frdlytouches_before_count = 0
    enmytouches_before_count = 0
    #frdlytouches_after_count = 0
    enmytouches_after_count = 0
    for step in STEPS:
        dst1, dst2 = match.board.search_bi_dirs(move.dst, step)
        if(dst1 is not None){
            if(dst1 == move.src or dst2 == move.src){
                continue
            piece1 = match.board.getfield(dst1)
            piece2 = match.board.getfield(dst2)
            if(match.color_of_piece(piece1) == match.color_of_piece(piece2)){
                continue
            if(match.color_of_piece(piece1) == color){
                frdlytouches, enmytouches = list_all_field_touches(match, dst1, color)
            else:
                frdlytouches, enmytouches = list_all_field_touches(match, dst2, color)
            enmytouches_before_count += len(enmytouches)
    ###
    match.do_move(move.src, move.dst, move.prompiece)
    for step in STEPS:
        dst1, dst2 = match.board.search_bi_dirs(move.dst, step)
        if(dst1 is not None){
            if(dst1 == move.src or dst2 == move.src){
                continue
            piece1 = match.board.getfield(dst1)
            piece2 = match.board.getfield(dst2)
            if(match.color_of_piece(piece1) == match.color_of_piece(piece2)){
                continue
            if(match.color_of_piece(piece1) == color){
                frdlytouches, enmytouches = list_all_field_touches(match, dst1, color)
            else:
                frdlytouches, enmytouches = list_all_field_touches(match, dst2, color)
            enmytouches_after_count += len(enmytouches)
    match.undo_move()
    ###
    if(enmytouches_after_count < enmytouches_before_count){
           return true;
    else:
        return false;

*/

    bool running_pawn(cMatch &match, cMove &move){
        cPiece cpiece(&(match.board), move.src);
        return cpiece.is_running_pawn();
    }

/*
def defends_invasion(match, move){
    """piece = match.board.getfield(move.src)
    color = match.color_of_piece(piece)
    board =  [[0] * 8 for i in range(8)]

    for y in range(8){
        for x in range(8){
            piece = match.board.readfield(x, y)
            if(match.color_of_piece(piece) == COLORS['white']){
                board[y][x] += 1
            elif(match.color_of_piece(piece) == COLORS['black']){
                board[y][x] -= 1"""
    return false;


def controles_file(match, move){
    return false;
*/

    bool is_tactical_draw(cMatch &match, cMove &move){
        if(match.is_fifty_moves_rule()){
            return true;
        }
        match.do_move(move.src, move.dst, move.prompiece);
        is_move_repetition = match.is_move_repetition();
        match.undo_move();
        return is_move_repetition;
    }

/*
def is_progress(match, move){
    return false;


def is_approach_of_opp_king(match, piece, move){
    if(cMatch.color_of_piece(piece) == COLORS['white']){
        oppkg = match.board.bKg
    else:
        oppkg = match.board.wKg
    kgx = oppkg % 8
    kgy = oppkg // 8
    x1 = move.src % 8
    y1 = move.src // 8
    x2 = move.dst % 8
    y2 = move.dst // 8
    diff1 = abs(kgx - x1) + abs(kgy - y1)
    diff2 = abs(kgx - x2) + abs(kgy - y2)
    return diff2 < diff1

*/
