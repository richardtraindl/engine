
#include "./rook.hpp"


using namespace std;

    cRook::cRook(cBoard *board, int pos) : cPiece::cPiece(board, pos){
    }

    unsigned cRook::DIRS_ARY[4] = 
        {DIRS["nth"], DIRS["sth"], DIRS["est"], DIRS["wst"]};
    int cRook::STEPS[4] = 
        {8, -8, 1, -1};
    int cRook::MV_STEPS[4][2] = 
        {{8, PIECES["blk"]}, {-8, PIECES["blk"]},  {1, PIECES["blk"]}, {-1, PIECES["blk"]}};

    int cRook::dir_for_move(int src, int dst){
        if(cBoard::is_inbounds_core(src, dst) == false){
            return DIRS["undef"];
        }
        if(cBoard::is_nth(src, dst)){
            return DIRS["nth"];
        }
        if(cBoard::is_sth(src, dst)){
            return DIRS["sth"];
        }
        if(cBoard::is_est(src, dst)){
            return DIRS["est"];
        }
        if(cBoard::is_wst(src, dst)){
            return DIRS["wst"];
        }
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

    cMove cRook::do_move(int dst, int prompiece, int movecnt, int *score){
        cMove move = cPiece::do_move(dst, prompiece, movecnt, score);
        int srcx = pos % 8;
        int srcy = pos / 8;
        if(color == COLORS["white"]){
            if(srcx == cBoard::COLS["A"] && srcy == cBoard::RANKS["1"] && 
               board->wRkA_first_move_on == -1){
                board->wRkA_first_move_on = movecnt;
            }
            if(srcx == cBoard::COLS["H"] && srcy == cBoard::RANKS["1"] && 
                 board->wRkH_first_move_on == -1){
                board->wRkH_first_move_on = movecnt;
            }
        }
        else{
            if(srcx == cBoard::COLS["A"] && srcy == cBoard::RANKS["8"] && 
               board->bRkA_first_move_on == -1){
                board->bRkA_first_move_on = movecnt;
            }
            if(srcx == cBoard::COLS["H"] && srcy == cBoard::RANKS["8"] && 
                 board->bRkH_first_move_on == -1){
                board->bRkH_first_move_on = movecnt;
            }
        }
        return move;
    }

    bool cRook::undo_move(cMove *move, int movecnt, int *score){
        if(cPiece::undo_move(move, movecnt, score) == false){
            return false;
        }
        if(piece == PIECES["wRk"]){
            if(board->wRkA_first_move_on != -1 && board->wRkA_first_move_on == movecnt){
                board->wRkA_first_move_on = -1;
            }
            if(board->wRkH_first_move_on != -1 && board->wRkH_first_move_on == movecnt){
                board->wRkH_first_move_on = -1;
            }
        }
        else{
            if(board->bRkA_first_move_on != -1 && board->bRkA_first_move_on == movecnt){
                board->bRkA_first_move_on = -1;
            }
            if(board->bRkH_first_move_on != -1 && board->bRkH_first_move_on == movecnt){
                board->bRkH_first_move_on = -1;
            }
        }
        return true;
    }

