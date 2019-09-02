
    #include "./searchforpiece.hpp"
    #include "./piece.hpp"
    #include "../values.hpp"


    int cSearchforRook::STEPS[4] = {8, -8, 1, -1};
    int cSearchforRook::MAXCNT = 7;
    int cSearchforRook::TARGETS[4] = {mWRK, mBRK, mWQU, mBQU};


    int cSearchforBishop::STEPS[4] = {9, -9, 7, -7};
    int cSearchforBishop::MAXCNT = 7;
    int cSearchforBishop::TARGETS[4] = {mWBP, mBBP, mWQU, mBQU};


    int cSearchforKing::STEPS[8] = {8, 9, 1, -7, -8, -9, -1, 7};
    int cSearchforKing::MAXCNT = 1;
    int cSearchforKing::TARGETS[2] = {mWKG, mBKG};    


    int cSearchforKnight::STEPS[8] = {17, 10, -6, -15, -17, -10, 6, 15};
    int cSearchforKnight::MAXCNT = 1;
    int cSearchforKnight::TARGETS[2] = {mWKN, mBKN};


    int cSearchforWhitePawn::STEPS[2] = {-7, -9};
    int cSearchforWhitePawn::MAXCNT = 1;
    int cSearchforWhitePawn::TARGETS[1] = {mWPW};


    int cSearchforBlackPawn::STEPS[2] = {9, 7};
    int cSearchforBlackPawn::MAXCNT = 1;
    int cSearchforBlackPawn::TARGETS[1] = {mBPW};    


    template<class cSearchforPiece>
    bool _is_field_touched(cBoard &board, int src, int color, int mode){
        for(auto &step : cSearchforPiece::STEPS){
            int dst = board.search(src, step, cSearchforPiece::MAXCNT);
            if(dst != cBoard::SNOK){
                int piece = board.getfield(dst);
                if(PIECES_COLORS[piece] != color){
                    continue;
                }
                for(const int target : cSearchforPiece::TARGETS){
                    if(piece == target){
                        if(mode == EVAL_MODES["ignore-pins"]){
                            return true;
                        }
                        if(mode == EVAL_MODES["only-pins-to-king"]){
                            cPiece cpiece(board, dst);
                            if(cpiece.is_move_stuck(src)){
                                break;
                            }
                            else{
                                return true;
                            }
                        }
                        else{ // mode == EVAL_MODES["all-pins"]
                            cPiece cpiece(board, dst);
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


    template<class cSearchforPiece>
    bool _is_field_touched_by_king(cBoard &board, int src, int color){
        for(auto &step : cSearchforPiece::STEPS){
            int dst = board.search(src, step, cSearchforPiece::MAXCNT);
            if(dst != cBoard::SNOK){
                int piece = board.getfield(dst);
                if(PIECES_COLORS[piece] != color){
                    continue;
                }
                for(auto &target : cSearchforPiece::TARGETS){
                    if(piece == target){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    
    template<class cSearchforPiece>
    void _collect_touches_for_both_colors(cBoard &board, int src, int friendlycolor, list<cTouch> &frdlytouches, list<cTouch> &enmytouches){
        for(auto &step : cSearchforPiece::STEPS){
            int dst = board.search(src, step, cSearchforPiece::MAXCNT);
            if(dst != cBoard::SNOK){
                int piece = board.getfield(dst);
                for(const int target : cSearchforPiece::TARGETS){
                    if(piece == target){
                        cPiece cpiece(board, dst);
                        if(PIECES_COLORS[piece] == friendlycolor){
                            frdlytouches.push_back(*(new cTouch(piece, dst)));
                        }
                        else{
                            enmytouches.push_back(*(new cTouch(piece, dst)));
                        }
                    }
                }
            }
        }
    }


    template<class cSearchforPiece>
    void _collect_touches_for_color(cBoard &board, int src, int color, list<cTouch> &touches){
        for(auto &step : cSearchforPiece::STEPS){
            int dst = board.search(src, step, cSearchforPiece::MAXCNT);
            if(dst != cBoard::SNOK){
                int piece = board.getfield(dst);
                if(PIECES_COLORS[piece] != color){
                    continue;
                }
                cPiece cpiece(board, dst);
                if(cpiece.is_move_stuck(src)){
                    continue;
                }
                for(const int target : cSearchforPiece::TARGETS){
                    if(piece == target){
                        touches.push_back(*(new cTouch(piece, dst)));
                    }
                }
            }
        }
    }


    bool is_field_touched(cBoard &board, int src, int color, int mode){
        if(_is_field_touched<cSearchforRook>(board, src, color, mode)){
            return true;
        }
        if(_is_field_touched<cSearchforBishop>(board, src, color, mode)){
            return true;
        }
        if(_is_field_touched_by_king<cSearchforKing>(board, src, color)){
            return true;
        }
        if(_is_field_touched<cSearchforKnight>(board, src, color, mode)){
            return true;
        }
        if(_is_field_touched<cSearchforWhitePawn>(board, src, color, mode)){
            return true;
        }
        if(_is_field_touched<cSearchforBlackPawn>(board, src, color, mode)){
            return true;
        }
        return false;
    }

    void collect_touches_for_both_colors(cBoard &board, int src, int color, list<cTouch> &friends, list<cTouch> &enmies){
        _collect_touches_for_both_colors<cSearchforRook>(board, src, color, friends, enmies);
        _collect_touches_for_both_colors<cSearchforBishop>(board, src, color, friends, enmies);
        _collect_touches_for_both_colors<cSearchforKnight>(board, src, color, friends, enmies);
        _collect_touches_for_both_colors<cSearchforKing>(board, src, color, friends, enmies);
        _collect_touches_for_both_colors<cSearchforWhitePawn>(board, src, color, friends, enmies);
        _collect_touches_for_both_colors<cSearchforBlackPawn>(board, src, color, friends, enmies);
    }

    void collect_touches_for_color(cBoard &board, int src, int color, list<cTouch> &touches){
        _collect_touches_for_color<cSearchforRook>(board, src, color, touches);
        _collect_touches_for_color<cSearchforBishop>(board, src, color, touches);
        _collect_touches_for_color<cSearchforKnight>(board, src, color, touches);
        _collect_touches_for_color<cSearchforKing>(board, src, color, touches);
        _collect_touches_for_color<cSearchforWhitePawn>(board, src, color, touches);
        _collect_touches_for_color<cSearchforBlackPawn>(board, src, color, touches);
    }

    void collect_long_distance_touches_for_color(cBoard &board, int src, int color, list<cTouch> &touches){
        _collect_touches_for_color<cSearchforRook>(board, src, color, touches);
        _collect_touches_for_color<cSearchforBishop>(board, src, color, touches);
    }

    void add_field_touches_beyond(cBoard &board, int color, cTouch &touch){
        collect_touches_for_both_colors(board, touch.pos, color, touch.supporter_beyond, touch.attacker_beyond);
    }
