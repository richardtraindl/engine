

    #include "./match.hpp"
    #include "./threading.hpp"


    /*uint32_t g_depthcnt[30] = { 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
    };

    uint32_t g_filterdetector[20] = { 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };*/

    cMatch::cMatch(){ 

        cBoard m_board;

    }


    // copy constructor
    cMatch::cMatch(const cMatch &match){

        m_score = match.m_score;

        for(int y = 0; y < 8; ++y){
            for(int x = 0; x < 8; ++x){
                m_board.m_fields[y][x] = match.m_board.m_fields[y][x];
            }
        }

        m_board.m_wKg_x = match.m_board.m_wKg_x;
        m_board.m_wKg_y = match.m_board.m_wKg_y;
        m_board.m_bKg_x = match.m_board.m_bKg_x;
        m_board.m_bKg_y = match.m_board.m_bKg_y;
        m_board.m_wKg_has_moved_at = match.m_board.m_wKg_has_moved_at;
        m_board.m_bKg_has_moved_at = match.m_board.m_bKg_has_moved_at;
        m_board.m_wRkA_has_moved_at = match.m_board.m_wRkA_has_moved_at;
        m_board.m_wRkH_has_moved_at = match.m_board.m_wRkH_has_moved_at;
        m_board.m_bRkA_has_moved_at = match.m_board.m_bRkA_has_moved_at;
        m_board.m_bRkH_has_moved_at = match.m_board.m_bRkH_has_moved_at;

        for(cMove move : match.m_minutes){
            m_minutes.push_back(move);
        }

    }


    cMatch::~cMatch(){
    }


    void cMatch::reset(){
        m_score = 0;

        m_minutes.clear();
        
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


    uint8_t cMatch::next_color(){

        if(m_minutes.size() % 2 == 0){
            return mWHITE;
        }
        else{
            return mBLACK;
        }

    }


    void cMatch::prnt_minutes(){

        for(cMove move : m_minutes){
            cout << move.format(false) << endl;
        }

    }


    string cMatch::fmt_moves(vector<cMove> &moves){

        string str_moves = "";

        for(cMove move : moves){
            str_moves += " [" + move.format(false) + "] ";
        }
        return str_moves;

    }


    void cMatch::prnt_moves(vector<cMove> &moves){

        uint8_t idx = 1;

        cout << "-------------------------------------------" << endl;
        for(cMove move : moves){
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


    void cMatch::calc_move(int32_t &rscore, vector<cMove> &rmoves, uint8_t maxdepth){

        time_t time_start = time(0);

        int32_t alpha = SCORES[mWKG] * 10;

        int32_t beta = SCORES[mBKG] * 10;

        uint8_t depth = 1;
        
        uint8_t status;

        if(is_endgame(status)){
            cout << "Endgame - status: " << to_string(status) << endl;

            calc_alphabeta_endgame(rscore, rmoves, depth, maxdepth, alpha, beta, status);
        }
        else{
            cout << "Opening or Middlegame - status: " << to_string(status) << endl;
            calc_alphabeta(rscore, rmoves, depth, maxdepth, alpha, beta);
        }

        m_prev_search.clear();

        for(cMove move : rmoves){
            m_prev_search.push_back(move);
        }

        cout << "\nmatch m_score: " << m_score;

        cout << "\nresult: " << rscore;

        cout << "\n" << cMatch::fmt_moves(rmoves) << endl;

        prnt_fmttime("\ncalc-time: ", time(0) - time_start);

    }


    bool cMatch::is_king_attacked(uint8_t attacker_color){

        if(attacker_color == mWHITE){
            return m_board.search_for_touching_piece(m_board.m_bKg_x, m_board.m_bKg_y, mWHITE);
        }
        else{
            return m_board.search_for_touching_piece(m_board.m_wKg_x, m_board.m_wKg_y, mBLACK);
        }

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
            gen_kg_supporting_moves(moves, kg_x, kg_y, piece.m_piece, piece.m_src_x, piece.m_src_y, color);
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
                pieces.clear();
                return false;
            }
        }

        bool found = false;
        
        //cout << "\ngen. züge: ";
        //for(cMove move : moves){
        //    cout << move.format() << endl;
        //}

        for(cMove move : moves){
            if(move.m_src_x == src_x && move.m_src_y == src_y &&
            move.m_dst_x == dst_x && move.m_dst_y == dst_y){
                found = true;
                break;
            }
        }
        pieces.clear();

        return found;

    }


    uint8_t cMatch::eval_status(){

        vector<cMove> moves;
        gen_moves(moves, next_color());

        uint8_t moves_size = moves.size();

        moves.clear();

        if(moves_size > 0){
            return STATUS_OPEN;
        }
        else{
            if(next_color() == mWHITE){
                if(is_king_attacked(mBLACK)){
                    return STATUS_WINNER_BLACK;
                }
            }
            else{
                if(is_king_attacked(mWHITE)){
                    return STATUS_WINNER_WHITE;
                }
            }
        }

        return STATUS_DRAW;

    }


    void cMatch::do_usr_move(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece){

        uint8_t srcpiece = m_board.getfield(src_x, src_y);

        uint8_t dstpiece = m_board.getfield(dst_x, dst_y);
        
        cMove move(src_x, src_y, dst_x, dst_y, srcpiece, dstpiece, prompiece, cMove::PRIO_STANDARD);

        do_move(move);

    }


    void cMatch::do_move(cMove &move){

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


    bool cMatch::is_three_times_repetition(cMove &move, uint8_t depth){

        cMatch *match = new cMatch(*this);

        match->do_move(move);

        uint8_t fields[8][8];

        match->m_board.copy_fields(fields);

        uint8_t equalcnt = 0;

        uint8_t maxcnt = min((uint8_t)(8 + depth), (uint8_t)match->m_minutes.size()); // 8

        for(uint8_t i = 0; i < maxcnt; ++i){
            //match->undo_move();
            match->undo_move();

            if(match->m_board.compare_fields(fields)){
                equalcnt++;
            }
        }

        delete match;

        return equalcnt >= 2;

    }


    // 100 Züge davor kein Bauernzug und keine Figur geschlagen
    bool cMatch::is_fifty_moves_rule(){

        if(m_minutes.size() < 100){
            return false;
        }

        int rulecnt = 0;

        int cnt = 0;

        cMatch *match = new cMatch(*this);

        for(vector<cMove>::reverse_iterator it = match->m_minutes.rbegin(); it != match->m_minutes.rend(); ++it){
            cnt++;

            if(cnt > 101){
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
    bool cMatch::does_move_touch_weak_piece(cMove &move){

        vector<cPiece> bpieces, wpieces;

        // supports
        vector<cPiece> supported_pieces;

        uint8_t color = PIECES_COLORS[move.m_srcpiece];

        uint8_t excl_dir = cBoard::eval_dir(move.m_src_x, move.m_src_y, move.m_dst_x, move.m_dst_y);

        // start search
        m_board.setfield(move.m_src_x, move.m_src_y, mBLK);

        m_board.search_for_touched_pieces(supported_pieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, color, excl_dir);

        m_board.setfield(move.m_src_x, move.m_src_y, move.m_srcpiece);
        // end search

        for(cPiece supp_piece : supported_pieces){
            if(m_board.is_piece_soft_pinned(supp_piece.m_piece, supp_piece.m_src_x, supp_piece.m_src_y)){
                supported_pieces.clear();
                return true;
            }

            m_board.search_for_all_touching_pieces(wpieces, bpieces, supp_piece.m_src_x, supp_piece.m_src_y);

            if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                if(wpieces.size() < bpieces.size()){
                    supported_pieces.clear();
                    wpieces.clear();
                    bpieces.clear();
                    return true;
                }
            }
            else{
                if(bpieces.size() < wpieces.size()){
                    supported_pieces.clear();
                    wpieces.clear();
                    bpieces.clear();
                    return true;
                }
            }
            wpieces.clear();
            bpieces.clear();
        }

        supported_pieces.clear();

        // attacks
        vector<cPiece> attacked_pieces;

        // start search
        m_board.setfield(move.m_src_x, move.m_src_y, mBLK);

        m_board.search_for_touched_pieces(attacked_pieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, REVERSED_COLORS[color], excl_dir);

        m_board.setfield(move.m_src_x, move.m_src_y, move.m_srcpiece);
        // end search

        for(cPiece att_piece : attacked_pieces){
            if(m_board.is_piece_soft_pinned(att_piece.m_piece, att_piece.m_src_x, att_piece.m_src_y)){
                attacked_pieces.clear();
                return true;
            }

            m_board.search_for_all_touching_pieces(wpieces, bpieces, att_piece.m_src_x, att_piece.m_src_y);

            if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                if(wpieces.size() == bpieces.size()){
                    attacked_pieces.clear();
                    wpieces.clear();
                    bpieces.clear();
                    return true;
                }
            }
            else{
                if(bpieces.size() == wpieces.size()){
                    attacked_pieces.clear();
                    wpieces.clear();
                    bpieces.clear();
                    return true;
                }
            }
            wpieces.clear();
            bpieces.clear();
        }

        attacked_pieces.clear();

        return false;

    }


    bool cMatch::is_good_field_for_piece(cPiece cpiece, uint8_t dst_x, uint8_t dst_y){

        vector<cPiece> wpieces, bpieces;

        m_board.setfield(cpiece.m_src_x, cpiece.m_src_y, mBLK);

        m_board.search_for_all_touching_pieces(wpieces, bpieces, dst_x, dst_y);

        m_board.setfield(cpiece.m_src_x, cpiece.m_src_y, cpiece.m_piece);

        if(PIECES_COLORS[cpiece.m_piece] == mWHITE){

            if(wpieces.size() < bpieces.size()){
                wpieces.clear();
                bpieces.clear();
                return false;
            }
            
            for(cPiece bpiece : bpieces){
                if(PIECES_RANKS[cpiece.m_piece] > PIECES_RANKS[bpiece.m_piece]){
                    wpieces.clear();
                    bpieces.clear();
                    return false;
                }
            }

        }
        else{
            if(bpieces.size() < wpieces.size()){
                wpieces.clear();
                bpieces.clear();
                return false;
            }

            for(cPiece wpiece : wpieces){
                if(PIECES_RANKS[cpiece.m_piece] > PIECES_RANKS[wpiece.m_piece]){
                    wpieces.clear();
                    bpieces.clear();
                    return false;
                }
            }
        }

        wpieces.clear();
        bpieces.clear();
        return true;

    }


    bool cMatch::filter(cMove &move, uint8_t depth, uint8_t maxdepth){
        
        if(depth > 12){ // && move.m_dstpiece == mBLK && move.is_en_passant() == false){
            return false;
        }

        // take en passant moves
        if(move.is_en_passant()){ 
            //g_filterdetector[1]++;
            return true; 
        }

        // take promotion moves
        if(move.m_prompiece != mBLK){ 
            //g_filterdetector[2]++;
            return true; 
        }

        // take good capture moves
        if(move.m_dstpiece != mBLK){ 
            // piece must be lower than captured piece
            if(PIECES_RANKS[move.m_srcpiece] < PIECES_RANKS[move.m_dstpiece]){
                //g_filterdetector[3]++;
                return true;
            }

            vector<cPiece> supporting_pieces;
            m_board.search_for_touching_pieces(supporting_pieces, move.m_dst_x, move.m_dst_y, PIECES_COLORS[move.m_dstpiece], true);

            // move is good because captured piece is not supported
            if(supporting_pieces.size() == 0){
                //g_filterdetector[4]++;
                return true; 
            }

            vector<cPiece> attacking_pieces;
            m_board.search_for_touching_pieces(attacking_pieces, move.m_dst_x, move.m_dst_y, PIECES_COLORS[move.m_srcpiece], true);
            
            // move is good because captured piece is more attacked than supported 
            if(attacking_pieces.size() > supporting_pieces.size()){
                //g_filterdetector[5]++;
                return true; 
            }

            // search for lowest supporter
            uint8_t min_supp = PIECES_RANKS[mWKG];
            for(cPiece supp_piece : supporting_pieces){
                if(PIECES_RANKS[supp_piece.m_piece] < min_supp){
                    min_supp = PIECES_RANKS[supp_piece.m_piece];
                }
            }
            // if lowest attacker is less than lowest supporter, move is good
            bool has_lower_attacker = false;
            for(cPiece att_piece : attacking_pieces){
                if(PIECES_RANKS[att_piece.m_piece] < min_supp){
                    has_lower_attacker = true;
                    break;
                }
            }

            supporting_pieces.clear();

            attacking_pieces.clear();

            if(has_lower_attacker){
                //g_filterdetector[6]++;
                return true;
            }
        }

        //if(depth > 6){ return false; } // 8

        if(move.m_srcpiece == mWPW && move.m_dst_y >= 6){
            //g_filterdetector[7]++;
            return true;
        }

        if(move.m_srcpiece == mBPW && move.m_dst_y <= 1){
            //g_filterdetector[8]++;
            return true;
        }

        //return does_move_touch_weak_piece(move);

        return false; 

    }


    bool cMatch::filter_endgame(cMove &move, uint8_t depth, uint8_t maxdepth, uint8_t status){

        bool prolong = false;

        uint8_t wkg_x, wkg_y, bkg_x, bkg_y; 

        if(move.m_srcpiece == mWKG){
            wkg_x = move.m_dst_x;
            wkg_y = move.m_dst_y;
            bkg_x = m_board.m_bKg_x;
            bkg_y = m_board.m_bKg_y;
        }
        else if(move.m_srcpiece == mBKG){
            wkg_x = m_board.m_wKg_x;
            wkg_y = m_board.m_wKg_y;
            bkg_x = move.m_dst_x;
            bkg_y = move.m_dst_y;
        }
        else{
            wkg_x = m_board.m_wKg_x;
            wkg_y = m_board.m_wKg_y;
            bkg_x = m_board.m_bKg_x;
            bkg_y = m_board.m_bKg_y;
        }

        if(status == cBoard::ENDGAME_STAT_100 || status == cBoard::ENDGAME_STAT_110 || status == cBoard::ENDGAME_STAT_120 || status == cBoard::ENDGAME_STAT_130){

            return depth <= maxdepth + 4;

            if(cBoard::is_margin_frame_ypos(wkg_y, bkg_y)){
                prolong = true;
            }
            else if(cBoard::is_corner_pos(wkg_x, wkg_y, bkg_x, bkg_y)){
                prolong = true;
            }
        }

        if(prolong && depth <= maxdepth + 4){
            return true;
        }
        else if(depth <= maxdepth + 2){
            return true;
        }

        return false;

    }


    void cMatch::append_newmove(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, cMove &move){

        rcandidates.clear();

        rcandidates.push_back(move);

        for(cMove nmove : newcandidates){
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
            gen_kg_supporting_moves(moves, kg_x, kg_y, attacking_piece.m_piece, attacking_piece.m_src_x, attacking_piece.m_src_y, color);
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

            eval_prio(move);

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

            eval_prio(move);

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

                    eval_prio(move);

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

                eval_prio(move);

                moves.push_back(move);

                if(dstpiece != mBLK){
                    break;
                }
            }
        }

    }


    void cMatch::gen_kg_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t king){

        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(uint8_t i = 0; i < size(steps); ++i){

            uint8_t dst_x = x;

            uint8_t dst_y = y;

            if(cBoard::is_inbounds(dst_x + steps[i][0], dst_y + steps[i][1])){

                dst_x += steps[i][0];
                dst_y += steps[i][1];

                uint8_t dstpiece = m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[king]){
                    continue;
                }

                bool is_clear = false;

                if(PIECES_COLORS[king] == mWHITE){
                    m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mBLK);

                    if(m_board.search_for_touching_piece(dst_x, dst_y, mBLACK) == mBLK){
                        is_clear = true;
                    }

                    m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mWKG);
                }
                else{
                    m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBLK);

                    if(m_board.search_for_touching_piece(dst_x, dst_y, mWHITE) == mBLK){
                        is_clear = true;
                    }

                    m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBKG);
                }

                if(is_clear == false){ 
                    continue; 
                }

                cMove move(x, y, dst_x, dst_y, king, dstpiece, mBLK, cMove::PRIO_STANDARD - 1);

                if(dstpiece != mBLK){ 
                    move.m_prio = cMove::PRIO_CAPTURE;
                }
                else if(does_move_touch_weak_piece(move)){
                    move.m_prio = cMove::PRIO_URGENT_TOUCH; 
                }

                moves.push_back(move);
            }

        }

    }


    void cMatch::gen_wkg_castling_moves(vector<cMove> &moves,  uint8_t x, uint8_t y){

        // short white castling
        if(m_board.m_wKg_has_moved_at == 0 && m_board.m_wRkH_has_moved_at == 0 && cBoard::is_inbounds(x + 3, y)){

            if(y == 0 && m_board.getfield(x + 1, y) == mBLK && m_board.getfield(x + 2, y) == mBLK && m_board.getfield(x + 3, y) == mWRK){

                m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mBLK);

                bool is_clear = true;

                for(uint8_t i = 0; i < 3; ++i){
                    if(m_board.search_for_touching_piece(x + i, y, mBLACK) != mBLK){
                        is_clear = false;
                        break;
                    }
                }

                m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mWKG);

                if(is_clear){
                    cMove move(x, y, x + 2, y, mWKG, mBLK, mBLK, cMove::PRIO_CASTLING);

                    if(does_move_touch_weak_piece(move)){
                        move.m_prio = cMove::PRIO_URGENT_TOUCH; 
                    }

                    moves.push_back(move);
                }
            }
        }

        // long white castling
        if(m_board.m_wKg_has_moved_at == 0 && m_board.m_wRkA_has_moved_at == 0 && cBoard::is_inbounds(x - 4, y)){

            if(y == 0 && m_board.getfield(x - 1, y) == mBLK && m_board.getfield(x - 2, y) == mBLK && m_board.getfield(x - 3, y) == mBLK && m_board.getfield(x - 4, y) == mWRK){

                m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mBLK);

                bool is_clear = true;

                for(uint8_t i = 0; i < 3; ++i){
                    if(m_board.search_for_touching_piece(x - i, y, mBLACK) != mBLK){
                        is_clear = false;
                        break;
                    }
                }

                m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mWKG);

                if(is_clear){
                    cMove move(x, y, x - 2, y, mWKG, mBLK, mBLK, cMove::PRIO_CASTLING);

                    if(does_move_touch_weak_piece(move)){
                        move.m_prio = cMove::PRIO_URGENT_TOUCH; 
                    }

                    moves.push_back(move);
                }
            }
        }

    }


    void cMatch::gen_bkg_castling_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        // short black castling
        if(m_board.m_bKg_has_moved_at == 0 && m_board.m_bRkH_has_moved_at == 0 && cBoard::is_inbounds(x + 3, y)){

            if(y == 7 && m_board.getfield(x + 1, y) == mBLK && m_board.getfield(x + 2, y) == mBLK && m_board.getfield(x + 3, y) == mBRK){

                m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBLK);

                bool is_clear = true;

                for(uint8_t i = 0; i < 3; ++i){
                    if(m_board.search_for_touching_piece(x + i, y, mWHITE) != mBLK){
                        is_clear = false;
                        break;
                    }
                }

                m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBKG);

                if(is_clear){
                    cMove move(x, y, x + 2, y, mBKG, mBLK, mBLK, cMove::PRIO_CASTLING);

                    if(does_move_touch_weak_piece(move)){
                        move.m_prio = cMove::PRIO_URGENT_TOUCH; 
                    }

                    moves.push_back(move);
                }
            }
        }

        // long black castling
        if(m_board.m_bKg_has_moved_at == 0 && m_board.m_bRkA_has_moved_at == 0 && cBoard::is_inbounds(x - 4, y)){

            if(y == 7 && m_board.getfield(x - 1, y) == mBLK && m_board.getfield(x - 2, y) == mBLK && m_board.getfield(x - 3, y) == mBLK && m_board.getfield(x - 4, y) == mBRK){

                m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBLK);

                bool is_clear = true;

                for(uint8_t i = 0; i < 3; ++i){
                    if(m_board.search_for_touching_piece(x - i, y, mWHITE) != mBLK){
                        is_clear = false;
                        break;
                    }
                }

                m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBKG);

                if(is_clear){
                    cMove move(x, y, x - 2, y, mBKG, mBLK, mBLK, cMove::PRIO_CASTLING);

                    if(does_move_touch_weak_piece(move)){
                        move.m_prio = cMove::PRIO_URGENT_TOUCH; 
                    }

                    moves.push_back(move);
                }
            }
        }

    }


    void cMatch::gen_kg_supporting_moves(vector<cMove> &moves, uint8_t kg_x, uint8_t kg_y, uint8_t attacking_piece, uint8_t attacking_x, uint8_t attacking_y, uint8_t color){
        
        vector<cPiece> defending_pieces;        

        m_board.search_for_touching_pieces(defending_pieces, attacking_x, attacking_y, color, true);

        for(cPiece defending_piece : defending_pieces){
            if(defending_piece.m_piece == mWKG || defending_piece.m_piece == mBKG){
                continue;
            }

            uint8_t pindir = m_board.eval_pindir(defending_piece.m_src_x, defending_piece.m_src_y, PIECES_COLORS[defending_piece.m_piece]);

            uint8_t dir = cBoard::eval_dir(defending_piece.m_src_x, defending_piece.m_src_y, attacking_x, attacking_y);

            if(pindir != mNO_DIR && pindir != dir){
                continue;
            }

            if(defending_piece.m_piece == mWPW){
                add_wpw_moves(moves, defending_piece.m_src_x, defending_piece.m_src_y, attacking_x, attacking_y, attacking_piece);
            }
            else if(defending_piece.m_piece == mBPW){
                add_bpw_moves(moves, defending_piece.m_src_x, defending_piece.m_src_y, attacking_x, attacking_y, attacking_piece);
            }
            else{
                moves.push_back(cMove(defending_piece.m_src_x, defending_piece.m_src_y, attacking_x, attacking_y, defending_piece.m_piece, attacking_piece, mBLK, 40));
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
                  for(cPiece defending_piece : defending_pieces){
                    if(defending_piece.m_piece == mWKG || defending_piece.m_piece == mBKG){
                        continue;
                    }

                    uint8_t pindir = m_board.eval_pindir(defending_piece.m_src_x, defending_piece.m_src_y, PIECES_COLORS[defending_piece.m_piece]);
                    uint8_t dir = cBoard::eval_dir(defending_piece.m_src_x, defending_piece.m_src_y, dst_x, dst_y);

                    if(pindir != mNO_DIR && pindir != dir){
                        continue;
                    }

                    if(defending_piece.m_piece == mWPW){
                        add_wpw_moves(moves, defending_piece.m_src_x, defending_piece.m_src_y, dst_x, dst_y, mBLK);
                    }
                    else if(defending_piece.m_piece == mBPW){
                        add_bpw_moves(moves, defending_piece.m_src_x, defending_piece.m_src_y, dst_x, dst_y, mBLK);
                    }
                    else{
                        moves.push_back(cMove(defending_piece.m_src_x, defending_piece.m_src_y, dst_x, dst_y, defending_piece.m_piece, mBLK, mBLK, 45));
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


    bool cMatch::sortByEngame_100(const cMove &a, const cMove &b){
        map<uint8_t, uint8_t> PIECES_RANKS_100 = {
            {mBLK, 0}, 
            {mWPW, 6}, 
            {mBPW, 6}, 
            {mWKN, 3}, 
            {mBKN, 3}, 
            {mWBP, 2}, 
            {mBBP, 2}, 
            {mWRK, 4}, 
            {mBRK, 4}, 
            {mWQU, 5}, 
            {mBQU, 5}, 
            {mWKG, 1}, 
            {mBKG, 1}
        };

        return (PIECES_RANKS_100[a.m_srcpiece] < PIECES_RANKS_100[b.m_srcpiece]);
    }


    void cMatch::calc_alphabeta(int32_t &rscore, vector<cMove> &rmoves, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta){

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
            rscore = eval_terminate(depth);
            return;
        }
        else if(moves.size() == 1 && depth == 1){
            cMove move = moves.back();
            rscore = m_score + eval_board(move); // eval_move(move, 0) + 
            rmoves.push_back(move);
            return;
        }
        else{
            if(depth == 1){
                if(m_minutes.size() > 0 && m_prev_search.size() > 2){
                    cMove last_move = m_minutes.back();

                    cMove search_move = m_prev_search.at(1);

                    if(last_move.compare(search_move)){
                        cMove candidate = m_prev_search.at(2);

                        for(cMove &move : moves){
                            if(candidate.compare(move)){
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
            start_alphabeta_threads(newscore, newmoves, moves, depth, maxdepth, alpha, beta);

            rscore = newscore;

            for(cMove nmove : newmoves){
                rmoves.push_back(nmove);
            }

            return;
        }

        for(cMove move : moves){
            // debug
            //g_depthcnt[depth]++;

            count++;

            newmoves.clear();

            bool skip = false;

            if(is_three_times_repetition(move, depth)){

                newscore = 0;

                skip = true;
            }
            else if(depth > maxdepth){
                // skip move if filter says move is not worth to search deeper
                if(filter(move, depth, maxdepth) == false){

                    newscore = m_score + eval_board(move); // eval_move(move, 0) + 

                    skip = true;
                }
            }

            if(skip == false){
                do_move(move);

                calc_alphabeta(newscore, newmoves, depth + 1, maxdepth, alpha, beta);

                undo_move();
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
        
        // debug
        /*if(depth == 1){

            for(uint8_t j = 0; j < 30; ++j){
                cout << to_string(j) << ": " << dec << g_depthcnt[j] << endl;
                g_depthcnt[j] = 0;
            }

            for(uint8_t j = 0; j < 20; ++j){
                cout << to_string(j) << ": " << dec << g_filterdetector[j]  << endl;
                g_filterdetector[j] = 0;
            }
        } */

    }


   void cMatch::calc_alphabeta_endgame(int32_t &rscore, vector<cMove> &rmoves, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta, uint8_t status){

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

            rscore = eval_terminate(depth);

            rmoves.clear();

            return;
        }
        else if(moves.size() == 1 && depth == 1){

            cMove move = moves.back();
            
            do_move(move);

            bool islib;

            rscore = m_score + eval_endgame_board(islib, move, depth, status);

            undo_move();

            rmoves.clear();
            rmoves.push_back(move);

            return;
        }
        else{
            if(depth == 1){
                if(m_minutes.size() > 0 && m_prev_search.size() > 2){
                    cMove last_move = m_minutes.back();

                    cMove search_move = m_prev_search.at(1);

                    if(last_move.compare(search_move)){
                        cMove candidate = m_prev_search.at(2);

                        for(cMove &move : moves){
                            if(candidate.compare(move)){
                                move.m_prio = min(move.m_prio, cMove::PRIO_PRE_CALC);
                                break;
                            }
                        }
                    }
                }
            }

            //if(status >= cBoard::ENDGAME_STAT_100 && status <= cBoard::ENDGAME_STAT_130){
                //sort(moves.begin(), moves.end(), sortByEngame_100);
            //}
            //else{
                sort(moves.begin(), moves.end(), sortByPrio);
            //}
        }

        if(depth == 1){
            start_alphabeta_endgame_threads(newscore, newmoves, moves, depth, maxdepth, alpha, beta, status);

            rscore = newscore;

            for(cMove nmove : newmoves){
                rmoves.push_back(nmove);
            }

            return;
        }

        for(cMove move : moves){
            // debug
            //g_depthcnt[depth]++;

            count++;

            newmoves.clear();

            bool skip = false;

            if(is_three_times_repetition(move, depth)){

                newscore = 0;

                skip = true;
            }
            else if(filter_endgame(move, depth, maxdepth, status) == false){

                do_move(move);

                bool islib;

                newscore = m_score + eval_endgame_board(islib, move, depth, status);

                undo_move();

                skip = true;
            }

            if(skip == false){

                do_move(move);

                calc_alphabeta_endgame(newscore, newmoves, depth + 1, maxdepth, alpha, beta, status);

                undo_move();
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
        
        // debug
        /*if(depth == 1){
            
            for(uint8_t j = 0; j < 30; ++j){
                cout << to_string(j) << ": " << dec << g_depthcnt[j] << endl;
                g_depthcnt[j] = 0;
            }

            for(uint8_t j = 0; j < 10; ++j){
                cout << to_string(j) << ": " << dec << g_filterdetector[j]  << endl;
                g_filterdetector[j] = 0;
            }
        } */

    }


    void cMatch::start_alphabeta_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta){

        cThreading threading(this, moves, alpha, beta);

        while(threading.has_finished() == false){

            threading.start(depth, maxdepth);

            sleep(0.6);

            threading.update_candidates();
        }

        threading.update_candidates();

        threading.fetch_candidates(rscore, rmoves);

        moves.clear();

    }


    void cMatch::start_alphabeta_endgame_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta, uint8_t status){

        cThreading threading(this, moves, alpha, beta);

        while(threading.has_finished() == false){

            threading.start_endgame(depth, maxdepth, status);

            sleep(0.6);

            threading.update_candidates();

        }

        threading.update_candidates();

        threading.fetch_candidates(rscore, rmoves);

        moves.clear();

    }


    void cMatch::eval_prio(cMove &move){

        uint8_t adjust = 0;

        if(m_board.is_piece_soft_pinned(move.m_srcpiece, move.m_src_x, move.m_src_y)){
            adjust = 30;
        }

        cPiece cpiece(move.m_srcpiece, move.m_src_x, move.m_src_y);

        bool is_good_dstfield = is_good_field_for_piece(cpiece, move.m_dst_x, move.m_dst_y);

        if(move.m_dstpiece != mBLK){
            if(PIECES_RANKS[move.m_srcpiece] <= PIECES_RANKS[move.m_dstpiece]){ 
                move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_CAPTURE + adjust));
            }
            else if(PIECES_RANKS[move.m_srcpiece] == PIECES_RANKS[move.m_dstpiece] && is_good_dstfield){
                move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_CAPTURE + adjust + 5));
            }
            else if(PIECES_RANKS[move.m_srcpiece] > PIECES_RANKS[move.m_dstpiece] && is_good_dstfield){
                move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_CAPTURE + adjust + 10));
            }
        }
        //else{
            if(is_good_dstfield){

                if(does_move_touch_weak_piece(move)){
                    move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_URGENT_TOUCH + adjust));
                }
                else{
                    // attack on King
                    vector<cPiece> touched_pieces;

                    uint8_t color = REVERSED_COLORS[PIECES_COLORS[move.m_srcpiece]];

                    m_board.setfield(move.m_src_x, move.m_src_y, mBLK);

                    m_board.search_for_touched_pieces(touched_pieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, color, mNO_DIR);

                    m_board.setfield(move.m_src_x, move.m_src_y, move.m_srcpiece);

                    for(cPiece cpiece : touched_pieces){
                        if(cpiece.m_piece == mWKG || cpiece.m_piece == mBKG){
                            move.m_prio = min(move.m_prio, uint8_t(cMove::PRIO_STANDARD + adjust - 5));
                            break;
                        }
                    }
                }

                if(move.m_srcpiece == mWPW && move.m_dst_y >= 6){
                    move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_RUNNING_PAWN + adjust));
                }
                else if(move.m_srcpiece == mBPW && move.m_dst_y <= 2){
                    move.m_prio = min(move.m_prio, (uint8_t)(cMove::PRIO_RUNNING_PAWN + adjust));
                }
            }
            else{
                // bad dstfield
                if(move.m_prio == 100){
                    move.m_prio = 120;
                }
            }
        //}

    }


    bool cMatch::is_opening(){

        return (m_minutes.size() <= 20);

    }


    bool cMatch::is_endgame(uint8_t &status){

        status = cBoard::ENDGAME_STAT_0;

        uint8_t wpwcnt = 0;

        uint8_t bpwcnt = 0;

        uint8_t wqucnt = 0;

        uint8_t bqucnt = 0;

        vector<cPiece> wofficers;

        vector<cPiece> bofficers;

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
            return false;
        }

        if(wpwcnt == 0 && bpwcnt == 0 && wqucnt == 0 && bqucnt == 0){

            if(wofficers.size() == 3 && bofficers.size() == 1){

                uint8_t kncnt = 0;

                uint8_t bpcnt = 0;

                bool is_wsquare;

                uint8_t elsecnt = 0;

                for(cPiece piece : wofficers){
                    if(piece.m_piece == mWKN){
                        kncnt++;
                    }
                    else if(piece.m_piece == mWBP){
                        bpcnt++;

                        is_wsquare = !(piece.m_src_x % 2 == piece.m_src_y % 2);
                    }
                    else{
                        elsecnt++;
                    }
                }

                if(kncnt == 1 && bpcnt == 1 && elsecnt == 1){
                    if(is_wsquare){
                        status = cBoard::ENDGAME_STAT_100;
                    }
                    else{
                        status = cBoard::ENDGAME_STAT_110;
                    }
                }

                return true;

            }
            else if(bofficers.size() == 3 && wofficers.size() == 1){

                uint8_t kncnt = 0;

                uint8_t bpcnt = 0;

                bool is_wsquare;

                uint8_t elsecnt = 0;

                for(cPiece piece : bofficers){
                    if(piece.m_piece == mBKN){
                        kncnt++;
                    }
                    else if(piece.m_piece == mBBP){
                        bpcnt++;
    
                        is_wsquare = !(piece.m_src_x % 2 == piece.m_src_y % 2);
                    }
                    else{
                        elsecnt++;
                    }
                }
                if(kncnt == 1 && bpcnt == 1 && elsecnt == 1){
                    if(is_wsquare){
                        status = cBoard::ENDGAME_STAT_120;
                    }
                    else{
                        status = cBoard::ENDGAME_STAT_130;
                    }
                }

                return true;

            }
        }

        return true;

    }


    int32_t cMatch::eval_terminate(uint8_t depth){

            if(is_king_attacked(REVERSED_COLORS[next_color()])){
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


    int32_t cMatch::eval_board(cMove &move){

        int32_t score = 0;

        // opening
        if(is_opening()){

            // penalty for move repetition
            if(m_minutes.size() >= 2){
                uint8_t prev_prev_pos = m_minutes.size() - 2;
                cMove pre_prev_move = m_minutes.at(prev_prev_pos);

                if(move.m_dst_x == pre_prev_move.m_src_x && move.m_dst_y ==  pre_prev_move.m_src_y && move.m_src_x == pre_prev_move.m_dst_x &&  move.m_src_y == pre_prev_move.m_dst_y){
                    if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                        score += SCORES[mBPLUS] * 4;
                    }
                    else{
                        score += SCORES[mWPLUS] * 4;
                    }
                }
            }

            // penalty for inactive pieces
            cPiece white_pieces[4] = { cPiece(mWKN, 1, 0), cPiece(mWKN, 6, 0), cPiece(mWBP, 2, 0), cPiece(mWBP, 5, 0) };
            uint8_t wcnt = 0;
            for(cPiece wpiece : white_pieces){
                if(m_board.getfield(wpiece.m_src_x, wpiece.m_src_y) == wpiece.m_piece){
                    wcnt++;
                }
            }
            if(wcnt > 2){
                score += SCORES[mBPLUS] * 2; // penalty
            }

            cPiece black_pieces[4] = { cPiece(mBKN, 1, 7), cPiece(mBKN, 6, 7), cPiece(mBBP, 2, 7), cPiece(mBBP, 5, 7) };
            uint8_t bcnt = 0;
            for(cPiece bpiece : black_pieces){
                if(m_board.getfield(bpiece.m_src_x, bpiece.m_src_y) == bpiece.m_piece){
                    bcnt++;
                }
            }
            if(bcnt > 2){
                score += SCORES[mWPLUS] * 2; // penalty
            }
            
            wcnt = 0;
            for(uint8_t x = 0; x <= 7; ++x){
                if(m_board.getfield(x, 1) == mWPW){
                    wcnt++;
                }
            }
            if(wcnt >= 6){
                score += SCORES[mBPLUS]; // penalty
            }

            bcnt = 0;
            for(uint8_t x = 0; x <= 7; ++x){
                if(m_board.getfield(x, 6) == mBPW){
                    bcnt++;
                }
            }
            if(bcnt >= 6){
                score += SCORES[mWPLUS]; // penalty
            }

            // pawns on central fields
            vector<cPiece> wpieces;
            vector<cPiece> bpieces;
            for(uint8_t y = 3; y <= 4; ++y){
                for(uint8_t x = 3; x <= 4; ++x){
                    m_board.search_for_touching_pieces(wpieces, x, y, mWHITE, true);

                    m_board.search_for_touching_pieces(bpieces, x, y, mBLACK, true);
                }
            }
            for(cPiece wpiece : wpieces){
                if(wpiece.m_piece == mWPW){
                    score += SCORES[mWPLUS];
                    if(wpiece.m_src_y == 3){
                        score += SCORES[mWPLUS]; // addition plus for 4th rank
                    }
                }
            }
            for(cPiece bpiece : bpieces){
                if(bpiece.m_piece == mBPW){
                    score += SCORES[mBPLUS];
                    if(bpiece.m_src_y == 4){
                        score += SCORES[mBPLUS]; // addition plus for 5th rank
                    }
                }
            }
            
            // penalty for f2 / f7 moved pawns
            if(m_board.getfield(5, 1) == mBLK){
                score += SCORES[mBPLUS] * 3; // penalty
            }
            else if(m_board.getfield(5, 6) == mBLK){
                score += SCORES[mWPLUS] * 3; // penalty
            }

        }
        
        // opening and middlegame
        uint8_t status;
        if(is_endgame(status) == false){
            
            int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

            //weak white king?
            for(uint8_t i = 0; i < size(steps); ++i){
                vector <cPiece> wpieces, bpieces;

                m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mBLK);

                if(m_board.is_inbounds(m_board.m_wKg_x + steps[i][0], m_board.m_wKg_y + steps[i][1])){
                    m_board.search_for_all_touching_pieces(wpieces, bpieces, m_board.m_wKg_x + steps[i][0], m_board.m_wKg_y + steps[i][1]);

                    if(wpieces.size() < bpieces.size()){
                        score += SCORES[mBPLUS] * 3; // penalty
                        m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mWKG);
                        break;
                    }
                }
                m_board.setfield(m_board.m_wKg_x, m_board.m_wKg_y, mWKG);
            }

            vector<cPiece> wpieces;
            m_board.search_dir_for_pieces(wpieces, m_board.m_wKg_x, m_board.m_wKg_y, 0, 1);

            if(wpieces.size() == 2){
                cPiece second = wpieces.at(1);
                if(second.m_piece == mBRK || second.m_piece == mBQU){
                    score += SCORES[mBPLUS] * 3; // penalty
                }
            }
            else if(wpieces.size() > 2){
                cPiece first = wpieces.at(0);
                cPiece second = wpieces.at(1);
                cPiece third = wpieces.at(2);
                if(second.m_piece == mBRK || second.m_piece == mBQU){
                    score += SCORES[mBPLUS] * 3; // penalty
                }
                else if((third.m_piece == mBRK || third.m_piece == mBQU)){
                    if((first.m_piece != mWPW && first.m_piece != mBPW) || (second.m_piece != mWPW && second.m_piece != mBPW)){
                        score += SCORES[mBPLUS] * 3; // penalty
                    }
                }
            }

            //weak black king?
            for(uint8_t i = 0; i < size(steps); ++i){
                vector <cPiece> wpieces, bpieces;

                m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBLK);

                if(m_board.is_inbounds(m_board.m_bKg_x + steps[i][0], m_board.m_bKg_y + steps[i][1])){
                    m_board.search_for_all_touching_pieces(wpieces, bpieces, m_board.m_bKg_x + steps[i][0], m_board.m_bKg_y + steps[i][1]);

                    if(bpieces.size() < wpieces.size()){
                        score += SCORES[mWPLUS] * 3; // penalty
                        m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBKG);
                        break;
                    }
                }
                m_board.setfield(m_board.m_bKg_x, m_board.m_bKg_y, mBKG);
            }

            vector<cPiece> bpieces;
            m_board.search_dir_for_pieces(bpieces, m_board.m_bKg_x, m_board.m_bKg_y, 0, -1);

            if(bpieces.size() == 2){
                cPiece second = bpieces.at(1);
                if(second.m_piece == mWRK || second.m_piece == mWQU){
                    score += SCORES[mWPLUS] * 3; // penalty
                }
            }
            else if(bpieces.size() > 2){
                cPiece first = bpieces.at(0);
                cPiece second = bpieces.at(1);
                cPiece third = bpieces.at(2);
                if(second.m_piece == mWRK || second.m_piece == mWQU){
                    score += SCORES[mWPLUS] * 3; // penalty
                }
                else if((third.m_piece == mWRK || third.m_piece == mWQU)){
                    if((first.m_piece != mBPW && first.m_piece != mWPW) || (second.m_piece != mBPW && second.m_piece != mWPW)){
                        score += SCORES[mWPLUS] * 3; // penalty
                    }
                }
            }

            //wrook on open file
            bool isopen = false;
            bool isinfiltrated = false;
            uint8_t rk_x, rk_y;

            rk_y = 0;

            for(uint8_t i = 0; i < 8; ++i){
                rk_x = i;

                if(m_board.getfield(rk_x, rk_y) == mWRK){
                    vector<cPiece> pieces; 

                    m_board.search_dir_for_pieces(pieces, rk_x, rk_y, 0, 1);

                    isopen = true;
                    for(cPiece piece : pieces){
                        if(piece.m_piece == mWPW){
                            isopen = false;
                        }
                    }

                    if(isopen){
                        break;
                    }
                }
            }

            if(isopen == false){
                rk_y = 6;

                for(uint8_t i = 0; i < 8; ++i){
                    rk_x = i;

                    if(m_board.getfield(rk_x, rk_y) == mWRK){
                        isinfiltrated = true;
                        break;
                    }
                }
            }

            if(isopen || isinfiltrated){
                vector<cPiece> wpieces, bpieces; 

                m_board.search_for_all_touching_pieces(wpieces, bpieces, rk_x, rk_y);

                if(wpieces.size() >= bpieces.size()){
                    score += SCORES[mWPLUS] * 2;
                }
            }
                    
            //brook on open file
            isopen = false;
            isinfiltrated = false;

            rk_y = 7;

            for(uint8_t i = 0; i < 8; ++i){
                rk_x = i;

                if(m_board.getfield(rk_x, rk_y) == mBRK){
                    vector<cPiece> pieces; 

                    m_board.search_dir_for_pieces(pieces, rk_x, rk_y, 0, -1);

                    isopen = true;
                    for(cPiece piece : pieces){
                        if(piece.m_piece == mBPW){
                            isopen = false;
                        }
                    }

                    if(isopen){
                        break;
                    }
                }
            }

            if(isopen == false){
                rk_y = 1;

                for(uint8_t i = 0; i < 8; ++i){
                    rk_x = i;

                    if(m_board.getfield(rk_x, rk_y) == mBRK){
                        isinfiltrated = true;
                        break;
                    }
                }
            }

            if(isopen || isinfiltrated){
                vector<cPiece> wpieces, bpieces; 

                m_board.search_for_all_touching_pieces(wpieces, bpieces, rk_x, rk_y);

                if(bpieces.size() >= wpieces.size()){
                    score += SCORES[mBPLUS] * 2;
                }
            }
        }

        return score;

    }


    int32_t cMatch::eval_endgame_board(bool &islib, cMove &move, uint8_t depth, uint8_t status){

        islib = false;

        int32_t score = 0;

        if(status == cBoard::ENDGAME_STAT_0){
        // check pawns
            for(uint8_t y = 1; y < 7; ++y){
                for(uint8_t x = 0; x < 7; ++x){

                    uint8_t pawn = m_board.getfield(x, y);

                    if(pawn == mWPW){
                        if(m_board.is_running_pawn(pawn, x, y)){
                            score += SCORES[mWPLUS] * y;
                        }
                    }

                    if(pawn == mBPW){
                        if(m_board.is_running_pawn(pawn, x, y)){
                            score += SCORES[mBPLUS] * (7 - y);
                        }
                    }
                }
            }
            
            return score;
        }

        if(status == cBoard::ENDGAME_STAT_100 || status == cBoard::ENDGAME_STAT_110){
            // white kn+bp

            if(m_board.is_margin_frame_pos(m_board.m_wKg_x, m_board.m_wKg_y, m_board.m_bKg_x, m_board.m_bKg_y)){
                if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                    cEndGame100_base endgame100_base;
                    uint8_t libidx = endgame100_base.eval_path_to_mate(status, m_board.m_fields);

                    if(libidx > 0){
                        islib = true;

                        return 100 + (libidx * 10) - depth;
                    }
                }
            }

            score += cEndGame100_base::eval_push_kg_to_margin(status, depth, m_board.m_fields, m_board.m_wKg_x, m_board.m_wKg_y, m_board.m_bKg_x, m_board.m_bKg_y);

            if(m_board.is_within_two_squares(mBKN, m_board.m_wKg_x, m_board.m_wKg_y)){
                score += 4;
            }
        }
        else if(status == cBoard::ENDGAME_STAT_120 || status == cBoard::ENDGAME_STAT_130){
            // black kn+bp

            if(m_board.is_margin_frame_pos(m_board.m_wKg_x, m_board.m_wKg_y, m_board.m_bKg_x, m_board.m_bKg_y)){
                if(PIECES_COLORS[move.m_srcpiece] == mBLACK){
                    cEndGame100_base endgame100_base;
                    uint8_t libidx = endgame100_base.eval_path_to_mate(status, m_board.m_fields);

                    if(libidx > 0){
                        islib = true;

                        return -100 - ((libidx * 10) - depth);
                    }
                }
            }

            score += cEndGame100_base::eval_push_kg_to_margin(status, depth, m_board.m_fields, m_board.m_wKg_x, m_board.m_wKg_y, m_board.m_bKg_x, m_board.m_bKg_y);

            if(m_board.is_within_two_squares(mBKN, m_board.m_bKg_x, m_board.m_bKg_y)){
                score -= 4;
            }
        }   

        return score;

    }

