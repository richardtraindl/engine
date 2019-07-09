#include <list> 
#include "./searchforpiece.hpp"
#include "../values.hpp"


    cTouch::cTouch(int _piece, int _pos){
        piece = _piece;
        pos = _pos;
    }

    cTouch::cTouch(){
    }

    cSearchForPiece::cSearchForPiece(){
    }

    int cSearchForPiece::STEPS[1] = {0};
    int cSearchForPiece::MAXCNT = 1;
    int cSearchForPiece::TARGETS[1] = {-1};

    bool cSearchForPiece::is_field_touched(cMatch *match, int src, int color, int mode){
        for(const int step : STEPS){
            int dst = match->board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                if(match->color_of_piece(piece) != color){
                    continue;
                }
                for(const int target : TARGETS){
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

    void cSearchForPiece::field_touches_for_both(cMatch *match, int src, int color, list<cTouch> *frdlytouches, list<cTouch> *enmytouches){
        for(const int step : STEPS){
            int dst = match->board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                for(const int target : TARGETS){
                    if(piece == target){
                        /*cpiece = match.obj_for_piece(piece, dst);
                        if(cpiece.is_move_stuck(src)){
                            break;
                        }*/
                        if(match->color_of_piece(piece) == color){
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

    void cSearchForPiece::field_touches_for_color(cMatch *match, int src, int color, list<cTouch> *touches){
        for(const int step : STEPS){
            int dst = match->board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                if(match->color_of_piece(piece) != color){
                    continue;
                }
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


    int SearchforRook::STEPS[4] = {8, -8, 1, -1};
    int SearchforRook::MAXCNT = 7;
    int SearchforRook::TARGETS[4] = {PIECES["wRk"], PIECES["bRk"], PIECES["wQu"], PIECES["bQu"]};


    int SearchforBishop::STEPS[4] = {9, -9, 7, -7};
    int SearchforBishop::MAXCNT = 7;
    int SearchforBishop::TARGETS[4] = {PIECES["wBp"], PIECES["bBp"], PIECES["wQu"], PIECES["bQu"]};


    int SearchforKing::STEPS[8] = {8, 9, 1, -7, -8, -9, -1, 7};
    int SearchforKing::MAXCNT = 1;
    int SearchforKing::TARGETS[2] = {PIECES["wKg"], PIECES["bKg"]};

    bool SearchforKing::is_field_touched(cMatch *match, int src, int color){
        for(const int step : STEPS){
            int dst = match->board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = match->board.getfield(dst);
                if(match->color_of_piece(piece) != color){
                    continue;
                }
                for(const int target : TARGETS){
                    if(piece == target){
                        return true;
                    }
                }
            }
        }
        return false;
    }


    int SearchforKnight::STEPS[8] = {17, 10, -6, -15, -17, -10, 6, 15};
    int SearchforKnight::MAXCNT = 1;
    int SearchforKnight::TARGETS[2] = {PIECES["wKn"], PIECES["bKn"]};


    int SearchforWhitePawn::STEPS[2] = {-7, -9};
    int SearchforWhitePawn::MAXCNT = 1;
    int SearchforWhitePawn::TARGETS[1] = {PIECES["wPw"]};


    int SearchforBlackPawn::STEPS[2] = {9, 7};
    int SearchforBlackPawn::MAXCNT = 1;
    int SearchforBlackPawn::TARGETS[1] = {PIECES["bPw"]};


    bool is_field_touched(cMatch *match, int src, int color, int mode){
        if(SearchforRook::is_field_touched(match, src, color, mode)){
            return true;
        }
        if(SearchforBishop::is_field_touched(match, src, color, mode)){
            return true;
        }
        if(SearchforKing::is_field_touched(match, src, color)){
            return true;
        }
        if(SearchforKnight::is_field_touched(match, src, color, mode)){
            return true;
        }
        if(SearchforWhitePawn::is_field_touched(match, src, color, mode)){
            return true;
        }
        if(SearchforBlackPawn::is_field_touched(match, src, color, mode)){
            return true;
        }
        return false;
    }

    void field_touches_for_both(cMatch *match, int src, int color, list<cTouch> *friends, list<cTouch> *enmies){
        // frdlytouches = []
        // enmytouches = []
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
