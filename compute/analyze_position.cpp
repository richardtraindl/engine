 
    #include <boost/multiprecision/cpp_int.hpp>
    #include "./analyze_position.hpp"

    using namespace boost::multiprecision;
    using namespace boost::multiprecision::literals;


    int score_traps_and_touches(cMatch *match){
        int score = 0;
        for(int idx = 0; idx < 64; ++idx){
            int piece = match->board.getfield(idx);
            if(piece == mBLK){
                continue;
            }
            cPiece cpiece(&(match->board), idx);
            score += cpiece.score_touches();
            if(cpiece.is_trapped()){
                score += SCORES[cpiece.piece] / 3;
            }
        }
        return score;
    }


    int score_controled_horizontal_files(cMatch *match){
        int score = 0;
        const uint256_t row7_row8_0 = 0x000000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF_cppui;
        const uint256_t row7_row8_1 = 0x000000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF_cppui;
        const uint256_t row7_row8_2 = 0x000000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF_cppui;
        const uint256_t row7_row8_3 = 0x000000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFF_cppui;
        uint256_t wrooks = match->board.fields;
        cBoard::mask_pieces(wrooks, mWRK);
        uint256_t wqueens = match->board.fields;
        cBoard::mask_pieces(wqueens, mWQU);
        if(wrooks & row7_row8 || wqueens & row7_row8){
            score += ATTACKED_SCORES[mBKN];
        }

        const uint256_t row1_row2 = 0xFFFFFFFFFFFFFFFF000000000000000000000000000000000000000000000000_cppui;
        uint256_t brooks = match.board.fields;
        cBoard::mask_pieces(brooks, mBRK);
        uint256_t bqueens = match->board.fields;
        cBoard::mask_pieces(bqueens, mBQU);
        if(brooks & row1_row2 || bqueens & row1_row2){
            score += ATTACKED_SCORES[mWKN];
        }
        return score;
    }


    int score_controled_vertical_files(match){
        int score = 0;
        const uint256_t wpwcolumn = 0x0000000010000000100000001000000010000000100000001000000000000000_cppui;
        const uint256_t wrkcolumn = 0x4000000040000000400000004000000040000000400000004000000040000000_cppui;
        const uint256_t wqucolumn = 0x5000000050000000500000005000000050000000500000005000000050000000_cppui;
        uint256_t wpawns = match->board.fields;
        cBoard::mask_pieces(wpawns, PIECES["wPw"]);
        if(wpawns & wpwcolumn == 0x0){
            uint256_t wrooks = match->board.fields;
            cBoard::mask_pieces(wrooks, PIECES["wRk"]);
            uint256_t wqueens = match->board.fields;
            cBoard::mask_pieces(wqueens, PIECES["wQu"]);
            if(wrooks & wrkcolumn || wqueens & wqucolumn){
                score += ATTACKED_SCORES[PIECES["bKn"]];
            }
        }
        const uint256_t bpwcolumn = 0x0000000090000000900000009000000090000000900000009000000000000000_cppui;
        const uint256_t brkcolumn = 0xC0000000C0000000C0000000C0000000C0000000C0000000C0000000C0000000_cppui;
        const uint256_t bqucolumn = 0xD0000000D0000000D0000000D0000000D0000000D0000000D0000000D0000000_cppui;
        uint256_t bpawns = match->board.fields;
        cBoard::mask_pieces(bpawns, PIECES["bPw"]);
        if(bpawns & bpwcolumn == 0x0){
            uint256_t brooks = match->board.fields;
            cBoard::mask_pieces(brooks, PIECES["bRk"]);
            uint256_t bqueens = match->board.fields;
            cBoard::mask_pieces(bqueens, PIECES["bQu"]);
            if(brooks & brkcolumn || bqueens & bqucolumn){
                score += ATTACKED_SCORES[PIECES["wKn"]];
            }
        }
        return score;
    }


    int score_kings_safety(cMatch *match){
        int value = 0;
        cKing cking = new cKing(match, match->board.wKg);
        if(cking.is_safe() == false){
            value += ATTACKED_SCORES[PIECES["wQu"]] * 5;
        }
        cking = cKing(match, match->board.bKg);
        if(cking.is_safe() == false){
            value += ATTACKED_SCORES[PIECES['bQu']] * 5;
        }
        return value;
    }


    int score_penalty_for_lost_castlings(cMatch *match){
        int score = 0;
        bool wcastling = false;
        bool bcastling = false;
        int idx = 0;
        for(list<cMove>::iterator it = match->minutes.begin(); it != match->minutes.end(); ++it){
            idx += 1;
            int piece = it->getprevfield(it->src);
            if(piece == PIECES["wKg"] || piece == PIECES["bKg"]){
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
        if(wcastling == false && (match->board.wKg_first_move_on != -1 ||
           (match->board.wRkA_first_move_on != -1 || match->board.wRkH_first_move_on != -1))){
            score += ATTACKED_SCORES[PIECES["wRk"]] * 2;
        }
        if(bcastling == false && (match->board.bKg_first_move_on != -1 ||
           (match->board.bRkA_first_move_on != -1 || match->board.bRkH_first_move_on  != -1))){
            score += ATTACKED_SCORES[PIECES["bRk"]] * 2;
        }
        return score;
    }


/* def score_penalty_for_multiple_moves(match):
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
            rate =  ATTACKED_SCORES[PIECES['wRk']]
        else:
            moves = black_moves
            rate =  ATTACKED_SCORES[PIECES['bRk']]
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


    int score_penalty_for_knight_bishop_on_baseline(cMatch *match){
        int score = 0;
        int idx;
        int knight;
        int bishop;
        int rate;
        for(int i = 0; i < 2; ++i){
            if(i == 0){
                idx = match->board.RANKS['1'] * 8;
                knight = PIECES['wKn'];
                bishop = PIECES['wBp'];
                rate = ATTACKED_SCORES[PIECES['wRk']];
            }
            else{
                idx = match.board.RANKS['8'] * 8;
                knight = PIECES['bKn'];
                bishop = PIECES['bBp'];
                rate = ATTACKED_SCORES[PIECES['bRk']];
            }
            for(int j = 0; j < 8; ++j){
                int piece = match->board.getfield(idx + j);
                if(piece == knight || piece == bishop){
                    score += rate;
                }
            }
        }
        return score;
    }


/* def score_weak_pawns(match):
    value = 0
    for idx in range(64):
        piece = match.board.getfield(idx)
        if(piece == PIECES['wPw']):
            cpawn = cWhitePawn(match, idx)
            if(cpawn.is_weak()):
                value += ATTACKED_SCORES[PIECES['wRk']]
        elif(piece == PIECES['bPw']):
            cpawn = cBlackPawn(match, idx)
            if(cpawn.is_weak()):
                value += ATTACKED_SCORES[PIECES['bRk']]
    return value */


    int score_penalty_for_weak_fianchetto(cMatch *match){
        int score = 0;
        int piece = match->board.getfield(cBoard::COLS["B"] + cBoard::RANKS["2"] * 8);
        if(piece == PIECES["blk"]){
            score += ATTACKED_SCORES[PIECES["wRk"]];
        }
        piece = match->board.getfield(cBoard::COLS["G"] + cBoard::RANKS["2"] * 8);
        if(piece == PIECES["blk"]){
            score += ATTACKED_SCORES[PIECES["wRk"]];
        }
        piece = match->board.getfield(cBoard::COLS["B"] + cBoard::RANKS["7"] * 8);
        if(piece == PIECES["blk"]){
            score += ATTACKED_SCORES[PIECES["bRk"]];
        }
        piece = match->board.getfield(cBoard::COLS["G"] + cBoard::RANKS["7"] * 8);
        if(piece == PIECES["blk"]){
            score += ATTACKED_SCORES[PIECES["bRk"]];
        }
        return score;
    }


    int score_opening(cMatch *match){
        int score = 0;
        // score += score_penalty_for_multiple_moves(match);
        score += score_penalty_for_knight_bishop_on_baseline(match);
        score += score_penalty_for_lost_castlings(match);
        // score += score_weak_pawns(match);
        score += score_penalty_for_weak_fianchetto(match);
        return score;
    }


    int score_middlegame(cMatch *match){
        int score = 0;
        score += score_penalty_for_knight_bishop_on_baseline(match);
        // score += score_weak_pawns(match)
        return score;
    }


    int score_endgame(cMatch *match){
        int score = 0;
        int whiterate = ATTACKED_SCORES[PIECES['bPw']]
        int white_step_rates = [ 0, 0, 1, 2, 3, 4, 5, 0]
        int blackrate = ATTACKED_SCORES[PIECES['wPw']]
        int black_step_rates = [0, 5, 4, 3, 2, 1, 0, 0 ]
        for(int idx = 0; idx < 64; ++idx){
            int piece = match->board.getfield(idx);
            if(piece == PIECES["wPw"]){
                cWhitePawn cpawn = cWhitePawn(match->board, idx);
                if(cpawn.is_running()){
                    score += whiterate;
                    score += whiterate * white_step_rates[(idx / 8)];
                }
            }
            if(piece == PIECES["bPw"]){
                cBlackPawn cpawn = cBlackPawn(match->board, idx);
                if(cpawn.is_running()){
                    score += blackrate;
                    score += blackrate * black_step_rates[(idx / 8)];
                }
            }
        }
        return score;
    }


    int score_position(cMatch *match, movecnt){
        int status = match.eval_status();
        if(movecnt == 0 && status != cMatch::STATUS["active"]){
            if(status == match.STATUS["winner_black"]){
                return SCORES[PIECES["wKg"]] + match->movecnt();
            }
            if(status == match.STATUS["winner_white"]){
                return SCORES[PIECES["bKg"]] - match->movecnt();
            }
            else{ // draw
                return SCORES[PIECES["blk"]];
            }
        }
        else{
            score = match->score;
            score += score_traps_and_touches(match);
            score += score_kings_safety(match);
            score += score_controled_horizontal_files(match);
            score += score_controled_vertical_files(match);
            if(match->is_opening()){
                score += score_opening(match);
            }
            if(match.is_endgame()){
                score += score_endgame(match);
            }
            else{
                score += score_middlegame(match);
            }
            return score;
        }
    }


    bool is_stormy(cMtch *match){
        int color = match.next_color();
        // is pawn on last row before promotion
        for(int x = 0; x < 8; ++x){
            int piece = match->board.getfield((x + 6 * 8));
            if(piece == PIECES["wPw"]){
                return true;
            }

        for(int x = 0; x < 8; ++x){
            int piece = match->board.getfield((x + 1 * 8));
            if(piece == PIECES["bPw"]){
                return true;
            }

        // attacks
        for(int idx = 0; idx < 64; ++idx){
            int piece = match->board.getfield(idx);
            if(piece == PIECES["blk"]){
                continue;
            }
            int piece_color = PIECES_COLOR[piece];

            list<cTouch> *friends, *enmies;
            collect_touches_for_both_colors(match->*board, idx, piece_color, friends, enmies);

            /* if(piece == PIECES['wKg'] or piece == PIECES['bKg']):
                if(len(enmytouches) > 0):
                    return True
                else:
                    continue */

            // if(len(enmytouches) > len(frdlytouches)):
            //    return True"""

            if(match->board.eval_pin_dir(idx) != DIRS["undef"] || match->board.eval_soft_pin_dir != DIRS["undef"]){
                return true;
            }

            for(list<cTouch>::iterator it = enmies->begin(); it != enmies->end()){
                if(PIECES_RANK[it->piece] < PIECES_RANK[piece]){
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

