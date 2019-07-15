
#include "./pawn.hpp"

    using namespace std;

    cPawn::cPawn(cBoard *board, int pos) : cPiece(board, pos){
    }

    bool cPawn::is_trapped(){
        return false; // pawn cannot be trapped
    }

    cMove *cPawn::do_move(int dst, int prompiece, int movecnt, int *score){
        int dstpiece_before_mv = board->getfield(dst);
        cMove *move = cPiece::do_move(dst, prompiece, movecnt, score);
        if(prompiece != PIECES["blk"]){
            board->setfield(dst, prompiece);
            *score -= SCORES[prompiece] - SCORES[piece];
        }
        if(dstpiece_before_mv == PIECES["blk"] && pos % 8 != dst % 8){
            int enpass;
            if(pos % 8 < dst % 8){
                enpass = pos + 1;
            }
            else{
                enpass = pos - 1;
            }
            int captpiece = board->getfield(enpass);
            board->setfield(enpass, PIECES["blk"]);
            *score += SCORES[captpiece];
        }
        return move;
    }

    bool cPawn::undo_move(cMove *move, int movecnt, int *score){
        if(cPiece::undo_move(move, movecnt, score) == false){
            return false;
        }
        int dstpiece_after_undo_mv = board->getfield(move->dst);
        if(move->prompiece && move->prompiece != PIECES["blk"]){
            int origin;
            if(color == COLORS["white"]){
                origin = PIECES["wPw"];
            }
            else{
                origin = PIECES["bPw"];
            }
            *score += SCORES[move->prompiece] - SCORES[origin];
        }
        if(dstpiece_after_undo_mv == PIECES["blk"] && move->src % 8 != move->dst % 8){
            int enemy;
            if(color == COLORS["white"]){
                enemy = PIECES["bPw"];
            }
            else{
                enemy = PIECES["wPw"];
            }
            *score -= SCORES[enemy];
        }
        return true;
    }

    bool cPawn::is_running(){
        int step, opp_pawn;
        if(color == COLORS["white"]){
            step = 8;
            opp_pawn = PIECES["bPw"];
        }
        else{
            step = -8;
            opp_pawn = PIECES["wPw"];
        }
        for(int idx = -1; idx < 2; ++idx){
            int src = pos + idx;
            int dst = board->search(src, step, 5);
            while(dst != PIECES["blk"]){
                int piece = board->getfield(dst);
                if(piece == opp_pawn){
                    return false;
                }
                dst = board->search(dst, step, 5);
            }
        }
        return true;
    }

