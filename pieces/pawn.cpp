
#include "./pawn.hpp"
#include "./searchforpiece.hpp"
#include "../values.hpp"
#include "../helper.hpp"

    using namespace std;

    bool cPawn::is_trapped(){
        return false; // pawn cannot be trapped
    }

    cMove cPawn::do_move(int dst, int prompiece, int movecnt){
        int dstpiece_before_mv = match->board.getfield(dst);
        cMove move = cPiece::do_move(dst, prompiece, movecnt);
        if(prompiece != PIECES["blk"]){
            match->board.setfield(dst, prompiece);
            match->score -= SCORES[prompiece] - SCORES[piece];
        }
        if(dstpiece_before_mv == PIECES["blk"] && pos % 8 != dst % 8){
            int enpass;
            if(pos % 8 < dst % 8){
                enpass = pos + 1;
            }
            else{
                enpass = pos - 1;
            }
            int captpiece = match->board.getfield(enpass);
            match->board.setfield(enpass, PIECES["blk"]);
            match->score += SCORES[captpiece];
        }
        return move;
    }

    bool cPawn::undo_move(cMove *move, int movecnt){
        if(cPiece::undo_move(move, movecnt) == false){
            return false;
        }
        int dstpiece_after_undo_mv = match->board.getfield(move->dst);
        if(move->prompiece && move->prompiece != PIECES["blk"]){
            int origin;
            if(color == COLORS["white"]){
                origin = PIECES["wPw"];
            }
            else{
                origin = PIECES["bPw"];
            }
            match->score += SCORES[move->prompiece] - SCORES[origin];
        }
        if(dstpiece_after_undo_mv == PIECES["blk"] && move->src % 8 != move->dst % 8){
            int enemy;
            if(color == COLORS["white"]){
                enemy = PIECES["bPw"];
            }
            else{
                enemy = PIECES["wPw"];
            }
            match->score -= SCORES[enemy];
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
            int dst = match->board.search(src, step, 5);
            while(dst != PIECES["blk"]){
                int piece = match->board.getfield(dst);
                if(piece == opp_pawn){
                    return false;
                }
                dst = match->board.search(dst, step, 5);
            }
        }
        return true;
    }

