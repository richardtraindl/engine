
#include "./bishop.hpp"


    using namespace std;

    cBishop::cBishop(cBoard *board, int pos) : cPiece(board, pos){
    }

    unsigned cBishop::DIRS_ARY[4] = 
        {DIRS["nth-est"], DIRS["sth-wst"], DIRS["nth-wst"], DIRS["sth-est"]};
    int cBishop::STEPS[4] = 
        {9, -9, 7, -7};
    int cBishop::MV_STEPS[4][2] = 
        {{9, PIECES["blk"]}, {-9, PIECES["blk"]}, {7, PIECES["blk"]}, {-7, PIECES["blk"]}};

    int cBishop::dir_for_move(int src, int dst){
        if(cBoard::is_inbounds_core(src, dst) == false){
            return DIRS["undef"];
        }
        if(board->is_nth_est(src, dst)){
            return DIRS["nth-est"];
        }
        if(board->is_sth_wst(src, dst)){
            return DIRS["sth-wst"];
        }
        if(board->is_sth_est(src, dst) == true){
            return DIRS["sth-est"];
        }
        if(board->is_nth_wst(src, dst) == true){
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

