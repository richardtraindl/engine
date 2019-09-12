 
    #include "./analyze_position.hpp"
    #include "./analyze_helper.hpp"
    #include "../compute/calc.hpp"
    #include "../pieces/piece.hpp"
    #include "../pieces/searchforpiece.hpp"
    #include "../values.hpp"


    int score_traps_and_touches(cMatch &match){
        int score = 0;
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBLK){
                continue;
            }
            cPiece cpiece(match.board, idx);
            score += cpiece.score_touches();
            if(cpiece.is_trapped()){
                score += SCORES[cpiece.piece] / 3;
            }
        }
        return score;
    }


    int score_controled_horizontal_files(cMatch &match){
        int score = 0;
        for(int idx = 8; idx < 16; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBRK || piece == mBQU){
                list<cTouch> friends, enmies;
                collect_touches_for_both_colors(match.board, idx, COLORS["black"], friends, enmies);
                if(friends.size() >= enmies.size()){
                    score += ATTACKED_SCORES[PIECES["wRk"]];
                    break;
                }
            }
        }
        for(int idx = 48; idx < 56; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mWRK || piece == mWQU){
                list<cTouch> friends, enmies;
                collect_touches_for_both_colors(match.board, idx, COLORS["white"], friends, enmies);
                if(friends.size() >= enmies.size()){
                    score += ATTACKED_SCORES[PIECES["bRk"]];
                    break;
                }
            }
        }
        return score;
    }


    int score_controled_vertical_files(cMatch &match){
        int score = 0;
        bool vertical_wrk_or_wqu, vertical_brk_or_bqu;
        for(int idx = 0; idx < 8; ++idx){
            vertical_wrk_or_wqu = false;
            vertical_brk_or_bqu = false;
            int dst = match.board.search(idx, 8, 7);
            while(dst != cBoard::SNOK){
                int piece = match.board.getfield(dst);
                if(piece == mWPW || piece == mBPW){
                    vertical_wrk_or_wqu = false;
                    vertical_brk_or_bqu = false;
                    break;
                }
                if(piece == mWRK || piece == mWQU){
                    list<cTouch> friends, enmies;
                    collect_touches_for_both_colors(match.board, dst, COLORS["white"], friends, enmies);
                    if(friends.size() >= enmies.size()){
                        vertical_wrk_or_wqu = true;
                    }
                }
                if(piece == mBRK || piece == mBQU){
                    list<cTouch> friends, enmies;
                    collect_touches_for_both_colors(match.board, dst, COLORS["black"], friends, enmies);
                    if(friends.size() >= enmies.size()){
                        vertical_brk_or_bqu = true;
                    }
                }
                dst = match.board.search(dst, 8, 6);
            }
            if(vertical_wrk_or_wqu){
                score += ATTACKED_SCORES[PIECES["bBp"]];
            }
            if(vertical_brk_or_bqu){
                score += ATTACKED_SCORES[PIECES["wBp"]];
            }
        }
        return score;
    }


    int score_kings_safety(cMatch &match){
        int value = 0;
        int color = COLORS["undef"];

        cPiece cwhite_piece(match.board, match.board.wKg);
        bool white_king_is_safe = cwhite_piece.is_king_safe();

        cPiece cblack_piece(match.board, match.board.bKg);
        bool black_king_is_safe = cblack_piece.is_king_safe();

        /*if(white_king_is_safe == false || black_king_is_safe == false){
            int color = search_for_checkmate(match);
        }
        if(color == COLORS["white"]){
            return SCORES[mWKG];
        }
        if(color == COLORS["black"]){
            return SCORES[mBKG];
        }*/
        
        if(white_king_is_safe == false){
            value += ATTACKED_SCORES[mWQU] * 5;
        }
        if(black_king_is_safe == false){
            value += ATTACKED_SCORES[mBQU] * 5;
        }
        return value;
    }


    int score_penalty_for_lost_castlings(cMatch &match){
        int score = 0;
        bool wcastling = false;
        bool bcastling = false;
        int idx = 0;
        /*for(cMove move : match.minutes){
            idx += 1;
            int piece = move.getprevfield(move.src);
            if(piece == mWKG || piece == mBKG){
                if(abs(move.src - move.dst) == 2){
                    if(idx % 2 == 1){
                        wcastling = true;
                    }
                    else{
                        bcastling = true;
                    }
                }
            }
        }*/
        if(wcastling == false && (match.board.wKg_first_move_on != -1 ||
           (match.board.wRkA_first_move_on != -1 || match.board.wRkH_first_move_on != -1))){
            score += ATTACKED_SCORES[mWRK] * 2;
        }
        if(bcastling == false && (match.board.bKg_first_move_on != -1 ||
           (match.board.bRkA_first_move_on != -1 || match.board.bRkH_first_move_on  != -1))){
            score += ATTACKED_SCORES[mBRK] * 2;
        }
        return score;
    }


