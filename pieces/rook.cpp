
#include "./rook.hpp"
#include "./searchforpiece.hpp"
#include "../values.hpp"
#include "../helper.hpp"

using namespace std;

    int cRook::DIRS_ARY[4] = {DIRS["nth"], DIRS["sth"], DIRS["est"], DIRS["wst"]};
    int cRook::STEPS[4] = {8, -8, 1, -1};
    int cRook::MV_STEPS[4][2] = {{8, PIECES["blk"]}, {-8, PIECES["blk"]},  {1, PIECES["blk"]}, {-1, PIECES["blk"]}};

    int cRook::dir_for_move(int src, int dst){
        /*
        if(match->board.is_inbounds_core(src, dst) == false){
            return DIRS["undef"];
        }
        if(is_nth(src, dst)){
            return DIRS["nth"];
        }
        if(is_sth(src, dst)){
            return DIRS["sth"];
        }
        if(is_est(src, dst)){
            return DIRS["est"];
        }
        if(is_wst(src, dst)){
            return DIRS["wst"];
        }*/
        return DIRS["undef"];
    }

    int cRook::step_for_dir(int dir){
        if(dir == DIRS["nth"]){
            return 8;
        }
        if(dir == DIRS["sth"]){
            return -8;
        }
        if(dir == DIRS["est"]){
            return 1;
        }
        if(dir == DIRS["wst"]){
            return -1;
        }
        return 0;
    }

    cMove cRook::do_move(int dst, int prompiece, int movecnt){
        int dstpiece = match->board.getfield(dst);
        cMove move = cPiece::do_move(dst, prompiece, movecnt);
        int srcx = pos % 8;
        int srcy = pos / 8;
        if(color == COLORS["white"]){
            if(srcx == match->board.COLS["A"] && srcy == match->board.RANKS["1"] && 
               match->board.wRkA_first_move_on == -1){
                match->board.wRkA_first_move_on = movecnt;
            }
            if(srcx == match->board.COLS["H"] && srcy == match->board.RANKS["1"] && 
                 match->board.wRkH_first_move_on == -1){
                match->board.wRkH_first_move_on = movecnt;
            }
        }
        else{
            if(srcx == match->board.COLS["A"] && srcy == match->board.RANKS["8"] && 
               match->board.bRkA_first_move_on == -1){
                match->board.bRkA_first_move_on = movecnt;
            }
            if(srcx == match->board.COLS["H"] && srcy == match->board.RANKS["8"] && 
                 match->board.bRkH_first_move_on == -1){
                match->board.bRkH_first_move_on = movecnt;
            }
        }
        return move;
    }

    bool cRook::undo_move(cMove *move, int movecnt){
        if(cPiece::undo_move(move, movecnt) == false){
            return false;
        }
        if(piece == PIECES["wRk"]){
            if(match->board.wRkA_first_move_on != -1 && match->board.wRkA_first_move_on == movecnt){
                match->board.wRkA_first_move_on = -1;
            }
            if(match->board.wRkH_first_move_on != -1 && match->board.wRkH_first_move_on == movecnt){
                match->board.wRkH_first_move_on = -1;
            }
        }
        else{
            if(match->board.bRkA_first_move_on != -1 && match->board.bRkA_first_move_on == movecnt){
                match->board.bRkA_first_move_on = -1;
            }
            if(match->board.bRkH_first_move_on != -1 && match->board.bRkH_first_move_on == movecnt){
                match->board.bRkH_first_move_on = -1;
            }
        }
        return true;
    }

