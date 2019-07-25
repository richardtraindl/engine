
    #include "./queen.hpp"
    #include "./rook.hpp"
    #include "./bishop.hpp"
    #include "../values.hpp"

    using namespace std;

    array<int, 8> cQueen::DIRS_ARY = {DIRS["nth"], DIRS["sth"], DIRS["est"], DIRS["wst"],
                                      DIRS["nth-est"], DIRS["sth-wst"], DIRS["nth-wst"], DIRS["sth-est"]};

    array<int, 8> cQueen::STEPS = {8, -8, 1, -1, 9, -9, 7, -7};

    array<int, 10> cQueen::MV_STEPS = {8, -8, 1, -1, 9, -9, 7, -7, 0, 0};

    int cQueen::MAXCNT = 7;

    cQueen::cQueen(cBoard *board, int pos) : cPiece(board, pos){
    }

    array<int, 8> cQueen::get_dirs_ary() { return DIRS_ARY; }

    array<int, 8> cQueen::get_steps() { return STEPS; }

    array<int, 10> cQueen::get_mv_steps() { return MV_STEPS; }

    int cQueen::get_maxcnt() { return MAXCNT; }

    int cQueen::dir_for_move(int src, int dst){
        cRook crook = cRook(board, pos);
        int dir = crook.dir_for_move(src, dst);
        if(dir != DIRS["undef"]){
            return dir;
        }
        cBishop cbishop = cBishop(board, pos);
        return cbishop.dir_for_move(src, dst);
    }
        
    int cQueen::step_for_dir(int dir){
        cRook crook = cRook(board, pos);
        int step = crook.step_for_dir(dir);
        if(step != 0){
            return step;
        }
        cBishop cbishop = cBishop(board, pos);
        return cbishop.step_for_dir(dir);
    }
   
    //bool cQueen::is_move_valid(int dst, int prompiece, list<cMove> *minutes){
    //   return cPiece::is_move_valid(dst, prompiece, minutes);
    //}
