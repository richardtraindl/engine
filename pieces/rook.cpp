
    #include "./rook.hpp"
    #include "../values.hpp"


using namespace std;

    array<int, 8> cRook::DIRS_ARY = {DIRS["nth"], DIRS["sth"], DIRS["est"], DIRS["wst"],
                                     DIRS["undef"], DIRS["undef"], DIRS["undef"], DIRS["undef"]};

    array<int, 8> cRook::STEPS = {8, -8, 1, -1, 0, 0, 0, 0};
    
    array<int, 10> cRook::MV_STEPS = {8, -8, 1, -1, 0, 0, 0, 0, 0, 0};

    int cRook::MAXCNT = 7;

    cRook::cRook(cBoard *board, int pos) : cPiece(board, pos){
    }

    array<int, 8> cRook::get_dirs_ary() { return DIRS_ARY; }

    array<int, 8> cRook::get_steps() { return STEPS; }

    array<int, 10> cRook::get_mv_steps() { return MV_STEPS; }

    int cRook::get_maxcnt() { return MAXCNT; }

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

    cMove *cRook::do_move(int dst, int prompiece, int movecnt, int *score){
        cMove *move = cPiece::do_move(dst, prompiece, movecnt, score);
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
        if(piece == mWRK){
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

