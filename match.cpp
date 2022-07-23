

    #include "./match.hpp"
    #include "./threading.hpp"


#ifdef DEBUG
    uint32_t g_filter_depth_cnt[100][20];
#endif


    cMatch::cMatch() : m_board(){ 
    }


    // copy constructor
    cMatch::cMatch(const cMatch &match) : m_board(match.m_board){

        m_score = match.m_score;

        for(const cMove &move : match.m_minutes){
            m_minutes.push_back(move);
        }

        for(const cMove &move : match.m_prev_search){
            m_prev_search.push_back(move);
        }

    }


    cMatch::~cMatch(){
    }


    void cMatch::reset(){
        m_score = 0;

        m_minutes.clear();

        m_prev_search.clear();

        m_board.m_fields[0][0] = mWRK;
        m_board.m_fields[0][1] = mWKN;
        m_board.m_fields[0][2] = mWBP; 
        m_board.m_fields[0][3] = mWQU; 
        m_board.m_fields[0][4] = mWKG; 
        m_board.m_fields[0][5] = mWBP;
        m_board.m_fields[0][6] = mWKN; 
        m_board.m_fields[0][7] = mWRK;

        for(uint8_t i = 0; i < 8; ++i){
            m_board.m_fields[1][i] = mWPW;
        }

        for(uint8_t j = 2; j < 6; ++j){
            for(uint8_t i = 0; i < 8; ++i){
                m_board.m_fields[j][i] = mBLK;
            }
        }

        for(uint8_t i = 0; i < 8; ++i){
            m_board.m_fields[6][i] = mBPW;
        }

        m_board.m_fields[7][0] = mBRK;
        m_board.m_fields[7][1] = mBKN;
        m_board.m_fields[7][2] = mBBP; 
        m_board.m_fields[7][3] = mBQU; 
        m_board.m_fields[7][4] = mBKG; 
        m_board.m_fields[7][5] = mBBP;
        m_board.m_fields[7][6] = mBKN; 
        m_board.m_fields[7][7] = mBRK;

        m_board.m_wKg_x = 4;
        m_board.m_wKg_y = 0;
        m_board.m_bKg_x = 4;
        m_board.m_bKg_y = 7;
        m_board.m_wKg_has_moved_at = 0;
        m_board.m_bKg_has_moved_at = 0;
        m_board.m_wRkA_has_moved_at = 0;
        m_board.m_wRkH_has_moved_at = 0;
        m_board.m_bRkA_has_moved_at = 0;
        m_board.m_bRkH_has_moved_at = 0;

    }


    uint8_t cMatch::next_color() const{

        if(m_minutes.size() % 2 == 0){
            return mWHITE;
        }
        else{
            return mBLACK;
        }

    }


    void cMatch::prnt_minutes() const{

        for(const cMove &move : m_minutes){
            cout << move.format(false) << endl;
        }

    }


    string cMatch::format_moves(vector<cMove> &moves){

        string str_moves = "";

        for(const cMove &move : moves){
            str_moves += " [" + move.format(false) + "] ";
        }
        return str_moves;

    }


    void cMatch::prnt_moves(vector<cMove> &moves){

        uint8_t idx = 1;

        cout << "-------------------------------------------" << endl;
        for(const cMove &move : moves){
            cout << "\n" << dec << idx << ". ";
            cout << move.format(false) << endl;
            idx++;
        }
        cout << "-------------------------------------------" << endl;

    }


    void cMatch::prnt_fmttime(string msg, int seconds){

        uint32_t sec, minute, hour, remaining;

        hour = seconds / (60 * 60);
        remaining = seconds % (60 * 60);
        minute = remaining / 60;
        sec = remaining % 60;

        cout << msg << setw(2) << setfill('0') << hour << ":";
        cout << setw(2) << setfill('0') <<  minute << ":";
        cout << setw(2) << setfill('0') << sec << endl;

    }


    void cMatch::calc_move(int32_t &rscore, vector<cMove> &rmoves){
        #ifdef DEBUG
          for(uint8_t d = 0; d < 100; ++d){
            for(uint8_t i = 0; i < 20; ++i){
              g_filter_depth_cnt[d][i] = 0;
            }
          }
        #endif

        time_t time_start = time(0);

        int32_t alpha = SCORES[mWKG] * 10;

        int32_t beta = SCORES[mBKG] * 10;

        uint8_t depth = 1;

        uint8_t maxdepth = 5;

        uint8_t stage = eval_stage();
        cout << "stage: " << to_string(stage) << endl;

        //if(stage != STAGE_OPENING && stage != STAGE_MIDDLE){
        //    cout << "Endgame - stage: " << to_string(stage) << endl;

        //    calc_alphabeta_endgame(rscore, rmoves, depth, maxdepth, alpha, beta, stage);
        //}
        //else{
        //    cout << "Opening or Middlegame - stage: " << to_string(stage) << endl;
            calc_alphabeta(rscore, rmoves, depth, maxdepth, alpha, beta, stage);
        //}

        m_prev_search.clear();

        for(const cMove &move : rmoves){
            m_prev_search.push_back(move);
        }

        cout << "\nmatch m_score: " << m_score;

        cout << "\nresult: " << rscore;

        cout << "\n" << cMatch::format_moves(rmoves) << endl;

        prnt_fmttime("\ncalc-time: ", time(0) - time_start);

        
        #ifdef DEBUG
          if(depth == 1){
            for(uint8_t d = 0; d < 100; ++d){

              cout << setw(3) << setfill(' ') << to_string(d) << ": ";

              for(uint8_t i = 0; i < 20; ++i){

                cout << to_string(i) << ": " << setw(8) << setfill(' ') << dec << g_filter_depth_cnt[d][i] << " ";

              }
              cout << endl;

            }
          }
        #endif

    }


    bool cMatch::is_move_valid(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece){

        vector <cMove> moves;
        vector <cPiece> pieces;
        uint8_t king, kg_x, kg_y, enmycolor;
        uint8_t piece = m_board.getfield(src_x, src_y);
        uint8_t color = PIECES_COLORS[piece];
        
        if(color != next_color()){
            return false;
        }

        if(color == mWHITE){
            king = mWKG;
            kg_x = m_board.m_wKg_x;
            kg_y = m_board.m_wKg_y;
            enmycolor = mBLACK;
        }
        else{
            king = mBKG;
            kg_x = m_board.m_bKg_x;
            kg_y = m_board.m_bKg_y;
            enmycolor = mWHITE;
        }

        m_board.search_for_touching_pieces(pieces, kg_x, kg_y, enmycolor, true);

        if(pieces.size() >= 2){
            gen_kg_moves(moves, kg_x, kg_y, king);
        }
        else if(pieces.size() == 1){
            gen_kg_moves(moves, kg_x, kg_y, king);
            cPiece piece = pieces.front();
            gen_kg_supporting_moves(moves, kg_x, kg_y, piece.m_piece, piece.m_xpos, piece.m_ypos, color);
        }
        else{
            if(piece == mWPW){ 
                gen_wpw_moves(moves, src_x, src_y); 
            }
            else if(piece == mBPW){ 
                gen_bpw_moves(moves, src_x, src_y); 
            }
            else if(piece == mWKG){ 
                gen_kg_moves(moves, src_x, src_y, mWKG);
                gen_wkg_castling_moves(moves, src_x, src_y);
            }
            else if(piece == mBKG){ 
                gen_kg_moves(moves, src_x, src_y, mBKG);
                gen_bkg_castling_moves(moves, src_x, src_y);
            }
            else if(piece == mWRK || piece == mBRK){ 
                gen_qrb_moves(moves, src_x, src_y, piece, 'r');
            }
            else if(piece == mWBP || piece == mBBP){ 
                gen_qrb_moves(moves, src_x, src_y, piece, 'b');
            }
            else if(piece == mWKN || piece == mBKN){ 
                gen_kn_moves(moves, src_x, src_y, piece); 
            }
            else if(piece == mWQU || piece == mBQU){
                gen_qrb_moves(moves, src_x, src_y, piece, 'q');
            }
            else{
                return false;
            }
        }

        bool found = false;
        
        //cout << "\ngen. züge: ";
        //for(const cMove &move : moves){
        //    cout << move.format() << endl;
        //}

        for(const cMove &move : moves){
            if(move.m_src_x == src_x && move.m_src_y == src_y &&
            move.m_dst_x == dst_x && move.m_dst_y == dst_y){
                found = true;
                break;
            }
        }

        return found;

    }


    uint8_t cMatch::eval_status(){

        vector<cMove> moves;
        gen_moves(moves, next_color());

        if(moves.size() > 0){
            return STATUS_OPEN;
        }
        else{
            if(next_color() == mWHITE){
                if(m_board.is_king_attacked(mBLACK)){
                    return STATUS_WINNER_BLACK;
                }
            }
            else{
                if(m_board.is_king_attacked(mWHITE)){
                    return STATUS_WINNER_WHITE;
                }
            }
        }

        return STATUS_DRAW;

    }


    void cMatch::prnt_status(uint8_t status) const{
        if(status == cMatch::STATUS_WINNER_WHITE){
            prnt_minutes();
            cout << "winner white!" << endl;
        }
        else if(status == cMatch::STATUS_WINNER_BLACK){
            prnt_minutes();
            cout << "winner black!" << endl;
        }
        else if(status == cMatch::STATUS_DRAW){
            prnt_minutes();
            cout << "draw!" << endl;
        }
    }


    void cMatch::do_usr_move(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece){

        uint8_t srcpiece = m_board.getfield(src_x, src_y);

        uint8_t dstpiece = m_board.getfield(dst_x, dst_y);
        
        cMove move(src_x, src_y, dst_x, dst_y, srcpiece, dstpiece, prompiece, cMove::PRIO_STANDARD);

        do_move(move);

    }


    void cMatch::do_move(const cMove &move){

        // promotion
        if(move.is_promotion()){
            m_board.setfield(move.m_src_x, move.m_src_y, mBLK);
            m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_prompiece);

            m_score += SCORES[move.m_dstpiece];
            if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                m_score += REVERSED_SCORES[move.m_prompiece] - REVERSED_SCORES[mWPW];
            }
            else{
                m_score += REVERSED_SCORES[move.m_prompiece] - REVERSED_SCORES[mBPW];
            }
            
            m_minutes.push_back(move);
            return;
        }

        // standard move
        m_board.setfield(move.m_src_x, move.m_src_y, mBLK);
        m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);

        m_score += SCORES[move.m_dstpiece];

        // add on for en passant
        if(move.is_en_passant()){
            uint8_t enpiece = m_board.getfield(move.m_dst_x, move.m_src_y);
            m_board.setfield(move.m_dst_x, move.m_src_y, mBLK);

            m_score += SCORES[enpiece];

            m_minutes.push_back(move);
            return;
        }

        if(move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){
            // castlings
            if(move.is_short_castling()){
                uint8_t rook = m_board.getfield(7, move.m_dst_y); // h == 7
                m_board.setfield(7, move.m_dst_y, mBLK);
                m_board.setfield(5, move.m_dst_y, rook); // f == 5
            }
            else if(move.is_long_castling()){
                uint8_t rook = m_board.getfield(0, move.m_dst_y); // a == 0
                m_board.setfield(0, move.m_dst_y, mBLK);
                m_board.setfield(3, move.m_dst_y, rook); // c == 3
            }

            // update flags in board
            if(move.m_srcpiece == mWKG){
                if(m_board.m_wKg_has_moved_at == 0){
                    m_board.m_wKg_has_moved_at = m_minutes.size() + 1;
                }
                m_board.m_wKg_x = move.m_dst_x;
                m_board.m_wKg_y = move.m_dst_y;
            }
            else{
                if(m_board.m_bKg_has_moved_at == 0){
                    m_board.m_bKg_has_moved_at = m_minutes.size() + 1;
                }
                m_board.m_bKg_x = move.m_dst_x;
                m_board.m_bKg_y = move.m_dst_y;
            }
        }
        else if(move.m_srcpiece == mWRK){
            // update flags in board
            if(move.m_src_x == 0 && move.m_src_y == 0 && 
               m_board.m_wRkA_has_moved_at == 0){
                m_board.m_wRkA_has_moved_at = m_minutes.size() + 1;
            }
            if(move.m_src_x == 7 && move.m_src_y == 0 && 
                m_board.m_wRkH_has_moved_at == 0){
                m_board.m_wRkH_has_moved_at = m_minutes.size() + 1;
            }
        }
        else if(move.m_srcpiece == mBRK){
            // update flags in board
            if(move.m_src_x == 0 && move.m_src_y == 7 && 
               m_board.m_bRkA_has_moved_at == 0){
                m_board.m_bRkA_has_moved_at = m_minutes.size() + 1;
            }
            if(move.m_src_x == 7 && move.m_src_y == 7 && 
                 m_board.m_bRkH_has_moved_at == 0){
                m_board.m_bRkH_has_moved_at = m_minutes.size() + 1;
            }
        }

        m_minutes.push_back(move);
    }


    bool cMatch::undo_move(){

        if(m_minutes.size() == 0){
            return false;
        }

        cMove move = m_minutes.back();

        // undo promotion
        if(move.is_promotion() && PIECES_COLORS[move.m_srcpiece] == mWHITE){
            m_board.setfield(move.m_src_x, move.m_src_y, mWPW);
            m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);
        
            m_score -= SCORES[move.m_dstpiece];
            m_score += SCORES[move.m_prompiece] - SCORES[mWPW];
        
            m_minutes.pop_back();
            return true;
        }
        else if(move.is_promotion() && PIECES_COLORS[move.m_srcpiece] == mBLACK){
            m_board.setfield(move.m_src_x, move.m_src_y, mBPW);
            m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);

            m_score -= SCORES[move.m_dstpiece];
            m_score += SCORES[move.m_prompiece] - SCORES[mBPW];

            m_minutes.pop_back();
            return true;
        }
        else{
            // undo standard move
            m_board.setfield(move.m_src_x, move.m_src_y, move.m_srcpiece);
            m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);

            m_score -= SCORES[move.m_dstpiece];
        }

        // undo en passant
        if(move.m_srcpiece == mWPW && move.is_en_passant()){
            m_board.setfield(move.m_dst_x, move.m_src_y, mBPW);

            m_score -= SCORES[mBPW];

            m_minutes.pop_back();
            return true;
        }
        else if(move.m_srcpiece == mBPW && move.is_en_passant()){
            m_board.setfield(move.m_dst_x, move.m_src_y, mWPW);

            m_score -= SCORES[mWPW];

            m_minutes.pop_back();
            return true;
        }

        // undo castling
        if(move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){
            if(move.is_short_castling()){
                uint8_t rook = m_board.getfield(5, move.m_src_y); // f == 5
                m_board.setfield(5, move.m_src_y, mBLK);
                m_board.setfield(7, move.m_src_y, rook); // h == 7
            }
            else if(move.is_long_castling()){
                uint8_t rook = m_board.getfield(3, move.m_src_y); // c == 3
                m_board.setfield(3, move.m_src_y, mBLK);
                m_board.setfield(0, move.m_src_y, rook); // a == 0
            }

            // update flags in board
            if(move.m_srcpiece == mWKG){
                if(m_board.m_wKg_has_moved_at != 0 && m_board.m_wKg_has_moved_at == m_minutes.size()){
                    m_board.m_wKg_has_moved_at = 0;
                }
                m_board.m_wKg_x = move.m_src_x;
                m_board.m_wKg_y = move.m_src_y;
            }
            else{
                if(m_board.m_bKg_has_moved_at != 0 && m_board.m_bKg_has_moved_at == m_minutes.size()){
                    m_board.m_bKg_has_moved_at = 0;
                }
                m_board.m_bKg_x = move.m_src_x;
                m_board.m_bKg_y = move.m_src_y;
            }
        }
        else if(move.m_srcpiece == mWRK){
            // update flags in board
            if(m_board.m_wRkA_has_moved_at != 0 && m_board.m_wRkA_has_moved_at == m_minutes.size()){
                m_board.m_wRkA_has_moved_at = 0;
            }
            if(m_board.m_wRkH_has_moved_at != 0 && m_board.m_wRkH_has_moved_at == m_minutes.size()){
                m_board.m_wRkH_has_moved_at = 0;
            }
        }
        else if(move.m_srcpiece == mBRK){
            // update flags in board
            if(m_board.m_bRkA_has_moved_at != 0 && m_board.m_bRkA_has_moved_at == m_minutes.size()){
                m_board.m_bRkA_has_moved_at = 0;
            }
            if(m_board.m_bRkH_has_moved_at != 0 && m_board.m_bRkH_has_moved_at == m_minutes.size()){
                m_board.m_bRkH_has_moved_at = 0;
            }
        }

        m_minutes.pop_back();
        return true;

    }


    bool cMatch::is_three_times_repetition(const cMove &move, uint8_t depth) const{

        cMatch *match = new cMatch(*this);

        match->do_move(move);

        uint8_t fields[8][8];

        match->m_board.copy_fields(fields);

        uint8_t equalcnt = 0;

        uint8_t maxcnt = min((uint8_t)(8 + depth), (uint8_t)match->m_minutes.size());

        for(uint8_t i = 0; i < maxcnt; ++i){
            match->undo_move();
            //match->undo_move(); 2x???

            if(match->m_board.compare_fields(fields)){
                equalcnt++;
            }
        }

        delete match;

        return equalcnt >= 2;

    }


    // 100 Züge davor kein Bauernzug und keine Figur geschlagen
    bool cMatch::is_fifty_moves_rule() const{

        if(m_minutes.size() < 100){
            return false;
        }

        int rulecnt = 0;

        int cnt = 0;

        cMatch *match = new cMatch(*this);

        for(vector<cMove>::reverse_iterator it = match->m_minutes.rbegin(); it != match->m_minutes.rend(); ++it){
            cnt++;

            if(cnt > 101){
                delete match;
                return false;
            }

            match->undo_move();

            if(it->m_dstpiece != mBLK || it->m_srcpiece == mWPW || it->m_srcpiece == mBPW){
                rulecnt = 0;
                continue;
            }

            rulecnt++;
            if(rulecnt >= 100){
                delete match;
                return true;
            }
        }

        delete match;
        return false;

    }


    //calculation
    bool cMatch::does_move_touch_vulnerable_piece(const cMove &move) const{

        cBoard board(m_board);
        board.setfield(move.m_src_x, move.m_src_y, mBLK);

        // supports and attacks
        vector<cPiece> supported, attacked;

        uint8_t excl_dir;

        if(move.m_srcpiece == mWPW || move.m_srcpiece == mBPW || move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){
            excl_dir = mNO_DIR;
        }
        else{
            excl_dir = cBoard::eval_dir(move.m_src_x, move.m_src_y, move.m_dst_x, move.m_dst_y);
        }

        if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
            board.search_all_dirs_for_pieces(supported, attacked, move.m_srcpiece, move.m_dst_x, move.m_dst_y, excl_dir);
        }
        else{
            board.search_all_dirs_for_pieces(attacked, supported, move.m_srcpiece, move.m_dst_x, move.m_dst_y, excl_dir);
        }

        // supports
        for(const cPiece &piece : supported){
            if(board.eval_pin_state(piece.m_piece, piece.m_xpos, piece.m_ypos) != cBoard::PINNED_NO){
                //cout << "1 " << reverse_lookup(PIECES, piece.m_piece) << " " << to_string(piece.m_src_x) << " " << to_string(piece.m_src_y) << endl;
                return true;
            }

            vector<cPiece> wpieces, bpieces;
            board.search_for_all_touching_pieces(wpieces, bpieces, piece.m_xpos, piece.m_ypos);

            if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                if(wpieces.size() < bpieces.size()){
                    //cout << "2" << endl;
                    return true;
                }
            }
            else{
                if(bpieces.size() < wpieces.size()){
                    //cout << "3" << endl;
                    return true;
                }
            }
        }

        // attacks
        for(const cPiece &piece : attacked){
            if(PIECES_RANKS[move.m_srcpiece] <= PIECES_RANKS[piece.m_piece]){
                return true;
            }
            
            if(board.eval_pin_state(piece.m_piece, piece.m_xpos, piece.m_ypos) != cBoard::PINNED_NO){
                //cout << "4" << endl;
                return true;
            }

            vector<cPiece> wpieces, bpieces;
            board.search_for_all_touching_pieces(wpieces, bpieces, piece.m_xpos, piece.m_ypos);

            if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                if(wpieces.size() > 0 && wpieces.size() == bpieces.size()){
                    //cout << "5 " << to_string(wpieces.size()) << " " << to_string(bpieces.size()) << endl;
                    return true;
                }
            }
            else{
                if(bpieces.size() > 0 && bpieces.size() == wpieces.size()){
                    //cout << "6" << endl;
                    return true;
                }
            }
        }

        return false;

    }


    bool cMatch::does_move_support_supply(const cMove &move) const{

        if(move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){ 
            return false; 
        }

        cBoard board(m_board);
        board.setfield(move.m_src_x, move.m_src_y, mBLK);
        board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);

        /*if(move.m_dstpiece == mBLK){
            board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);    
        }
        else{
            if(board.eval_pin_state(move.m_dstpiece, move.m_dst_x, move.m_dst_y) != cBoard::PINNED_HARD){
                board.setfield(move.m_dst_x, move.m_dst_y, mBLK);
            }
        }*/

        vector<cPiece> touching;
        board.search_for_touching_pieces(touching, move.m_src_x, move.m_src_y, PIECES_COLORS[move.m_srcpiece], false);

        vector<cPiece> pieces, others;
        if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
            board.search_all_dirs_for_pieces(others, pieces, mBLK, move.m_src_x, move.m_src_y, mNO_DIR);
        }
        else{
            board.search_all_dirs_for_pieces(pieces, others, mBLK, move.m_src_x, move.m_src_y, mNO_DIR);
        }

        for(const cPiece &touchg : touching){

            for(const cPiece &piece : pieces){
                uint8_t dir = cBoard::eval_dir(piece.m_xpos, piece.m_ypos, move.m_src_x, move.m_src_y);

                if((dir == mEST_WST || dir == mSTH_NTH) && (touchg.m_piece == mWRK || touchg.m_piece == mBRK || touchg.m_piece == mWQU || touchg.m_piece == mBQU)){
                    return true;
                }

                if((dir == mSTHWST_NTHEST || dir == mSTHEST_NTHWST) && (touchg.m_piece == mWBP || touchg.m_piece == mBBP || touchg.m_piece == mWQU || touchg.m_piece == mBQU)){
                     return true;
                }
            }

        }

        return false;

    }


    bool cMatch::continue_search(const cMove &move, const uint8_t depth, const uint8_t maxdepth, const uint8_t stage){

        if(stage == STAGE_ENDGAME_100 || stage == STAGE_ENDGAME_110 || stage == STAGE_ENDGAME_120 || stage == STAGE_ENDGAME_130){
            return depth <= maxdepth + 2;
        }

        // take en passant moves
        if(move.is_en_passant()){ 
            #ifdef DEBUG
              g_filter_depth_cnt[depth][0]++;
            #endif
            return true; 
        }
        // end -- take en passant moves


        cBoard board(m_board);
        board.setfield(move.m_src_x, move.m_src_y, mBLK);
        board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);


        // take moves pushing to promotion last and second last rank
        if(move.m_srcpiece == mWPW && move.m_dst_y >= 6 && board.is_running_pawn(move.m_srcpiece, move.m_dst_x, move.m_dst_y)){
            #ifdef DEBUG
              g_filter_depth_cnt[depth][1]++;
            #endif
            return true;
        }

        if(move.m_srcpiece == mBPW && move.m_dst_y <= 1 && board.is_running_pawn(move.m_srcpiece, move.m_dst_x, move.m_dst_y)){
            #ifdef DEBUG
              g_filter_depth_cnt[depth][2]++;
            #endif
            return true;
        }
        // end -- take moves pushing to promotion


        // take promotion moves
        /*if(move.m_prompiece != mBLK){
            #ifdef DEBUG
              g_filterdetector[1]++;
            #endif
            return true; 
        }*/


        // take king attacking moves
        if(depth <= maxdepth + 2){
            if(board.is_king_attacked(PIECES_COLORS[move.m_srcpiece])){
                #ifdef DEBUG
                  g_filter_depth_cnt[depth][3]++;
                #endif
                return true; 
            }
        }
        // end -- take king attacking moves


        // take good capture moves
        int32_t mscore = score_move(move);
        if((PIECES_COLORS[move.m_srcpiece] == mWHITE && mscore >= SCORES[mBPW]) || (PIECES_COLORS[move.m_srcpiece] == mBLACK && mscore <= SCORES[mWPW])){
                #ifdef DEBUG
                  g_filter_depth_cnt[depth][4]++;
                #endif
                return true;
            }

        /*if(move.m_dstpiece != mBLK){ 

            // captured piece is equal/higher as scrpiece
            if(PIECES_RANKS[move.m_dstpiece] > PIECES_RANKS[move.m_srcpiece]){
                #ifdef DEBUG
                  g_filterdetector[4]++;
                #endif
                return true;
            }

            cField field(&board, move.m_dst_x, move.m_dst_y);

            // dstfield is clear
            if(field.is_clear() || (PIECES_COLORS[move.m_srcpiece] == mWHITE && field.is_only_wtouched()) || (PIECES_COLORS[move.m_srcpiece] == mBLACK && field.is_only_btouched())){
                #ifdef DEBUG
                  if(field.is_clear()){
                    g_filterdetector[5]++;
                  }
                  else if(PIECES_COLORS[move.m_srcpiece] == mWHITE && field.is_only_wtouched()){
                    g_filterdetector[6]++;
                  }
                  else if(PIECES_COLORS[move.m_srcpiece] == mBLACK && field.is_only_btouched()){
                    g_filterdetector[7]++;
                  }
                #endif
                return true;
            }

            //dstfield is save
            int8_t state = field.eval_advant_score();
            if((PIECES_COLORS[move.m_srcpiece] == mWHITE && state >= 2) || (PIECES_COLORS[move.m_srcpiece] == mBLACK && state <= -2)){
                #ifdef DEBUG
                  g_filterdetector[10]++;
                #endif
                return true;
            }
        }*/
        // end -- take good capture moves


        /*
        // take good capture moves
        if(move.m_dstpiece != mBLK){ 
            // piece must be lower than captured piece
            if(PIECES_RANKS[move.m_srcpiece] < PIECES_RANKS[move.m_dstpiece]){
                #ifdef DEBUG
                  g_filterdetector[3]++;
                #endif
                return true;
            }

            vector<cPiece> wpieces, bpieces;
            board.search_for_all_touching_pieces(wpieces, bpieces, move.m_dst_x, move.m_dst_y);
            
            // remove pinned pieces
            for(vector <cPiece>::iterator it = wpieces.begin(); it != wpieces.end();){
                if(board.eval_pin_state(it->m_piece, it->m_src_x, it->m_src_y) != cBoard::PINNED_NO){
                    it = wpieces.erase(it);
                }
                else{
                    it++;
                }
            }

            for(vector <cPiece>::iterator it = bpieces.begin(); it != bpieces.end();){
                if(board.eval_pin_state(it->m_piece, it->m_src_x, it->m_src_y) != cBoard::PINNED_NO){
                    it = bpieces.erase(it);
                }
                else{
                    it++;
                }
            }

            // move is good because captured piece is not supported
            if((PIECES_COLORS[move.m_dstpiece] == mWHITE && wpieces.size() == 0) || (PIECES_COLORS[move.m_dstpiece] == mBLACK && bpieces.size() == 0)){
                #ifdef DEBUG
                  g_filterdetector[4]++;
                #endif
                return true; 
            }

            // move is good because captured piece is more attacked than supported 
            if((PIECES_COLORS[move.m_srcpiece] == mWHITE && wpieces.size() > bpieces.size()) || (PIECES_COLORS[move.m_srcpiece] == mBLACK && bpieces.size() > wpieces.size())){
                #ifdef DEBUG
                  g_filterdetector[5]++;
                #endif
                return true; 
            }

            // search for lowest supporter
            uint8_t min_wpiece = PIECES_RANKS[mWKG];
            for(const cPiece &piece : wpieces){
                if(PIECES_RANKS[piece.m_piece] < min_wpiece){
                    min_wpiece = PIECES_RANKS[piece.m_piece];
                }
            }

            uint8_t min_bpiece = PIECES_RANKS[mWKG];
            for(const cPiece &piece : bpieces){
                if(PIECES_RANKS[piece.m_piece] < min_bpiece){
                    min_bpiece = PIECES_RANKS[piece.m_piece];
                }
            }

            // if lowest attacker is less than lowest supporter, move is good
            if((PIECES_COLORS[move.m_srcpiece] == mWHITE && min_wpiece < min_bpiece) || (PIECES_COLORS[move.m_srcpiece] == mBLACK && min_bpiece < min_wpiece)){
                #ifdef DEBUG
                  g_filterdetector[6]++;
                #endif
                return true;
            }
        }*/


        // take sacrifying moves that make room for another attacker
        if(does_move_support_supply(move)){
            #ifdef DEBUG
              g_filter_depth_cnt[depth][5]++;
            #endif
            return true;
        }
        // end -- take sacrifying moves that make room for another attacker


        // every search has to come to an end
        if(depth >= maxdepth + 6){
            return false;
        }
        // end -- every search has to come to an end


        // due to perfomance do this check only for interesting moves
        /*if(depth < maxdepth + 2){
            do_move(move);
            bool stormy = is_stormy();
            undo_move();

            if(stormy){
                #ifdef DEBUG
                  g_filterdetector[6]++;
                #endif
                return true;
            }
        }*/


        // take passed pawn moves
        /*if(stage != STAGE_OPENING && stage != STAGE_MIDDLE && board.is_passed_pawn(move.m_srcpiece, move.m_dst_x, move.m_dst_y)){
            if((move.m_srcpiece == mWPW && move.m_dst_y >= 5) || (move.m_srcpiece == mBPW && move.m_dst_y <= 2)){
                return true;
            }
        }*/


        // running pawns
        /*for(uint8_t x = 0; x < 8; ++x){
            for(uint8_t y = 5; y < 7; ++y){

                uint8_t piece = board.getfield(x, y);

                if(piece == mWPW){
                    if(board.is_running_pawn(piece, x, y)){
                        #ifdef DEBUG
                          g_filterdetector[7]++;
                        #endif
                        return true;
                    }
                }

            }
        }

        for(uint8_t x = 0; x < 8; ++x){
            for(uint8_t y = 2; y > 0; --y){

                uint8_t piece = board.getfield(x, y);

                if(piece == mBPW){
                    if(board.is_running_pawn(piece, x, y)){
                        #ifdef DEBUG
                          g_filterdetector[8]++;
                        #endif
                        return true;
                    }
                }

            }
        }*/
        // end -- running pawns


        if(stage == STAGE_ENDGAME){
            return depth <= maxdepth + 2; 
        }

        return false;

    }


    bool cMatch::new_is_search_continue(const cMove &move, const uint8_t depth, const uint8_t maxdepth, const uint8_t stage){

        if(stage == STAGE_ENDGAME_100 || stage == STAGE_ENDGAME_110 || stage == STAGE_ENDGAME_120 || stage == STAGE_ENDGAME_130){
            return depth <= maxdepth + 2;
        }

        // take en passant moves
        if(move.is_en_passant()){ 
            #ifdef DEBUG
              g_filter_depth_cnt[depth][0]++;
            #endif
            return true; 
        }
        // end -- take en passant moves


        cBoard board(m_board);
        board.setfield(move.m_src_x, move.m_src_y, mBLK);
        board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);


        // take moves pushing to promotion last and second last rank
        if((move.m_srcpiece == mWPW && move.m_dst_y >= 6) || (move.m_srcpiece == mBPW && move.m_dst_y <= 1)){
            #ifdef DEBUG
              g_filter_depth_cnt[depth][1]++;
            #endif
            return true;
        }

        // take promotion moves
        /*if(move.m_prompiece != mBLK){
            #ifdef DEBUG
              g_filterdetector[1]++;
            #endif
            return true; 
        }*/
        // end -- take moves pushing to promotion


        // take king attacking moves
        if(depth <= maxdepth + 2){
            if(board.is_king_attacked(PIECES_COLORS[move.m_srcpiece])){
                #ifdef DEBUG
                  g_filter_depth_cnt[depth][3]++;
                #endif
                return true; 
            }
        }
        // end -- take king attacking moves


        // take good capture moves
        int32_t mscore = score_move(move);
        if((PIECES_COLORS[move.m_srcpiece] == mWHITE && mscore >= SCORES[mBPW]) || (PIECES_COLORS[move.m_srcpiece] == mBLACK && mscore <= SCORES[mWPW])){
            #ifdef DEBUG
              g_filter_depth_cnt[depth][4]++;
            #endif
            return true;
        }
        // end -- take good capture moves


        // take sacrifying moves that make room for another attacker
        /*if(does_move_support_supply(move)){
            #ifdef DEBUG
              g_filter_depth_cnt[depth][5]++;
            #endif
            return true;
        }*/
        // end -- take sacrifying moves that make room for another attacker


        // every search has to come to an end
        if(depth >= maxdepth + 6){
            return false;
        }
        // end -- every search has to come to an end


        // attacks and support
        vector<cPiece> found_pieces;
        cPiece piece(move.m_srcpiece, move.m_dst_x, move.m_dst_y);
        board.new_search_for_touched_pieces(found_pieces, piece, false);

        for(const cPiece &found_piece : found_pieces){
            cField field_before(&m_board, found_piece.m_xpos, found_piece.m_ypos);
            int8_t score_before = field_before.eval_score();

            if(PIECES_COLORS[move.m_srcpiece] == PIECES_COLORS[found_piece.m_piece]){
                if(field_before.is_clear()){
                    continue;
                }

                if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                    if(field_before.is_only_wtouched() || score_before >= 0){
                        continue;
                    }
                }
                else{
                    if(field_before.is_only_btouched() || score_before <= 0){
                        continue;
                    }
                }
            }

            cField field_after(&board, found_piece.m_xpos, found_piece.m_ypos);
            int8_t score_after = field_after.eval_score();

            if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                if((score_after - 2) > score_before){
                    return true;
                }
            }
            else{
                if((score_after + 2) < score_before){
                    return true;
                }
            }

        }
        // end -- attacks and support


        // attacks and support disclosures



        // end -- attacks and support disclosures


        // due to perfomance do this check only for interesting moves
        /*if(depth < maxdepth + 2){
            do_move(move);
            bool stormy = is_stormy();
            undo_move();

            if(stormy){
                #ifdef DEBUG
                  g_filterdetector[6]++;
                #endif
                return true;
            }
        }*/


        if(stage == STAGE_ENDGAME){
            return depth <= maxdepth + 2; 
        }

        return false;

    }


    void cMatch::append_newmove(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, const cMove &move){

        rcandidates.clear();

        rcandidates.push_back(move);

        for(const cMove &nmove : newcandidates){
            rcandidates.push_back(nmove);
        }

    }


    void cMatch::gen_moves(vector<cMove> &moves, uint8_t color){

        uint8_t king, kg_x, kg_y, enmycolor;

        if(color == mWHITE){
            king = mWKG;
            kg_x = m_board.m_wKg_x;
            kg_y = m_board.m_wKg_y;
            enmycolor = mBLACK;
        }
        else{
            king = mBKG;
            kg_x = m_board.m_bKg_x;
            kg_y = m_board.m_bKg_y;
            enmycolor = mWHITE;
        }

        vector<cPiece> attacking_pieces;
        m_board.search_for_touching_pieces(attacking_pieces, kg_x, kg_y, enmycolor, true);

        if(attacking_pieces.size() >= 2){
            gen_kg_moves(moves, kg_x, kg_y, king);
        }
        else if(attacking_pieces.size() == 1){
            gen_kg_moves(moves, kg_x, kg_y, king);

            cPiece attacking_piece = attacking_pieces.front();
            gen_kg_supporting_moves(moves, kg_x, kg_y, attacking_piece.m_piece, attacking_piece.m_xpos, attacking_piece.m_ypos, color);
        }
        else{
            for(uint8_t y = 0; y < 8; ++y){

                for(uint8_t x = 0; x < 8; ++x){

                    uint8_t piece = m_board.getfield(x, y);

                    if(PIECES_COLORS[piece] == color){
                        if(piece == mWPW){ 
                            gen_wpw_moves(moves, x, y); 
                            continue; 
                        }

                        if(piece == mBPW){ 
                            gen_bpw_moves(moves, x, y); 
                            continue; 
                        }

                        if(piece == mWKG){ 
                            gen_kg_moves(moves, x, y, mWKG);
                            gen_wkg_castling_moves(moves, x, y);
                            continue; 
                        }

                        if(piece == mBKG){ 
                            gen_kg_moves(moves, x, y, mBKG);
                            gen_bkg_castling_moves(moves, x, y);
                            continue; 
                        }

                        if(piece == mWRK || piece == mBRK){ 
                            gen_qrb_moves(moves, x, y, piece, 'r');
                            continue; 
                        }

                        if(piece == mWBP || piece == mBBP){ 
                            gen_qrb_moves(moves, x, y, piece, 'b');
                            continue; 
                        }

                        if(piece == mWKN || piece == mBKN){ 
                            gen_kn_moves(moves, x, y, piece); 
                            continue;
                        }

                        if(piece == mWQU || piece == mBQU){
                            gen_qrb_moves(moves, x, y, piece, 'q');
                            continue; 
                        }

                    }

                }

            }

            attacking_pieces.clear();
        }

    }


    void cMatch::add_wpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece){

        if(src_y == 6 && dst_y == 7){
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWQU, cMove::PRIO_PROMOTION));
            
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWRK, cMove::PRIO_PROMOTION + 1));
            
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWBP, cMove::PRIO_PROMOTION + 2));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWKN, cMove::PRIO_PROMOTION + 3));
        }
        else{
            cMove move(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mBLK, cMove::PRIO_STANDARD);

            //set_prio_for_move(move);

            moves.push_back(move);
        }
    }


    void cMatch::gen_wpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        uint8_t pindir = m_board.eval_pindir(x, y, mWHITE);

        if((pindir == mNO_DIR || pindir == mSTHWST_NTHEST) && cBoard::is_inbounds(x + 1, y + 1)){
            uint8_t dstpiece = m_board.getfield(x + 1, y + 1);

            if(PIECES_COLORS[dstpiece] == mBLACK){
                add_wpw_moves(moves, x, y, x + 1, y + 1, dstpiece);
            }
            else if(dstpiece == mBLK && y == 4){ // en passant
                cMove prevmove = m_minutes.back();

                uint8_t enpiece = m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                if(enpiece == mBPW && prevmove.m_src_y == 6 && prevmove.m_dst_y == 4 && prevmove.m_src_x == prevmove.m_dst_x && x + 1 == prevmove.m_src_x){
                    moves.push_back(cMove(x, y, x + 1, y + 1, mWPW, dstpiece, mBLK, cMove::PRIO_EN_PASSANT));
                }
            }
        }

        if((pindir == mNO_DIR || pindir == mSTHEST_NTHWST) && cBoard::is_inbounds(x - 1, y + 1)){

            uint8_t dstpiece = m_board.getfield(x - 1, y + 1);

            if(PIECES_COLORS[dstpiece] == mBLACK){
                add_wpw_moves(moves, x, y, x - 1, y + 1, dstpiece);
            }
            else if(dstpiece == mBLK && y == 4){ // en passant
                cMove prevmove = m_minutes.back();

                uint8_t enpiece = m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                if(enpiece == mBPW && prevmove.m_src_y == 6 && prevmove.m_dst_y == 4 && prevmove.m_src_x == prevmove.m_dst_x && x - 1 == prevmove.m_src_x){
                    moves.push_back(cMove(x, y, x - 1, y + 1, mWPW, dstpiece, mBLK, cMove::PRIO_EN_PASSANT));
                }
            }
        }

        if(pindir == mNO_DIR || pindir == mSTH_NTH){
            if(cBoard::is_inbounds(x, y + 1)){
                uint8_t dstpiece = m_board.getfield(x, y + 1);

                if(dstpiece == mBLK){
                    add_wpw_moves(moves, x, y, x, y + 1, dstpiece);
                }
            }
            if(cBoard::is_inbounds(x, y + 2)){
                uint8_t dstpiece = m_board.getfield(x, y + 2);

                uint8_t midpiece = m_board.getfield(x, y + 1);

                if(midpiece == mBLK && dstpiece == mBLK && y == 1){
                    add_wpw_moves(moves, x, y, x, y + 2, dstpiece);
                }
            }
        }

    }


    void cMatch::add_bpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece){

        if(src_y == 1 && dst_y == 0){
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBQU, cMove::PRIO_PROMOTION));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBRK, cMove::PRIO_PROMOTION + 1));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBBP, cMove::PRIO_PROMOTION + 2));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBKN, cMove::PRIO_PROMOTION + 3));
        }
        else{
            cMove move(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBLK, cMove::PRIO_STANDARD);

            //set_prio_for_move(move);

            moves.push_back(move);
        }

    }


    void cMatch::gen_bpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        uint8_t pindir = m_board.eval_pindir(x, y, mBLACK);

        if((pindir == mNO_DIR || pindir == mSTHEST_NTHWST) && cBoard::is_inbounds(x + 1, y - 1)){
            
            uint8_t dstpiece = m_board.getfield(x + 1, y - 1);
            if(PIECES_COLORS[dstpiece] == mWHITE){
                add_bpw_moves(moves, x, y, x + 1, y - 1, dstpiece);
            }
            else if(dstpiece == mBLK && y == 3){ // en passant
                cMove prevmove = m_minutes.back();

                uint8_t enpiece = m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                if(enpiece == mWPW && prevmove.m_src_y == 1 && prevmove.m_dst_y == 3 && prevmove.m_src_x == prevmove.m_dst_x && x + 1 == prevmove.m_src_x){
                    moves.push_back(cMove(x, y, x + 1, y - 1, mBPW, dstpiece, mBLK, cMove::PRIO_EN_PASSANT));
                }
            }
        }

        if((pindir == mNO_DIR || pindir == mSTHWST_NTHEST) && cBoard::is_inbounds(x - 1, y - 1)){
            uint8_t dstpiece = m_board.getfield(x - 1, y - 1);

            if(PIECES_COLORS[dstpiece] == mWHITE){
                add_bpw_moves(moves, x, y, x - 1, y - 1, dstpiece);
            }
            else if(dstpiece == mBLK && y == 3){ // en passant
                cMove prevmove = m_minutes.back();

                uint8_t enpiece = m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                if(enpiece == mWPW && prevmove.m_src_y == 1 && prevmove.m_dst_y == 3 && prevmove.m_src_x == prevmove.m_dst_x && x - 1 == prevmove.m_src_x){
                    moves.push_back(cMove(x, y, x - 1, y - 1, mBPW, dstpiece, mBLK, cMove::PRIO_EN_PASSANT));
                }
            }
        }

        if(pindir == mNO_DIR || pindir == mSTH_NTH){
            if(cBoard::is_inbounds(x, y - 1)){
                uint8_t dstpiece = m_board.getfield(x, y - 1);

                if(dstpiece == mBLK){
                    add_bpw_moves(moves, x, y, x, y - 1, dstpiece);
                }
            }

            if(cBoard::is_inbounds(x, y - 2)){
                uint8_t dstpiece = m_board.getfield(x, y - 2);

                uint8_t midpiece = m_board.getfield(x, y - 1);

                if(midpiece == mBLK && dstpiece == mBLK && y == 6){
                    add_bpw_moves(moves, x, y, x, y - 2, dstpiece);
                }
            }
        }

    }


    void cMatch::gen_kn_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t knight){

        uint8_t pindir = m_board.eval_pindir(x, y, PIECES_COLORS[knight]);
        if(pindir != mNO_DIR){ 
            return; 
        }

        int8_t steps[][2] = { { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

        for(uint8_t i = 0; i < size(steps); ++i){

            if(cBoard::is_inbounds(x + steps[i][0], y + steps[i][1])){

                uint8_t dst_x = x + steps[i][0];
                uint8_t dst_y = y + steps[i][1];

                uint8_t dstpiece = m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] != PIECES_COLORS[knight]){
                    cMove move(x, y, dst_x, dst_y, knight, dstpiece, mBLK, cMove::PRIO_STANDARD);

                    set_prio_for_move(move);

                    moves.push_back(move);
                }
            }
        }

    }


    void cMatch::gen_qrb_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t piece, char _switch){

        uint8_t pindir = m_board.eval_pindir(x, y, PIECES_COLORS[piece]);
        
        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        uint8_t idx, maxidx;

        if(_switch == 'q'){ idx = 0; maxidx = 8; }
        else if(_switch == 'r'){ idx = 0; maxidx = 4; }
        else if(_switch == 'b'){ idx = 4; maxidx = 8; }
        else{ return; }

        for(; idx < maxidx; ++idx){
            uint8_t dst_x = x;
            uint8_t dst_y = y;

            for(uint8_t j = 0; j < 7; ++j){
                if(cBoard::is_inbounds(dst_x + steps[idx][0], dst_y + steps[idx][1]) == false){
                    break;
                }

                dst_x += steps[idx][0];
                dst_y += steps[idx][1];

                if(pindir != mNO_DIR){
                    if(pindir != cBoard::eval_dir(x, y, dst_x, dst_y)){ 
                        break; 
                    }
                }

                uint8_t dstpiece = m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[piece]){
                    break;
                }

                cMove move(x, y, dst_x, dst_y, piece, dstpiece, mBLK, cMove::PRIO_STANDARD);

                set_prio_for_move(move);

                moves.push_back(move);

                if(dstpiece != mBLK){
                    break;
                }
            }
        }

    }


    void cMatch::gen_kg_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t king){

        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        cBoard board(m_board);

        uint8_t enmycolor;

        if(PIECES_COLORS[king] == mWHITE){
            enmycolor = mBLACK;

            board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);
        }
        else{
            enmycolor = mWHITE;

            board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);
        }

        for(uint8_t i = 0; i < size(steps); ++i){

            uint8_t dst_x = x;
            uint8_t dst_y = y;

            if(cBoard::is_inbounds(dst_x + steps[i][0], dst_y + steps[i][1])){

                dst_x += steps[i][0];
                dst_y += steps[i][1];

                uint8_t dstpiece = board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[king]){
                    continue;
                }

                bool is_clear = false;

                if(board.search_for_touching_piece(dst_x, dst_y, enmycolor) == mBLK){
                    is_clear = true;
                }

                if(is_clear == false){ 
                    continue; 
                }

                cMove move(x, y, dst_x, dst_y, king, dstpiece, mBLK, cMove::PRIO_STANDARD - 1);

                if(dstpiece != mBLK){ 
                    move.m_prio = cMove::PRIO_CAPTURE;
                }
                else if(does_move_touch_vulnerable_piece(move)){
                    // priorize weaker because piece might be already touched before move 
                    move.m_prio = (cMove::PRIO_URGENT_TOUCH + 5); 
                }

                moves.push_back(move);
            }
        }


    }


    void cMatch::gen_wkg_castling_moves(vector<cMove> &moves,  uint8_t x, uint8_t y){

        cBoard board(m_board);
        board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);

        // short white castling
        if(m_board.m_wKg_has_moved_at == 0 && m_board.m_wRkH_has_moved_at == 0 && cBoard::is_inbounds(x + 3, y)){

            if(y == 0 && m_board.getfield(x + 1, y) == mBLK && m_board.getfield(x + 2, y) == mBLK && m_board.getfield(x + 3, y) == mWRK){

                bool is_clear = true;

                for(uint8_t i = 0; i < 3; ++i){
                    if(board.search_for_touching_piece(x + i, y, mBLACK) != mBLK){
                        is_clear = false;
                        break;
                    }
                }

                if(is_clear){
                    cMove move(x, y, x + 2, y, mWKG, mBLK, mBLK, cMove::PRIO_CASTLING);

                    if(does_move_touch_vulnerable_piece(move)){
                        // priorize weaker because piece might be already touched before move 
                        move.m_prio = (cMove::PRIO_URGENT_TOUCH + 5); 
                    }

                    set_prio_for_castling(move);

                    moves.push_back(move);
                }
            }
        }

        // long white castling
        if(m_board.m_wKg_has_moved_at == 0 && m_board.m_wRkA_has_moved_at == 0 && cBoard::is_inbounds(x - 4, y)){

            if(y == 0 && m_board.getfield(x - 1, y) == mBLK && m_board.getfield(x - 2, y) == mBLK && m_board.getfield(x - 3, y) == mBLK && m_board.getfield(x - 4, y) == mWRK){

                bool is_clear = true;

                for(uint8_t i = 0; i < 3; ++i){
                    if(board.search_for_touching_piece(x - i, y, mBLACK) != mBLK){
                        is_clear = false;
                        break;
                    }
                }

                if(is_clear){
                    cMove move(x, y, x - 2, y, mWKG, mBLK, mBLK, cMove::PRIO_CASTLING);

                    if(does_move_touch_vulnerable_piece(move)){
                        // priorize weaker because piece might be already touched before move 
                        move.m_prio = (cMove::PRIO_URGENT_TOUCH + 5); 
                    }

                    set_prio_for_castling(move);

                    moves.push_back(move);
                }
            }
        }

    }


    void cMatch::gen_bkg_castling_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        cBoard board(m_board);
        board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);

        // short black castling
        if(m_board.m_bKg_has_moved_at == 0 && m_board.m_bRkH_has_moved_at == 0 && cBoard::is_inbounds(x + 3, y)){

            if(y == 7 && m_board.getfield(x + 1, y) == mBLK && m_board.getfield(x + 2, y) == mBLK && m_board.getfield(x + 3, y) == mBRK){

                bool is_clear = true;

                for(uint8_t i = 0; i < 3; ++i){
                    if(board.search_for_touching_piece(x + i, y, mWHITE) != mBLK){
                        is_clear = false;
                        break;
                    }
                }

                if(is_clear){
                    cMove move(x, y, x + 2, y, mBKG, mBLK, mBLK, cMove::PRIO_CASTLING);

                    if(does_move_touch_vulnerable_piece(move)){
                        // priorize weaker because piece might be already touched before move 
                        move.m_prio = (cMove::PRIO_URGENT_TOUCH + 5); 
                    }

                    set_prio_for_castling(move);

                    moves.push_back(move);
                }
            }
        }

        // long black castling
        if(m_board.m_bKg_has_moved_at == 0 && m_board.m_bRkA_has_moved_at == 0 && cBoard::is_inbounds(x - 4, y)){

            if(y == 7 && m_board.getfield(x - 1, y) == mBLK && m_board.getfield(x - 2, y) == mBLK && m_board.getfield(x - 3, y) == mBLK && m_board.getfield(x - 4, y) == mBRK){

                bool is_clear = true;

                for(uint8_t i = 0; i < 3; ++i){
                    if(board.search_for_touching_piece(x - i, y, mWHITE) != mBLK){
                        is_clear = false;
                        break;
                    }
                }

                if(is_clear){
                    cMove move(x, y, x - 2, y, mBKG, mBLK, mBLK, cMove::PRIO_CASTLING);

                    if(does_move_touch_vulnerable_piece(move)){
                        // priorize weaker because piece might be already touched before move 
                        move.m_prio = (cMove::PRIO_URGENT_TOUCH + 5); 
                    }

                    set_prio_for_castling(move);

                    moves.push_back(move);
                }
            }
        }

    }


    void cMatch::gen_kg_supporting_moves(vector<cMove> &moves, uint8_t kg_x, uint8_t kg_y, uint8_t attacking_piece, uint8_t attacking_x, uint8_t attacking_y, uint8_t color){

        vector<cPiece> defending_pieces;        

        m_board.search_for_touching_pieces(defending_pieces, attacking_x, attacking_y, color, true);

        for(const cPiece &defending_piece : defending_pieces){
            if(defending_piece.m_piece == mWKG || defending_piece.m_piece == mBKG){
                continue;
            }

            uint8_t pindir = m_board.eval_pindir(defending_piece.m_xpos, defending_piece.m_ypos, PIECES_COLORS[defending_piece.m_piece]);

            uint8_t dir = cBoard::eval_dir(defending_piece.m_xpos, defending_piece.m_ypos, attacking_x, attacking_y);

            if(pindir != mNO_DIR && pindir != dir){
                continue;
            }

            if(defending_piece.m_piece == mWPW){
                add_wpw_moves(moves, defending_piece.m_xpos, defending_piece.m_ypos, attacking_x, attacking_y, attacking_piece);
            }
            else if(defending_piece.m_piece == mBPW){
                add_bpw_moves(moves, defending_piece.m_xpos, defending_piece.m_ypos, attacking_x, attacking_y, attacking_piece);
            }
            else{
                moves.push_back(cMove(defending_piece.m_xpos, defending_piece.m_ypos, attacking_x, attacking_y, defending_piece.m_piece, attacking_piece, mBLK, 40));
            }
        }
        defending_pieces.clear();

        uint8_t dir = cBoard::eval_dir(kg_x, kg_y, attacking_x, attacking_y);
     
        if(dir != mNO_DIR){
            int8_t step_x, step_y;
            uint8_t dst_x = attacking_x;
            uint8_t dst_y = attacking_y;

            if(dir == mEST_WST){
                if(kg_x > attacking_x){
                    step_x = 1;
                    step_y = 0;
                }
                else{
                    step_x = -1;
                    step_y = 0;
                }
            }
            else if(dir == mSTH_NTH){
                if(kg_y > attacking_y){
                    step_x = 0;
                    step_y = 1;
                }
                else{
                    step_x = 0;
                    step_y = -1;
                }
            }
            else if(dir == mSTHWST_NTHEST){
                if(kg_y > attacking_y){
                    step_x = 1;
                    step_y = 1;
                }
                else{
                    step_x = -1;
                    step_y = -1;
                }
            }        
            else if(dir == mSTHEST_NTHWST){
                if(kg_y > attacking_y){
                    step_x = -1;
                    step_y = 1;
                }
                else{
                    step_x = 1;
                    step_y = -1;
                }
            }
            else{ // practically not possible
                return;
            }

            vector<cPiece> defending_pieces;
            dst_x += step_x;
            dst_y += step_y;
            while(!(dst_x == kg_x && dst_y == kg_y)){
                m_board.search_for_touching_pieces(defending_pieces, dst_x, dst_y, color, false);
                  for(const cPiece &defending_piece : defending_pieces){
                    if(defending_piece.m_piece == mWKG || defending_piece.m_piece == mBKG){
                        continue;
                    }

                    uint8_t pindir = m_board.eval_pindir(defending_piece.m_xpos, defending_piece.m_ypos, PIECES_COLORS[defending_piece.m_piece]);
                    uint8_t dir = cBoard::eval_dir(defending_piece.m_xpos, defending_piece.m_ypos, dst_x, dst_y);

                    if(pindir != mNO_DIR && pindir != dir){
                        continue;
                    }

                    if(defending_piece.m_piece == mWPW){
                        add_wpw_moves(moves, defending_piece.m_xpos, defending_piece.m_ypos, dst_x, dst_y, mBLK);
                    }
                    else if(defending_piece.m_piece == mBPW){
                        add_bpw_moves(moves, defending_piece.m_xpos, defending_piece.m_ypos, dst_x, dst_y, mBLK);
                    }
                    else{
                        moves.push_back(cMove(defending_piece.m_xpos, defending_piece.m_ypos, dst_x, dst_y, defending_piece.m_piece, mBLK, mBLK, 45));
                    }
                }

                defending_pieces.clear();

                dst_x += step_x;
                dst_y += step_y;
            }
        }

    }


    bool cMatch::sortByPrio(const cMove &a, const cMove &b){
        return a.m_prio < b.m_prio;
    }


    bool cMatch::sortByRank(const cPiece &a, const cPiece &b){
        return (PIECES_RANKS[a.m_piece] < PIECES_RANKS[b.m_piece]);
    }


    void cMatch::calc_alphabeta(int32_t &rscore, vector<cMove> &rmoves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage){

        vector<cMove> newmoves;

        int32_t newscore;

        u_int8_t count = 0;

        bool maximizing = (next_color() == mWHITE);

        if(maximizing){
            rscore = SCORES[mWKG] * 10;
        }
        else{
            rscore = SCORES[mBKG] * 10;
        }

        vector<cMove> moves;
        gen_moves(moves, next_color());
 
        if(moves.size() == 0){
            rscore = score_terminate(depth);

            return;
        }
        else if(moves.size() == 1 && depth == 1){
            cMove move = moves.back();

            rscore = m_score + score_board(move, depth, stage);

            rmoves.push_back(move);

            return;
        }
        else{
            if(depth == 1){
                if(m_minutes.size() > 0 && m_prev_search.size() > 2){
                    cMove last_move = m_minutes.back();

                    cMove first_search_move = m_prev_search.at(0);

                    if(last_move.compare(first_search_move)){
                        cMove second_search_move = m_prev_search.at(1);

                        for(cMove &move : moves){
                            if(second_search_move.compare(move)){
                                move.m_prio = min(move.m_prio, cMove::PRIO_PRE_CALC);
                                break;
                            }
                        }
                    }
                }
            }

            sort(moves.begin(), moves.end(), sortByPrio);
        }

        if(depth == 1){
            #ifdef DEBUG
              //g_depthcnt[mvidx] = max((uint8_t)g_depthcnt[mvidx], (uint8_t)depth);
              //g_depthcnt[depth] ++;
            #endif

            start_alphabeta_threads(newscore, newmoves, moves, depth, maxdepth, alpha, beta, stage);

            rscore = newscore;

            for(cMove &nmove : newmoves){
                rmoves.push_back(nmove);
            }

            return;
        }

        for(const cMove &move : moves){
            #ifdef DEBUG
              //g_depthcnt[mvidx] = max((uint8_t)g_depthcnt[mvidx], (uint8_t)depth);
              //g_depthcnt[depth]++;
            #endif

            count++;

            newmoves.clear();

            bool dosearch = true;

            if(is_three_times_repetition(move, depth)){
                newscore = 0;

                dosearch = false;
            }
            else if(depth >= maxdepth){
                //continue_search
                if(new_is_search_continue(move, depth, maxdepth, stage) == false){
                    // skip move if it is not worth to search deeper
                    newscore = m_score + score_board(move, depth, stage);

                     dosearch = false;
                }
            }

            if(dosearch){
                if(stage == STAGE_ENDGAME_100 || stage == STAGE_ENDGAME_110 || stage == STAGE_ENDGAME_120 || stage == STAGE_ENDGAME_130){
                    int32_t tmpscore = score_board(move, depth, stage);
                    m_score += tmpscore;
                    do_move(move);

                    calc_alphabeta(newscore, newmoves, depth + 1, maxdepth, alpha, beta, stage);

                    undo_move();
                    m_score -= tmpscore;
                }
                else{
                    do_move(move);

                    calc_alphabeta(newscore, newmoves, depth + 1, maxdepth, alpha, beta, stage);

                    undo_move();
                }
            }

            if(maximizing){
                if(newscore > rscore){
                    rscore = newscore;
                    append_newmove(rmoves, newmoves, move);
                }

                alpha = max(rscore, alpha);

                if(alpha >= beta){
                    break;
                }
            }
            else{
                if(newscore < rscore){
                    rscore = newscore;
                    append_newmove(rmoves, newmoves, move);
                }

                beta = min(rscore, beta);

                if(beta <= alpha){
                    break;
                }
            }
        }

        moves.clear();

    }


    void cMatch::start_alphabeta_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage){

        cThreading threading(this, moves, alpha, beta);

        while(threading.has_finished() == false){

            threading.start(depth, maxdepth, stage);

            sleep(0.6);

            threading.update_candidates();
        }

        threading.update_candidates();

        threading.fetch_candidates(rscore, rmoves);

        moves.clear();

    }


    void cMatch::set_prio_for_move(cMove &move){

        // eval state for move
        cBoard board(m_board);

        board.setfield(move.m_src_x, move.m_src_y, mBLK);
        board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);

        //cField field(&board, move.m_dst_x, move.m_dst_y);
        //int8_t state = field.eval_advant_score();
        int32_t mscore = score_move(move);
        // eval state for move - end

        bool is_good_dstfield = (mscore >= 0 && PIECES_COLORS[move.m_srcpiece] == mWHITE) || (mscore <= 0 && PIECES_COLORS[move.m_srcpiece] == mBLACK);

        // capture
        if(move.m_dstpiece != mBLK){
            if(PIECES_RANKS[move.m_srcpiece] <= PIECES_RANKS[move.m_dstpiece]){ 
                move.m_prio = min(move.m_prio, (uint8_t)cMove::PRIO_CAPTURE);
            }
            else if(PIECES_RANKS[move.m_srcpiece] == PIECES_RANKS[move.m_dstpiece] && is_good_dstfield){
                move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_CAPTURE + 5));
            }
            else if(PIECES_RANKS[move.m_srcpiece] > PIECES_RANKS[move.m_dstpiece] && is_good_dstfield){
                move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_CAPTURE + 10));
            }
            else{
                move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_STANDARD - 3));
            }
        }
        else{
            if(is_good_dstfield){
                if(does_move_touch_vulnerable_piece(move)){
                    move.m_prio = min(move.m_prio, (uint8_t)cMove::PRIO_URGENT_TOUCH);
                }
                else{
                    // attack on King
                    vector<cPiece> wpieces, bpieces;

                    //cBoard board(m_board);
                    //board.setfield(move.m_src_x, move.m_src_y, mBLK);

                    board.search_all_dirs_for_pieces(wpieces, bpieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, mNO_DIR);

                    if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                        for(const cPiece &piece : bpieces){
                            if(piece.m_piece == mBKG){
                                move.m_prio = min(move.m_prio, uint8_t(cMove::PRIO_STANDARD + 5));
                                break;
                            }
                        }
                    }
                    else{
                        for(const cPiece &piece : wpieces){
                            if(piece.m_piece == mWKG){
                                move.m_prio = min(move.m_prio, uint8_t(cMove::PRIO_STANDARD + 5));
                                break;
                            }
                        }
                    }
                }

                if(move.m_srcpiece == mWPW && move.m_dst_y >= 6){
                    move.m_prio = min(move.m_prio, (uint8_t)cMove::PRIO_ADVANCED_PAWN);
                }
                else if(move.m_srcpiece == mBPW && move.m_dst_y <= 1){
                    move.m_prio = min(move.m_prio, (uint8_t)cMove::PRIO_ADVANCED_PAWN);
                }
            }
            else{
                // bad dstfield
                if(move.m_prio == cMove::PRIO_STANDARD){
                    move.m_prio = 120;
                }
            }
        }
        //}

    }


    void cMatch::set_prio_for_castling(cMove &move){

        move.m_prio = cMove::PRIO_CASTLING;

        cBoard board(m_board);

        // eval state for rook move
        uint8_t king = board.getfield(move.m_src_x, move.m_src_y);
        board.setfield(move.m_src_x, move.m_src_y, mBLK);
        board.setfield(move.m_dst_x, move.m_dst_y, king);

        uint8_t rook, rk_src_x, rk_dst_x;
        if(move.m_dst_x == 6){
            rk_src_x = 7;
            rk_dst_x = 5;
            rook = board.getfield(rk_src_x, move.m_src_y);
        }
        else{
            rk_src_x = 0;
            rk_dst_x = 3;
            rook = board.getfield(rk_src_x, move.m_src_y);
        }

        cMove rkmove(rk_src_x, move.m_src_y, rk_dst_x, move.m_src_y, rook, mBLK, mBLK, cMove::PRIO_STANDARD);

        board.setfield(rk_src_x, move.m_src_y, mBLK);
        board.setfield(rk_dst_x, move.m_dst_y, rook);

        //cField field(&board, rk_dst_x, move.m_dst_y);
        //int8_t state = field.eval_advant_score();
         int32_t mscore = score_move(move);
        //int8_t state = board.eval_field_state(rk_dst_x, move.m_dst_y);
        // eval state for rook move - end

        bool is_good_dstfield = (mscore >= 0 && PIECES_COLORS[move.m_srcpiece] == mWHITE) || (mscore <= 0 && PIECES_COLORS[move.m_srcpiece] == mBLACK);

        if(is_good_dstfield){

            if(does_move_touch_vulnerable_piece(rkmove)){
                move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_URGENT_TOUCH - 1));
            }
            else{
                // attack on King
                vector<cPiece> wpieces, bpieces;

                board.search_all_dirs_for_pieces(wpieces, bpieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, mNO_DIR);
                
                if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                    for(const cPiece &piece : bpieces){
                        if(piece.m_piece == mBKG){
                            move.m_prio = min(move.m_prio, uint8_t(cMove::PRIO_CASTLING - 5));
                            break;
                        }
                    }
                }
                else{
                    for(const cPiece &piece : wpieces){
                        if(piece.m_piece == mWKG){
                            move.m_prio = min(move.m_prio, uint8_t(cMove::PRIO_CASTLING - 5));
                            break;
                        }
                    }
                }
            }
        }
        else{
            // bad dstfield
            if(move.m_prio == cMove::PRIO_STANDARD){
                move.m_prio = 120;
            }
        }

    }


    uint8_t cMatch::eval_stage() const{
        // opening
        if(m_minutes.size() <= 30){
            return STAGE_OPENING;
        }
        
        //endgame or middlegame
        uint8_t wpwcnt = 0;
        uint8_t bpwcnt = 0;
        uint8_t wqucnt = 0;
        uint8_t bqucnt = 0;

        vector<cPiece> wofficers, bofficers;

        for(uint8_t y = 0; y < 8; ++y){
            for(uint8_t x = 0; x < 8; ++x){
                uint8_t piece = m_board.getfield(x, y);

                if(piece == mBLK){
                    continue;
                }
                else if(piece == mWPW){
                    wpwcnt++;
                }
                else if(piece == mBPW){
                    bpwcnt++;
                }
                else if(piece == mWKG || piece == mBKG){
                    continue;
                }
                else if(PIECES_COLORS[piece] == mWHITE){
                    if(piece == mWQU){
                        wqucnt++;
                    }
                    else{
                        wofficers.push_back(cPiece(piece, x, y));
                    }
                }
                else{
                    if(piece == mBQU){
                        bqucnt++;
                    }
                    else{
                        bofficers.push_back(cPiece(piece, x, y));
                    }
                }
            }
        }

        uint8_t offcnt = wqucnt + bqucnt + wofficers.size() + bofficers.size();
        if(offcnt > 6){
            cout << to_string(offcnt) << endl;
            return STAGE_MIDDLE;
        }

        if(wpwcnt == 0 && bpwcnt == 0 && wqucnt == 0 && bqucnt == 0){
            if(wofficers.size() == 3 && bofficers.size() == 1){
                uint8_t kncnt = 0;
                uint8_t bpcnt = 0;
                uint8_t squcolor = mBLANK;
                uint8_t elsecnt = 0;

                for(const cPiece &piece : wofficers){
                    if(piece.m_piece == mWKN){
                        kncnt++;
                    }
                    else if(piece.m_piece == mWBP){
                        bpcnt++;

                        if(piece.m_xpos % 2 == piece.m_ypos % 2){
                            squcolor = mBLACK;
                        }
                        else{
                            squcolor = mWHITE;
                        }
                    }
                    else{
                        elsecnt++;
                    }
                }

                if(kncnt == 1 && bpcnt == 1 && elsecnt == 1){
                    if(squcolor == mWHITE){
                        return STAGE_ENDGAME_100;
                    }
                    else{
                        return STAGE_ENDGAME_110;
                    }
                }

                return STAGE_ENDGAME;

            }
            else if(bofficers.size() == 3 && wofficers.size() == 1){
                uint8_t kncnt = 0;
                uint8_t bpcnt = 0;
                uint8_t squcolor = mBLANK;
                uint8_t elsecnt = 0;

                for(const cPiece &piece : bofficers){
                    if(piece.m_piece == mBKN){
                        kncnt++;
                    }
                    else if(piece.m_piece == mBBP){
                        bpcnt++;
    
                        if(piece.m_xpos % 2 == piece.m_ypos % 2){
                            squcolor = mBLACK;
                        }
                        else{
                            squcolor = mWHITE;
                        }
                    }
                    else{
                        elsecnt++;
                    }
                }
                if(kncnt == 1 && bpcnt == 1 && elsecnt == 1){
                    if(squcolor == mWHITE){
                        return STAGE_ENDGAME_120;
                    }
                    else{
                        return STAGE_ENDGAME_130;
                    }
                }

                return STAGE_ENDGAME;

            }
        }

        return STAGE_ENDGAME;

    }


    int32_t cMatch::score_terminate(uint8_t depth) const{

            if(m_board.is_king_attacked(REVERSED_COLORS[next_color()])){
                if(next_color() == mWHITE){
                    return (SCORES[mWKG]) + depth;
                }
                else{
                    return (SCORES[mBKG]) - depth;
                }
            }
            else{
                return 0; // draw
            }

    }


    /*bool cMatch::is_stormy() const{

        // is pawn on last row before promotion
        for(uint8_t x = 0; x < 8; ++x){
            if(m_board.getfield(x, 6) == mWPW){
                return true;
            }
            if(m_board.getfield(x, 1) == mBPW){
                return true;
            }
        }

        // touches on pieces
        for(uint8_t y = 0; y < 8; ++y){
            for(uint8_t x = 0; x < 8; ++x){

                uint8_t piece = m_board.getfield(x, y);

                if(piece != mBLK){
                    cField field(&m_board, x, y);
                    int8_t state = field.eval_advant_score();
                    //int8_t state = m_board.eval_field_state(x, y);

                    if((PIECES_COLORS[piece] == mWHITE && state >= 3) || (PIECES_COLORS[piece] == mBLACK && state <= -3)){
                        return true;
                    }
                }
            }
        }

        return false;

    }*/


    int32_t cMatch::score_move(const cMove &move) const{

        int32_t score = SCORES[move.m_dstpiece];
        int32_t aprev_score = 0;
        int32_t sprev_score = 0;
        
        bool supiswhite;

        vector<cPiece> attpieces, suppieces;
        cBoard board(m_board);
        board.setfield(move.m_src_x, move.m_src_y, mBLK);
        board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);
        
        if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
            board.search_for_all_touching_pieces(suppieces, attpieces, move.m_dst_x, move.m_dst_y);

            supiswhite = true;
        }
        else{
            board.search_for_all_touching_pieces(attpieces, suppieces, move.m_dst_x, move.m_dst_y);

            supiswhite = false;
        }

        if(attpieces.size() == 0){
            return score;
        }

        if(suppieces.size() == 0){
            return score + SCORES[move.m_srcpiece];
        }

        sort(attpieces.begin(), attpieces.end(), sortByRank);
        sort(suppieces.begin(), suppieces.end(), sortByRank);

        bool attacking = true;

        vector <cPiece>::iterator ait = attpieces.begin();
        vector <cPiece>::iterator sit = suppieces.begin();

        while(ait != attpieces.end() || sit != suppieces.end()){
            if(attacking){
                if(ait == attpieces.begin() && sit == suppieces.begin()){
                    score += SCORES[move.m_srcpiece];
                }
                else{
                    if(sit != suppieces.end()){
                        score += SCORES[sit->m_piece];
                        sit++;
                    }
                    else{
                        return score;
                    }

                    if((supiswhite && sprev_score > score) || (supiswhite == false && sprev_score < score)){
                        return sprev_score;
                    }
                    else{
                        sprev_score = score;
                    }
                }
            }
            else{
                if(ait != attpieces.end()){
                    score += SCORES[ait->m_piece];
                    ait++;
                }
                else{
                    return score;
                }

                if((supiswhite == false && aprev_score > score) || (supiswhite && aprev_score < score)){
                    return aprev_score;
                }
                else{
                    aprev_score = score;
                }
            }

            attacking = !attacking;
        }

        return score;

    }


    /*int32_t cMatch::score_move(const cMove &move) const{

        int32_t score = SCORES[move.m_dstpiece];
        int32_t wprev_score = 0;
        int32_t bprev_score = 0;

        vector<cPiece> wpieces, bpieces;
        cBoard board(m_board);
        board.setfield(move.m_src_x, move.m_src_y, mBLK);
        board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);
        board.search_for_all_touching_pieces(wpieces, bpieces, move.m_dst_x, move.m_dst_y);
        
        bool maximizing;
        if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
            maximizing = false; // white has already piece moved
        }
        else{
            maximizing = true;
        }

        sort(wpieces.begin(), wpieces.end(), sortByRank);
        sort(bpieces.begin(), bpieces.end(), sortByRank);

        vector <cPiece>::iterator wit = wpieces.begin();
        vector <cPiece>::iterator bit = bpieces.begin();

        bool start = true;

        while(wit != wpieces.end() || bit != bpieces.end()){
            if(maximizing){
                wprev_score = score;

                if(start){
                    start = false;
                    score += SCORES[move.m_srcpiece];
                }
                else{
                    if(bit != bpieces.end()){
                        score += SCORES[bit->m_piece];
                        bit++;
                    }
                }

                if(bprev_score < score){
                    return bprev_score;
                }
            }
            else{
                bprev_score = score;

                if(start){
                    start = false;
                    score += SCORES[move.m_srcpiece];
                }
                else{
                    if(wit != wpieces.end()){
                        score += SCORES[wit->m_piece];
                        wit++;
                    }
                }

                if(wprev_score > score){
                    return wprev_score;
                }                
            }

            maximizing = !maximizing;
        }
        
        return score;

    }*/

    /*int8_t cMatch::score_move(const cMove &move) const{
        int8_t score = 0;
        //int8_t sumsup = 0;
        int8_t sumatt = 0;
        uint8_t suptouchcnt = 0;
        uint8_t atttouchcnt = 0;
        uint8_t supsoftpincnt = 0;
        uint8_t attsoftpincnt = 0;
        uint8_t lowestatt, lowestsup;
        int8_t adjust;

        vector<cPiece> supporters, attackers;
        if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
            m_board.search_for_all_touching_pieces(supporters, attackers, move.m_dst_x, move.m_dst_y);

            //lowestsup = mWKG;
            //lowestatt = mBKG;
            adjust = 1;
        }
        else{
            m_board.search_for_all_touching_pieces(attackers, supporters, move.m_dst_x, move.m_dst_y);

            //lowestsup = mBKG;
            //lowestatt = mWKG;
            adjust = -1;
        }

        sort(supporters.begin(), supporters.end(), sortByRank);

        for(const cPiece &piece : supporters){
            uint8_t state = m_board.eval_pin_state(piece.m_piece, piece.m_src_x, piece.m_src_y);

            if(state == cBoard::PINNED_NO){
                suptouchcnt++;
            }
            else if(state == cBoard::PINNED_SOFT){
                supsoftpincnt++;
                sumsup += PIECES_RANKS[piece.m_piece];
            }
            else{
                sumsup += PIECES_RANKS[piece.m_piece];
                continue;
            }

            if(PIECES_RANKS[piece.m_piece] < PIECES_RANKS[lowestsup]){
                lowestsup = piece.m_piece;
            }
        }

        sort(attackers.begin(), attackers.end(), sortByRank);

        for(const cPiece &piece : attackers){
            uint8_t state = m_board.eval_pin_state(piece.m_piece, piece.m_src_x, piece.m_src_y);

            if(state == cBoard::PINNED_NO){
                atttouchcnt++;
            }
            else if(state == cBoard::PINNED_SOFT){
                attsoftpincnt++;
                sumatt += PIECES_RANKS[piece.m_piece];
            }
            else{
                sumatt += PIECES_RANKS[piece.m_piece];
                continue;
            }

            if(PIECES_RANKS[piece.m_piece] < PIECES_RANKS[lowestatt]){
                lowestatt = piece.m_piece;
            }
        }


        //if(supporters.size() == 0){
        //    lowestsup = mBLK;
        //}

        //if(attackers.size() == 0){
        //    lowestatt = mBLK;
        //}


        score += PIECES_RANKS[move.m_dstpiece];

        if(supporters.size() == 0 && attackers.size() == 0){
            // do nothing
        }
        else if(supporters.size() == 0 && attackers.size() > 0){
            score -= PIECES_RANKS[move.m_srcpiece];
        }
        else if(supporters.size() > 0 && attackers.size() == 0){
            // do nothing
        }
        else if(PIECES_RANKS[attackers.at(0).m_piece] > PIECES_RANKS[move.m_srcpiece] || PIECES_RANKS[attackers.at(0).m_piece] > PIECES_RANKS[supporters.at(0).m_piece]){
            return (score * adjust);
        }
        else if(supporters.size() < attackers.size()){
            score -= PIECES_RANKS[move.m_srcpiece];
            
            int8_t sum = 0;
            for(uint8_t i = 0; i < supporters.size(); ++i){
                cPiece supporter = supporters.at(i);
                cPiece attacker = attackers.at(i);

                if(PIECES_RANKS[supporter.m_piece] < PIECES_RANKS[attacker.m_piece]){
                    sum += PIECES_RANKS[attacker.m_piece];
                    sum -= PIECES_RANKS[supporter.m_piece];
                }
                else{
                    break;
                }
            }

            score += sum;

            if(PIECES_RANKS[lowestatt] <= PIECES_RANKS[lowestsup]){
                score -= 2;
            }
            else{
                score -= 1;
            }
        }
        else if(supporters.size() == attackers.size()){
            score -= PIECES_RANKS[move.m_srcpiece];

            int8_t sum = 0;
            for(uint8_t i = 0; i < supporters.size(); ++i){
                cPiece supporter = supporters.at(i);
                cPiece attacker = attackers.at(i);

                if(PIECES_RANKS[supporter.m_piece] < PIECES_RANKS[attacker.m_piece]){
                    sum += PIECES_RANKS[attacker.m_piece];
                    sum -= PIECES_RANKS[supporter.m_piece];
                }
                else{
                    break;
                }
            }

            score += sum;

            //score += (PIECES_RANKS[lowestatt] - PIECES_RANKS[lowestsup]);

            if(PIECES_RANKS[lowestatt] < PIECES_RANKS[lowestsup]){
                score -= 1;
            }
            else{
                // do nothing;
            }
        }
        else if(supporters.size() > attackers.size()){
            score -= PIECES_RANKS[move.m_srcpiece];

            int8_t sum = 0;
            for(uint8_t i = 0; i < attackers.size(); ++i){
                cPiece supporter = supporters.at(i);
                cPiece attacker = attackers.at(i);

                if(PIECES_RANKS[supporter.m_piece] < PIECES_RANKS[attacker.m_piece]){
                    sum += PIECES_RANKS[attacker.m_piece];
                    sum -= PIECES_RANKS[supporter.m_piece];
                }
                else{
                    break;
                }
            }

            score += sum;

            //score += PIECES_RANKS[lowestatt];

            if(PIECES_RANKS[lowestatt] < PIECES_RANKS[lowestsup]){
                score += 1;
            }
            else{
                score += 2;
            }
        }

        return (score * adjust);

    }*/


    int32_t cMatch::score_board(const cMove &move, const uint8_t depth, const uint8_t stage) const{

        int32_t score = 0;

        if(stage == STAGE_ENDGAME_100 || stage == STAGE_ENDGAME_110){
            // white kn+bp
            score += cEndGame100_base::eval_push_kg_to_margin(stage, depth, m_board.m_fields, m_board.m_wKg_x, m_board.m_wKg_y, m_board.m_bKg_x, m_board.m_bKg_y);

            if(m_board.is_within_three_squares(mBKN, m_board.m_wKg_x, m_board.m_wKg_y)){
                score += 4;
            }

            return score;

        }
        else if(stage == STAGE_ENDGAME_120 || stage == STAGE_ENDGAME_130){
            // black kn+bp
            score += cEndGame100_base::eval_push_kg_to_margin(stage, depth, m_board.m_fields, m_board.m_wKg_x, m_board.m_wKg_y, m_board.m_bKg_x, m_board.m_bKg_y);

            if(m_board.is_within_three_squares(mBKN, m_board.m_bKg_x, m_board.m_bKg_y)){
                score -= 4;
            }

            return score;

        }   


        // opening
        if(stage == STAGE_OPENING){

            // penalty for move repetition
            if(m_minutes.size() >= 2){
                uint8_t prev_prev_pos = m_minutes.size() - 2;
                cMove pre_prev_move = m_minutes.at(prev_prev_pos);

                if(move.m_dst_x == pre_prev_move.m_src_x && move.m_dst_y ==  pre_prev_move.m_src_y && move.m_src_x == pre_prev_move.m_dst_x &&  move.m_src_y == pre_prev_move.m_dst_y){
                    if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                        score += SCORES[mBPLUS4];
                    }
                    else{
                        score += SCORES[mWPLUS4];
                    }
                }
            }
            // end -- penalty for move repetition


            // penalty for inactive white pieces
            cPiece white_pieces[4] = { 
                cPiece(mWKN, 1, 0), 
                cPiece(mWKN, 6, 0), 
                cPiece(mWBP, 2, 0), 
                cPiece(mWBP, 5, 0) };
            uint8_t wcnt = 0;
            for(const cPiece &wpiece : white_pieces){
                if(m_board.getfield(wpiece.m_xpos, wpiece.m_ypos) == wpiece.m_piece){
                    wcnt++;
                }
            }
            if(wcnt > 1){
                score += SCORES[mBPLUS1] * wcnt; // penalty
            }
            // end -- penalty for inactive white pieces


            // penalty for inactive black pieces
            cPiece black_pieces[4] = { 
                cPiece(mBKN, 1, 7), 
                cPiece(mBKN, 6, 7), 
                cPiece(mBBP, 2, 7), 
                cPiece(mBBP, 5, 7) };
            uint8_t bcnt = 0;
            for(const cPiece &bpiece : black_pieces){
                if(m_board.getfield(bpiece.m_xpos, bpiece.m_ypos) == bpiece.m_piece){
                    bcnt++;
                }
            }
            if(bcnt > 1){
                score += SCORES[mWPLUS1] * bcnt; // penalty
            }
            // end -- penalty for inactive black pieces


            // penalty for inactive white pawns
            wcnt = 0;
            for(uint8_t x = 0; x < 8; ++x){
                if(m_board.getfield(x, 1) == mWPW){
                    wcnt++;
                }
            }
            if(wcnt >= 6){
                score += SCORES[mBPLUS1]; // penalty
            }
            // end -- penalty for inactive white pawns


            // penalty for inactive black pawns
            bcnt = 0;
            for(uint8_t x = 0; x < 8; ++x){
                if(m_board.getfield(x, 6) == mBPW){
                    bcnt++;
                }
            }
            if(bcnt >= 6){
                score += SCORES[mWPLUS1]; // penalty
            }
            // end -- penalty for inactive black pawns


            // pawns on central fields
            vector<cPiece> wpieces;
            vector<cPiece> bpieces;
            for(uint8_t y = 3; y <= 4; ++y){
                for(uint8_t x = 3; x <= 4; ++x){
                    m_board.search_for_touching_pieces(wpieces, x, y, mWHITE, true);

                    m_board.search_for_touching_pieces(bpieces, x, y, mBLACK, true);
                }
            }
            for(cPiece &wpiece : wpieces){
                if(wpiece.m_piece == mWPW){
                    score += SCORES[mWPLUS1];
                    if(wpiece.m_ypos == 3){
                        score += SCORES[mWPLUS1]; // addition plus for 4th rank
                    }
                }
            }
            for(cPiece &bpiece : bpieces){
                if(bpiece.m_piece == mBPW){
                    score += SCORES[mBPLUS1];
                    if(bpiece.m_ypos == 4){
                        score += SCORES[mBPLUS1]; // addition plus for 5th rank
                    }
                }
            }
            // end -- pawns on central fields


            // penalty for f2 / f7 moved pawns
            if(m_board.getfield(5, 1) != mWPW){
                score += SCORES[mBPLUS3]; // penalty
            }
            else if(m_board.getfield(5, 6) != mBPW){
                score += SCORES[mWPLUS3]; // penalty
            }
            // end -- penalty for f2 / f7 moved pawns

        }


        // opening and middlegame

        // passed pawns
        for(uint8_t y = 4; y < 7; ++y){
            for(uint8_t x = 0; x < 8; ++x){
                uint8_t pawn = m_board.getfield(x, y);
                if(pawn == mWPW){
                    if(m_board.is_passed_pawn(pawn, x, y)){
                        score += SCORES[mWPLUS1] * y;
                    }
                }
            }
        }

        for(uint8_t y = 3; y > 0; --y){
            for(uint8_t x = 0; x < 8; ++x){
                uint8_t pawn = m_board.getfield(x, y);
                if(pawn == mBPW){
                    if(m_board.is_passed_pawn(pawn, x, y)){
                        score += SCORES[mBPLUS1] * (7 - y);
                    }
                }
            }
        }
        // end -- passed pawns


        // weak wking and bking
        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        cBoard board(m_board);

        // penalty for weak wking
        bool weak_wk = false;

        board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);

        for(uint8_t i = 0; i < size(steps); ++i){
            vector <cPiece> wpieces, bpieces;

            if(cBoard::is_inbounds(board.m_wKg_x + steps[i][0], board.m_wKg_y + steps[i][1])){
                board.search_for_all_touching_pieces(wpieces, bpieces, board.m_wKg_x + steps[i][0], board.m_wKg_y + steps[i][1]);

                if(wpieces.size() < bpieces.size()){
                    weak_wk = true;
                    break;
                }
            }
        }

        if(weak_wk == false){
            // search north dir for brook or bqueen
            vector<cPiece> pieces;
            m_board.search_dir_for_pieces(pieces, m_board.m_wKg_x, m_board.m_wKg_y, 0, 1);

            if(pieces.size() == 2){
                cPiece second = pieces.at(1);
                if(second.m_piece == mBRK || second.m_piece == mBQU){
                    weak_wk = true;
                }
            }
            else if(pieces.size() > 2){
                cPiece first = pieces.at(0);
                cPiece second = pieces.at(1);
                cPiece third = pieces.at(2);
                if(second.m_piece == mBRK || second.m_piece == mBQU){
                    weak_wk = true;
                }
                else if((third.m_piece == mBRK || third.m_piece == mBQU)){
                    if((first.m_piece != mWPW && first.m_piece != mBPW) || (second.m_piece != mWPW && second.m_piece != mBPW)){
                        weak_wk = true;
                    }
                }
            }
        }

        if(weak_wk){
            score += SCORES[mBPLUS3]; // penalty
        }

        board.setfield(board.m_wKg_x, board.m_wKg_y, mWKG);
        // end -- penalty for weak wking


        //penalty for weak black king
        bool weak_bk = false;

        board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);

        for(uint8_t i = 0; i < size(steps); ++i){
            vector <cPiece> wpieces, bpieces;

            if(cBoard::is_inbounds(board.m_bKg_x + steps[i][0], board.m_bKg_y + steps[i][1])){
                board.search_for_all_touching_pieces(wpieces, bpieces, board.m_bKg_x + steps[i][0], board.m_bKg_y + steps[i][1]);

                if(bpieces.size() < wpieces.size()){
                    weak_bk = true;
                    break;
                }
            }
        }

        if(weak_bk == false){
            // search south dir for wrook or wqueen
            vector<cPiece> bpieces;
            m_board.search_dir_for_pieces(bpieces, m_board.m_bKg_x, m_board.m_bKg_y, 0, -1);

            if(bpieces.size() == 2){
                cPiece second = bpieces.at(1);
                if(second.m_piece == mWRK || second.m_piece == mWQU){
                    weak_bk = true;
                }
            }
            else if(bpieces.size() > 2){
                cPiece first = bpieces.at(0);
                cPiece second = bpieces.at(1);
                cPiece third = bpieces.at(2);
                if(second.m_piece == mWRK || second.m_piece == mWQU){
                    weak_bk = true;
                }
                else if((third.m_piece == mWRK || third.m_piece == mWQU)){
                    if((first.m_piece != mBPW && first.m_piece != mWPW) || (second.m_piece != mBPW && second.m_piece != mWPW)){
                        weak_bk = true;
                    }
                }
            }
        }
        
        if(weak_bk){
            score += SCORES[mWPLUS3]; // penalty
        }

        board.setfield(board.m_bKg_x, board.m_bKg_y, mBKG);
        // end -- penalty for weak bking


        // wrook on open file
        for(uint8_t y = 0; y < 2; ++y){
            for(uint8_t x = 0; x < 8; ++x){
                if(m_board.getfield(x, y) == mWRK){
                    bool isopen = true;

                    vector<cPiece> pieces; 
                    m_board.search_dir_for_pieces(pieces, x, y, 0, 1);

                    for(const cPiece &piece : pieces){
                        if(piece.m_piece == mWPW){
                            isopen = false;
                            break;
                        }
                    }

                    if(isopen){
                        vector<cPiece> wpieces, bpieces; 
                        m_board.search_for_all_touching_pieces(wpieces, bpieces, x, y);

                        if(wpieces.size() >= bpieces.size()){
                            score += SCORES[mWPLUS2];
                            break; // score only once
                        }
                    }

                }
            }
        }
        // end -- wrook on open file


        // brook on open file
        for(uint8_t y = 7; y < 6; --y){
            for(uint8_t x = 0; x < 8; ++x){
                if(m_board.getfield(x, y) == mBRK){
                    bool isopen = true;

                    vector<cPiece> pieces; 
                    m_board.search_dir_for_pieces(pieces, x, y, 0, -1);

                    for(const cPiece &piece : pieces){
                        if(piece.m_piece == mBPW){
                            isopen = false;
                            break;
                        }
                    }

                    if(isopen){
                        vector<cPiece> wpieces, bpieces; 
                        m_board.search_for_all_touching_pieces(wpieces, bpieces, x, y);

                        if(bpieces.size() >= wpieces.size()){
                            score += SCORES[mBPLUS2];
                            break; // score only once
                        }
                    }

                }
            }
        }
        // end -- brook on open file


        // wrook or wqueen on 7th rank
        for(uint8_t x = 0; x < 8; ++x){
            uint8_t piece = m_board.getfield(x, 6);
            if(piece == mWRK || piece == mWQU){
                vector<cPiece> wpieces, bpieces; 
                m_board.search_for_all_touching_pieces(wpieces, bpieces, x, 6);

                if(wpieces.size() >= bpieces.size()){
                    score += SCORES[mWPLUS2];
                    break; // score only once
                }
            }
        }
        // end -- wrook or wqueen on 7th rank


        // brook or bquen on second rank
        for(uint8_t x = 0; x < 8; ++x){
            uint8_t piece = m_board.getfield(x, 1);
            if(piece == mBRK || piece == mBQU){
                vector<cPiece> wpieces, bpieces; 
                m_board.search_for_all_touching_pieces(wpieces, bpieces, x, 1);

                if(bpieces.size() >= wpieces.size()){
                    score += SCORES[mBPLUS2];
                    break; // score only once
                }
            }
        }
        // end -- brook or bqueen on second rank


        // attacks and supports
        vector<cPiece> wpieces, bpieces;

        board.setfield(move.m_src_x, move.m_src_y, mBLK);
        board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);

        board.search_all_dirs_for_pieces(wpieces, bpieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, mNO_DIR);

        if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
            score += (wpieces.size() + bpieces.size());
        }
        else{
            score -= (wpieces.size() + bpieces.size());
        }

        //board.setfield(move.m_src_x, move.m_src_y, move.m_srcpiece);
        //board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);
        // end -- attacks and supports


        return score;

    }

