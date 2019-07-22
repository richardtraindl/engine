
#include "./knight.hpp"

    using namespace std;

    array<int, 8> cKnight::STEPS = {17, 10, -6, -15, -17, -10, 6, 15};

    array<int, 10> cKnight::MV_STEPS = {17, 10, -6, -15, -17, -10, 6, 15, 0, 0};

    int cKnight::MAXCNT = 1;

    cKnight::cKnight(cBoard *board, int pos) : cPiece(board, pos){
    }

    array<int, 8> cKnight::get_steps() { return STEPS; }

    array<int, 10> cKnight::get_mv_steps() { return MV_STEPS; }

    int cKnight::get_maxcnt() { return MAXCNT; }

    bool cKnight::is_trapped(){
        return false; //knight cannot be trapped
    }

    bool cKnight::is_move_valid(int dst, int prompiece, list<cMove> *minutes){
        bool flag = false;
        for(const int step : STEPS){
            if(pos + step == dst && board->is_inbounds(pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){
            return false;
        }
        int pin_dir = board->eval_pin_dir(pos);
        if(pin_dir != DIRS["undef"]){
            return false;
        }
        int dstpiece = board->getfield(dst);
        if(PIECES_COLORS[dstpiece] == color){
            return false;
        }
        return true;
    }
