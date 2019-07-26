 
    #include <boost/multiprecision/cpp_int.hpp>
    #include "./analyze_position.hpp"
    #include "../pieces/piece.hpp"
    #include "../pieces/searchforpiece.hpp"
    #include "../values.hpp"

    using namespace boost::multiprecision;
    using namespace boost::multiprecision::literals;


    int score_traps_and_touches(cMatch &match){
        int score = 0;
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBLK){
                continue;
            }
            cPiece cpiece(&(match.board), idx);
            score += cpiece.score_touches();
            if(cpiece.is_trapped()){
                score += SCORES[cpiece.piece] / 3;
            }
        }
        return score;
    }


    /*int score_controled_horizontal_files(cMatch &match){
        int score = 0;
        const uint256_t row7_row8_0 = 0x000000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF_cppui;
        const uint256_t row7_row8_1 = 0x000000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF_cppui;
        const uint256_t row7_row8_2 = 0x000000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF_cppui;
        const uint256_t row7_row8_3 = 0x000000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF_cppui;
        uint256_t wrooks = match.board.fields;
        cBoard::mask_pieces(wrooks, mWRK);
        uint256_t wqueens = match.board.fields;
        cBoard::mask_pieces(wqueens, mWQU);
        if(wrooks & row7_row8 || wqueens & row7_row8){
            score += ATTACKED_SCORES[mBKN];
        }

        const uint256_t row1_row2 = 0xFFFFFFFFFFFFFFFF000000000000000000000000000000000000000000000000_cppui;
        uint256_t brooks = match.board.fields;
        cBoard::mask_pieces(brooks, mBRK);
        uint256_t bqueens = match.board.fields;
        cBoard::mask_pieces(bqueens, mBQU);
        if(brooks & row1_row2 || bqueens & row1_row2){
            score += ATTACKED_SCORES[mWKN];
        }
        return score;
    }


    int score_controled_vertical_files(cMatch &match){
        int score = 0;
        const uint256_t wpwcolumn = 0x0000000010000000100000001000000010000000100000001000000000000000_cppui;
        const uint256_t wrkcolumn = 0x4000000040000000400000004000000040000000400000004000000040000000_cppui;
        const uint256_t wqucolumn = 0x5000000050000000500000005000000050000000500000005000000050000000_cppui;
        uint256_t wpawns = match.board.fields;
        cBoard::mask_pieces(wpawns, mWPW);
        if(wpawns & wpwcolumn == 0x0){
            uint256_t wrooks = match.board.fields;
            cBoard::mask_pieces(wrooks, mWRK);
            uint256_t wqueens = match.board.fields;
            cBoard::mask_pieces(wqueens, mWQU);
            if(wrooks & wrkcolumn || wqueens & wqucolumn){
                score += ATTACKED_SCORES[mBKN];
            }
        }
        const uint256_t bpwcolumn = 0x0000000090000000900000009000000090000000900000009000000000000000_cppui;
        const uint256_t brkcolumn = 0xC0000000C0000000C0000000C0000000C0000000C0000000C0000000C0000000_cppui;
        const uint256_t bqucolumn = 0xD0000000D0000000D0000000D0000000D0000000D0000000D0000000D0000000_cppui;
        uint256_t bpawns = match.board.fields;
        cBoard::mask_pieces(bpawns, mBPW);
        if(bpawns & bpwcolumn == 0x0){
            uint256_t brooks = match.board.fields;
            cBoard::mask_pieces(brooks, mBRK);
            uint256_t bqueens = match.board.fields;
            cBoard::mask_pieces(bqueens, mBQU);
            if(brooks & brkcolumn || bqueens & bqucolumn){
                score += ATTACKED_SCORES[mWKN];
            }
        }
        return score;
    }*/


    int score_kings_safety(cMatch &match){
        int value = 0;
        cPiece cwpiece(&(match.board), match.board.wKg);
        if(cwpiece.is_safe_king() == false){
            value += ATTACKED_SCORES[mWQU] * 5;
        }
        cPiece cbpiece(&(match.board), match.board.bKg);
        if(cbpiece.is_safe_king() == false){
            value += ATTACKED_SCORES[mBQU] * 5;
        }
        return value;
    }


    int score_penalty_for_lost_castlings(cMatch &match){
        int score = 0;
        bool wcastling = false;
        bool bcastling = false;
        int idx = 0;
        for(list<cMove>::iterator it = match.minutes.begin(); it != match.minutes.end(); ++it){
            idx += 1;
            int piece = it->getprevfield(it->src);
            if(piece == mWKG || piece == mBKG){
                if(abs(it->src - it->dst) == 2){
                    if(idx % 2 == 1){
                        wcastling = true;
                    }
                    else{
                        bcastling = true;
                    }
                }
            }
        }
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


/* def score_weak_pawns(cMatch &match):
    value = 0
    for idx in range(64):
        piece = match.board.getfield(idx)
        if(piece == mWPW):
            cpawn = cWhitePawn(match, idx)
            if(cpawn.is_weak()):
                value += ATTACKED_SCORES[mWRK]
        elif(piece == mBPW):
            cpawn = cBlackPawn(match, idx)
            if(cpawn.is_weak()):
                value += ATTACKED_SCORES[mBRK]
    return value */


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
        // score += score_weak_pawns(match)
        return score;
    }


    int score_endgame(cMatch &match){
        int score = 0;
        int whiterate = ATTACKED_SCORES[mBPW];
        int white_step_rates[8] = { 0, 0, 1, 2, 3, 4, 5, 0};
        int blackrate = ATTACKED_SCORES[mWPW];
        int black_step_rates[8] = {0, 5, 4, 3, 2, 1, 0, 0} ;
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mWPW){
                cPiece cpiece(&(match.board), idx);
                if(cpiece.is_running_pawn()){
                    score += whiterate;
                    score += whiterate * white_step_rates[(idx / 8)];
                }
            }
            if(piece == mBPW){
                cPiece cpiece(&(match.board), idx);
                if(cpiece.is_running_pawn()){
                    score += blackrate;
                    score += blackrate * black_step_rates[(idx / 8)];
                }
            }
        }
        return score;
    }


    int score_position(cMatch &match, int movecnt){
        int score;
        int status = match.eval_status();
        if(movecnt == 0 && status != cMatch::STATUS["active"]){
            cout << "eval_status" << endl;
            if(status == cMatch::STATUS["winner_black"]){
                return SCORES[mWKG] + match.movecnt();
            }
            if(status == cMatch::STATUS["winner_white"]){
                return SCORES[mBKG] - match.movecnt();
            }
            else{ // draw
                return SCORES[mBLK];
            }
        }
        else{
            score = match->score;
            score += score_traps_and_touches(match);
            //score += score_kings_safety(match);
            //score += score_controled_horizontal_files(match);
            //score += score_controled_vertical_files(match);
            /*if(match.is_opening()){
                score += score_opening(match);
            }*/
            /*if(match.is_endgame()){
                score += score_endgame(match);
            }
            else{
                score += score_middlegame(match);
            }*/
            return score;
        }
    }


    bool is_stormy(cMatch &match){
        int color = match.next_color();
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

            list<cTouch> *friends, *enmies;
            collect_touches_for_both_colors(&(match.board), idx, piece_color, friends, enmies);

            /* if(piece == mWKG or piece == mBKG):
                if(len(enmytouches) > 0):
                    return True
                else:
                    continue */

            // if(len(enmytouches) > len(frdlytouches)):
            //    return True"""

            if(match.board.eval_pin_dir(idx) != DIRS["undef"]){
                 // ||                match->board.eval_soft_pin_dir != DIRS["undef"]){
                return true;
            }

            for(list<cTouch>::iterator it = enmies->begin(); it != enmies->end(); ++it){
                if(PIECES_RANKS[it->piece] < PIECES_RANKS[piece]){
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

