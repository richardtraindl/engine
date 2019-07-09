#include <list> 
#include "./searchforpiece.hpp"
#include "../values.hpp"


    cTouch::cTouch(int _piece, int _pos){
        piece = _piece;
        pos = _pos;
    }

    cTouch::cTouch(){
    }


    int cSearchHelper::int RKSTEPS[4];
    int cSearchHelper::RKSTEPS[4] = {8, -8, 1, -1}; 
    int cSearchHelper::RKMAXCNT = 7;
    int cSearchHelper::RKTARGETS[4] = {PIECES["wRk"], PIECES["wQu"], PIECES["bRk"], PIECES["wQu"]};
    int cSearchHelper::BPSTEPS[4] = {9, -9, 7, -7};
    int cSearchHelper::BPMAXCNT = 7;
    int cSearchHelper::BPTARGETS[4] = {PIECES["wBp"], PIECES["wQu"], PIECES["bBp"], PIECES["wQu"]};
    int cSearchHelper::KGSTEPS[8] = {8, 9, 1, -7, -8, -9, -1, 7};
    int cSearchHelper::KGMAXCNT = 1;
    int cSearchHelper::KGTARGETS[2] = {PIECES["wKg"], PIECES["bKg"]};
    int stacSearchHelper::KNSTEPS[8] = {17, 10, -6, -15, -17, -10, 6, 15};
    int cSearchHelper::KNMAXCNT = 1;
    int cSearchHelper::KNTARGETS[2] = {PIECES["wKn"], PIECES["bKn"]};
    int cSearchHelper::WPWSTEPS[2] = {-7, -9};
    int cSearchHelper::WPWMAXCNT = 1;
    int cSearchHelper::WPWTARGETS[1] = {PIECES["wPw"]};
    int cSearchHelper::BPWSTEPS[2] = {9, 7};
    int cSearchHelper::BPWMAXCNT = 1;
    int cSearchHelper::BPWTARGETS[1] = {PIECES["bPw"]};

    bool cSearchHelper::_is_field_touched(cMatch *match, int src, int mode, int steps[], int maxcnt, int targets[]){
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

    bool _is_field_touched_by_king(cMatch *match, int src, int steps[], int maxcnt, int targets[]){
        for(const int step : steps){
            int dst = match->board.search(src, step, maxcnt);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                for(const int target : targets){
                    if(piece == target){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void _collect_frdly_and_enemy_touches(cMatch *match, int src, int frdlycolor, int steps[], int maxcnt, int targets[], list<cTouch> *frdlytouches, list<cTouch> *enmytouches){
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

    void _collect_touches(cMatch *match, int src, int steps[], int maxcnt, int targets[], list<cTouch> *touches){
        for(const int step : steps){
            int dst = match->board.search(src, step, maxcnt);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                /*cpiece = match.obj_for_piece(piece, dst);
                if(cpiece.is_move_stuck(src)){
                    continue;
                }*/
                for(const int target : targets){
                    if(piece == target){
                        touches->push_back(cTouch(piece, dst));
                    }
                }
            }
        }
    }

    cSearchHelper::bool is_field_touched(cMatch *match, int src, int color, int mode){
        if(color == COLORS["white"]){
            int rktargets[2] = {PIECES["wRk"], PIECES["wQu"]};
            int bptargets[2] = {PIECES["wBp"], PIECES["wQu"]};
            int kgtargets[1] = {PIECES["wKg"]};
            int kntargets[1] = {PIECES["wKn"]};
            // wpw
            if(_is_field_touched(match, src, color, mode, wpwsteps, wpwmaxcnt, wpwtargets)){
                return true;
            }
        }
        else{
            int rktargets[2] = {PIECES["bRk"], PIECES["wQu"]};
            int bptargets[2] = {PIECES["bBp"], PIECES["wQu"]};
            int kgtargets[1] = {PIECES["bKg"]};
            int kntargets[1] = {PIECES["bKn"]};
            // bpw
            if(_is_field_touched(match, src, color, mode, bpwsteps, bpwmaxcnt, bpwtargets)){
                return true;
            }
        }
        if(_is_field_touched(cMatch *match, int src, int mode, rksteps, rkmaxcnt, rktargets){)){
            return true;
        }
        if(_is_field_touched(match, src, color, mode, bpsteps, bpmaxcnt, bptargets)){
            return true;
        }
        if(_is_field_touched_by_king(match, src, kgsteps, kgmaxcnt, kgtargets)){
            return true;
        }
        if(_is_field_touched(match, src, mode, knsteps, knmaxcnt, kntargets)){
            return true;
        }
        return false;
    }

    void collect_frdly_and_enemy_touches(cMatch *match, int src, int frdlycolor, list<cTouch> *friends, list<cTouch> *enemies){
        _collect_frdly_and_enemy_touches(match, src, frdlycolor, rksteps[], rkmaxcnt, rktargets[], friends, enemies);
        _collect_frdly_and_enemy_touches(match, src, frdlycolor, bpsteps[], bpmaxcnt, bptargets[], friends, enemies);
        _collect_frdly_and_enemy_touches(match, src, frdlycolor, kgsteps[], kgmaxcnt, kgtargets[], friends, enemies);
        _collect_frdly_and_enemy_touches(match, src, frdlycolor, knsteps[], knmaxcnt, kntargets[], friends, enemies);
        _collect_frdly_and_enemy_touches(match, src, frdlycolor, wpwsteps[], wpwmaxcnt, wpwtargets[], friends, enemies);
        _collect_frdly_and_enemy_touches(match, src, frdlycolor, bpwsteps[], bpwmaxcnt, bpwtargets[], friends, enemies);
    }

    list<cTouch> collect_touches_for_color(cMatch *match, int src, int color){
        list<cTouch> touches;
        if(color == COLORS["white"]){
            int rktargets[2] = {PIECES["wRk"], PIECES["wQu"]};
            int bptargets[2] = {PIECES["wBp"], PIECES["wQu"]};
            int kgtargets[1] = {PIECES["wKg"]};
            int kntargets[1] = {PIECES["wKn"]};
            // wpw
            _collect_touches(match, src, wpwsteps[], wpwmaxcnt, wpwtargets[], touches);
        }
        else{
            int rktargets[2] = {PIECES["bRk"], PIECES["wQu"]};
            int bptargets[2] = {PIECES["bBp"], PIECES["wQu"]};
            int kgtargets[1] = {PIECES["bKg"]};
            int kntargets[1] = {PIECES["bKn"]};
            // bpw
            _collect_touches(match, src, bpwsteps[], bpwmaxcnt, bpwtargets[], touches);
        }
        _collect_touches(match, src, rksteps[], rkmaxcnt, rktargets[], touches);
        _collect_touches(match, src, bpsteps[], bpmaxcnt, bptargets[], touches);
        _collect_touches(match, src, kgsteps[], kgmaxcnt, kgtargets[], touches);
        _collect_touches(match, src, knsteps[], knmaxcnt, kntargets[], touches);
        return touches;
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
