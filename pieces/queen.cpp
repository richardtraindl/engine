
#include "./queen.hpp"
#include "./rook.hpp"
#include "./bishop.hpp"
    
    using namespace std;

    array<int, 8> cQueen::DIRS_ARY = {DIRS["nth"], DIRS["sth"], DIRS["est"], DIRS["wst"],
                                      DIRS["nth-est"], DIRS["sth-wst"], DIRS["nth-wst"], DIRS["sth-est"]};

    array<int, 8> cQueen::STEPS = {8, -8, 1, -1, 9, -9, 7, -7};

    int cQueen::MAXCNT = 7;

    cQueen::cQueen(cBoard *board, int pos) : cPiece(board, pos){
    }

    array<int, 8> cQueen::get_dirs_ary() { return DIRS_ARY; }

    array<int, 8> cQueen::get_steps() { return STEPS; }

    int cQueen::get_maxcnt() { return MAXCNT; }

    int cQueen::dir_for_move(int src, int dst){
        int dir = cRook::dir_for_move(src, dst);
        if(dir != DIRS["undef"]){
            return dir;
        }
        return cBishop::dir_for_move(src, dst);
    }
        
    int cQueen::step_for_dir(int dir){
        int step = cRook::step_for_dir(dir);
        if(step != 0){
            return step;
        }
        return cBishop::step_for_dir(dir);
    }
   
