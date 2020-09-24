
    #include "./match.hpp"


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
        for(int idx = 0; idx < 8; ++idx){
            board.field[idx] = match.board.field[idx];
        }

        for(cMove move : match.minutes){
            minutes.push_back(move); 
            //(*new cMove(move.prevfields, move.src, move.dst, move.prompiece)));
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


    int cMatch::next_color(){
        if(minutes.size() % 2 == 0){
            return mWHITE;
        }
        else{
            return mBLACK;
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
        if(minutes.size() < 100){
            return false;
        }
        int rulecnt = 0;
        int cnt = 0;
        cMatch newmatch = *this;
        for(list<cMove>::reverse_iterator it = minutes.rbegin(); it != minutes.rend(); ++it){
            cnt++;
            if(cnt > 101){
                return false;
            }

            newmatch.undo_move();

            if(newmatch.board.read(it->dst) != mBLK){ 
                rulecnt = 0;
                continue;
            }
            if(newmatch.board.read(it->src) == mWPW ||
               newmatch.board.read(it->src) == mBPW){
                rulecnt = 0;
                continue;
            }
            rulecnt++;
            if(rulecnt >= 100){
                return true;
            }
        }
        return false;
    }


    bool cMatch::is_three_times_rep(){
        int equalcnt = 1;
        cMatch newmatch = *this;
        for(int cnt = 0; (cnt < (int)newmatch.minutes.size() && cnt <= 8); ++cnt){
            newmatch.undo_move();
            if(board.compare(newmatch.board)){
                equalcnt++;
            }
        }
        return equalcnt >= 3;
    }


    void cMatch::do_move(uint64_t src, uint64_t dst, uint8_t prompiece){
        uint8_t srcpiece = board.read(src);
        uint8_t dstpiece = board.read(dst);

        if((srcpiece == mWKG || srcpiece == mBKG) &&
           ((src >> 2) == dst || (src << 2) == dst)){
            if((src >> 2) == dst){
                cMove move(MOVE_TYPE["short-castling"], src, dst, prompiece, board.field);
                minutes.push_back(move);
                uint64_t rkpos = (dst >> 1);
                uint8_t rkpiece = board.read(rkpos);
                board.write(src, mBLK);
                board.write(dst, srcpiece);
                board.write(rkpos, mBLK);
                board.write((dst << 1), rkpiece);
                return;
            }
            else{
                cMove move(MOVE_TYPE["long-castling"], src, dst, prompiece, board.field);
                minutes.push_back(move);
                uint64_t rkpos = (dst << 2);
                uint8_t rkpiece = board.read(rkpos);
                board.write(src, mBLK);
                board.write(dst, srcpiece);
                board.write(rkpos, mBLK);
                board.write((dst >> 1), rkpiece);
                return;
            }
        }

        if(srcpiece == mWPW && dstpiece == mBLK &&
           (src >> 8) != dst && (src >> 16) != dst){
            cMove move(MOVE_TYPE["en-passant"], src, dst, prompiece, board.field);
            minutes.push_back(move);
            uint64_t eppos = (dst << 8);
            uint8_t eppiece = board.read(eppos);
            board.write(src, mBLK);
            board.write(dst, srcpiece);
            board.write(eppos, mBLK);
            score += SCORES[eppiece];
            return;
        }

        if(srcpiece == mBPW && dstpiece == mBLK &&
           (src << 8) != dst && (src << 16) != dst){
            cMove move(MOVE_TYPE["en-passant"], src, dst, prompiece, board.field);
            minutes.push_back(move);
            uint64_t eppos = (dst >> 8);
            uint8_t eppiece = board.read(eppos);
            board.write(src, mBLK);
            board.write(dst, srcpiece);
            board.write(eppos, mBLK);
            score += SCORES[eppiece];
            return;
        }

        if((srcpiece == mWPW || srcpiece == mBPW) && prompiece != mBLK){
            if(dstpiece == mBLK){
                cMove move(MOVE_TYPE["standard"], src, dst, prompiece, board.field);
                minutes.push_back(move);
            }
            else{
                cMove move(MOVE_TYPE["capture"], src, dst, prompiece, board.field);
                minutes.push_back(move);
            }
            board.write(src, mBLK);
            board.write(dst, prompiece);
            score += REVERSED_SCORES[prompiece] - REVERSED_SCORES[srcpiece];
            return;
        }

        if(dstpiece == mBLK){
            cMove move(MOVE_TYPE["standard"], src, dst, prompiece, board.field);
            minutes.push_back(move);
        }
        else{
            cMove move(MOVE_TYPE["capture"], src, dst, prompiece, board.field);
            minutes.push_back(move);
        }
        board.write(src, mBLK);
        board.write(dst, srcpiece);
        score += SCORES[dstpiece];
     }


    bool cMatch::undo_move(){
        cMove move;
        if(minutes.size() > 0){
            move = minutes.back();
        }
        else{
            cout << "error" << endl;
            return false;
        }

        for(uint8_t i = 0; i < 8; ++i){
            board.field[i] = move.prev_field[i];
        }

        // read board after reset to previous
        uint8_t prev_dstpiece = board.read(move.dst);

        uint8_t pawn;
        if(move.prompiece != mBLK){
            if(cBoard::is_piece_white(move.prompiece)){
                pawn = mWPW;
            }
            else{
                pawn = mBPW;
            }
            score -= SCORES[prev_dstpiece];
            score += SCORES[move.prompiece] - SCORES[pawn];
        }
        else{
            score -= SCORES[prev_dstpiece];
        }
        
        minutes.pop_back();
        return true;
        // delete move;
    }

    /*int cMatch::eval_status(){
        if(board.is_move_available(minutes)){
            return STATUS["open"];
        }
        else{
            if(next_color() == COLORS["black"]){
                uint64_t bkg = board.field[1] & board.field[2];
                if(is_field_touched(board, bkg, COLORS["white"], EVAL_MODES["ignore-pins"])){
                    return STATUS["winner-white"];
                }
            }
            else{
                uint64_t wkg = board.field[0] & board.field[2];
                if(is_field_touched(board, wkg, COLORS["black"], EVAL_MODES["ignore-pins"])){
                    return STATUS["winner-black"];
                }
            }
        }
        return STATUS["draw"];
    }*/


    void cMatch::prnt_minutes(){
        for(cMove move : minutes){
            cout << move.format() << endl;
        }
    }
