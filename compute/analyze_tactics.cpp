
    #include "./analyze_tactics.hpp"
    #include "../board.hpp"
    #include "../pieces/piece.hpp"
    #include "../pieces/piece_ext2.hpp"
    #include "../pieces/searchforpiece.hpp"
    #include "../values.hpp"


    // ToDo new!!!
    void _search_all_pindirs(cMatch &match, int color, int pos, int excl_pos, list<int*> &pindirs){
        int dirs[8] = {8, -8, 1, -1, 9, -9, 7, -7};
        int piece = match.board.getfield(pos);
        for(int i = 0; i < 8; ++i){
            int first, second, first_piece, second_piece;
            if(match.board.search_for_two_pieces_within_dir(first, second, pos, dirs[i], 7)){
                first_piece = match.board.getfield(first);
                second_piece = match.board.getfield(second);
                if(PIECES_COLORS[first_piece] == color && 
                   PIECES_COLORS[second_piece] == REVERSED_COLORS[color] && 
                   PIECES_RANKS[piece] > PIECES_RANKS[first_piece] &&  
                   PIECES_RANKS[piece] > PIECES_RANKS[second_piece]){
                    if(second_piece == mWQU || second_piece == mBQU){
                        int *int_ary = new int[2];
                        *int_ary = first;
                        *(int_ary + 1) = second;
                        pindirs.push_back(int_ary);
                    }
                    if(i < 4 && (second_piece == mWRK || second_piece == mBRK)){ 
                        int *int_ary = new int[2];
                        *int_ary = first;
                        *(int_ary + 1) = second;
                        pindirs.push_back(int_ary);
                    }
                    if(i >= 4 && (second_piece == mWBP || second_piece == mBBP)){
                          int *int_ary = new int[2];
                        *int_ary = first;
                        *(int_ary + 1) = second;
                        pindirs.push_back(int_ary);
                    }
                }
            }
        }
    }


    bool is_supported_running_pawn(cMatch &match, cTouch &supported){
        if(match.is_endgame() == false){
            return false;
        }
        cPiece cpiece(match.board, supported.pos);
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


    bool captures(cMatch &match, cPrioMove &priomove){
        int dstpiece = match.board.getfield(priomove.dst);
        if(dstpiece != mBLK){
            return true;
        }
        else{
            return false;
        }
    }

    bool captures_enpassant(cMatch &match, int piece, cPrioMove &priomove){
        if((piece == mWPW || piece == mBPW) && (priomove.src % 8) != (priomove.dst %8)){
            return true;
        }
        else{
            return false;
        }
    }


    bool forks(int piece, list<cTouch> &from_dstfield_attacked){
        if(from_dstfield_attacked.size() < 2){
            return false;
        }
        int count = 0;
        //for(list<cTouch>::iterator it = from_dstfield_attacked.begin(); it != from_dstfield_attacked.end(); ++it){
        for(cTouch touch : from_dstfield_attacked){
            if(touch.supporter_beyond.size() == 0 || PIECES_RANKS[touch.piece] >= PIECES_RANKS[piece]){
                count += 1;
            }
        }
        return count >= 2;
    }


    bool defends_fork(cMatch &match, int piece, int dstpiece, cPrioMove &priomove){
        list<cTouch> supported, attacked;
        int forking, forking_piece;
        if(dstpiece == mBLK){
            if(piece == mWPW){
                cPiece cpiece(match.board, priomove.src);
                if(is_ep_move_ok_for_whitepawn(&cpiece, priomove.dst, match.minutes)){
                    forking = priomove.dst - 8;
                    forking_piece = match.board.getfield(forking);
                }
                else{
                    return false;
                }
            }
            if(piece == mBPW){
                cPiece cpiece(match.board, priomove.src);
                if(is_ep_move_ok_for_blackpawn(&cpiece, priomove.dst, match.minutes)){
                    forking = priomove.dst + 8;
                    forking_piece = match.board.getfield(forking);
                }
                else{
                    return false;
                }
            }
            return false;
        }
        else{
            forking = priomove.dst;
            forking_piece = dstpiece;
        }
        cPiece cforking_piece(match.board, forking);
        cforking_piece.find_supported_and_attacked(supported, attacked);
        return forks(forking_piece, attacked);
    }


    bool threatens_fork(cMatch &match, int piece, cPrioMove &priomove){
        list<cTouch> supported, attacked;
        list<cMove *> moves;
        bool is_fork_threat = false;

        cMatch newmatch = match;
        newmatch.do_move(priomove.src, priomove.dst, priomove.prompiece);
        int first_move_dir = cPiece::dir_for_move(piece, priomove.src, priomove.dst);
        cPiece cpiece(newmatch.board, priomove.dst);
        cpiece.generate_moves(match, moves);
        for(cMove *move : moves){
            cPiece cnewpiece(newmatch.board, move->dst);
            int second_move_dir = cPiece::dir_for_move(piece, move->src, move->dst);
            if(first_move_dir == second_move_dir || REVERSE_DIRS[first_move_dir] == second_move_dir){
                continue;
            }
            int newdstpiece = newmatch.board.getfield(move->dst);
            newmatch.board.setfield(move->src, mBLK);
            newmatch.board.setfield(move->dst, piece);
            supported.clear();
            attacked.clear();
            cnewpiece.find_supported_and_attacked(supported, attacked);
            if(forks(piece, attacked)){
                is_fork_threat = true;
                newmatch.board.setfield(move->src, piece);
                newmatch.board.setfield(move->dst, newdstpiece);
                break;
            }
            newmatch.board.setfield(move->src, piece);
            newmatch.board.setfield(move->dst, newdstpiece);
        }
        return is_fork_threat;
    }


    bool flees(cMatch &match, int piece, cPrioMove &priomove){
        list<cTouch> frdlytouches_new, enmytouches_new;
        list<cTouch> frdlytouches_old, enmytouches_old;
        int lower_enmy_cnt_old = 0;
        int lower_enmy_cnt_new = 0;
        if(piece == mWKG || piece == mBKG){
            return false;
        }
        collect_touches_for_both_colors(match.board, priomove.src, PIECES_COLORS[piece], frdlytouches_old, enmytouches_old);

        match.do_move(priomove.src, priomove.dst, priomove.prompiece);
        collect_touches_for_both_colors(match.board, priomove.dst, PIECES_COLORS[piece], frdlytouches_new, enmytouches_new);
        match.undo_move();

        if(enmytouches_old.size() > 0 && frdlytouches_old.size() < frdlytouches_new.size()){
            return true;
        }
        if(enmytouches_old.size() > enmytouches_new.size()){
            return true;
        }
        for(auto &enmy : enmytouches_old){
            if(PIECES_RANKS[enmy.piece] < PIECES_RANKS[piece]){
                lower_enmy_cnt_old += 1;
            }
        }
        for(auto &enmy : enmytouches_new){
            if(PIECES_RANKS[enmy.piece] < PIECES_RANKS[piece]){
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

    void find_touches_after_move(cMatch &match, cPrioMove &priomove, list<cTouch> &supported, list<cTouch> &attacked){
        match.do_move(priomove.src, priomove.dst, priomove.prompiece);
        cPiece cpiece(match.board, priomove.dst);
        cpiece.find_supported_and_attacked(supported, attacked);
        match.undo_move();
    }


    void find_rook_touches_after_castling(cMatch &match, cPrioMove &priomove, int &rook, list<cTouch> &supported, list<cTouch> &attacked){
        match.do_move(priomove.src, priomove.dst, priomove.prompiece);
        if((priomove.src % 8) + 2 == priomove.dst % 8){
            rook = match.board.getfield(priomove.dst - 1);
            cPiece cpiece(match.board, priomove.dst - 1);
            cpiece.find_supported_and_attacked(supported, attacked);
        }
        else{
            rook = match.board.getfield(priomove.dst + 1);
            cPiece cpiece(match.board, priomove.dst + 1);
            cpiece.find_supported_and_attacked(supported, attacked);
        }
        match.undo_move();
    }


    void fill_supported(cMatch &match, int piece, cPrioMove &priomove, list<cTouch> &supported, cAnalyzeField &analyzedst, list<cExclude*> &excludes){
        for(cTouch touch : supported){
            int weight = weight_for_support(match, piece, priomove, touch, analyzedst);
            if(is_supported_running_pawn(match, touch)){
                priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["supports-running-pawn"], weight, PIECES_RANKS[piece]));
                if(find_excluded(excludes, priomove.src, touch.pos, DIRS["undef"], cTactic::DOMAINS["supports-running-pawn"])){
                    priomove.downgrade(cTactic::DOMAINS["supports-running-pawn"]);
                }
                else{
                    excludes.push_back(new cExclude(priomove.src, touch.pos, DIRS["undef"], cTactic::DOMAINS["supports-running-pawn"]));
                }
            }
            else{
                if(touch.attacker_beyond.size() > 0){
                    priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["supports"], weight, PIECES_RANKS[piece]));
                    if(find_excluded(excludes, priomove.src, touch.pos, DIRS["undef"], cTactic::DOMAINS["supports"])){
                        priomove.downgrade(cTactic::DOMAINS["supports"]);
                    }
                    else{
                        excludes.push_back(new cExclude(priomove.src, touch.pos, DIRS["undef"], cTactic::DOMAINS["supports"]));
                    }
                }
                else{
                    continue;
                }
            }
        }
    }


    void fill_attacked(cMatch &match, int piece, cPrioMove &priomove, list<cTouch> &attacked, cAnalyzeField &analyzedst, list<cExclude*> &excludes){
        for(cTouch touch : attacked){
            int weight = weight_for_attack(match, piece, priomove, touch, analyzedst);
            if(touch.piece == mWKG || touch.piece == mBKG){
                priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["attacks-king"], weight, PIECES_RANKS[piece]));
                if(find_excluded(excludes, priomove.src, touch.pos, DIRS["undef"], cTactic::DOMAINS["attacks-king"])){
                    priomove.downgrade(cTactic::DOMAINS["attacks-king"]);
                }
                else{
                    excludes.push_back(new cExclude(priomove.src, touch.pos, DIRS["undef"], cTactic::DOMAINS["attacks-king"]));
                }
            }
            else{
                priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["attacks"], weight, PIECES_RANKS[piece]));
                if(find_excluded(excludes, priomove.src, touch.pos, DIRS["undef"], cTactic::DOMAINS["attacks"])){
                    priomove.downgrade(cTactic::DOMAINS["attacks"]);
                }
                else{
                    excludes.push_back(new cExclude(priomove.src, touch.pos, DIRS["undef"], cTactic::DOMAINS["attacks"]));
                }
            }
        }
    }


    bool does_unpin(cMatch &match, int piece, cPrioMove &priomove){
        list<int*> pindirs_before, pindirs_after;
        _search_all_pindirs(match, PIECES_COLORS[piece], priomove.src, priomove.dst, pindirs_before);
        match.do_move(priomove.src, priomove.dst, priomove.prompiece);
        _search_all_pindirs(match, PIECES_COLORS[piece], priomove.src, priomove.dst, pindirs_after);
        match.undo_move();
        if(pindirs_after.size() < pindirs_before.size()){
            return true;
        }
        for(auto &pbefore : pindirs_before){
            bool identical = false;
            for(auto &pafter : pindirs_after){
                if(pbefore[0] == pafter[0]){
                    identical = true;
                }
            }
            if(identical == false){
                return true;
            }
        }
        return false;
    }


    bool defends_check(cMatch &match){
        if(match.next_color() == COLORS["white"]){
            return is_field_touched(match.board, match.board.wKg, COLORS["black"], EVAL_MODES["ignore-pins"]);
        }
        else{
            return is_field_touched(match.board, match.board.bKg, COLORS["white"], EVAL_MODES["ignore-pins"]);
        }
    }


    void find_disclosures(cMatch &match, int piece, cPrioMove &priomove, list<cTouch> &discl_supported, list<cTouch> &discl_attacked){
        int mvdir = cPiece::dir_for_move(piece, priomove.src, priomove.dst);
        int STEPS[4] = {8, 1, 9, 7};
        match.do_move(priomove.src, priomove.dst, priomove.prompiece);
        for(auto &step : STEPS){
            int first, second;
            if(match.board.search_bi_dirs(first, second, priomove.src, step, 7)){
                if(first == priomove.dst || second  == priomove.dst){
                    continue;
                }
                int piece1 = match.board.getfield(first);
                int piece2 = match.board.getfield(second);
                if(PIECES_COLORS[piece1] == PIECES_COLORS[piece2] &&
                   PIECES_COLORS[piece1] != PIECES_COLORS[piece]){
                    continue;
                }
                if(PIECES_COLORS[piece1] == PIECES_COLORS[piece] &&
                   (((step == 8 || step == 1) && cPiece::is_queen_or_rook(piece1)) ||
                    ((step == 9 || step == 7) && cPiece::is_queen_or_bishop(piece1)))){
                    if(PIECES_COLORS[piece2] == PIECES_COLORS[piece]){
                        discl_supported.push_back(*(new cTouch(piece2, second)));
                    }
                    else{
                        discl_attacked.push_back(*(new cTouch(piece2, second)));
                    }
                }

                if(PIECES_COLORS[piece2] == PIECES_COLORS[piece] &&
                   (((step == 8 || step == 1) && cPiece::is_queen_or_rook(piece2)) ||
                    ((step == 9 || step == 7) && cPiece::is_queen_or_bishop(piece2)))){
                    if(PIECES_COLORS[piece1] == PIECES_COLORS[piece]){
                        discl_supported.push_back(*(new cTouch(piece1, first)));
                    }
                    else{
                        discl_attacked.push_back(*(new cTouch(piece1, first)));
                    }
                }
            }
        }
        match.undo_move();
    }


    bool blocks(cMatch &match, int piece, cPrioMove &priomove){
        int STEPS[4] = {8, 1, 9, -7};
        list<cTouch> touches_before, touches_after;
        int oppcolor = REVERSED_COLORS[PIECES_COLORS[piece]];
        for(int i = 0; i < 2; ++i){
            if(i == 1){
                match.do_move(priomove.src, priomove.dst, priomove.prompiece);
            }
            for(auto &step : STEPS){
                int dst1, dst2;
                if(match.board.search_bi_dirs(dst1, dst2, priomove.dst, step, 7)){
                    if(dst1 == priomove.src || dst2 == priomove.src){
                        continue;
                    }
                    int piece1 = match.board.getfield(dst1);
                    int piece2 = match.board.getfield(dst2);
                    if(PIECES_COLORS[piece1] == PIECES_COLORS[piece2]){
                        continue;
                    }
                    if(PIECES_COLORS[piece1] == PIECES_COLORS[piece]){
                        if(i == 0){
                            collect_touches_for_color(match.board, dst1, oppcolor, touches_before);
                        }
                        else{
                            collect_touches_for_color(match.board, dst1, oppcolor, touches_after);
                        }
                    }
                    else{
                        if(i == 0){
                            collect_touches_for_color(match.board, dst2, oppcolor, touches_before);
                        }
                        else{
                            collect_touches_for_color(match.board, dst2, oppcolor, touches_after);
                        }
                    }
                }
            }
            if(i == 1){
                match.undo_move();
            }
        }
        if(touches_after.size() < touches_before.size()){
               return true;
        }
        else{
            return false;
        }
    }


    bool running_pawn(cMatch &match, cPrioMove &priomove){
        cPiece cpiece(match.board, priomove.src);
        return cpiece.is_running_pawn();
    }


    /*
    bool defends_invasion(cMatch &match, cPrioMove &priomove){
        return false;
    }
    */

    /*
    bool controles_file(cMatch &match, cPrioMove &priomove){
        return false;
    }
    */


    bool is_tactical_draw(cMatch &match, cPrioMove &priomove){
        if(match.is_fifty_moves_rule()){
            return true;
        }
        match.do_move(priomove.src, priomove.dst, priomove.prompiece);
        bool is_three_times_rep = match.is_three_times_rep();
        match.undo_move();
        return is_three_times_rep;
    }


    bool is_progress(cMatch &match, cPrioMove &priomove){
        return false;
    }


    bool is_approach_to_opp_king(cMatch &match, int piece, cPrioMove &priomove){
        if(piece != PIECES["wKg"] && piece != PIECES["bKg"]){
            return false;
        }
        
        int oppkg;
        if(PIECES_COLORS[piece] == COLORS["white"]){
            oppkg = match.board.bKg;
        }
        else{
            oppkg = match.board.wKg;
        }
        int kgx = oppkg % 8;
        int kgy = oppkg / 8;
        int x1 = priomove.src % 8;
        int y1 = priomove.src / 8;
        int x2 = priomove.dst % 8;
        int y2 = priomove.dst / 8;
        int diff1 = abs(kgx - x1) + abs(kgy - y1);
        int diff2 = abs(kgx - x2) + abs(kgy - y2);
        return diff2 < diff1;
    }


    bool leads_pawn_to_promotion(cMatch &match, int piece, cPrioMove &priomove){
        return false;
    }

