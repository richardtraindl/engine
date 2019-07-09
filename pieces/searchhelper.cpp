#include <list> 
#include "./searchforpiece.hpp"
#include "../values.hpp"


    cTouch::cTouch(int _piece, int _pos){
        piece = _piece;
        pos = _pos;
    }

    cTouch::cTouch(){
    }

    bool _is_field_touched(cMatch *match, int src, int mode, int steps[], int maxcnt, int targets[]){
        for(const int step : steps){
            int dst = match->board.search(src, step, maxcnt);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                for(const int target : targets){
                    if(piece == target){
                        if(mode == cMatch::EVAL_MODES["ignore-pins"]){
                            return true;
                        }
                        if(mode == cMatch::EVAL_MODES["only-pins-to-king"]){
                            return true;
                            /*cpiece = match.obj_for_piece(piece, dst);
                            if(cpiece.is_move_stuck(src)){
                                break;
                            }
                            else{
                                return true;
                            }*/
                        }
                        else{ // mode == EVAL_MODES["all-pins"]
                            return true;
                            /*cpiece = match.obj_for_piece(piece, dst);
                            if(cpiece.is_move_stuck(src) or match.is_soft_pin(src)[0]){
                                break;
                            }
                            else{
                                return true;
                            }*/
                        }
                    }
                }
            }
        }
        return false;
    }

    bool _is_field_touched_by_king(cMatch *match, int src, int color, int steps[], int maxcnt, int targets[]){
        for(const int step : steps){
            int dst = match->board.search(src, step, maxcnt);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                if(match->color_of_piece(piece) != color){
                    continue;
                }
                for(const int target : targets){
                    if(piece == target){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void _search_for_frdly_and_enemy_pieces(cMatch *match, int src, int frdlycolor, int steps[], int maxcnt, int targets[], list<cTouch> *frdlytouches, list<cTouch> *enmytouches){
        for(const int step : steps){
            int dst = match->board.search(src, step, maxcnt);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                for(const int target : targets){
                    if(piece == target){
                        /*cpiece = match.obj_for_piece(piece, dst);
                        if(cpiece.is_move_stuck(src)){
                            break;
                        }*/
                        if(match->color_of_piece(piece) == frdlycolor){
                            cTouch touch; //  = cTouch(piece, dst);
                            frdlytouches->push_back(touch);
                        }
                        else{
                            cTouch touch; // = cTouch(piece, dst);
                            enmytouches->push_back(touch);
                        }
                    }
                }
            }
        }
    }

    void _search_for_pieces(cMatch *match, int src, int steps[], int maxcnt, int targets[], list<cTouch> *touches){
        for(const int step : STEPS){
            int dst = match->board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                /*cpiece = match.obj_for_piece(piece, dst);
                if(cpiece.is_move_stuck(src)){
                    continue;
                }*/
                for(const int target : TARGETS){
                    if(piece == target){
                        touches->push_back(cTouch(piece, dst));
                    }
                }
            }
        }
    }

    cSearchHelper::bool is_field_touched(cMatch *match, int src, int color, int mode){
        int rksteps[4] = {8, -8, 1, -1};
        int bpsteps[4] = {9, -9, 7, -7};
        int kgsteps[8] = {8, 9, 1, -7, -8, -9, -1, 7};
        int knsteps[8] = {17, 10, -6, -15, -17, -10, 6, 15};
        
        int rkmaxcnt = 7;
        int bpmaxcnt = 7;
        int kgmaxcnt = 1;
        int knmaxcnt = 1;
        int pwmaxcnt = 1;

        if(color == COLORS["white"]){
            int pwsteps[2] = {-7, -9};
            int rktargets[2] = {PIECES["wRk"], PIECES["wQu"]};
            int bptargets[2] = {PIECES["wBp"], PIECES["wQu"]};
            int kgtargets[1] = {PIECES["wKg"]};
            int kntargets[1] = {PIECES["wKn"]};
            int pwtargets[1] = {PIECES["wPw"]};
            // wpw
            if(_is_field_touched(match, src, color, mode, pwsteps, pwmaxcnt, pwtargets)){
                return true;
            }
        }
        else{
            int pwsteps[2] = {9, 7};
            int rktargets[2] = {PIECES["bRk"], PIECES["wQu"]};
            int bptargets[2] = {PIECES["bBp"], PIECES["wQu"]};
            int kgtargets[1] = {PIECES["bKg"]};
            int kntargets[1] = {PIECES["bKn"]};
            int pwtargets[1] =  {PIECES["bPw"]};
            // bpw
            if(_is_field_touched(match, src, color, mode, pwsteps, pwmaxcnt, pwtargets)){
                return true;
            }
        }
        if(_is_field_touched(cMatch *match, int src, int mode, rksteps, rkmaxcnt, rktargets){)){
            return true;
        }
        if(_is_field_touched(match, src, color, mode, bpsteps, bpmaxcnt, bptargets)){
            return true;
        }
        if(_is_field_touched_by_king(match, src, color, kgsteps, kgmaxcnt, kgtargets)){
            return true;
        }
        if(_is_field_touched(match, src, mode, knsteps, knmaxcnt, kntargets)){
            return true;
        }
        return false;
    }
    

    void field_touches_for_both(cMatch *match, int src, int color, list<cTouch> *friends, list<cTouch> *enmies){
        // frdlytouches = []
        // enmytouches = []
                    int rktargets[2] = {PIECES["wRk"], PIECES["bRk"], PIECES["wQu"], PIECES["bQu"]};
            int bptargets[2] = {PIECES["wBp"], PIECES["bBp"], PIECES["wQu"], PIECES["bQu"]};
            int kgtargets[1] = {PIECES["wKg"], PIECES["bKg"]};
            int kntargets[1] = {PIECES["wKn"], PIECES["bKn"]};
            int wpwtargets[1] = {PIECES["wPw"]};
            int bpwtargets[1] =  {PIECES["bPw"]};
        SearchforRook::field_touches_for_both(match, src, color, friends, enmies);
        SearchforBishop::field_touches_for_both(match, src, color, friends, enmies);
        SearchforKnight::field_touches_for_both(match, src, color, friends, enmies);
        SearchforKing::field_touches_for_both(match, src, color, friends, enmies);
        SearchforWhitePawn::field_touches_for_both(match, src, color, friends, enmies);
        SearchforBlackPawn::field_touches_for_both(match, src, color, friends, enmies);
    }

    void add_field_touches_beyond(cMatch *match, int color, cTouch *ctouch){
        list<cTouch> supporter_beyond = ctouch->supporter_beyond;
        list<cTouch> attacker_beyond = ctouch->attacker_beyond;
        SearchforRook::field_touches_for_both(match, ctouch->pos, color, &supporter_beyond, &attacker_beyond);
        SearchforBishop::field_touches_for_both(match, ctouch->pos, color, &supporter_beyond, &attacker_beyond);
        SearchforKnight::field_touches_for_both(match, ctouch->pos, color, &supporter_beyond, &attacker_beyond);
        SearchforKing::field_touches_for_both(match, ctouch->pos, color, &supporter_beyond, &attacker_beyond);
        SearchforWhitePawn::field_touches_for_both(match, ctouch->pos, color, &supporter_beyond, &attacker_beyond);
        SearchforBlackPawn::field_touches_for_both(match, ctouch->pos, color, &supporter_beyond, &attacker_beyond);
    }

    list<cTouch> field_touches_for_color(cMatch *match, int src, int color){
        list<cTouch> touches;
        SearchforRook::field_touches_for_color(match, src, color, &touches);
        SearchforBishop::field_touches_for_color(match, src, color, &touches);
        SearchforKnight::field_touches_for_color(match, src, color, &touches);
        SearchforKing::field_touches_for_color(match, src, color, &touches);
        SearchforWhitePawn::field_touches_for_color(match, src, color, &touches);
        SearchforBlackPawn::field_touches_for_color(match, src, color, &touches);
        return touches;
    }
