
    #include "./match.hpp"
    #include "./generator.hpp"


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

    map<string, uint8_t> cMatch::STATUS = {
        {"open", 10},
        {"draw", 11}, 
        {"winner-white", 12}, 
        {"winner-black", 13}
    };

    map<string, uint8_t> cMatch::LEVELS = {
        {"blitz", 0}, 
        {"low", 1}, 
        {"medium", 2}, 
        {"high", 3}
    };

    map<uint8_t, uint16_t> cMatch::SECS_PER_MOVE = {
        {LEVELS["blitz"], 15}, 
        {LEVELS["low"], 30}, 
        {LEVELS["medium"], 60}, 
        {LEVELS["high"], 90}
    };

    map<string, uint8_t> RETURN_CODES = {
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
            score += SCORES[dstpiece];
            score += REVERSED_SCORES[prompiece];
            score += SCORES[srcpiece];
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
        uint8_t prev_srcpiece = board.read(move.src);
        uint8_t prev_dstpiece = board.read(move.dst);

        // promotion
        if(move.prompiece != mBLK){
            if(cBoard::is_piece_white(move.prompiece)){
                score += REVERSED_SCORES[prev_dstpiece];
                score += SCORES[move.prompiece];
                score += REVERSED_SCORES[prev_srcpiece];
            }
            else{
                score += REVERSED_SCORES[prev_dstpiece];
                score += SCORES[move.prompiece];
                score += REVERSED_SCORES[prev_srcpiece];
            }
        }
        // en passant white pawn
        else if(prev_srcpiece == mWPW && prev_dstpiece == mBLK){
            if((move.src >> 8) != move.dst && (move.src >> 16) != move.dst){
                score -= SCORES[mBPW];
            }
        }
        // en passant black pawn
        else if(prev_srcpiece == mBPW && prev_dstpiece == mBLK){
            if((move.src << 8) != move.dst && (move.src << 16) != move.dst){
                score -= SCORES[mWPW];
            }
        }
        else{
            score -= SCORES[prev_dstpiece];
        }

        minutes.pop_back();
        return true;
    }


    uint8_t cMatch::eval_status(){
        list<cGMove *> moves;
        cGenerator generator(this);

        generator.gen_moves(moves);
        if(moves.size() > 0){
            return STATUS["open"];
        }
        else{
            if(next_color() == mBLACK){
                uint64_t kg_pos = board.read_bkg_pos();
                if(board.is_square_enemy_touched(mWHITE, kg_pos)){
                    return STATUS["winner-white"];
                }
            }
            else{
                uint64_t kg_pos = board.read_wkg_pos();
                if(board.is_square_enemy_touched(mBLACK, kg_pos)){
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


    void cMatch::import(string filename){
        score = 0;

        level = LEVELS["blitz"];

        board.field[0] = 0xFFFF000000000000;
        board.field[1] = 0x000000000000FFFF;
        board.field[2] = 0x0800000000000008;
        board.field[3] = 0x1000000000000010;
        board.field[4] = 0x8100000000000081;
        board.field[5] = 0x2400000000000024;
        board.field[6] = 0x4200000000000042;
        board.field[7] = 0x00FF00000000FF00;
        
        minutes.clear();

        string line;
        ifstream myfile(filename);
        if(myfile.is_open()){
            uint64_t src, dst;
            uint8_t prompiece;

            while(getline(myfile, line)){
                if(line.length() < 5){
                    break;
                }

                if(line.length() == 10){
                    string str_piece = { line[7], line[8], line[9] };
                    prompiece = PIECES[str_piece];
                }
                else{
                    prompiece = mBLK;
                }

                string str_src = { line[0], line[1] };
                src = coord_to_pos(str_src);

                string str_dst = { line[3], line[4] };
                dst = coord_to_pos(str_dst);
                   
                do_move(src, dst, prompiece);
            }

            myfile.close();
        }
    }

