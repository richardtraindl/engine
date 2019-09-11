
    #include "./piece_ext1.hpp"
    #include "../values.hpp"


    cMove *do_move_from_ext(cPiece *cpiece, int dst, int prev_dstpiece, int prompiece, int movecnt, int &score){
        cMove *move = new cMove(cpiece->pos, dst, prev_dstpiece, prompiece);
        cpiece->board->setfield(move->src, mBLK);
        cpiece->board->setfield(move->dst, cpiece->piece);
        score += SCORES[prev_dstpiece];
        
        if(cpiece->piece == mWBP || cpiece->piece == mBBP || 
           cpiece->piece == mWKN || cpiece->piece == mBKN ||
           cpiece->piece == mWQU || cpiece->piece == mBQU){
            return move;
        }

        if(cpiece->piece == mWPW){
            if(prompiece != mBLK){
                cpiece->board->setfield(dst, prompiece);
                score += REVERSED_SCORES[prompiece] - REVERSED_SCORES[mWPW];
                return move;
            }
            if(prev_dstpiece == mBLK && cpiece->pos % 8 < dst % 8){
                int enpass = cpiece->pos + 1;
                int captpiece = cpiece->board->getfield(enpass);
                cpiece->board->setfield(enpass, mBLK);
                score += SCORES[captpiece];
                return move;
            }
            if(prev_dstpiece == mBLK && cpiece->pos % 8 > dst % 8){
                int enpass = cpiece->pos - 1;
                int captpiece = cpiece->board->getfield(enpass);
                cpiece->board->setfield(enpass, mBLK);
                score += SCORES[captpiece];
                return move;
            }
            return move;
        }

        if(cpiece->piece == mBPW){
            if(prompiece != mBLK){
                cpiece->board->setfield(dst, prompiece);
                score += REVERSED_SCORES[prompiece] - REVERSED_SCORES[mBPW];
                return move;
            }
            if(prev_dstpiece == mBLK && cpiece->pos % 8 < dst % 8){
                int enpass = cpiece->pos + 1;
                int captpiece = cpiece->board->getfield(enpass);
                cpiece->board->setfield(enpass, mBLK);
                score += SCORES[captpiece];
                return move;
            }
            if(prev_dstpiece == mBLK && cpiece->pos % 8 > dst % 8){
                int enpass = cpiece->pos - 1;
                int captpiece = cpiece->board->getfield(enpass);
                cpiece->board->setfield(enpass, mBLK);
                score += SCORES[captpiece];
                return move;
            }
            return move;
        }

        if(cpiece->piece == mWKG || cpiece->piece == mBKG){
            if(cpiece->pos - dst == -2){
                int rook = cpiece->board->getfield(cpiece->pos + 3);
                cpiece->board->setfield(cpiece->pos + 3, mBLK);
                cpiece->board->setfield(dst - 1, rook);
            }
            if(cpiece->pos - dst == 2){
                int rook = cpiece->board->getfield(cpiece->pos - 4);
                cpiece->board->setfield(cpiece->pos - 4, mBLK);
                cpiece->board->setfield(dst + 1, rook);
            }
            if(cpiece->piece == mWKG){
                if(cpiece->board->wKg_first_move_on == -1){
                    cpiece->board->wKg_first_move_on = movecnt;
                }
                cpiece->board->wKg = dst;
            }
            else{
                if(cpiece->board->bKg_first_move_on == -1){
                    cpiece->board->bKg_first_move_on = movecnt;
                }
                cpiece->board->bKg = dst;
            }
            return move;
        }

        if(cpiece->piece == mWRK){
            int srcx = cpiece->pos % 8;
            int srcy = cpiece->pos / 8;
            if(srcx == cBoard::COLS["A"] && srcy == cBoard::RANKS["1"] && 
               cpiece->board->wRkA_first_move_on == -1){
                cpiece->board->wRkA_first_move_on = movecnt;
            }
            if(srcx == cBoard::COLS["H"] && srcy == cBoard::RANKS["1"] && 
                cpiece->board->wRkH_first_move_on == -1){
                cpiece->board->wRkH_first_move_on = movecnt;
            }
            return move;
        }
        if(cpiece->piece == mBRK){
            int srcx = cpiece->pos % 8;
            int srcy = cpiece->pos / 8;
            if(srcx == cBoard::COLS["A"] && srcy == cBoard::RANKS["8"] && 
               cpiece->board->bRkA_first_move_on == -1){
                cpiece->board->bRkA_first_move_on = movecnt;
            }
            if(srcx == cBoard::COLS["H"] && srcy == cBoard::RANKS["8"] && 
                 cpiece->board->bRkH_first_move_on == -1){
                cpiece->board->bRkH_first_move_on = movecnt;
            }
            return move;
        }
        return move;
    }


    bool undo_move_from_ext(cPiece *cpiece, cMove &move, int movecnt, int &score){
        cpiece->board->setfield(move.src, cpiece->piece);
        cpiece->board->setfield(move.dst, move.prev_dstpiece);
        score -= SCORES[move.prev_dstpiece];

        if(cpiece->piece == mWBP || cpiece->piece == mBBP || 
           cpiece->piece == mWKN || cpiece->piece == mBKN ||
           cpiece->piece == mWQU || cpiece->piece == mBQU){
            return true;
        }

        if(cpiece->piece == mWPW){
            if(move.prompiece != mBLK){
                cpiece->board->setfield(move.src, mWPW);
                score += SCORES[move.prompiece] - SCORES[mWPW];
                return true;

            }
            if(move.prev_dstpiece == mBLK && move.src % 8 != move.dst % 8){
                cpiece->board->setfield((move.dst - 8), mBPW);
                score -= SCORES[mBPW];
                return true;
            }
            return true;
        }

        if(cpiece->piece == mBPW){
            if(move.prompiece != mBLK){
                cpiece->board->setfield(move.src, mBPW);
                score += SCORES[move.prompiece] - SCORES[mBPW];
                return true;
            }
            if(move.prev_dstpiece == mBLK && move.src % 8 != move.dst % 8){
                cpiece->board->setfield((move.dst + 8), mWPW);
                score -= SCORES[mWPW];
                return true;
            }
            return true;
        }

        if(cpiece->piece == mWRK){
            if(cpiece->board->wRkA_first_move_on != -1 && cpiece->board->wRkA_first_move_on == movecnt){
                cpiece->board->wRkA_first_move_on = -1;
            }
            if(cpiece->board->wRkH_first_move_on != -1 && cpiece->board->wRkH_first_move_on == movecnt){
                cpiece->board->wRkH_first_move_on = -1;
            }
            return true;
        }

        if(cpiece->piece == mBRK){
            if(cpiece->board->bRkA_first_move_on != -1 && cpiece->board->bRkA_first_move_on == movecnt){
                cpiece->board->bRkA_first_move_on = -1;
            }
            if(cpiece->board->bRkH_first_move_on != -1 && cpiece->board->bRkH_first_move_on == movecnt){
                cpiece->board->bRkH_first_move_on = -1;
            }
            return true;
        }

        if(cpiece->piece == mWKG || cpiece->piece == mBKG){
            if(move.src - move.dst == -2){
                int rook = cpiece->board->getfield(move.src + 1);
                cpiece->board->setfield(move.src + 1, mBLK);
                cpiece->board->setfield(move.dst + 1, rook);
            }
            if(move.src - move.dst == 2){
                int rook = cpiece->board->getfield(move.src - 1);
                cpiece->board->setfield(move.src - 1, mBLK);
                cpiece->board->setfield(move.dst -2 , rook);
            }
            if(cpiece->piece == mWKG){
                if(cpiece->board->wKg_first_move_on != -1 && cpiece->board->wKg_first_move_on == movecnt){
                    cpiece->board->wKg_first_move_on = -1;
                }
                cpiece->board->wKg = move.src;
                return true;
            }
            else{
                if(cpiece->board->bKg_first_move_on != -1 && cpiece->board->bKg_first_move_on == movecnt){
                    cpiece->board->bKg_first_move_on = -1;
                }
                cpiece->board->bKg = move.src;
                return true;
            }
        }
        return true;
    }

