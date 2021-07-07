

    #include "./match.hpp"


    // debug
    //bool debug = false;


    const uint8_t MAXTHREADS = 4;


    cMatch::cMatch(){ 
        cBoard board;
    }


    // copy constructor
    cMatch::cMatch(const cMatch &match){

        score = match.score;

        for(int y = 0; y < 8; ++y){
            for(int x = 0; x < 8; ++x){
                board.fields[y][x] = match.board.fields[y][x];
            }
        }
        board.wKg_x = match.board.wKg_x;
        board.wKg_y = match.board.wKg_y;
        board.bKg_x = match.board.bKg_x;
        board.bKg_y = match.board.bKg_y;
        board.wKg_has_moved_at = match.board.wKg_has_moved_at;
        board.bKg_has_moved_at = match.board.bKg_has_moved_at;
        board.wRkA_has_moved_at = match.board.wRkA_has_moved_at;
        board.wRkH_has_moved_at = match.board.wRkH_has_moved_at;
        board.bRkA_has_moved_at = match.board.bRkA_has_moved_at;
        board.bRkH_has_moved_at = match.board.bRkH_has_moved_at;

        for(cMove move : match.minutes){
            minutes.push_back(move);
        }

    }


    cMatch::~cMatch(){
    }


    void cMatch::reset(){
        score = 0;

        minutes.clear();
        
        board.fields[0][0] = mWRK;
        board.fields[0][1] = mWKN;
        board.fields[0][2] = mWBP; 
        board.fields[0][3] = mWQU; 
        board.fields[0][4] = mWKG; 
        board.fields[0][5] = mWBP;
        board.fields[0][6] = mWKN; 
        board.fields[0][7] = mWRK;

        for(uint8_t i = 0; i < 8; ++i){
            board.fields[1][i] = mWPW;
        }
        
        for(uint8_t j = 2; j < 6; ++j){
            for(uint8_t i = 0; i < 8; ++i){
                board.fields[j][i] = mBLK;
            }
        }

        for(uint8_t i = 0; i < 8; ++i){
            board.fields[6][i] = mBPW;
        }

        board.fields[7][0] = mBRK;
        board.fields[7][1] = mBKN;
        board.fields[7][2] = mBBP; 
        board.fields[7][3] = mBQU; 
        board.fields[7][4] = mBKG; 
        board.fields[7][5] = mBBP;
        board.fields[7][6] = mBKN; 
        board.fields[7][7] = mBRK;

        board.wKg_x = 4;
        board.wKg_y = 0;
        board.bKg_x = 4;
        board.bKg_y = 7;
        board.wKg_has_moved_at = 0;
        board.bKg_has_moved_at = 0;
        board.wRkA_has_moved_at = 0;
        board.wRkH_has_moved_at = 0;
        board.bRkA_has_moved_at = 0;
        board.bRkH_has_moved_at = 0;

    }


    uint8_t cMatch::next_color(){

        if(minutes.size() % 2 == 0){
            return mWHITE;
        }
        else{
            return mBLACK;
        }

    }


    void cMatch::prnt_minutes(){

        for(cMove move : minutes){
            cout << move.format() << endl;
        }

    }


    string cMatch::fmt_moves(vector<cMove> &moves){

        string str_moves = "";

        for(cMove move : moves){
            //str_moves += to_string(move->src_x) + " ";
            //str_moves += to_string(move->src_y) + " ";
            //str_moves += to_string(move->dst_x) + " ";
            //str_moves += to_string(move->dst_y) + " ";
            str_moves += " [" + move.format() + "] ";
        }
        return str_moves;

    }


    void cMatch::prnt_moves(vector<cMove> &moves){

        uint8_t idx = 1;

        cout << "-------------------------------------------" << endl;
        for(cMove move : moves){
            cout << "\n" << dec << idx << ". ";
            cout << move.format() << endl;
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


    void cMatch::calc_move(int32_t &calc_score, vector<cMove> &moves, uint8_t maxdepth){

        time_t time_start = time(0);

        bool maximizing = (next_color() == mWHITE);

        int32_t alpha = SCORES[mWKG] * 10;

        int32_t beta = SCORES[mBKG] * 10;

        uint8_t depth = 1;

        if(MAXTHREADS > 1){
            array<vector<cMove>, MAXTHREADS> candidates;

            array<cMatch*, MAXTHREADS> matches;

            thread threads[MAXTHREADS];

            int32_t scores[MAXTHREADS];

            for(uint8_t idx = 0; idx < MAXTHREADS; ++idx){
                cMatch *match = new cMatch(*this);
                matches[idx] = match; //new cMatch(*this);
                threads[idx] = thread(&cMatch::alphabeta, match, ref(scores[idx]), ref(candidates[idx]), depth, maxdepth, alpha, beta, maximizing, idx + 1);
            }

            for(uint8_t idx = 0; idx < MAXTHREADS; ++idx){
                threads[idx].join();
                cout << "\nthread #" << (idx + 1) << " joined ";
                cout << scores[idx] << " " << fmt_moves(candidates[idx]) << endl;
                delete matches[idx];
            }

            calc_score = scores[0];
            moves.assign(candidates[0].begin(), candidates[0].end());
            for(uint8_t idx = 1; idx < MAXTHREADS; ++idx){
                if(maximizing){
                    if(scores[idx] > calc_score){
                        calc_score = scores[idx];
                        moves.clear();
                        moves.assign(candidates[idx].begin(), candidates[idx].end());
                    }
                }
                else{
                    if(scores[idx] < calc_score){
                        calc_score = scores[idx];
                        moves.clear();
                        moves.assign(candidates[idx].begin(), candidates[idx].end());
                    }
                }
            }
        }
        else{
            alphabeta(calc_score, moves, depth, maxdepth, alpha, beta, maximizing, 0);
        }

        cout << "\nmatch score: " << score;

        cout << "\nresult: " << calc_score;

        cout << "\n" << fmt_moves(moves) << endl;

        prnt_fmttime("\ncalc-time: ", time(0) - time_start);

    }


    void cMatch::calc_move_v2(int32_t &calc_score, vector<cMove> &moves, uint8_t maxdepth){

        time_t time_start = time(0);

        bool maximizing = (next_color() == mWHITE);

        int32_t alpha = SCORES[mWKG] * 10;

        int32_t beta = SCORES[mBKG] * 10;

        uint8_t depth = 1;

        alphabeta_v2(calc_score, moves, depth, maxdepth, alpha, beta, maximizing, 0);

        cout << "\nmatch score: " << score;

        cout << "\nresult: " << calc_score;

        cout << "\n" << fmt_moves(moves) << endl;

        prnt_fmttime("\ncalc-time: ", time(0) - time_start);

    }


    bool cMatch::is_king_attacked(uint8_t attacker_color){

        if(attacker_color == mWHITE){
            return board.search_for_touching_piece(board.bKg_x, board.bKg_y, mWHITE);
        }
        else{
            return board.search_for_touching_piece(board.wKg_x, board.wKg_y, mBLACK);
        }

    }


    bool cMatch::is_move_valid(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece){

        vector <cMove> moves;
        vector <cPiece> pieces;
        uint8_t king, kg_x, kg_y, enmycolor;
        uint8_t piece = board.getfield(src_x, src_y);
        uint8_t color = PIECES_COLORS[piece];
        
        if(color != next_color()){
            return false;
        }

        if(color == mWHITE){
            king = mWKG;
            kg_x = board.wKg_x;
            kg_y = board.wKg_y;
            enmycolor = mBLACK;
        }
        else{
            king = mBKG;
            kg_x = board.bKg_x;
            kg_y = board.bKg_y;
            enmycolor = mWHITE;
        }

        board.search_for_touching_pieces(pieces, kg_x, kg_y, enmycolor, true);

        if(pieces.size() >= 2){
            gen_kg_moves(moves, kg_x, kg_y, king);
        }
        else if(pieces.size() == 1){
            gen_kg_moves(moves, kg_x, kg_y, king);
            cPiece piece = pieces.front();
            gen_kg_supporting_moves(moves, kg_x, kg_y, piece.piece, piece.src_x, piece.src_y, color);
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
            if(move.src_x == src_x && move.src_y == src_y &&
            move.dst_x == dst_x && move.dst_y == dst_y){
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

        uint8_t srcpiece = board.getfield(src_x, src_y);

        uint8_t dstpiece = board.getfield(dst_x, dst_y);
        
        cMove move(src_x, src_y, dst_x, dst_y, srcpiece, dstpiece, prompiece);

        do_move(move);

    }


    void cMatch::do_move(cMove &move){

        // promotion
        if(move.is_promotion()){
            board.setfield(move.src_x, move.src_y, mBLK);
            board.setfield(move.dst_x, move.dst_y, move.prompiece);

            score += SCORES[move.dstpiece];
            if(PIECES_COLORS[move.srcpiece] == mWHITE){
                score += REVERSED_SCORES[move.prompiece] - REVERSED_SCORES[mWPW];
            }
            else{
                score += REVERSED_SCORES[move.prompiece] - REVERSED_SCORES[mBPW];
            }
            
            minutes.push_back(move);
            return;
        }

        // standard move
        board.setfield(move.src_x, move.src_y, mBLK);
        board.setfield(move.dst_x, move.dst_y, move.srcpiece);

        score += SCORES[move.dstpiece];

        // add on for en passant
        if(move.is_en_passant()){
            uint8_t enpiece = board.getfield(move.dst_x, move.src_y);
            board.setfield(move.dst_x, move.src_y, mBLK);

            score += SCORES[enpiece];

            minutes.push_back(move);
            return;
        }

        if(move.srcpiece == mWKG || move.srcpiece == mBKG){
            // castlings
            if(move.is_short_castling()){
                uint8_t rook = board.getfield(7, move.dst_y); // h == 7
                board.setfield(7, move.dst_y, mBLK);
                board.setfield(5, move.dst_y, rook); // f == 5
            }
            else if(move.is_long_castling()){
                uint8_t rook = board.getfield(0, move.dst_y); // a == 0
                board.setfield(0, move.dst_y, mBLK);
                board.setfield(3, move.dst_y, rook); // c == 3
            }

            // update flags in board
            if(move.srcpiece == mWKG){
                if(board.wKg_has_moved_at == 0){
                    board.wKg_has_moved_at = minutes.size() + 1;
                }
                board.wKg_x = move.dst_x;
                board.wKg_y = move.dst_y;
            }
            else{
                if(board.bKg_has_moved_at == 0){
                    board.bKg_has_moved_at = minutes.size() + 1;
                }
                board.bKg_x = move.dst_x;
                board.bKg_y = move.dst_y;
            }
        }
        else if(move.srcpiece == mWRK){
            // update flags in board
            if(move.src_x == 0 && move.src_y == 0 && 
               board.wRkA_has_moved_at == 0){
                board.wRkA_has_moved_at = minutes.size() + 1;
            }
            if(move.src_x == 7 && move.src_y == 0 && 
                board.wRkH_has_moved_at == 0){
                board.wRkH_has_moved_at = minutes.size() + 1;
            }
        }
        else if(move.srcpiece == mBRK){
            // update flags in board
            if(move.src_x == 0 && move.src_y == 7 && 
               board.bRkA_has_moved_at == 0){
                board.bRkA_has_moved_at = minutes.size() + 1;
            }
            if(move.src_x == 7 && move.src_y == 7 && 
                 board.bRkH_has_moved_at == 0){
                board.bRkH_has_moved_at = minutes.size() + 1;
            }
        }

        minutes.push_back(move);
    }


    bool cMatch::undo_move(){

        cMove move;
        if(minutes.size() > 0){
            move = minutes.back();
        }
        else{
            return false;
        }

        // undo promotion
        if(move.is_promotion() && PIECES_COLORS[move.srcpiece] == mWHITE){
            board.setfield(move.src_x, move.src_y, mWPW);
            board.setfield(move.dst_x, move.dst_y, move.dstpiece);
        
            score -= SCORES[move.dstpiece];
            score += SCORES[move.prompiece] - SCORES[mWPW];
        
            minutes.pop_back();
            return true;
        }
        else if(move.is_promotion() && PIECES_COLORS[move.srcpiece] == mBLACK){
            board.setfield(move.src_x, move.src_y, mBPW);
            board.setfield(move.dst_x, move.dst_y, move.dstpiece);

            score -= SCORES[move.dstpiece];
            score += SCORES[move.prompiece] - SCORES[mBPW];

            minutes.pop_back();
            return true;
        }
        else{
            // undo standard move
            board.setfield(move.src_x, move.src_y, move.srcpiece);
            board.setfield(move.dst_x, move.dst_y, move.dstpiece);

            score -= SCORES[move.dstpiece];
        }

        // undo en passant
        if(move.srcpiece == mWPW && move.is_en_passant()){
            board.setfield(move.dst_x, move.src_y, mBPW);

            score -= SCORES[mBPW];

            minutes.pop_back();
            return true;
        }
        else if(move.srcpiece == mBPW && move.is_en_passant()){
            board.setfield(move.dst_x, move.src_y, mWPW);

            score -= SCORES[mWPW];

            minutes.pop_back();
            return true;
        }

        // undo castling
        if(move.srcpiece == mWKG || move.srcpiece == mBKG){
            if(move.is_short_castling()){
                uint8_t rook = board.getfield(5, move.src_y); // f == 5
                board.setfield(5, move.src_y, mBLK);
                board.setfield(7, move.src_y, rook); // h == 7
            }
            else if(move.is_long_castling()){
                uint8_t rook = board.getfield(3, move.src_y); // c == 3
                board.setfield(3, move.src_y, mBLK);
                board.setfield(0, move.src_y, rook); // a == 0
            }

            // update flags in board
            if(move.srcpiece == mWKG){
                if(board.wKg_has_moved_at != 0 && board.wKg_has_moved_at == minutes.size()){
                    board.wKg_has_moved_at = 0;
                }
                board.wKg_x = move.src_x;
                board.wKg_y = move.src_y;
            }
            else{
                if(board.bKg_has_moved_at != 0 && board.bKg_has_moved_at == minutes.size()){
                    board.bKg_has_moved_at = 0;
                }
                board.bKg_x = move.src_x;
                board.bKg_y = move.src_y;
            }
        }
        else if(move.srcpiece == mWRK){
            // update flags in board
            if(board.wRkA_has_moved_at != 0 && board.wRkA_has_moved_at == minutes.size()){
                board.wRkA_has_moved_at = 0;
            }
            if(board.wRkH_has_moved_at != 0 && board.wRkH_has_moved_at == minutes.size()){
                board.wRkH_has_moved_at = 0;
            }
        }
        else if(move.srcpiece == mBRK){
            // update flags in board
            if(board.bRkA_has_moved_at != 0 && board.bRkA_has_moved_at == minutes.size()){
                board.bRkA_has_moved_at = 0;
            }
            if(board.bRkH_has_moved_at != 0 && board.bRkH_has_moved_at == minutes.size()){
                board.bRkH_has_moved_at = 0;
            }
        }

        minutes.pop_back();
        return true;

    }


    //calculation
    bool cMatch::is_piece_soft_pinned(uint8_t piece, uint8_t piece_x, uint8_t piece_y){
        return false;
        uint8_t piece1, piece1_x, piece1_y, piece2, piece2_x, piece2_y;

        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        uint8_t idx = 0;
        while(idx < 8){
            piece1 = board.search_dir_for_piece(piece1_x, piece1_y, piece_x, piece_y, steps[idx][0], steps[idx][1], 6);
            if(piece1 == mBLK){ idx += 2; continue; }

            piece2 = board.search_dir_for_piece(piece2_x, piece2_y, piece_x, piece_y, steps[idx + 1][0], steps[idx + 1][1], 6);
            if(piece2 == mBLK){ idx += 2; continue; }

            if((PIECES_COLORS[piece1] == PIECES_COLORS[piece2])){
                idx += 2; continue;
            }

            if(PIECES_COLORS[piece1] == PIECES_COLORS[piece]){
                if((idx < 4) && (piece2 == mWRK || piece2 == mBRK || piece2 == mWQU || piece2 == mBQU)){
                    if(PIECES_RANKS[piece1] > PIECES_RANKS[piece] && PIECES_RANKS[piece2] < PIECES_RANKS[piece1]){
                        return true;
                    }
                }
                if((idx >= 4) && (piece2 == mWBP || piece2 == mBBP || piece2 == mWQU || piece2 == mBQU)){
                    if(PIECES_RANKS[piece1] > PIECES_RANKS[piece] && PIECES_RANKS[piece2] < PIECES_RANKS[piece1]){
                        return true;
                    }
                }
            }
            else if(PIECES_COLORS[piece2] == PIECES_COLORS[piece]){
                if((idx < 4) && (piece1 == mWRK || piece1 == mBRK || piece1 == mWQU || piece1 == mBQU)){
                    if(PIECES_RANKS[piece2] > PIECES_RANKS[piece] && PIECES_RANKS[piece1] < PIECES_RANKS[piece2]){
                        return true;
                    }
                }
                if((idx >= 4) && (piece1 == mWBP || piece1 == mBBP || piece1 == mWQU || piece1 == mBQU)){
                    if(PIECES_RANKS[piece2] > PIECES_RANKS[piece] && PIECES_RANKS[piece1] < PIECES_RANKS[piece2]){
                        return true;
                    }
                }
            }

            idx += 2; 
        }

        return false;

    }


    bool cMatch::filter(cMove &move, uint8_t depth){

        // take en passant moves
        if(move.is_en_passant()){ 
            return true; 
        }

        // take promotion moves
        if(move.prompiece != mBLK){ 
            return true; 
        }

        // take good capture moves
        if(move.dstpiece != mBLK){ 
            // piece must be lower than captured piece
            if(PIECES_RANKS[move.srcpiece] < PIECES_RANKS[move.dstpiece]){
                return true;
            }

            vector<cPiece> supporting_pieces;
            board.search_for_touching_pieces(supporting_pieces, move.dst_x, move.dst_y, REVERSED_COLORS[PIECES_COLORS[move.srcpiece]], true);

            // move is good because captured piece is not supported
            if(supporting_pieces.size() == 0){
                return true; 
            }

            vector<cPiece> attacking_pieces;
            board.search_for_touching_pieces(attacking_pieces, move.dst_x, move.dst_y, PIECES_COLORS[move.srcpiece], true);
            
            // move is good because captured piece is more attacked than supported 
            if(attacking_pieces.size() > supporting_pieces.size()){
                return true; 
            }

            // search for lowest supporter
            uint8_t min_supp = PIECES_RANKS[mWKG];
            for(cPiece supp_piece : supporting_pieces){
                if(PIECES_RANKS[supp_piece.piece] < min_supp){
                    min_supp = PIECES_RANKS[supp_piece.piece];
                }
            }
            // if lowest attacker is less than lowest supporter, move is good
            bool has_lower_attacker = false;
            for(cPiece att_piece : attacking_pieces){
                if(PIECES_RANKS[att_piece.piece] < min_supp){
                    has_lower_attacker = true;
                    break;
                }
            }

            supporting_pieces.clear();

            attacking_pieces.clear();

            if(has_lower_attacker){
                return true;
            }
        }

        //if(depth > 6){ return false; } // 8

        if(move.srcpiece == mWPW && move.dst_y >= 6){
            return true;
        }

        if(move.srcpiece == mBPW && move.dst_y <= 2){
            return true;
        }

        vector<cPiece> touched_pieces;

        uint8_t excl_dir = cBoard::eval_dir(move.src_x, move.src_y, move.dst_x, move.dst_y);

        board.piece_search_for_touched_pieces(touched_pieces, move.srcpiece, move.dst_x, move.dst_y, PIECES_COLORS[PIECES_COLORS[move.srcpiece]], excl_dir);

        bool pinned = false;
        for(cPiece touched_piece : touched_pieces){
            if(is_piece_soft_pinned(touched_piece.piece, touched_piece.src_x, touched_piece.src_y)){
                pinned = true;
                break;
            }
        }
        touched_pieces.clear();

        return pinned;

    }


    void cMatch::append_newmove(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, cMove &move){

        rcandidates.clear();

        rcandidates.push_back(move);

        for(cMove nmove : newcandidates){
            rcandidates.push_back(nmove);
        }

    }


    bool cMatch::is_calc_term(int32_t &score, const vector<cMove> &moves, uint8_t depth){

        if(moves.size() == 0){
            if(is_king_attacked(REVERSED_COLORS[next_color()])){
                if(next_color() == mWHITE){
                    score = (SCORES[mWKG] * 9) + depth;
                }
                else{
                    score = (SCORES[mBKG] * 9) - depth;
                }
            }
            else{
                score = 0; // draw
            }

            return true;
        }

        return false; 

    }


    void cMatch::gen_moves(vector<cMove> &moves, uint8_t color){

        uint8_t king, kg_x, kg_y, enmycolor;

        if(color == mWHITE){
            king = mWKG;
            kg_x = board.wKg_x;
            kg_y = board.wKg_y;
            enmycolor = mBLACK;
        }
        else{
            king = mBKG;
            kg_x = board.bKg_x;
            kg_y = board.bKg_y;
            enmycolor = mWHITE;
        }

        vector<cPiece> attacking_pieces;
        board.search_for_touching_pieces(attacking_pieces, kg_x, kg_y, enmycolor, true);

        if(attacking_pieces.size() >= 2){
            gen_kg_moves(moves, kg_x, kg_y, king);
        }
        else if(attacking_pieces.size() == 1){
            gen_kg_moves(moves, kg_x, kg_y, king);

            cPiece attacking_piece = attacking_pieces.front();
            gen_kg_supporting_moves(moves, kg_x, kg_y, attacking_piece.piece, attacking_piece.src_x, attacking_piece.src_y, color);
        }
        else{
            for(uint8_t y = 0; y < 8; ++y){
                for(uint8_t x = 0; x < 8; ++x){
                    uint8_t piece = board.getfield(x, y);
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
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWQU));
            
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWRK));
            
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWBP));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWKN));
        }
        else{
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mBLK));
        }
    }


    void cMatch::gen_wpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        uint8_t pindir = board.eval_pindir(x, y, mWHITE);

        if((pindir == mNO_DIR || pindir == mSTHWST_NTHEST) && cBoard::is_inbounds(x + 1, y + 1)){
            uint8_t dstpiece = board.getfield(x + 1, y + 1);

            if(PIECES_COLORS[dstpiece] == mBLACK){
                add_wpw_moves(moves, x, y, x + 1, y + 1, dstpiece);
            }
            else if(dstpiece == mBLK && y == 4){ // en passant
                cMove prevmove = minutes.back();
                uint8_t enpiece = board.getfield(prevmove.dst_x, prevmove.dst_y);
                if(enpiece == mBPW && prevmove.src_y == 6 && prevmove.dst_y == 4 && prevmove.src_x == prevmove.dst_x && x + 1 == prevmove.src_x){
                    moves.push_back(cMove(x, y, x + 1, y + 1, mWPW, dstpiece, mBLK));
                }
            }
        }

        if((pindir == mNO_DIR || pindir == mSTHEST_NTHWST) && cBoard::is_inbounds(x - 1, y + 1)){

            uint8_t dstpiece = board.getfield(x - 1, y + 1);

            if(PIECES_COLORS[dstpiece] == mBLACK){
                add_wpw_moves(moves, x, y, x - 1, y + 1, dstpiece);
            }
            else if(dstpiece == mBLK && y == 4){ // en passant
                cMove prevmove = minutes.back();
                uint8_t enpiece = board.getfield(prevmove.dst_x, prevmove.dst_y);
                if(enpiece == mBPW && prevmove.src_y == 6 && prevmove.dst_y == 4 && prevmove.src_x == prevmove.dst_x && x - 1 == prevmove.src_x){
                    moves.push_back(cMove(x, y, x - 1, y + 1, mWPW, dstpiece, mBLK));
                }
            }
        }

        if(pindir == mNO_DIR || pindir == mSTH_NTH){
            if(cBoard::is_inbounds(x, y + 1)){
                uint8_t dstpiece = board.getfield(x, y + 1);
                if(dstpiece == mBLK){
                    add_wpw_moves(moves, x, y, x, y + 1, dstpiece);
                }
            }
            if(cBoard::is_inbounds(x, y + 2)){
                uint8_t dstpiece = board.getfield(x, y + 2);
                uint8_t midpiece = board.getfield(x, y + 1);
                if(midpiece == mBLK && dstpiece == mBLK && y == 1){
                  moves.push_back(cMove(x, y, x, y + 2, mWPW, dstpiece, mBLK));
                }
            }
        }

    }


    void cMatch::add_bpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece){

        if(src_y == 1 && dst_y == 0){
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBQU));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBRK));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBBP));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBKN));
        }
        else{
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBLK));
        }

    }


    void cMatch::gen_bpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        uint8_t pindir = board.eval_pindir(x, y, mBLACK);

        if((pindir == mNO_DIR || pindir == mSTHEST_NTHWST) && cBoard::is_inbounds(x + 1, y - 1)){
            
            uint8_t dstpiece = board.getfield(x + 1, y - 1);
            if(PIECES_COLORS[dstpiece] == mWHITE){
                add_bpw_moves(moves, x, y, x + 1, y - 1, dstpiece);
            }
            else if(dstpiece == mBLK && y == 3){ // en passant
                cMove prevmove = minutes.back();
                uint8_t enpiece = board.getfield(prevmove.dst_x, prevmove.dst_y);
                if(enpiece == mWPW && prevmove.src_y == 1 && prevmove.dst_y == 3 && prevmove.src_x == prevmove.dst_x && x + 1 == prevmove.src_x){
                    moves.push_back(cMove(x, y, x + 1, y - 1, mBPW, dstpiece, mBLK));
                }
            }
        }

        if((pindir == mNO_DIR || pindir == mSTHWST_NTHEST) && cBoard::is_inbounds(x - 1, y - 1)){
            uint8_t dstpiece = board.getfield(x - 1, y - 1);
            if(PIECES_COLORS[dstpiece] == mWHITE){
                add_bpw_moves(moves, x, y, x - 1, y - 1, dstpiece);
            }
            else if(dstpiece == mBLK && y == 3){ // en passant
                cMove prevmove = minutes.back();
                uint8_t enpiece = board.getfield(prevmove.dst_x, prevmove.dst_y);
                if(enpiece == mWPW && prevmove.src_y == 1 && prevmove.dst_y == 3 && prevmove.src_x == prevmove.dst_x && x - 1 == prevmove.src_x){
                    moves.push_back(cMove(x, y, x - 1, y - 1, mBPW, dstpiece, mBLK));
                }
            }
        }

        if(pindir == mNO_DIR || pindir == mSTH_NTH){
            if(cBoard::is_inbounds(x, y - 1)){
                uint8_t dstpiece = board.getfield(x, y - 1);
                if(dstpiece == mBLK){
                    add_bpw_moves(moves, x, y, x, y - 1, dstpiece);
                }
            }

            if(cBoard::is_inbounds(x, y - 2)){
                uint8_t dstpiece = board.getfield(x, y - 2);
                uint8_t midpiece = board.getfield(x, y - 1);
                if(midpiece == mBLK && dstpiece == mBLK && y == 6){
                    moves.push_back(cMove(x, y, x, y - 2, mBPW, dstpiece, mBLK));
                }
            }
        }

    }


    void cMatch::gen_kn_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t knight){

        uint8_t pindir = board.eval_pindir(x, y, PIECES_COLORS[knight]);
        if(pindir != mNO_DIR){ 
            return; 
        }

        int8_t steps[][2] = { { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

        for(uint8_t i = 0; i < 8; ++i){
            if(cBoard::is_inbounds(x + steps[i][0], y + steps[i][1])){
                uint8_t dst_x = x + steps[i][0];
                uint8_t dst_y = y + steps[i][1];

                uint8_t dstpiece = board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] != PIECES_COLORS[knight]){
                    moves.push_back(cMove(x, y, dst_x, dst_y, knight, dstpiece, mBLK));
                }
            }
        }

    }


    void cMatch::gen_qrb_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t piece, char _switch){

        uint8_t pindir = board.eval_pindir(x, y, PIECES_COLORS[piece]);
        
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

                uint8_t dstpiece = board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[piece]){
                    break;
                }

                moves.push_back(cMove(x, y, dst_x, dst_y, piece, dstpiece, mBLK));

                if(dstpiece != mBLK){
                    break;
                }
            }
        }

    }


    void cMatch::gen_kg_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t king){

        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(uint8_t idx = 0; idx < 8; ++idx){
            uint8_t dst_x = x;
            uint8_t dst_y = y;
            for(uint8_t j = 0; j < 1; ++j){
                if(cBoard::is_inbounds(dst_x + steps[idx][0], dst_y + steps[idx][1]) == false){
                    break;
                }
                dst_x += steps[idx][0];
                dst_y += steps[idx][1];

                uint8_t dstpiece = board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[king]){
                    break;
                }

                bool is_clear = false;
                if(PIECES_COLORS[king] == mWHITE){
                    board.setfield(board.wKg_x, board.wKg_y, mBLK);
                    if(board.search_for_touching_piece(dst_x, dst_y, mBLACK) == mBLK){
                        is_clear = true;
                    }
                    board.setfield(board.wKg_x, board.wKg_y, mWKG);
                }
                else{
                    board.setfield(board.bKg_x, board.bKg_y, mBLK);
                    if(board.search_for_touching_piece(dst_x, dst_y, mWHITE) == mBLK){
                        is_clear = true;
                    }
                    board.setfield(board.bKg_x, board.bKg_y, mBKG);
                }
                if(is_clear == false){ 
                    break; 
                }

                moves.push_back(cMove(x, y, dst_x, dst_y, king, dstpiece, mBLK));
            }
        }

    }


    void cMatch::gen_wkg_castling_moves(vector<cMove> &moves,  uint8_t x, uint8_t y){

        // short white castling
        if(board.wKg_has_moved_at == 0 && board.wRkH_has_moved_at == 0 && cBoard::is_inbounds(x + 3, y)){
            if(y == 0 && board.getfield(x + 1, y) == mBLK && board.getfield(x + 2, y) == mBLK && board.getfield(x + 3, y) == mWRK){
                board.setfield(board.wKg_x, board.wKg_y, mBLK);
                bool is_clear = true;
                for(uint8_t i = 0; i < 3; ++i){
                    if(board.search_for_touching_piece(x + i, y, mBLACK) != mBLK){
                        is_clear = false;
                        break;
                    }
                }
                board.setfield(board.wKg_x, board.wKg_y, mWKG);
                if(is_clear){
                    moves.push_back(cMove(x, y, x + 2, y, mWKG, mBLK, mBLK));
                }
            }
        }

        // long white castling
        if(board.wKg_has_moved_at == 0 && board.wRkA_has_moved_at == 0 && cBoard::is_inbounds(x - 4, y)){
            if(y == 0 && board.getfield(x - 1, y) == mBLK && board.getfield(x - 2, y) == mBLK && board.getfield(x - 3, y) == mBLK && board.getfield(x - 4, y) == mWRK){
                board.setfield(board.wKg_x, board.wKg_y, mBLK);
                bool is_clear = true;
                for(uint8_t i = 0; i < 3; ++i){
                    if(board.search_for_touching_piece(x - i, y, mBLACK) != mBLK){
                        is_clear = false;
                        break;
                    }
                }
                board.setfield(board.wKg_x, board.wKg_y, mWKG);
                if(is_clear){
                    moves.push_back(cMove(x, y, x - 2, y, mWKG, mBLK, mBLK));
                }
            }
        }

    }


    void cMatch::gen_bkg_castling_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        // short black castling
        if(board.bKg_has_moved_at == 0 && board.bRkH_has_moved_at == 0 && cBoard::is_inbounds(x + 3, y)){
            if(y == 7 && board.getfield(x + 1, y) == mBLK && board.getfield(x + 2, y) == mBLK && board.getfield(x + 3, y) == mBRK){
                board.setfield(board.bKg_x, board.bKg_y, mBLK);
                bool is_clear = true;
                for(uint8_t i = 0; i < 3; ++i){
                    if(board.search_for_touching_piece(x + i, y, mWHITE) != mBLK){
                        is_clear = false;
                        break;
                    }
                }
                board.setfield(board.bKg_x, board.bKg_y, mBKG);
                if(is_clear){
                    moves.push_back(cMove(x, y, x + 2, y, mBKG, mBLK, mBLK));
                }
            }
        }

        // long black castling
        if(board.bKg_has_moved_at == 0 && board.bRkA_has_moved_at == 0 && cBoard::is_inbounds(x - 4, y)){
            if(y == 7 && board.getfield(x - 1, y) == mBLK && board.getfield(x - 2, y) == mBLK && board.getfield(x - 3, y) == mBLK && board.getfield(x - 4, y) == mBRK){
                board.setfield(board.bKg_x, board.bKg_y, mBLK);
                bool is_clear = true;
                for(uint8_t i = 0; i < 3; ++i){
                    if(board.search_for_touching_piece(x - i, y, mWHITE) != mBLK){
                        is_clear = false;
                        break;
                    }
                }
                board.setfield(board.bKg_x, board.bKg_y, mBKG);
                if(is_clear){
                    moves.push_back(cMove(x, y, x - 2, y, mBKG, mBLK, mBLK));
                }
            }
        }

    }


    void cMatch::gen_kg_supporting_moves(vector<cMove> &moves, uint8_t kg_x, uint8_t kg_y, uint8_t attacking_piece, uint8_t attacking_x, uint8_t attacking_y, uint8_t color){
        
        vector<cPiece> defending_pieces;        

        board.search_for_touching_pieces(defending_pieces, attacking_x, attacking_y, color, true);

        for(cPiece defending_piece : defending_pieces){
            if(defending_piece.piece == mWKG || defending_piece.piece == mBKG){
                continue;
            }

            uint8_t pindir = board.eval_pindir(defending_piece.src_x, defending_piece.src_y, PIECES_COLORS[defending_piece.piece]);

            uint8_t dir = cBoard::eval_dir(defending_piece.src_x, defending_piece.src_y, attacking_x, attacking_y);

            if(pindir != mNO_DIR && pindir != dir){
                continue;
            }

            if(defending_piece.piece == mWPW){
                add_wpw_moves(moves, defending_piece.src_x, defending_piece.src_y, attacking_x, attacking_y, attacking_piece);
            }
            else if(defending_piece.piece == mBPW){
                add_bpw_moves(moves, defending_piece.src_x, defending_piece.src_y, attacking_x, attacking_y, attacking_piece);
            }
            else{
                moves.push_back(cMove(defending_piece.src_x, defending_piece.src_y, attacking_x, attacking_y, defending_piece.piece, attacking_piece, mBLK));
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
                board.search_for_touching_pieces(defending_pieces, dst_x, dst_y, color, false);
                  for(cPiece defending_piece : defending_pieces){
                    if(defending_piece.piece == mWKG || defending_piece.piece == mBKG){
                        continue;
                    }

                    uint8_t pindir = board.eval_pindir(defending_piece.src_x, defending_piece.src_y, PIECES_COLORS[defending_piece.piece]);
                    uint8_t dir = cBoard::eval_dir(defending_piece.src_x, defending_piece.src_y, dst_x, dst_y);

                    if(pindir != mNO_DIR && pindir != dir){
                        continue;
                    }

                    if(defending_piece.piece == mWPW){
                        add_wpw_moves(moves, defending_piece.src_x, defending_piece.src_y, dst_x, dst_y, mBLK);
                    }
                    else if(defending_piece.piece == mBPW){
                        add_bpw_moves(moves, defending_piece.src_x, defending_piece.src_y, dst_x, dst_y, mBLK);
                    }
                    else{
                        moves.push_back(cMove(defending_piece.src_x, defending_piece.src_y, dst_x, dst_y, defending_piece.piece, mBLK, mBLK));
                    }
                }

                defending_pieces.clear();

                dst_x += step_x;
                dst_y += step_y;
            }
        }

    }


    void cMatch::alphabeta(int32_t &calc_score, vector<cMove> &rcandidates, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta, bool maximizing, uint8_t threadid){

        vector<cMove> newcandidates;
        int32_t newscore, bestscore;
        u_int8_t count = 0;

        // debug
        //if(debug == true){
        //    calc_score = 0;
        //    return;
        //}

        if(maximizing){
            bestscore = SCORES[mWKG] * 10;
        }
        else{
            bestscore = SCORES[mBKG] * 10;
        }

        vector<cMove> moves;
        gen_moves(moves, next_color());

        if(is_calc_term(calc_score, moves, depth)){
            moves.clear();
            return;
        }

        for(cMove move : moves){
            count++;
            newcandidates.clear();
            bool skip = false;

            if(depth == 1){
                if(threadid != 0 && (count % MAXTHREADS) + 1 != threadid){ 
                    this_thread::sleep_for(chrono::milliseconds(100));
                    continue;
                }

                cout << "\n" << to_string(count) << "(" + to_string(moves.size()) + ") CURRENT SEARCH from thread #" << to_string(threadid) << ": [" + move.format() + "] " << fmt_moves(newcandidates) << endl;
            }

            if(depth > maxdepth){
                // skip move if filter says move is not worth to search deeper...
                if(filter(move, depth) == false){
                    newscore = score + eval_board(move);
                    skip = true;
                }
            }

            //if(depth > 5 && depth <= 15){ 
            //    cout << ". "; 
            //}
            //else if(depth > 15 && depth <= 20){ 
            //    cout << ": "; 
            //}
            //else if(depth > 20){ 
            //    cout << "_ ";
            //}

            if(skip == false){
                // debug
                //uint8_t newfields[8][8];
                //board.debug_copy_fields(newfields);

                do_move(move);

                alphabeta(newscore, newcandidates, depth + 1, maxdepth, alpha, beta, !maximizing, threadid);

                // debug
                //if(debug == true){
                //    calc_score = 0;
                //    return;
                //}

                undo_move();
            }

            // debug
            //if(board.debug_check_flags() == false || board.debug_compare_fields(newfields) == false){
                //cout << "bug found: " << move->format() << endl;
                //board.prnt();
                //prnt_minutes();
                //debug = true;
                //return 0;
            //}

            if(maximizing){
                if(newscore > bestscore){
                    bestscore = newscore;
                    alpha = max(bestscore, alpha);
                    append_newmove(rcandidates, newcandidates, move);
                    if(depth == 1){
                        cout << "\n!!!CANDIDATE from thread #" << to_string(threadid) << ": " << dec << bestscore << fmt_moves(rcandidates) << endl;
                    }
                    if(alpha >= beta){
                        break;
                    }
                }
            }
            else{
                if(newscore < bestscore){
                    bestscore = newscore;
                    beta = min(bestscore, beta);
                    append_newmove(rcandidates, newcandidates, move);
                    if(depth == 1){
                        cout << "\n!!!CANDIDATE from thread #" << to_string(threadid) << ": " << dec << bestscore << fmt_moves(rcandidates) << endl;
                    }
                    if(beta <= alpha){
                        break;
                    }
                }
            }
        }

        moves.clear();

        calc_score = bestscore;

    }


    void cMatch::alphabeta_v2(int32_t &calc_score, vector<cMove> &rcandidates, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta, bool maximizing, uint8_t threadid){

        vector<cMove> newcandidates;
        int32_t newscore, bestscore;
        u_int8_t count = 0;

        // debug
        //if(debug == true){
        //    calc_score = 0;
        //    return;
        //}

        if(maximizing){
            bestscore = SCORES[mWKG] * 10;
        }
        else{
            bestscore = SCORES[mBKG] * 10;
        }

        vector<cMove> moves;
        gen_moves(moves, next_color());

        if(is_calc_term(calc_score, moves, depth)){
            moves.clear();
            return;
        }

        for(cMove move : moves){
            count++;
            newcandidates.clear();
            bool skip = false;

            
            if(depth == 1){
                cout << "\n" << to_string(count) << "(" + to_string(moves.size()) + ") CURRENT SEARCH from thread #" << to_string(threadid) << ": [" + move.format() + "] " << fmt_moves(newcandidates) << endl;
            }

            if(depth == 5){
                if(threadid != 0 && (count % MAXTHREADS) + 1 != threadid){ 
                    continue;
                }
            }

            if(depth > maxdepth){
                // skip move if filter says move is not worth to search deeper...
                if(filter(move, depth) == false){
                    newscore = score + eval_board(move);
                    skip = true;
                }
            }

            //if(depth > 5 && depth <= 15){ 
            //    cout << ". "; 
            //}
            //else if(depth > 15 && depth <= 20){ 
            //    cout << ": "; 
            //}
            //else if(depth > 20){ 
            //    cout << "_ ";
            //}

            if(skip == false){
                // debug
                //uint8_t newfields[8][8];
                //board.debug_copy_fields(newfields);

                do_move(move);

                if(depth == 4 && MAXTHREADS > 1){

                    array<vector<cMove>, MAXTHREADS> candidates;

                    array<cMatch*, MAXTHREADS> matches;

                    thread threads[MAXTHREADS];

                    int32_t scores[MAXTHREADS];

                    for(uint8_t idx = 0; idx < MAXTHREADS; ++idx){
                        cMatch *match = new cMatch(*this);
                        matches[idx] = match; //new cMatch(*this);
                        threads[idx] = thread(&cMatch::alphabeta, match, ref(scores[idx]), ref(candidates[idx]), depth + 1, maxdepth, alpha, beta, maximizing, idx + 1);
                    }

                    for(uint8_t idx = 0; idx < MAXTHREADS; ++idx){
                        threads[idx].join();
                        //cout << "\nthread #" << (idx + 1) << " joined ";
                        //cout << scores[idx] << " " << fmt_moves(candidates[idx]) << endl;
                        delete matches[idx];
                    }

                    newscore = scores[0];
                    newcandidates.assign(candidates[0].begin(), candidates[0].end());
                    for(uint8_t idx = 1; idx < MAXTHREADS; ++idx){
                        if(maximizing){
                            if(scores[idx] > newscore){
                                newscore = scores[idx];
                                newcandidates.clear();
                                newcandidates.assign(candidates[idx].begin(), candidates[idx].end());
                            }
                        }
                        else{
                            if(scores[idx] < newscore){
                                newscore = scores[idx];
                                newcandidates.clear();
                                newcandidates.assign(candidates[idx].begin(), candidates[idx].end());
                            }
                        }
                    }
                }
                else{
                    alphabeta(newscore, newcandidates, depth + 1, maxdepth, alpha, beta, !maximizing, threadid);
                }

                // debug
                //if(debug == true){
                //    calc_score = 0;
                //    return;
                //}

                undo_move();
            }

            // debug
            //if(board.debug_check_flags() == false || board.debug_compare_fields(newfields) == false){
                //cout << "bug found: " << move->format() << endl;
                //board.prnt();
                //prnt_minutes();
                //debug = true;
                //return 0;
            //}

            if(maximizing){
                if(newscore > bestscore){
                    bestscore = newscore;
                    alpha = max(bestscore, alpha);
                    append_newmove(rcandidates, newcandidates, move);
                    if(depth == 1){
                        cout << "\n!!!CANDIDATE from thread #" << to_string(threadid) << ": " << dec << bestscore << fmt_moves(rcandidates) << endl;
                    }
                    if(alpha >= beta){
                        break;
                    }
                }
            }
            else{
                if(newscore < bestscore){
                    bestscore = newscore;
                    beta = min(bestscore, beta);
                    append_newmove(rcandidates, newcandidates, move);
                    if(depth == 1){
                        cout << "\n!!!CANDIDATE from thread #" << to_string(threadid) << ": " << dec << bestscore << fmt_moves(rcandidates) << endl;
                    }
                    if(beta <= alpha){
                        break;
                    }
                }
            }
        }

        moves.clear();

        calc_score = bestscore;

    }


    bool cMatch::is_running_pawn(uint8_t piece, uint8_t src_x, uint8_t src_y){

        int8_t xsteps[3] = { 1, 0, -1 };
        

        for(uint8_t i = 0; i < 3; ++i){
            if(cBoard::is_inbounds(src_x + xsteps[i], src_y) == false){
                continue;
            }

            if(PIECES_COLORS[piece] == mWHITE){
                for(uint8_t y = src_y; y < 7; ++y){
                    if(board.getfield(src_x + xsteps[i], y) == mBPW){
                        return false;
                    }
                }
            }
            else{
                for(uint8_t y = src_y; y > 0; --y){
                    if(board.getfield(src_x + xsteps[i], y) == mWPW){
                        return false;
                    }
                }
            }
        }

        return true;

    }


    int32_t cMatch::eval_board(cMove &move){

        int32_t rscore = 0;

        // opening
        if(minutes.size() <= 20){

            // penalty for move repetition
            if(minutes.size() >= 2){
                uint8_t prev_prev_pos = minutes.size() - 2;
                cMove pre_prev_move = minutes.at(prev_prev_pos);

                if(move.dst_x == pre_prev_move.src_x && move.dst_y ==  pre_prev_move.src_y && move.src_x == pre_prev_move.dst_x &&  move.src_y == pre_prev_move.dst_y){
                    if(PIECES_COLORS[move.srcpiece] == mWHITE){
                        rscore += SCORES[mBPLUS] * 4;
                    }
                    else{
                        rscore += SCORES[mWPLUS] * 4;
                    }
                }
            }

            // penalty for inactive pieces
            cPiece white_pieces[4] = { cPiece(mWKN, 1, 0), cPiece(mWKN, 6, 0), cPiece(mWBP, 2, 0), cPiece(mWBP, 5, 0) };
            uint8_t wcnt = 0;
            for(cPiece wpiece : white_pieces){
                if(board.getfield(wpiece.src_x, wpiece.src_y) == wpiece.piece){
                    wcnt++;
                }
            }
            if(wcnt > 2){
                rscore += SCORES[mBPLUS]; // penalty
            }

            cPiece black_pieces[4] = { cPiece(mBKN, 1, 7), cPiece(mBKN, 6, 7), cPiece(mBBP, 2, 7), cPiece(mBBP, 5, 7) };
            uint8_t bcnt = 0;
            for(cPiece bpiece : black_pieces){
                if(board.getfield(bpiece.src_x, bpiece.src_y) == bpiece.piece){
                    bcnt++;
                }
            }
            if(bcnt > 2){
                rscore += SCORES[mWPLUS]; // penalty
            }
            
            wcnt = 0;
            for(uint8_t x = 0; x <= 7; ++x){
                if(board.getfield(x, 1) == mWPW){
                    wcnt++;
                }
            }
            if(wcnt >= 6){
                rscore += SCORES[mBPLUS]; // penalty
            }

            bcnt = 0;
            for(uint8_t x = 0; x <= 7; ++x){
                if(board.getfield(x, 6) == mBPW){
                    bcnt++;
                }
            }
            if(bcnt >= 6){
                rscore += SCORES[mWPLUS]; // penalty
            }

            // pawns on central fields
            vector<cPiece> wpieces;
            vector<cPiece> bpieces;
            for(uint8_t y = 3; y <= 4; ++y){
                for(uint8_t x = 3; x <= 4; ++x){
                    board.search_for_touching_pieces(wpieces, x, y, mWHITE, true);

                    board.search_for_touching_pieces(bpieces, x, y, mBLACK, true);
                }
            }
            for(cPiece wpiece : wpieces){
                if(wpiece.piece == mWPW){
                    rscore += SCORES[mWPLUS];
                    if(wpiece.src_y == 3){
                        rscore += SCORES[mWPLUS]; // addition plus for 4th rank
                    }
                }
            }
            for(cPiece bpiece : bpieces){
                if(bpiece.piece == mBPW){
                    rscore += SCORES[mBPLUS];
                    if(bpiece.src_y == 4){
                        rscore += SCORES[mBPLUS]; // addition plus for 5th rank
                    }
                }
            }
            
            // penalty for f2 / f7 moved pawns
            if(board.getfield(5, 1) == mBLK){
                rscore += SCORES[mBPLUS] * 3; // penalty
            }
            else if(board.getfield(5, 6) == mBLK){
                rscore += SCORES[mWPLUS] * 3; // penalty
            }

        }
        
        // opening and middlegame
        if(minutes.size() <= 32){
            
            int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

            //weak white king?
            for(uint8_t i = 0; i < 8; ++i){
                vector <cPiece> wpieces, bpieces;

                board.setfield(board.wKg_x, board.wKg_y, mBLK);

                if(board.is_inbounds(board.wKg_x + steps[i][0], board.wKg_y + steps[i][1])){
                    board.search_for_all_touching_pieces(wpieces, bpieces, board.wKg_x + steps[i][0], board.wKg_y + steps[i][1]);

                    if(wpieces.size() < bpieces.size()){
                        rscore += SCORES[mBPLUS] * 3; // penalty
                        board.setfield(board.wKg_x, board.wKg_y, mWKG);
                        break;
                    }
                }
                board.setfield(board.wKg_x, board.wKg_y, mWKG);
            }

            vector<cPiece> wpieces;
            board.search_dir_for_pieces(wpieces, board.wKg_x, board.wKg_y, 0, 1);

            if(wpieces.size() == 2){
                cPiece second = wpieces.at(1);
                if(second.piece == mBRK || second.piece == mBQU){
                    rscore += SCORES[mBPLUS] * 3; // penalty
                }
            }
            else if(wpieces.size() > 2){
                cPiece first = wpieces.at(0);
                cPiece second = wpieces.at(1);
                cPiece third = wpieces.at(2);
                if(second.piece == mBRK || second.piece == mBQU){
                    rscore += SCORES[mBPLUS] * 3; // penalty
                }
                else if((third.piece == mBRK || third.piece == mBQU)){
                    if((first.piece != mWPW && first.piece != mBPW) || (second.piece != mWPW && second.piece != mBPW)){
                        rscore += SCORES[mBPLUS] * 3; // penalty
                    }
                }
            }

            //weak black king?
            for(uint8_t i = 0; i < 8; ++i){
                vector <cPiece> wpieces, bpieces;

                board.setfield(board.bKg_x, board.bKg_y, mBLK);

                if(board.is_inbounds(board.bKg_x + steps[i][0], board.bKg_y + steps[i][1])){
                    board.search_for_all_touching_pieces(wpieces, bpieces, board.bKg_x + steps[i][0], board.bKg_y + steps[i][1]);

                    if(bpieces.size() < wpieces.size()){
                        rscore += SCORES[mWPLUS] * 3; // penalty
                        board.setfield(board.bKg_x, board.bKg_y, mBKG);
                        break;
                    }
                }
                board.setfield(board.bKg_x, board.bKg_y, mBKG);
            }

            vector<cPiece> bpieces;
            board.search_dir_for_pieces(bpieces, board.bKg_x, board.bKg_y, 0, -1);

            if(bpieces.size() == 2){
                cPiece second = bpieces.at(1);
                if(second.piece == mWRK || second.piece == mWQU){
                    rscore += SCORES[mWPLUS] * 3; // penalty
                }
            }
            else if(bpieces.size() > 2){
                cPiece first = bpieces.at(0);
                cPiece second = bpieces.at(1);
                cPiece third = bpieces.at(2);
                if(second.piece == mWRK || second.piece == mWQU){
                    rscore += SCORES[mWPLUS] * 3; // penalty
                }
                else if((third.piece == mWRK || third.piece == mWQU)){
                    if((first.piece != mBPW && first.piece != mWPW) || (second.piece != mBPW && second.piece != mWPW)){
                        rscore += SCORES[mWPLUS] * 3; // penalty
                    }
                }
            }

        }

        // endgame - check pawns
        if(minutes.size() >= 32){
            for(uint8_t y = 1; y < 7; ++y){
                for(uint8_t x = 0; x < 7; ++x){
                    uint8_t pawn = board.getfield(x, y);
                    if(pawn == mWPW){
                        if(is_running_pawn(pawn, x, y)){
                            rscore += SCORES[mWPLUS] * y;
                        }
                    }
                    if(pawn == mBPW){
                        if(is_running_pawn(pawn, x, y)){
                            rscore += SCORES[mBPLUS] * (7 - y);
                        }
                    }
                }
            }
        }

        return rscore;

    }

