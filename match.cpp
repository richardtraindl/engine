
#include <iostream>
#include "./match.hpp"


    cMatch::cMatch(){ 
        created_at = time(0); 
        status = STATUS["active"];
        score = 0;
        level = LEVELS["blitz"];
    } 
    cMatch::cMatch(const cMatch &obj){
    } // copy constructor

    map<string, unsigned> cMatch::STATUS = {
        {"active", 10},
        {"draw", 11}, 
        {"winner-white", 12}, 
        {"winner-black", 13}
    };

    map<string, unsigned> cMatch::LEVELS = {
        {"blitz", 0}, 
        {"low", 1}, 
        {"medium", 2}, 
        {"high", 3}
    };

    map<int, unsigned> cMatch::SECS_PER_MOVE = {
        {LEVELS["blitz"], 15}, 
        {LEVELS["low"], 30}, 
        {LEVELS["medium"], 60}, 
        {LEVELS["high"], 90}
    };

    map<string, unsigned> RETURN_CODES = {
        {"ok", 10}, 
        {"draw", 11}, 
        {"winner-white", 12}, 
        {"winner-black", 13}, 
        {"match-cancelled", 14}, 
        {"wrong-color", 15}, 
        {"piece-error", 16},
        {"king-attacked-error", 17}, 
        {"format-error", 18}, 
        {"out-of-bounds", 19},
        {"general-error", 20}
    };

    void cMatch::update_attributes(){
        for(list<cMove>::iterator it = minutes.begin(); it != minutes.end(); ++it){
            if(board.wKg_first_move_on == -1 && 
               it->src % 8 == cBoard::COLS["E"] && it->src / 8 == cBoard::RANKS["1"]){
                board.wKg_first_move_on = movecnt();
                continue;
            }
            if(board.bKg_first_move_on == -1 && 
               it->src % 8 == cBoard::COLS["E"] && it->src / 8 == cBoard::RANKS["8"]){
                board.bKg_first_move_on = movecnt();
                continue;
            }
            if(board.wRkA_first_move_on == -1 && 
               it->src % 8 == cBoard::COLS["A"] && it->src / 8 == cBoard::RANKS["1"]){
                board.wRkA_first_move_on = movecnt();
                continue;
            }
            if(board.wRkH_first_move_on == -1 && 
               it->src % 8 == cBoard::COLS["H"] && it->src / 8 == cBoard::RANKS["1"]){
                board.wRkH_first_move_on = movecnt();
                continue;
            }
            if(board.bRkA_first_move_on == -1 && 
               it->src % 8 == cBoard::COLS["A"] && it->src / 8 == cBoard::RANKS["8"]){
                board.bRkA_first_move_on = movecnt();
                continue;
            }
            if(board.bRkH_first_move_on == -1 && 
               it->src % 8 == cBoard::COLS["H"] && it->src / 8 == cBoard::RANKS["8"]){
                board.bRkH_first_move_on = movecnt();
                continue;
            }
        }
        score = 0;
        for(int idx = 0; idx < 64; ++idx){
            unsigned piece = board.getfield(idx);
            score -= SCORES[(int)piece];
            if(piece == PIECES["wKg"]){
                board.wKg = idx;
                continue;
            }
            if(piece == PIECES["bKg"]){
                board.bKg = idx;
            }
        }
    }

    int cMatch::movecnt(){
        return minutes.size();
    }

    unsigned cMatch::next_color(){
        if(minutes.size() % 2 == 0){
            return COLORS["white"];
        }
        else{
            return COLORS["black"];
        }
    }

    bool cMatch::is_opening(){
        return minutes.size() <= 20;
    }

    bool cMatch::is_endgame(){
        return minutes.size() >= 60;
    }

    // 100 Züge davor kein Bauernzug und keine Figur geschlagen
    bool cMatch::is_fifty_moves_rule(){
        int cnt = 0;
        int maxlen = minutes.size();
        for(list<cMove>::reverse_iterator it = minutes.rbegin(); it != minutes.rend(); ++it){
            unsigned srcpiece = it->getprevfield(it->src);
            unsigned dstpiece = it->getprevfield(it->dst);
            if(srcpiece == PIECES["wPw"] || srcpiece == PIECES["bPw"] || dstpiece != PIECES["blk"]){
                cnt = 0;
            }
            else{
                cnt += 1;
                if(cnt > 100){
                    return true;
                }
                if(maxlen - cnt < 100){
                    return false;
                }
            }
        }
        return false;
    }

    bool cMatch::is_move_repetition(){
        cMatch newmatch = *this;
        int count = 0;
        int maxcnt = min((int)newmatch.minutes.size(), 8);
        for(int i = 0; i < maxcnt; ++i){
            newmatch.undo_move();
            if(newmatch.board.comparefields(board.fields)){
                count += 2;
            }
        }
        return count >= 2;
    }

    cMove *cMatch::do_move(unsigned src, unsigned dst, unsigned prompiece){
        cPiece *cpiece = obj_for_piece(&board, src);
        if(cpiece != NULL){
            cMove *move = cpiece->do_move(dst, prompiece, movecnt() + 1, &score);
            minutes.push_back(*move);
            return move;
        }
        else{
            return NULL;
        }
    }

    bool cMatch::undo_move(){
        cMove move;
        if(minutes.size() > 0){
            move = minutes.back();
        }
        else{
            return false;
        }
        unsigned piece = board.getfield(move.dst);
        if(move.prompiece != PIECES["blk"]){
            bool flag;
            if(PIECES_COLOR[piece] == COLORS["white"]){
                cWhitePawn *cpawn = new cWhitePawn(&board, move.dst);
                flag = cpawn->undo_move(&move, movecnt(), &score);
            }
            else{
                cBlackPawn *cpawn = new cBlackPawn(&board, move.dst);
                flag = cpawn->undo_move(&move, movecnt(), &score);
            }
            minutes.pop_back();
            return flag;
        }
        else{
            cPiece *cpiece = obj_for_piece(&board, move.dst);
            if(cpiece != NULL){
                bool flag = cpiece->undo_move(&move, movecnt(), &score);
                minutes.pop_back();
                return flag;
            }
            else{
                minutes.pop_back();
                return false;
            }
        }
    }

    unsigned cMatch::eval_status(){
        if(board.is_move_available(&minutes)){
            return STATUS["active"];
        }
        else{
            if(next_color() == COLORS["white"]){
                if(is_field_touched(&board, board.wKg, COLORS["black"], EVAL_MODES["ignore-pins"])){
                    return STATUS["winner_black"];
                }
            }
            else{
                if(is_field_touched(&board, board.bKg, COLORS["white"], EVAL_MODES["ignore-pins"])){
                    return STATUS["winner_white"];
                }
            }
        }
        return STATUS["draw"];
    }
 
