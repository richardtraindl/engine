
#include "./blackpawn.hpp"
#include "./piece.hpp"
    
    using namespace std;

    cBlackPawn::cBlackPawn(cBoard *board, int pos) : cPiece(board, pos), cPawn(board, pos){
    }

    int cBlackPawn::STEPS[2] = {-9, -7};
    int cBlackPawn::MAXCNT = 1;
    int cBlackPawn::MV_STEPS_RANK7[4][2] = {{-8, PIECES["blk"]},  {-16, PIECES["blk"]}, {-7, PIECES["blk"]}, {-9, PIECES["blk"]}};
    int cBlackPawn::MV_STEPS_RANK2[12][2] = {{-8, PIECES["bQu"]}, {-8, PIECES["bRk"]},  {-8, PIECES["bBp"]}, {-8, PIECES["bKn"]},
                                             {-9, PIECES["bQu"]}, {-9, PIECES["bRk"]},  {-9, PIECES["bBp"]}, {-9, PIECES["bKn"]},
                                             {-7, PIECES["bQu"]}, {-7, PIECES["bRk"]},  {-7, PIECES["bBp"]}, {-7, PIECES["bKn"]}};
    int cBlackPawn::MV_STEPS[3][2] = {{-8, PIECES["blk"]}, {-7, PIECES["blk"]}, {-9, PIECES["blk"]}};

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
        for(int step : MV_STEPS_RANK7){
            if((pos + step[0]) == dst && cBoard::is_inbounds(pos, dst, step[0])){
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
            if(PIECES_COLOR[dstpiece] != COLORS["white"]){
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

