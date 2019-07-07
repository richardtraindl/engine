
#include "./knight.hpp"
#include "./searchforpiece.hpp"
#include "../values.hpp"
#include "../helper.hpp"

    using namespace std;


    int cKnight::DIRS_ARY[1] = {0};
    int cKnight::STEPS[8] = {17, 10, -6, -15, -17, -10, 6, 15};
    int cKnight::MV_STEPS[8][2] = {{17, PIECES["blk"]},  {10, PIECES["blk"]},  {-6, PIECES["blk"]}, 
                                   {-15, PIECES["blk"]}, {-17, PIECES["blk"]}, {-10, PIECES["blk"]}, 
                                   {6, PIECES["blk"]},   {15, PIECES["blk"]}};
    int cKnight::MAXCNT = 1;

    bool cKnight::is_trapped(){
        return false; //knight cannot be trapped
    }

    bool cKnight::is_move_valid(int dst, int prompiece){
        bool flag = false;
        for(const int step : STEPS){
            if(pos + step == dst && match->board.is_inbounds(pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){
            return false;
        }
        int pin_dir = DIRS["undef"]; // match->eval_pin_dir(pos);
        if(pin_dir != DIRS["undef"]){
            return false;
        }
        int dstpiece = match->board.getfield(dst);
        if(match->color_of_piece(dstpiece) == color){
            return false;
        }
        return true;
    }



