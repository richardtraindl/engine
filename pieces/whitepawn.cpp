
#include "./whitepawn.hpp"
#include "./piece.hpp"

    using namespace std;

    cWhitePawn::cWhitePawn(cBoard *board, int pos) : cPiece(board, pos), cPawn(board, pos){
    }

    int cWhitePawn::STEPS[2] = {9, 7};
    int cWhitePawn::MAXCNT = 1;
    int cWhitePawn::MV_STEPS_RANK2[4][2] = {{8, PIECES["blk"]}, {16, PIECES["blk"]}, {9, PIECES["blk"]}, {7, PIECES["blk"]}};
    int cWhitePawn::MV_STEPS_RANK7[12][2] = {{8, PIECES["wQu"]}, {8, PIECES["wRk"]}, {8, PIECES["wBp"]}, {8, PIECES["wKn"]},
                                             {9, PIECES["wQu"]}, {9, PIECES["wRk"]}, {9, PIECES["wBp"]}, {9, PIECES["wKn"]},
                                             {7, PIECES["wQu"]}, {7, PIECES["wRk"]}, {7, PIECES["wBp"]}, {7, PIECES["wKn"]}};
    int cWhitePawn::MV_STEPS[3][2] = {{8, PIECES["blk"]}, {9, PIECES["blk"]}, {7, PIECES["blk"]}};

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

    bool cWhitePawn::is_move_valid(int dst, int prompiece, list<cMove> *minutes){
        bool flag = false;
        for(int *step : MV_STEPS_RANK2){
            if((pos + *step) == dst && cBoard::is_inbounds(pos, dst, *step)){
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
        else{
            return false;
        }
        // check fields
        if(move_dir == DIRS["nth"]){
            if(dstpiece != PIECES["blk"]){
                return false;
            }
            if(pos - dst == -16){
                int midpiece = board->getfield(pos + 8);
                if(midpiece != PIECES["blk"]){
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
        if((dst / 8) == 7 && prompiece != PIECES["wQu"] && prompiece != PIECES["wRk"] && 
           prompiece != PIECES["wBp"] && prompiece != PIECES["wKn"]){
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
        if(dstpiece == PIECES["blk"] && enemy == PIECES["bPw"]){
            if((lastmove.src / 8) - (lastmove.dst / 8) == 2 && 
               (lastmove.dst / 8) == (pos / 8) && 
               (lastmove.dst % 8) == (dst % 8) && 
               (lastmove.dst / 8) - (dst / 8) == -1){
                return true;
            }
        }
        return false;
    }

