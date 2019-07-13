
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
        {"blitz", 15}, 
        {"low", 30}, 
        {"medium", 60}, 
        {"high", 90}
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
            //newmatch.undo_move();
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

    bool cMatch::is_king_after_move_attacked(unsigned src, unsigned dst){
        unsigned srcpiece = board.getfield(src);
        unsigned dstpiece;
        unsigned pawnenmy = PIECES["blk"];
        bool flag;
        if(srcpiece == PIECES["wPw"]){
            cWhitePawn *cpawn = new cWhitePawn(&board, src);
            if(cpawn->is_ep_move_ok(dst, &minutes)){
                pawnenmy = board.getfield(dst);
                board.setfield(dst, PIECES["blk"]);
            }
        }
        if(srcpiece == PIECES["bPw"]){
            cBlackPawn *cpawn = new cBlackPawn(&board, src);
            if(cpawn->is_ep_move_ok(dst, &minutes)){
                pawnenmy = board.getfield(dst);
                board.setfield(dst, PIECES["blk"]);
            }
        }
        board.setfield(src, PIECES["blk"]);
        dstpiece = board.getfield(dst);
        board.setfield(dst, srcpiece);
        if(PIECES_COLOR[srcpiece] == COLORS["white"]){
            flag = is_field_touched(&board, board.wKg, COLORS["black"], EVAL_MODES["ignore-pins"]);
        }
        else{
            flag = is_field_touched(&board, board.bKg, COLORS["white"], EVAL_MODES["ignore-pins"]);
        }
        board.setfield(dst, dstpiece);
        board.setfield(src, srcpiece);
        if(pawnenmy != PIECES["blk"]){
            board.setfield(dst, pawnenmy);
        }
        return flag;
    }

    bool cMatch::is_move_valid(unsigned src, unsigned dst, unsigned prompiece){
        unsigned piece = board.getfield(src);
        cPiece *cpiece = obj_for_piece(&board, src);
        if(cpiece == NULL){
            return false; // RETURN_CODES["general-error"]
        }
        unsigned direction = cpiece->dir_for_move(src, dst);
        int step = cpiece->step_for_dir(direction);
        if(board.is_inbounds(src, dst, step) == false){
            return false; // RETURN_CODES["out-of-bounds"]
        }
        if(next_color() != PIECES_COLOR[piece]){
            return false; // RETURN_CODES["wrong-color"]
        }
        if(piece != PIECES["wKg"] && piece != PIECES["bKg"]){
            if(is_king_after_move_attacked(src, dst)){
                return false; // RETURN_CODES["king-attacked-error"]
            }
        }
        if(cpiece->is_move_valid(dst, prompiece)){
            return true; // RETURN_CODES["ok"]
        }
        else{
            return false; // RETURN_CODES["piece-error"]
        }
    }

    bool cMatch::is_move_available(){
        unsigned color = next_color();
        for(int idx = 0; idx < 64; ++idx){
            unsigned piece = board.getfield(idx);
            if(piece != PIECES["blk"] && color == PIECES_COLOR[piece]){
                cPiece *cpiece = obj_for_piece(&board, idx);
                for(vector<pair<int, unsigned>>::iterator it = cpiece->MV_STEPS.begin(); it != cpiece->MV_STEPS.end(); ++it){
                    int count = 0;
                    unsigned dst = cpiece->pos + it->first;
                    while(board.is_inbounds(cpiece->pos, dst, it->first) && count < cpiece->MAXCNT){
                        count += 1;
                        if(is_move_valid(cpiece->pos, dst, it->second)){
                            return true;
                        }
                        dst += it->first;
                    }
                }
            }
        }
        return false;
    }

    unsigned cMatch::evaluate_status(){
        if(is_move_available()){
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
 
    unsigned cMatch::eval_pin_dir(unsigned src){
        unsigned piece = board.getfield(src);
        unsigned color = PIECES_COLOR[piece];
        unsigned kg_pos;
        unsigned first, second, fstpiece, sndpiece;
        unsigned enmyfaces[2];
        unsigned dir_ary[2];
        int step;
        if(color == COLORS["white"]){
            kg_pos = board.wKg;
        }
        else{
            kg_pos = board.bKg;
        }        
        for(int j = 0; j < 2; ++j){
            if(j == 0){
                dir_ary[0] = cRook::DIRS_ARY[0];
                dir_ary[1] = cRook::DIRS_ARY[2];
                if(color == COLORS["white"]){
                    enmyfaces[0] = PIECES["bRk"];
                    enmyfaces[1] = PIECES["bQu"];
                }
                else{
                    enmyfaces[0] = PIECES["wBp"];
                    enmyfaces[1] = PIECES["wQu"];
                }
            }
            else{
                dir_ary[0] = cBishop::DIRS_ARY[0];
                dir_ary[1] = cBishop::DIRS_ARY[2];
                if(color == COLORS["white"]){
                    enmyfaces[0] = PIECES["bBp"];
                    enmyfaces[1] = PIECES["bQu"];
                }
                else{
                    enmyfaces[0] = PIECES["wBp"];
                    enmyfaces[1] = PIECES["wQu"];
                }
            }
            for(unsigned i = 0; i < size(dir_ary); ++i){
                if(j == 0){
                    step = cRook::step_for_dir(dir_ary[i]);
                }
                else{
                    step = cBishop::step_for_dir(dir_ary[i]);
                }
                if(board.search_bi_dirs(&first, &second, src, step, 6)){
                    fstpiece = board.getfield(first);
                    sndpiece = board.getfield(second);
                    if(PIECES_COLOR[fstpiece] != PIECES_COLOR[sndpiece]){
                        if(PIECES_COLOR[fstpiece] == color && kg_pos == first &&
                           (enmyfaces[0] == sndpiece || enmyfaces[1] == sndpiece)){
                            return dir_ary[i];
                        }
                        if(PIECES_COLOR[sndpiece] == color && kg_pos == second && 
                           (enmyfaces[0] == fstpiece || enmyfaces[1] == fstpiece)){
                            return REVERSE_DIRS[dir_ary[i]];
                        }
                    }
                }
            }
        }
        return DIRS["undef"];
    }

    unsigned cMatch::eval_soft_pin_dir(unsigned src){
        unsigned piece = board.getfield(src);
        unsigned color = PIECES_COLOR[piece];
        unsigned first, second, fstpiece, sndpiece;
        unsigned enmyfaces[2];
        unsigned dir_ary[2];
        int step;
        for(int j = 0; j < 2; ++j){
            if(j == 0){
                dir_ary[0] = cRook::DIRS_ARY[0];
                dir_ary[1] = cRook::DIRS_ARY[2];
                if(color == COLORS["white"]){
                    enmyfaces[0] = PIECES["bRk"];
                    enmyfaces[1] = PIECES["bQu"];
                }
                else{
                    enmyfaces[0] = PIECES["wBp"];
                    enmyfaces[1] = PIECES["wQu"];
                }
            }
            else{
                dir_ary[0] = cBishop::DIRS_ARY[0];
                dir_ary[1] = cBishop::DIRS_ARY[2];
                if(color == COLORS["white"]){
                    enmyfaces[0] = PIECES["bBp"];
                    enmyfaces[1] = PIECES["bQu"];
                }
                else{
                    enmyfaces[0] = PIECES["wBp"];
                    enmyfaces[1] = PIECES["wQu"];
                }
            }
            for(unsigned i = 0; i < size(dir_ary); ++i){
                if(j == 0){
                    step = cRook::step_for_dir(dir_ary[i]);
                }
                else{
                    step = cBishop::step_for_dir(dir_ary[i]);
                }
                if(board.search_bi_dirs(&first, &second, src, step, 6)){
                    fstpiece = board.getfield(first);
                    sndpiece = board.getfield(second);
                    if(PIECES_COLOR[fstpiece] != PIECES_COLOR[sndpiece]){
                        if(PIECES_COLOR[fstpiece] == color && 
                           PIECES_RANK[fstpiece] > PIECES_RANK[sndpiece] &&
                           PIECES_RANK[fstpiece] > PIECES_RANK[piece] &&
                           (enmyfaces[0] == sndpiece || enmyfaces[1] == sndpiece)){
                            return dir_ary[i];
                        }
                        if(PIECES_COLOR[sndpiece] == color && 
                           PIECES_RANK[sndpiece] > PIECES_RANK[fstpiece] &&
                           PIECES_RANK[sndpiece] > PIECES_RANK[piece] &&
                           (enmyfaces[0] == fstpiece || enmyfaces[1] == fstpiece)){
                            return REVERSE_DIRS[dir_ary[i]];
                        }
                    }
                }
            }
        }
        return DIRS["undef"];
    }
