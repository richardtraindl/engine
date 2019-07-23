
    #include "./whitepawn.hpp"
    #include "../values.hpp"

    using namespace std;

    array<int, 8> cWhitePawn::STEPS = {9, 7, 0, 0, 0, 0, 0, 0};

    array<int, 10> cWhitePawn::MV_STEPS = {8, 16, 9, 7, 0, 0, 0, 0, 0, 0};

    array<int, 4> cWhitePawn::PROM_PIECES = {mWQU, mWRK, mWBP, mWKN};

    int cWhitePawn::MAXCNT = 1;

    cWhitePawn::cWhitePawn(cBoard *board, int pos) : cPiece(board, pos){
    }

    array<int, 8> cWhitePawn::get_steps() { return STEPS; }

    array<int, 10> cWhitePawn::get_mv_steps() { return MV_STEPS; }

    array<int, 4> cWhitePawn::get_prom_pieces(){
        if(pos >= 48){
            return PROM_PIECES;
        }
        else{
            return cPiece::PROM_PIECES;
        }
    }

    int cWhitePawn::get_maxcnt() { return MAXCNT; }

    int cWhitePawn::dir_for_move(int src, int dst){
        if(src == dst){
            return DIRS["undef"];
        }
        if(cBoard::is_inbounds_core(src, dst) == false){
            return DIRS["undef"];
        }
        if(src + 8 == dst || src + 16 == dst){
            return DIRS["nth"];
        }
        if(src + 9 == dst && src % 8 < dst % 8){
            return DIRS["nth-est"];
        }
        if(src + 7 == dst && src % 8 > dst % 8){
            return DIRS["nth-wst"];
        }
        return DIRS["undef"];
    }

    int cWhitePawn::step_for_dir(int dir){
        if(dir == DIRS["nth"]){
            return 8;
        }
        if(dir == DIRS["nth-est"]){
            return 9;
        }
        if(dir == DIRS["nth-wst"]){
            return 7;
        }
        return 0;
    }

    bool cWhitePawn::is_trapped(){
        return false; // pawn cannot be trapped
    }

    bool cWhitePawn::is_move_valid(int dst, int prompiece, list<cMove> *minutes){
        bool flag = false;
        for(auto &step : MV_STEPS){
            if((pos + step) == dst && cBoard::is_inbounds(pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){
            return false;
        }
        // check double step from second renk
        if(dst - pos == 16 && pos > 15){
            return false;
        }
        int move_dir = dir_for_move(pos, dst);
        if(move_dir == DIRS["undef"]){
            return false;
        }
        int pin_dir = board->eval_pin_dir(pos);
        int dstpiece = board->getfield(dst);
        // check pins
        if(pin_dir != DIRS["undef"]){
            if(move_dir == DIRS["nth"]){
                if(pin_dir != DIRS["nth"] && pin_dir != DIRS["sth"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
            if(move_dir == DIRS["nth-wst"]){
                if(pin_dir != DIRS["nth-wst"] && pin_dir != DIRS["sth-est"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
            if(move_dir == DIRS["nth-est"]){
                if(pin_dir != DIRS["nth-est"] && pin_dir != DIRS["sth-wst"] && pin_dir != DIRS["undef"]){
                    return false;
                }
            }
        }
        // check fields
        if(move_dir == DIRS["nth"]){
            if(dstpiece != mBLK){
                return false;
            }
            if(dst - pos == 16){
                int midpiece = board->getfield(pos + 8);
                if(midpiece != mBLK){
                    return false;
                }
            }
        }
        if(move_dir == DIRS["nth-est"] || move_dir == DIRS["nth-wst"]){
            if(PIECES_COLORS[dstpiece] != COLORS["black"]){
                return is_ep_move_ok(dst, minutes);
            }
        }
        // check promotion
        if((dst / 8) == 7 && prompiece != mWQU && prompiece != mWRK && 
           prompiece != mWBP && prompiece != mWKN){
            return false;
        }
        if((dst / 8) < 7 && prompiece != PIECES["blk"]){
            return false;
        }
        return true;
    }

    bool cWhitePawn::is_ep_move_ok(int dst, list<cMove> *minutes){
        cMove lastmove;
        if(minutes->empty()){
            return false;
        }
        else{
            lastmove = minutes->back();
        }
        int dstpiece = board->getfield(dst);
        int enemy = board->getfield(lastmove.dst);
        if(dstpiece == mBLK && enemy == mBPW){
            if((lastmove.src / 8) - (lastmove.dst / 8) == 2 && 
               (lastmove.dst / 8) == (pos / 8) && 
               (lastmove.dst % 8) == (dst % 8) && 
               (lastmove.dst / 8) - (dst / 8) == -1){
                return true;
            }
        }
        return false;
    }

    cMove *cWhitePawn::do_move(int dst, int prompiece, int movecnt, int *score){
        int dstpiece_before_mv = board->getfield(dst);
        cMove *move = cPiece::do_move(dst, prompiece, movecnt, score);
        /*if(prompiece != PIECES["blk"]){
            board->setfield(dst, prompiece);
            *score -= SCORES[prompiece] - SCORES[piece];
        }
        if(dstpiece_before_mv == PIECES["blk"] && pos % 8 != dst % 8){
            int enpass;
            if(pos % 8 < dst % 8){
                enpass = pos + 1;
            }
            else{
                enpass = pos - 1;
            }
            int captpiece = board->getfield(enpass);
            board->setfield(enpass, PIECES["blk"]);
            *score += SCORES[captpiece];
        }*/
        return move;
    }

    bool cWhitePawn::undo_move(cMove *move, int movecnt, int *score){
        if(cPiece::undo_move(move, movecnt, score) == false){
            return false;
        }
        int dstpiece_after_undo_mv = board->getfield(move->dst);
        if(move->prompiece && move->prompiece != mBLK){
            *score += SCORES[move->prompiece] - SCORES[mWPW];
        }
        if(dstpiece_after_undo_mv == PIECES["blk"] && move->src % 8 != move->dst % 8){
            *score -= SCORES[mBPW];
        }
        return true;
    }

    bool cWhitePawn::is_running(){
        int step = 8;
        int opp_pawn = mBPW;
        for(int idx = -1; idx < 2; ++idx){
            int src = pos + idx;
            int dst = board->search(src, step, 5);
            while(dst != mBLK){
                int piece = board->getfield(dst);
                if(piece == opp_pawn){
                    return false;
                }
                dst = board->search(dst, step, 5);
            }
        }
        return true;
    }
