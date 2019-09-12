
    #include <iostream>
    #include "./match.hpp"
    #include "./pieces/piece.hpp"
    #include "./pieces/searchforpiece.hpp"
    #include "./helper.hpp"
    #include "./values.hpp"


    cMatch::cMatch(){ 
        created_at = time(0); 
        score = 0;
        level = LEVELS["blitz"];
        cBoard board;
    } 
    cMatch::cMatch(const cMatch &match){
        created_at = time(0);
        score = match.score;
        level = match.level;

        //board.fields[0] = match.board.fields[0];
        //board.fields[1] = match.board.fields[1];
        //board.fields[2] = match.board.fields[2];
        //board.fields[3] = match.board.fields[3];
        board.wKg = match.board.wKg;
        board.bKg = match.board.bKg;
        board.wKg_first_move_on = match.board.wKg_first_move_on;
        board.bKg_first_move_on = match.board.bKg_first_move_on;
        board.wRkA_first_move_on = match.board.wRkA_first_move_on;
        board.wRkH_first_move_on = match.board.wRkH_first_move_on;
        board.bRkA_first_move_on = match.board.bRkA_first_move_on;
        board.bRkH_first_move_on = match.board.bRkH_first_move_on;

        for(cMove move : match.minutes){
            minutes.push_back(move); //(*new cMove(move.prevfields, move.src, move.dst, move.prompiece)));
        }
    } // copy constructor

    cMatch::~cMatch(){
    }

    map<string, int> cMatch::STATUS = {
        {"open", 10},
        {"draw", 11}, 
        {"winner-white", 12}, 
        {"winner-black", 13}
    };

    map<string, int> cMatch::LEVELS = {
        {"blitz", 0}, 
        {"low", 1}, 
        {"medium", 2}, 
        {"high", 3}
    };

    map<int, int> cMatch::SECS_PER_MOVE = {
        {LEVELS["blitz"], 15}, 
        {LEVELS["low"], 30}, 
        {LEVELS["medium"], 60}, 
        {LEVELS["high"], 90}
    };

    map<string, int> RETURN_CODES = {
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
        for(cMove move : minutes){
            if(board.wKg_first_move_on == -1 && 
               move.src % 8 == cBoard::COLS["E"] && move.src / 8 == cBoard::RANKS["1"]){
                board.wKg_first_move_on = minutes.size();
                continue;
            }
            if(board.bKg_first_move_on == -1 && 
               move.src % 8 == cBoard::COLS["E"] && move.src / 8 == cBoard::RANKS["8"]){
                board.bKg_first_move_on = minutes.size();
                continue;
            }
            if(board.wRkA_first_move_on == -1 && 
               move.src % 8 == cBoard::COLS["A"] && move.src / 8 == cBoard::RANKS["1"]){
                board.wRkA_first_move_on = minutes.size();
                continue;
            }
            if(board.wRkH_first_move_on == -1 && 
               move.src % 8 == cBoard::COLS["H"] && move.src / 8 == cBoard::RANKS["1"]){
                board.wRkH_first_move_on = minutes.size();
                continue;
            }
            if(board.bRkA_first_move_on == -1 && 
               move.src % 8 == cBoard::COLS["A"] && move.src / 8 == cBoard::RANKS["8"]){
                board.bRkA_first_move_on = minutes.size();
                continue;
            }
            if(board.bRkH_first_move_on == -1 && 
               move.src % 8 == cBoard::COLS["H"] && move.src / 8 == cBoard::RANKS["8"]){
                board.bRkH_first_move_on = minutes.size();
                continue;
            }
        }
        score = 0;
        for(int idx = 0; idx < 64; ++idx){
            int piece = board.getfield(idx);
            score -= SCORES[(int)piece];
            if(piece == mWKG){
                board.wKg = idx;
                continue;
            }
            if(piece == mBKG){
                board.bKg = idx;
            }
        }
    }

    int cMatch::next_color(){
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
        int wofficers, bofficers;
        board.eval_count_of_officers(wofficers, bofficers);
        return wofficers < 3 && bofficers < 3;
    }

    // 100 Züge davor kein Bauernzug und keine Figur geschlagen
    bool cMatch::is_fifty_moves_rule(){
        int rulecnt = 0;
        int cnt = 0;
        cMatch newmatch = *this;
        for(list<cMove>::reverse_iterator it = minutes.rbegin(); it != minutes.rend(); ++it){
            cnt++;
            if(cnt > 101){
                return false;
            }
            if(it->prev_dstpiece != mBLK){
                rulecnt = 0;
                continue;
            }
            newmatch.undo_move();
            if(newmatch.board.getfield(it->src) == mWPW ||
               newmatch.board.getfield(it->src) == mBPW){
                rulecnt = 0;
                continue;
            }
            rulecnt++;
            if(rulecnt > 100){
                return true;
            }
        }
        return false;
    }

    bool cMatch::is_three_times_rep(){
        return false;
        /*int count = 0;
        int equalcnt = 1;
        for(list<cMove>::reverse_iterator it = minutes.rbegin(); it != minutes.rend(); ++it){
            if(*(it->prevfields) == board.fields[0] &&
               *(it->prevfields + 1) == board.fields[1] &&
               *(it->prevfields + 2) == board.fields[2] &&
               *(it->prevfields + 3) == board.fields[3]){
                equalcnt++;
            }
            count++;
            if(count > 8){
                break;
            }
        }
        return equalcnt >= 3;*/
    }

    cMove *cMatch::do_move(int src, int dst, int prompiece){
        cPiece cpiece(board, src);
        cMove *move = cpiece.do_move(dst, board.getfield(dst), prompiece, minutes.size() + 1, score);
        minutes.push_back(*move);
        return move;
    }

    bool cMatch::undo_move(){
        cMove move;
        if(minutes.size() > 0){
            move = minutes.back();
        }
        else{
            cout << "return " << endl;
            return false;
        }
        cPiece cpiece(board, move.dst);
        cpiece.undo_move(move, minutes.size(), score);
        minutes.pop_back();
        return true;
        // delete move;
    }

    int cMatch::eval_status(){
        if(board.is_move_available(minutes)){
            return STATUS["open"];
        }
        else{
            if(next_color() == COLORS["black"]){
                if(is_field_touched(board, board.bKg, COLORS["white"], EVAL_MODES["ignore-pins"])){
                    return STATUS["winner-white"];
                }
            }
            else{
                if(is_field_touched(board, board.wKg, COLORS["black"], EVAL_MODES["ignore-pins"])){
                    return STATUS["winner-black"];
                }
            }
        }
        return STATUS["draw"];
    }


    void cMatch::prnt_minutes(){
        for(cMove move : minutes){
            cout << move.format() << endl;
        }
    }
