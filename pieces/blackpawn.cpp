
#include "./blackpawn.hpp"
#include "./piece.hpp"
    
    using namespace std;

    array<int, 8> cBlackPawn::STEPS = {-9, -7, 0, 0, 0, 0, 0, 0};

    array<int, 3> cBlackPawn::MV_STEPS = {-8, -7, -9};

    array<int, 4> cBlackPawn::MV_STEPS_START = {-8, -16, -7, -9};
    
    array<int, 4> cBlackPawn::PROM_PIECES = {mBQU, mBRK, mBBP, mBKN};

    int cBlackPawn::MAXCNT = 1;

    cBlackPawn::cBlackPawn(cBoard *board, int pos) : cPiece(board, pos), cPawn(board, pos){
    }

    array<int, 8> cBlackPawn::get_steps() { return STEPS; }

    array<int, 3> cBlackPawn::get_mv_steps() { return MV_STEPS; }

    array<int, 4> cBlackPawn::get_mv_steps_start() { return MV_STEPS_START; }

    int cBlackPawn::get_maxcnt() { return MAXCNT; }

    int cBlackPawn::dir_for_move(int src, int dst){
        if(src == dst){
            return DIRS["undef"];
        }
        if(cBoard::is_inbounds_core(src, dst) == false){
            return DIRS["undef"];
        }
        if(src - 8 == dst || src - 16 == dst){
            return DIRS["sth"];
        }
        if(src - 9 == dst && src % 8 > dst % 8){
            return DIRS["sth-wst"];
        }
        if(src - 7 == dst && src % 8 < dst % 8){
            return DIRS["sth-est"];
        }
        return DIRS["undef"];
    }

    int cBlackPawn::step_for_dir(int dir){
        if(dir == DIRS["sth"]){
            return -8;
        }
        if(dir == DIRS["sth-wst"]){
            return -9;
        }
        if(dir == DIRS["sth-est"]){
            return -7;
        }
        return DIRS["undef"];
    }

    bool cBlackPawn::is_move_valid(int dst, int prompiece, list<cMove> *minutes){
        bool flag = false;
        for(auto &step : MV_STEPS_START){
            if((pos + step) == dst && cBoard::is_inbounds(pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){
            return false;
        }
        int move_dir = dir_for_move(pos, dst);
        if(move_dir == DIRS["undef"]){
            return false;
        }
        int pin_dir = board->eval_pin_dir(pos);
        int dstpiece = board->getfield(dst);
        // check pins
        if(move_dir == DIRS["sth"]){
            if(pin_dir != DIRS["nth"] && pin_dir != DIRS["sth"] && pin_dir != DIRS["undef"]){
                return false;
            }
        }
        if(move_dir == DIRS["sth-wst"]){
            if(pin_dir != DIRS["nth-est"] && pin_dir != DIRS["sth-wst"] && pin_dir != DIRS["undef"]){
                return false;
            }
        }
        if(move_dir == DIRS["sth-est"]){
            if(pin_dir != DIRS["nth-wst"] && pin_dir != DIRS["sth-est"] && pin_dir != DIRS["undef"]){
                return false;
            }
        }
        else{
            return false;
        }
        // check fields
        if(move_dir == DIRS["sth"]){
            if(dstpiece != PIECES["blk"]){
                return false;
            }
            if(pos - dst == 16){
                int midpiece = board->getfield(pos - 8);
                if(midpiece != PIECES["blk"]){
                    return false;
                }
            }
        }
        if(move_dir == DIRS["sth-est"] || move_dir == DIRS["sth-wst"]){
            if(PIECES_COLORS[dstpiece] != COLORS["white"]){
                return is_ep_move_ok(dst, minutes);
            }
        }
        // check promotion
        if((dst / 8) == 0 && prompiece != PIECES["bQu"] && prompiece != PIECES["bRk"] && 
           prompiece != PIECES["bBp"] && prompiece != PIECES["bKn"]){
            return false;
        }
        if((dst / 8) > 0 && prompiece != PIECES["blk"]){
            return false;
        }
        return true;
    }

    bool cBlackPawn::is_ep_move_ok(int dst, list<cMove> *minutes){
        cMove lastmove;
        if(minutes->empty()){
            return false;
        }
        else{
            lastmove = minutes->back();
        }
        int dstpiece = board->getfield(dst);
        int enemy = board->getfield(lastmove.dst);
        if(dstpiece == PIECES["blk"] && enemy == PIECES["wPw"]){
            if((lastmove.src / 8) - (lastmove.dst / 8) == -2 && 
               (lastmove.dst / 8) == (pos / 8) && 
               (lastmove.dst % 8) == (dst % 8) && 
               (lastmove.dst / 8) - (dst / 8) == 1){
                return true;
            }
        }
        return false;
    }

