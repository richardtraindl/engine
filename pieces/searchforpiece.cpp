
    #include "./searchforpiece.hpp"
    #include "./piece.hpp"
    #include "../values.hpp"

    int cSearchforRook::STEPS[4] = {8, -8, 1, -1};
    int cSearchforRook::MAXCNT = 7;
    int cSearchforRook::TARGETS[4] = {mWRK, mBRK, mWQU, mBQU};

    bool cSearchforRook::_is_field_touched(cBoard &board, int src, int color, int mode){
        for(const int step : STEPS){
            int dst = board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = board.getfield(dst);
                if(PIECES_COLORS[piece] != color){
                    continue;
                }
                for(const int target : TARGETS){
                    if(piece == target){
                        if(mode == EVAL_MODES["ignore-pins"]){
                            return true;
                        }
                        if(mode == EVAL_MODES["only-pins-to-king"]){
                            cPiece cpiece(&board, dst);
                            if(cpiece.is_move_stuck(src)){
                                break;
                            }
                            else{
                                return true;
                            }
                        }
                        else{ // mode == EVAL_MODES["all-pins"]
                            cPiece cpiece(&board, dst);
                            if(cpiece.is_move_stuck(src) || board.eval_soft_pin_dir(src) != DIRS["undef"]){
                                break;
                            }
                            else{
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    void cSearchforRook::_collect_touches_for_both_colors(cBoard &board, int src, int friendlycolor, list<cTouch> &frdlytouches, list<cTouch> &enmytouches){
        for(const int step : STEPS){
            int dst = board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = board.getfield(dst);
                for(const int target : TARGETS){
                    if(piece == target){
                        cPiece cpiece(&board, dst);
                        if(cpiece.is_move_stuck(src)){
                             break;
                        }
                        if(PIECES_COLORS[piece] == friendlycolor){
                            cTouch touch(piece, dst);
                            frdlytouches.push_back(touch);
                        }
                        else{
                            cTouch touch(piece, dst);
                            enmytouches.push_back(touch);
                        }
                    }
                }
            }
        }
    }

    void cSearchforRook::_collect_touches_for_color(cBoard &board, int src, int color, list<cTouch> &touches){
        for(const int step : STEPS){
            int dst = board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = board.getfield(dst);
                if(PIECES_COLORS[piece] != color){
                    continue;
                }
                cPiece cpiece(&board, dst);
                if(cpiece.is_move_stuck(src)){
                    continue;
                }
                for(const int target : TARGETS){
                    if(piece == target){
                        touches.push_back(cTouch(piece, dst));
                    }
                }
            }
        }
    }


    int cSearchforBishop::STEPS[4] = {9, -9, 7, -7};
    int cSearchforBishop::MAXCNT = 7;
    int cSearchforBishop::TARGETS[4] = {mWBP, mBBP, mWQU, mBQU};


    int cSearchforKing::STEPS[8] = {8, 9, 1, -7, -8, -9, -1, 7};
    int cSearchforKing::MAXCNT = 7;
    int cSearchforKing::TARGETS[2] = {mWKG, mBKG};
    bool cSearchforKing::_is_field_touched(cBoard &board, int src, int color){
        for(const int step : STEPS){
            int dst = board.search(src, step, MAXCNT);
            if(dst != -1){
                int piece = board.getfield(dst);
                if(PIECES_COLORS[piece] != color){
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


    int cSearchforKnight::STEPS[8] = {17, 10, -6, -15, -17, -10, 6, 15};
    int cSearchforKnight::MAXCNT = 1;
    int cSearchforKnight::TARGETS[2] = {mWKN, mBKN};


    int cSearchforWhitePawn::STEPS[2] = {-7, -9};
    int cSearchforWhitePawn::MAXCNT = 1;
    int cSearchforWhitePawn::TARGETS[1] = {mWPW};


    int cSearchforBlackPawn::STEPS[2] = {9, 7};
    int cSearchforBlackPawn::MAXCNT = 1;
    int cSearchforBlackPawn::TARGETS[1] = {mBPW};


    bool is_field_touched(cBoard &board, int src, int color, int mode){
        if(cSearchforRook::_is_field_touched(board, src, color, mode)){
            return true;
        }
        if(cSearchforBishop::_is_field_touched(board, src, color, mode)){
            return true;
        }
        if(cSearchforKing::_is_field_touched(board, src, color)){
            return true;
        }
        if(cSearchforKnight::_is_field_touched(board, src, color, mode)){
            return true;
        }
        if(cSearchforWhitePawn::_is_field_touched(board, src, color, mode)){
            return true;
        }
        if(cSearchforBlackPawn::_is_field_touched(board, src, color, mode)){
            return true;
        }
        return false;
    }

    void collect_touches_for_both_colors(cBoard &board, int src, int color, list<cTouch> &friends, list<cTouch> &enmies){
        cSearchforRook::_collect_touches_for_both_colors(board, src, color, friends, enmies);
        cSearchforBishop::_collect_touches_for_both_colors(board, src, color, friends, enmies);
        cSearchforKnight::_collect_touches_for_both_colors(board, src, color, friends, enmies);
        cSearchforKing::_collect_touches_for_both_colors(board, src, color, friends, enmies);
        cSearchforWhitePawn::_collect_touches_for_both_colors(board, src, color, friends, enmies);
        cSearchforBlackPawn::_collect_touches_for_both_colors(board, src, color, friends, enmies);
    }

    void collect_touches_for_color(cBoard &board, int src, int color, list<cTouch> &touches){
        cSearchforRook::_collect_touches_for_color(board, src, color, touches);
        cSearchforBishop::_collect_touches_for_color(board, src, color, touches);
        cSearchforKnight::_collect_touches_for_color(board, src, color, touches);
        cSearchforKing::_collect_touches_for_color(board, src, color, touches);
        cSearchforWhitePawn::_collect_touches_for_color(board, src, color, touches);
        cSearchforBlackPawn::_collect_touches_for_color(board, src, color, touches);
    }

    void add_field_touches_beyond(cBoard &board, int color, cTouch &touch){
        collect_touches_for_both_colors(board, touch.pos, color, touch.supporter_beyond, touch.attacker_beyond);
    }
