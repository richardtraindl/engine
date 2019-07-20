
#include "./bishop.hpp"


    using namespace std;

    array<int, 8> cBishop::DIRS_ARY = {DIRS["nth-est"], DIRS["sth-wst"], DIRS["nth-wst"], DIRS["sth-est"],
                                       DIRS["undef"], DIRS["undef"], DIRS["undef"], DIRS["undef"]};

    array<int, 8> cBishop::STEPS = {9, -9, 7, -7, 0, 0, 0, 0};

    int cBishop::MAXCNT = 7;

    cBishop::cBishop(cBoard *board, int pos) : cPiece(board, pos){
    }
    
    array<int, 8> cBishop::get_dirs_ary() { return DIRS_ARY; }

    array<int, 8> cBishop::get_steps() { return STEPS; }

    int cBishop::get_maxcnt() { return MAXCNT; }

    int cBishop::dir_for_move(int src, int dst){
        if(cBoard::is_inbounds_core(src, dst) == false){
            return DIRS["undef"];
        }
        if(cBoard::is_nth_est(src, dst)){
            return DIRS["nth-est"];
        }
        if(cBoard::is_sth_wst(src, dst)){
            return DIRS["sth-wst"];
        }
        if(cBoard::is_sth_est(src, dst) == true){
            return DIRS["sth-est"];
        }
        if(cBoard::is_nth_wst(src, dst) == true){
            return DIRS["nth-wst"];
        }
        else{
            return DIRS["undef"];
        }
    }
    
    int cBishop::step_for_dir(int dir){
        if(dir == DIRS["nth-est"]){
            return 9;
        }
        if(dir == DIRS["sth-wst"]){
            return -9;
        }
        if(dir == DIRS["nth-wst"]){
            return 7;
        }
        if(dir == DIRS["sth-est"]){
            return -7;
        }
        else{
            return 0;
        }
    }

