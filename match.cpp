
    #include <boost/multiprecision/cpp_int.hpp>
    #include "./match.hpp"
    #include "./pieces/piece.hpp"
    #include "./pieces/searchforpiece.hpp"
    #include "./helper.hpp"
    #include "./values.hpp"


    using namespace boost::multiprecision;

    cMatch::cMatch(){ 
        created_at = time(0); 
        status = STATUS["active"];
        score = 0;
        level = LEVELS["blitz"];
    } 
    cMatch::cMatch(const cMatch &obj){
    } // copy constructor

    map<string, int> cMatch::STATUS = {
        {"active", 10},
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

    int cMatch::movecnt(){
        return minutes.size();
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
        return minutes.size() >= 60;
    }

    // 100 Züge davor kein Bauernzug und keine Figur geschlagen
    bool cMatch::is_fifty_moves_rule(){
        int cnt = 0;
        int maxlen = minutes.size();
        for(list<cMove>::reverse_iterator it = minutes.rbegin(); it != minutes.rend(); ++it){
            int srcpiece = it->getprevfield(it->src);
            int dstpiece = it->getprevfield(it->dst);
            if(srcpiece == mWPW || srcpiece == mBPW || dstpiece != mBLK){
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
            if(newmatch.board.fields == board.fields){
                count += 2;
            }
        }
        return count >= 2;
    }

    cMove *cMatch::do_move(int src, int dst, int prompiece){
        cPiece cpiece(board, src);
        cMove *move = cpiece.do_move(dst, prompiece, movecnt() + 1, score);
        minutes.push_back(*move);
        return move;
    }

    void cMatch::undo_move(){
        cMove move;
        if(minutes.size() > 0){
            move = minutes.back();
        }
        else{
            cout << "return " << endl;
            return;
        }
        cPiece cpiece(board, move.dst);
        cpiece.undo_move(move, movecnt(), score);
        minutes.pop_back();
        // delete move;
    }

    int cMatch::eval_status(){
        if(board.is_move_available(minutes)){
            return STATUS["active"];
        }
        else{
            if(next_color() == COLORS["white"]){
                if(is_field_touched(board, board.wKg, COLORS["black"], EVAL_MODES["ignore-pins"])){
                    return STATUS["winner_black"];
                }
            }
            else{
                if(is_field_touched(board, board.bKg, COLORS["white"], EVAL_MODES["ignore-pins"])){
                    return STATUS["winner_white"];
                }
            }
        }
        return STATUS["draw"];
    }


    void cMatch::prnt_minutes(){
        for(list<cMove>::iterator it = minutes.begin(); it != minutes.end(); ++it){
            cout << it->format() << endl;
        }
    }