/* def score_penalty_for_multiple_moves(cMatch &match):
    value = 0
    white_moves = []
    black_moves = []
    for i in range(2):
        for move in match.minutes:
            if(move.captpiece):
                continue
            if(move.count % 2 == 1):
                white_moves.append(move)
            else:
                black_moves.append(move)
    for i in range(2):
        if(i == 0):
            moves = white_moves
            rate =  ATTACKED_SCORES[mWRK]
        else:
            moves = black_moves
            rate =  ATTACKED_SCORES[mBRK]
        idx = 0
        for move in moves:
            idx += 1
            mvtcnt = 0
            if(idx == len(moves)):
                break
            lower_move = move
            for higher_move in moves[idx:]:
                if(lower_move.dst == higher_move.src):
                    lower_move = higher_move
                    mvtcnt += 1
            if(mvtcnt >= 2):
                value += rate
    return value */


    int score_penalty_for_knight_bishop_on_baseline(cMatch &match){
        int score = 0;
        int idx;
        int knight;
        int bishop;
        int rate;
        for(int i = 0; i < 2; ++i){
            if(i == 0){
                idx = match.board.RANKS["1"] * 8;
                knight = mWKN;
                bishop = mWBP;
                rate = ATTACKED_SCORES[mWRK];
            }
            else{
                idx = match.board.RANKS["8"] * 8;
                knight = mBKN;
                bishop = mBBP;
                rate = ATTACKED_SCORES[mBRK];
            }
            for(int j = 0; j < 8; ++j){
                int piece = match.board.getfield(idx + j);
                if(piece == knight || piece == bishop){
                    score += rate;
                }
            }
        }
        return score;
    }


    int score_penalty_for_weak_fianchetto(cMatch &match){
        int score = 0;
        int piece = match.board.getfield(cBoard::COLS["B"] + cBoard::RANKS["2"] * 8);
        if(piece == mBLK){
            score += ATTACKED_SCORES[mWRK];
        }
        piece = match.board.getfield(cBoard::COLS["G"] + cBoard::RANKS["2"] * 8);
        if(piece == mBLK){
            score += ATTACKED_SCORES[mWRK];
        }
        piece = match.board.getfield(cBoard::COLS["B"] + cBoard::RANKS["7"] * 8);
        if(piece == mBLK){
            score += ATTACKED_SCORES[mBRK];
        }
        piece = match.board.getfield(cBoard::COLS["G"] + cBoard::RANKS["7"] * 8);
        if(piece == mBLK){
            score += ATTACKED_SCORES[mBRK];
        }
        return score;
    }


    int score_opening(cMatch &match){
        int score = 0;
        // score += score_penalty_for_multiple_moves(match);
        score += score_penalty_for_knight_bishop_on_baseline(match);
        score += score_penalty_for_lost_castlings(match);
        // score += score_weak_pawns(match);
        score += score_penalty_for_weak_fianchetto(match);
        return score;
    }


    int score_middlegame(cMatch &match){
        int score = 0;
        score += score_penalty_for_knight_bishop_on_baseline(match);
        score += score_controled_horizontal_files(match);
        score += score_controled_vertical_files(match);
        // score += score_weak_pawns(match)
        return score;
    }


    int score_endgame(cMatch &match){
        int score = 0;
        int whiterate = ATTACKED_SCORES[mBPW];
        int white_step_rates[8] = { 0, 1, 2, 8, 12, 16, 20, 0};
        int blackrate = ATTACKED_SCORES[mWPW];
        int black_step_rates[8] = { 0, 20, 16, 12, 8, 2, 1, 0} ;
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mWPW){
                cPiece cpiece(match.board, idx);
                if(cpiece.is_running_pawn()){
                    score += whiterate * white_step_rates[(idx / 8)];
                }
            }
            if(piece == mBPW){
                cPiece cpiece(match.board, idx);
                if(cpiece.is_running_pawn()){
                    score += blackrate * black_step_rates[(idx / 8)];
                }
            }
        }
        return score;
    }


    int score_position(cMatch &match, int movecnt){
        int score;
        int status = match.eval_status();
        if(movecnt == 0 && status != cMatch::STATUS["open"]){
            if(status == cMatch::STATUS["winner-black"]){
                return SCORES[mWKG] + match.minutes.size();
            }
            if(status == cMatch::STATUS["winner-white"]){
                return SCORES[mBKG] - match.minutes.size();
            }
            else{ // draw
                return SCORES[mBLK];
            }
        }
        else{
            score = match.score;
            score += score_traps_and_touches(match);
            score += score_kings_safety(match);
            if(match.is_opening()){
                score += score_opening(match);
            }
            else{
                if(match.is_endgame()){
                    score += score_endgame(match);
                }
                else{
                    score += score_middlegame(match);
                }
            }
            return score;
        }
    }


    bool is_stormy(cMatch &match){
        // is pawn on last row before promotion
        for(int x = 0; x < 8; ++x){
            int piece = match.board.getfield((x + 6 * 8));
            if(piece == mWPW){
                return true;
            }
        }
        for(int x = 0; x < 8; ++x){
            int piece = match.board.getfield((x + 1 * 8));
            if(piece == mBPW){
                return true;
            }
        }
        // attacks
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBLK){
                continue;
            }
            int piece_color = PIECES_COLORS[piece];

            list<cTouch> friends, enmies;
            collect_touches_for_both_colors(match.board, idx, piece_color, friends, enmies);

            /* if(piece == mWKG or piece == mBKG):
                if(len(enmytouches) > 0):
                    return True
                else:
                    continue */

            // if(len(enmytouches) > len(frdlytouches)):
            //    return True"""

            if(match.board.eval_pin_dir(idx) != DIRS["undef"]){
                return true;
            }

            for(cTouch touch : enmies){
                if(PIECES_RANKS[touch.piece] < PIECES_RANKS[piece]){
                    return true;
                }

                /* enmyfriends, enmyenemies = list_all_field_touches(match, Match.color_of_piece(enmy.piece), enmy.fieldx, enmy.fieldy)
                if(len(enmyenemies) == 0):
                    print("is_stormy: enmyenemies == 0")
                    return True */
            }
        }
        return false;
    }

