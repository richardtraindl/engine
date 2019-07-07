
#include "./queen.hpp"
#include "./searchforpiece.hpp"
#include "../values.hpp"
#include "../helper.hpp"

using namespace std;


    int cQueen::DIRS_ARY[8] = 
        {DIRS["nth"],     DIRS["sth"],     DIRS["est"],     DIRS["wst"],
         DIRS["nth-est"], DIRS["sth-wst"], DIRS["nth-wst"], DIRS["sth-est"]};
    int cQueen::STEPS[8] = 
        {8, -8, 1, -1, 9, -9, 7, -7};
    int cQueen::MV_STEPS[8][2] = 
        {{8, PIECES["blk"]}, {-8, PIECES["blk"]},  {1, PIECES["blk"]}, {-1, PIECES["blk"]}, 
         {9, PIECES["blk"]}, {-9, PIECES["blk"]},  {7, PIECES["blk"]}, {-7, PIECES["blk"]}};

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
   
