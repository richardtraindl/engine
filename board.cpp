

    #include "./board.hpp"


    cBoard::cBoard(){ 
    }


    // copy constructor
    cBoard::cBoard(const cBoard &board){

        for(uint8_t y = 0; y < 8; ++y){
            for(uint8_t x = 0; x < 8; ++x){
                m_fields[y][x] = board.m_fields[y][x];
            }
        }

        m_wKg_x = board.m_wKg_x;
        m_wKg_y = board.m_wKg_y;
        m_bKg_x = board.m_bKg_x;
        m_bKg_y = board.m_bKg_y;
        m_wKg_has_moved_at = board.m_wKg_has_moved_at;
        m_bKg_has_moved_at = board.m_bKg_has_moved_at;
        m_wRkA_has_moved_at = board.m_wRkA_has_moved_at;
        m_wRkH_has_moved_at = board.m_wRkH_has_moved_at;
        m_bRkA_has_moved_at = board.m_bRkA_has_moved_at;
        m_bRkH_has_moved_at = board.m_bRkH_has_moved_at;

    }


    uint8_t cBoard::getfield(uint8_t x, uint8_t y) const{
        return m_fields[y][x];
    }

    
    void cBoard::setfield(uint8_t x, uint8_t y, uint8_t value){
        m_fields[y][x] = (uint8_t)value;
    }


    bool cBoard::is_inbounds(uint8_t x, uint8_t y){
        return (x >= 0 && x <= 7 && y >= 0 && y <= 7);
    }


    bool cBoard::is_king_attacked(uint8_t attacker_color) const{

        if(attacker_color == mWHITE){
            return search_for_touching_piece(m_bKg_x, m_bKg_y, mWHITE);
        }
        else{
            return search_for_touching_piece(m_wKg_x, m_wKg_y, mBLACK);
        }

    }


    uint8_t cBoard::max_diff(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){

        uint8_t diffx = abs(x1 - x2);

        uint8_t diffy = abs(y1 - y2);

        return max(diffx, diffy);
    }


    uint8_t cBoard::diff_to_margin(uint8_t x, uint8_t y){

        uint8_t diffx = min((uint8_t)(7 - x), x);

        uint8_t diffy = min((uint8_t)(7 - y), y);

        return min(diffx, diffy);
    }


    bool cBoard::is_margin_frame_pos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){

        return ((y1 <= 2 && y2 <= 2) || (y1 >= 5 && y2 >= 5) || (x1 <= 2 && x2 <= 2) || (x1 >= 5 && x2 >= 5));

    }


    bool cBoard::is_margin_frame_ypos(uint8_t y1, uint8_t y2){

        return ((y1 <= 2 && y2 <= 2) || (y1 >= 5 && y2 >= 5));

    }


    bool cBoard::is_corner_pos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){

        return ((y1 <= 2 && x1 <= 2 && y2 <= 2 && x2 <= 2 ) || (y1 <= 2 && x1 >= 5 && y2 <= 2 && x2 >= 5) || (y1 >= 5 && x1 >= 5 && y2 >= 5 && x2 >= 2 ) || (y1 >= 5 && x1 <= 2 && y2 >= 5 && x2 <= 2));

    }


    bool cBoard::is_within_three_squares(uint8_t piece, uint8_t src_x, uint8_t src_y) const{

        for(int8_t a = -3; a <= 3; ++a){
            for(int8_t b = -3; b <= 3; ++b){
                if(is_inbounds(src_x + a, src_y + b)){
                    if(getfield(src_x + a, src_y + b) == piece){
                        return true;
                    }
                }
            }
        }

        return false;

    }


    bool cBoard::is_passed_pawn(uint8_t piece, uint8_t src_x, uint8_t src_y) const{

        if(piece == mWPW){
            if(src_y >= 6){
                return true;
            }

            for(uint8_t y = (src_y + 1); y < 7; ++y){
                for(int8_t i = -1; i <= 1; ++i){

                    if(is_inbounds((src_x + i), y)){
                        if(getfield((src_x + i), y) == mBPW){
                            return false;
                        }
                    }

                }
            }
        }
        else if(piece == mBPW){
            if(src_y <= 1){
                return true;
            }

            for(uint8_t y = (src_y - 1); y > 0; --y){
                for(int8_t i = -1; i <= 1; ++i){

                    if(is_inbounds((src_x + i), y)){
                        if(getfield((src_x + i), y) == mWPW){
                            return false;
                        }
                    }

                }
            }
        }
        else{
            return false;
        }

        return true;

    }


    bool cBoard::is_running_pawn(uint8_t piece, uint8_t src_x, uint8_t src_y) const{

        if(piece == mWPW){
            for(uint8_t y = (src_y + 1); y < 7; ++y){
                for(int8_t i = -1; i <= 1; ++i){

                    if(is_inbounds((src_x + i), y)){
                        uint8_t piece2 = getfield((src_x + i), y);

                        if(piece2 == mBPW){
                            return false;
                        }

                        if(i == 0 && PIECES_COLORS[piece2] == mBLACK){
                            return false;
                        }
                    }

                }
            }
        }
        else if(piece == mBPW){
            for(uint8_t y = (src_y - 1); y > 0; --y){
                for(int8_t i = -1; i <= 1; ++i){

                    if(is_inbounds((src_x + i), y)){
                        uint8_t piece2 = getfield((src_x + i), y);

                        if(piece2 == mWPW){
                            return false;
                        }

                        if(i == 0 && PIECES_COLORS[piece2] == mWHITE){
                            return false;
                        }
                    }

                }
            }
        }
        else{
            return false;
        }

        return true;

    }


    uint8_t cBoard::search_dir_for_piece(uint8_t &dst_x, uint8_t &dst_y, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y, uint8_t maxcnt) const{

        dst_x = src_x;
        dst_y = src_y;
        for(uint8_t cnt = 0; cnt < maxcnt; ++cnt){
            if(is_inbounds(dst_x + step_x, dst_y + step_y) == false){
                return mBLK;
            }
            dst_x += step_x;
            dst_y += step_y;

            uint8_t piece = getfield(dst_x, dst_y);
            if(piece != mBLK){ 
                return piece; 
            }
        }
        return mBLK;

    }


    void cBoard::search_dir_for_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y) const{

        uint8_t dst_x = src_x;
        uint8_t dst_y = src_y;
        while(is_inbounds(dst_x + step_x, dst_y + step_y)){
            uint8_t piece = getfield(dst_x, dst_y);
            if(piece != mBLK){
                pieces.push_back(cPiece(piece, dst_x, dst_y));
            }
            dst_x += step_x;
            dst_y += step_y;
        }

    }


    void cBoard::search_all_dirs_for_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, uint8_t piece, uint8_t piece_x, uint8_t piece_y, uint8_t excl_dir) const{

        uint8_t dstpiece, dst_x, dst_y;

        uint8_t idx, maxidx, cnt;

        if(piece == mWPW){
            idx = 4; maxidx = 6; cnt = 1;
        }
        else if(piece == mBPW){
            idx = 6; maxidx = 8; cnt = 1;
        }
        else if(piece == mWRK || piece == mBRK){
            idx = 0; maxidx = 4; cnt = 7;
        }
        else if(piece == mWBP || piece == mBBP){
            idx = 4; maxidx = 8; cnt = 7;
        }
        else if(piece == mWKG || piece == mBKG){
            idx = 0; maxidx = 8; cnt = 1;
        }
        else if(piece == mWQU || piece == mBQU){
            idx = 0; maxidx = 8; cnt = 8;
        }
        else{
            if(piece == mWKN || piece == mBKN){
                int8_t steps[][2] = { { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

                for(uint8_t i = 0; i < size(steps); ++i){
                    if(is_inbounds((piece_x + steps[i][0]), (piece_y + steps[i][1]))){
                        uint8_t dir = eval_dir(piece_x, piece_y, steps[i][0], steps[i][1]);
                        if(excl_dir != mNO_DIR && excl_dir == dir){
                            continue;
                        }
                    }
                    else{
                        continue;
                    }

                    dstpiece = search_dir_for_piece(dst_x, dst_y, piece_x, piece_y, steps[i][0], steps[i][1], 1);

                    if(PIECES_COLORS[dstpiece] == mWHITE){
                        wpieces.push_back(cPiece(dstpiece, dst_x, dst_y));
                    }
                    else if(PIECES_COLORS[dstpiece] == mBLACK){
                        bpieces.push_back(cPiece(dstpiece, dst_x, dst_y));
                    }
                }
            }
            return;
        }


        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };

        for(; idx < maxidx; ++idx){
            if(is_inbounds((piece_x + steps[idx][0]), (piece_y + steps[idx][1]))){
                uint8_t dir = eval_dir(piece_x, piece_y, (piece_x + steps[idx][0]), (piece_y + steps[idx][1]));

                if(excl_dir != mNO_DIR && excl_dir == dir){
                    continue;
                }
            }
            else{
                continue;
            }

            dstpiece = search_dir_for_piece(dst_x, dst_y, piece_x, piece_y, steps[idx][0], steps[idx][1], cnt);

            if(PIECES_COLORS[dstpiece] == mWHITE){
                wpieces.push_back(cPiece(dstpiece, dst_x, dst_y));
            }
            else if(PIECES_COLORS[dstpiece] == mBLACK){
                bpieces.push_back(cPiece(dstpiece, dst_x, dst_y));
            }
        }

    }


    bool cBoard::search_for_touching_piece(uint8_t src_x, uint8_t src_y, uint8_t color) const{

        uint8_t piece, dst_x, dst_y;

        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(uint8_t i = 0; i < size(steps); ++i){

            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 7);

            if(PIECES_COLORS[piece] == color){
                if(piece == mWQU || piece == mBQU){
                    return true;
                }

                if((i < 4) && (piece == mWRK || piece == mBRK)){
                    return true;
                }

                if((i >= 4) && (piece == mWBP || piece == mBBP)){
                    return true;
                }
            }
        }

        for(uint8_t i = 0; i < size(steps); ++i){

            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 1);

            if(PIECES_COLORS[piece] == color){
                if(piece == mWKG || piece == mBKG){
                    return true;
                }
            }
        }

        int8_t kn_steps[][2] = { { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

        for(uint8_t i = 0; i < size(kn_steps); ++i){

            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, kn_steps[i][0], kn_steps[i][1], 1);

            if(PIECES_COLORS[piece] == color){
                if(piece == mWKN || piece == mBKN){
                    return true;
                }
            }
        }

        if(color == mWHITE){
            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, -1, -1, 1);
            if(piece == mWPW){
                return true;
            }

            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, 1, -1, 1);
            if(piece == mWPW){
                return true;
            }
        }
        else if(color == mBLACK){
            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, 1, 1, 1);
            if(piece == mBPW){
                return true;
            }

            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, -1, 1, 1);
            if(piece == mBPW){
                return true;
            }
        }

        return false;

    }


    void cBoard::search_for_touching_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, uint8_t color, bool touching_only) const{

        uint8_t piece, dst_x, dst_y;

        // qu, rk and bp
        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(uint8_t i = 0; i < size(steps); ++i){
            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 7);

            if(PIECES_COLORS[piece] == color){
                if(piece == mWQU || piece == mBQU){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                    continue;
                }

                if((i < 4) && (piece == mWRK || piece == mBRK)){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                    continue;
                }

                if((i >= 4) && (piece == mWBP || piece == mBBP)){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                    continue;
                }
            }
        }
        
        // kg
        for(uint8_t i = 0; i < size(steps); ++i){
            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 1);

            if(PIECES_COLORS[piece] == color){
                if(piece == mWKG || piece == mBKG){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                }
            }
        }

        // kn
        int8_t kn_steps[][2] = { { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

        for(uint8_t i = 0; i < size(kn_steps); ++i){
             piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, kn_steps[i][0], kn_steps[i][1], 1);

            if(PIECES_COLORS[piece] == color){
                if(piece == mWKN || piece == mBKN){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                }
            }
        }

        // white pawn attacks
        if(color == mWHITE){
            if(touching_only){
                piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, -1, -1, 1);
                if(piece == mWPW){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                }

                piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, 1, -1, 1);
                if(piece == mWPW){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                }
            }
            else{
                if(is_inbounds(src_x, src_y - 1)){
                    if(getfield(src_x, src_y - 1) == mWPW){
                        pieces.push_back(cPiece(mWPW, src_x, src_y - 1));
                    }
                    if(getfield(src_x, src_y - 1) == mBLK){
                        if(src_y - 2 == 1){
                            if(getfield(src_x, src_y - 2) == mWPW){
                                pieces.push_back(cPiece(mWPW, src_x, src_y - 2));
                            }
                        }
                    }
                }
            }
        }

        // black pawn attacks
        if(color == mBLACK){
            if(touching_only){
                piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, 1, 1, 1);
                if(piece == mBPW){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                }

                piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, -1, 1, 1);
                if(piece == mBPW){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                }
            }
            else{
                if(is_inbounds(src_x, src_y + 1)){
                    if(getfield(src_x, src_y + 1) == mBPW){
                        pieces.push_back(cPiece(mBPW, src_x, src_y + 1));
                    }
                    if(getfield(src_x, src_y + 1) == mBLK){
                        if(src_y + 2 == 6){
                            if(getfield(src_x, src_y + 2) == mBPW){
                                pieces.push_back(cPiece(mBPW, src_x, src_y + 2));
                            }
                        }
                    }
                }
            }
        }

    }


    void cBoard::search_for_all_touching_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, uint8_t src_x, uint8_t src_y) const{

        uint8_t piece, dst_x, dst_y;

        // qu, rk and bp
        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(uint8_t i = 0; i < size(steps); ++i){
            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 7);

            if(piece == mBLK){
                continue;
            }

            // skip if piece is pinned - start
            uint8_t pindir = eval_pindir(dst_x, dst_y, PIECES_COLORS[piece]);
            
            uint8_t search_dir = eval_dir(src_x, src_y, dst_x, dst_y);

            if(pindir != mNO_DIR && pindir != search_dir){
                continue;
            }
            // skip if piece is pinned - end

            if(piece == mWQU){
                wpieces.push_back(cPiece(piece, dst_x, dst_y));
                continue;
            }
            else if(piece == mBQU){
                bpieces.push_back(cPiece(piece, dst_x, dst_y));
                continue;
            }
            else if(i < 4 && piece == mWRK){
                wpieces.push_back(cPiece(piece, dst_x, dst_y));
                continue;
            }
            else if(i < 4 && piece == mBRK){
                bpieces.push_back(cPiece(piece, dst_x, dst_y));
                continue;
            }
            else if(i >= 4 && piece == mWBP){
                wpieces.push_back(cPiece(piece, dst_x, dst_y));
                continue;
            }
            else if(i >= 4 && piece == mBBP){
                bpieces.push_back(cPiece(piece, dst_x, dst_y));
                continue;
            }
        }
        
        // kg
        for(uint8_t i = 0; i < size(steps); ++i){
            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 1);

            if(piece == mWKG){
                wpieces.push_back(cPiece(piece, dst_x, dst_y));
            }
            else if(piece == mBKG){
                bpieces.push_back(cPiece(piece, dst_x, dst_y));
            }
        }

        // kn
        int8_t kn_steps[][2] = { { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

        for(uint8_t i = 0; i < size(kn_steps); ++i){
             piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, kn_steps[i][0], kn_steps[i][1], 1);

            if(piece == mWKN){
                wpieces.push_back(cPiece(piece, dst_x, dst_y));
            }
            else if(piece == mBKN){
                bpieces.push_back(cPiece(piece, dst_x, dst_y));
            }
        }

        // white pawn attacks
        piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, -1, -1, 1);
        if(piece == mWPW){
            wpieces.push_back(cPiece(piece, dst_x, dst_y));
        }

        piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, 1, -1, 1);
        if(piece == mWPW){
            wpieces.push_back(cPiece(piece, dst_x, dst_y));
        }

        // black pawn attacks
        piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, 1, 1, 1);
        if(piece == mBPW){
            bpieces.push_back(cPiece(piece, dst_x, dst_y));
        }

        piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, -1, 1, 1);
        if(piece == mBPW){
            bpieces.push_back(cPiece(piece, dst_x, dst_y));
        }

    }


    void cBoard::new_search_for_touched_pieces(vector<cPiece> &found_pieces, cPiece &piece, bool excl_pindir) const{

        uint8_t pindir = mNO_DIR;

        uint8_t startidx, endidx, maxcnt;

        const int8_t steps[20][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 }, { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

        if(excl_pindir){
            pindir = eval_pindir(piece.m_xpos, piece.m_ypos, PIECES_COLORS[piece.m_piece]);
        }

        if(piece.m_piece == mWPW){
            startidx = 16;
            endidx = 17;
            maxcnt = 1;
        }
        else if(piece.m_piece == mBPW){
            startidx = 18;
            endidx = 19;
            maxcnt = 1;
        }
        else if(piece.m_piece == mWKN || piece.m_piece == mBKN){
            startidx = 8;
            endidx = 15;
            maxcnt = 1;
        }
        else if(piece.m_piece == mWRK || piece.m_piece == mBRK){
            startidx = 0;
            endidx = 3;
            maxcnt = 7;
        }
        else if(piece.m_piece == mWBP || piece.m_piece == mBBP){
            startidx = 4;
            endidx = 7;
            maxcnt = 7;
        }
        else if(piece.m_piece == mWKG || piece.m_piece == mBKG){
            startidx = 0;
            endidx = 7;
            maxcnt = 1;
        }
        else if(piece.m_piece == mWQU || piece.m_piece == mBQU){
            startidx = 0;
            endidx = 7;
            maxcnt = 7;
        }
        else{
            return;
        }

        for(uint8_t i = startidx; i <= endidx; ++i){

            if(is_inbounds((piece.m_xpos + steps[i][0]), (piece.m_ypos + steps[i][1]))){
                uint8_t dir = eval_dir(piece.m_xpos, piece.m_ypos, (piece.m_xpos + steps[i][0]), (piece.m_ypos + steps[i][1]));

                if(pindir != mNO_DIR && dir != pindir){
                    continue;
                }
            }
            else{
                continue;
            }

            uint8_t dst_x, dst_y;
            uint8_t dstpiece = search_dir_for_piece(dst_x, dst_y, piece.m_xpos, piece.m_ypos, steps[i][0], steps[i][1], maxcnt);

            if(dstpiece != mBLK){
                found_pieces.push_back(cPiece(dstpiece, dst_x, dst_y));
            }

        }

    }


    void cBoard::new_search_for_touching_pieces(vector<cTouchedPiece> &found_pieces, uint8_t src_x, uint8_t src_y, vector<uint8_t> &search_dirs, bool excl_pindir) const{

        uint8_t startidx, endidx;

        int8_t steps[8][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(const uint8_t &search_dir : search_dirs){

            if(search_dir == mEST_WST){
                startidx = 0;
                endidx = 1;
            }
            else if(search_dir == mSTH_NTH){
                startidx = 2;
                endidx = 3;
            }
            else if(search_dir == mSTHWST_NTHEST){
                startidx = 3;
                endidx = 4;
            }
            else if(search_dir == mSTHEST_NTHWST){
                startidx = 5;
                endidx = 6;
            }
            else{
                continue;
            }

            for(uint8_t i = startidx; i <= endidx; ++i){

                // TODO pindir

                uint8_t dst_x, dst_y;
                uint8_t dstpiece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 7);

                if(dstpiece != mBLK){
                    found_pieces.push_back(cTouchedPiece(dstpiece, dst_x, dst_y, mNO_DIR));
                }

            }
        
        }

    }


    uint8_t cBoard::eval_dir(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){

        if(y1 == y2){ return mEST_WST; }
        if(x1 == x2){ return mSTH_NTH; }
        if(x1 - y1 == x2 - y2){ return mSTHWST_NTHEST; }
        if(x1 + y1 == x2 + y2){ return mSTHEST_NTHWST; }
        if( (abs(x1 + y1) == 2) && (abs(x2 + y2) == 1) ){ return mKNIGHT_DIR; }
        if( (abs(x1 + y1) == 1) && (abs(x2 + y2) == 2) ){ return mKNIGHT_DIR; }
        else{ return mNO_DIR; }

    }


    uint8_t cBoard::eval_pindir(uint8_t src_x, uint8_t src_y, uint8_t color) const{

        uint8_t king, kg_x, kg_y, enmycolor;

        if(color == mWHITE){
            king = mWKG;
            kg_x = m_wKg_x;
            kg_y = m_wKg_y;
            enmycolor = mBLACK;
        }
        else{
            king = mBKG;
            kg_x = m_bKg_x;
            kg_y = m_bKg_y;
            enmycolor = mWHITE;
        }

        uint8_t dir = eval_dir(src_x, src_y, kg_x, kg_y);
        if(dir == mNO_DIR){ return mNO_DIR; }

        if(dir == mEST_WST){
            uint8_t dst1_x, dst1_y, dst2_x, dst2_y;
            uint8_t piece1 = search_dir_for_piece(dst1_x, dst1_y, src_x, src_y, 1, 0, 7);
            uint8_t piece2 = search_dir_for_piece(dst2_x, dst2_y, src_x, src_y, -1, 0, 7);

            if((piece1 == king || piece2 == king) && (PIECES_COLORS[piece1] == enmycolor || PIECES_COLORS[piece2] == enmycolor)){
                if(piece1 == king && (piece2 == mWQU || piece2 == mBQU || piece2 == mWRK || piece2 == mBRK)){
                    return mEST_WST;
                }
                if(piece2 == king && (piece1 == mWQU || piece1 == mBQU || piece1 == mWRK || piece1 == mBRK)){
                    return mEST_WST;
                }
            }
            
            return mNO_DIR;
        }

        if(dir == mSTH_NTH){
            uint8_t dst1_x, dst1_y, dst2_x, dst2_y;
            uint8_t piece1 = search_dir_for_piece(dst1_x, dst1_y, src_x, src_y, 0, 1, 7);
            uint8_t piece2 = search_dir_for_piece(dst2_x, dst2_y, src_x, src_y, 0, -1, 7);

            if((piece1 == king || piece2 == king) && (PIECES_COLORS[piece1] == enmycolor || PIECES_COLORS[piece2] == enmycolor)){
                if(piece1 == king && (piece2 == mWQU || piece2 == mBQU || piece2 == mWRK || piece2 == mBRK)){
                    return mSTH_NTH;
                }
                if(piece2 == king && (piece1 == mWQU || piece1 == mBQU || piece1 == mWRK || piece1 == mBRK)){
                    return mSTH_NTH;
                }
            }
            
            return mNO_DIR;
        }

        if(dir == mSTHWST_NTHEST){
            uint8_t dst1_x, dst1_y, dst2_x, dst2_y;
            uint8_t piece1 = search_dir_for_piece(dst1_x, dst1_y, src_x, src_y, 1, 1, 7);
            uint8_t piece2 = search_dir_for_piece(dst2_x, dst2_y, src_x, src_y, -1, -1, 7);

            if((piece1 == king || piece2 == king) && (PIECES_COLORS[piece1] == enmycolor || PIECES_COLORS[piece2] == enmycolor)){
                if(piece1 == king && (piece2 == mWQU || piece2 == mBQU || piece2 == mWBP || piece2 == mBBP)){
                    return mSTHWST_NTHEST;
                }
                if(piece2 == king && (piece1 == mWQU || piece1 == mBQU || piece1 == mWBP || piece1 == mBBP)){
                    return mSTHWST_NTHEST;
                }
            }
            
            return mNO_DIR;
        }

        if(dir == mSTHEST_NTHWST){
            uint8_t dst1_x, dst1_y, dst2_x, dst2_y;
            uint8_t piece1 = search_dir_for_piece(dst1_x, dst1_y, src_x, src_y, -1, 1, 7);
            uint8_t piece2 = search_dir_for_piece(dst2_x, dst2_y, src_x, src_y, 1, -1, 7);
 
            if((piece1 == king || piece2 == king) && (PIECES_COLORS[piece1] == enmycolor || PIECES_COLORS[piece2] == enmycolor)){
                if(piece1 == king && (piece2 == mWQU || piece2 == mBQU || piece2 == mWBP || piece2 == mBBP)){
                    return mSTHEST_NTHWST;
                }
                if(piece2 == king && (piece1 == mWQU || piece1 == mBQU || piece1 == mWBP || piece1 == mBBP)){
                    return mSTHEST_NTHWST;
                }
            }
            
            return mNO_DIR;
        }

        return mNO_DIR;

    }


    uint8_t cBoard::eval_pin_state(uint8_t piece, uint8_t x, uint8_t y) const{

        uint8_t state = PINNED_NO;

        uint8_t piece1, dst1_x, dst1_y, piece2, dst2_x, dst2_y;

        int8_t steps1[][2] = { { 1, 0 }, { 0, 1 }, { 1, 1 }, { -1, 1 } };

        int8_t steps2[][2] = { { -1, 0 }, { 0, -1 }, { -1, -1 }, { 1, -1 } };

        for(uint8_t i = 0; i < size(steps1); ++i){
            piece1 = search_dir_for_piece(dst1_x, dst1_y, x, y, steps1[i][0], steps1[i][1], 6);
            if(piece1 == mBLK){ continue; }

            piece2 = search_dir_for_piece(dst2_x, dst2_y, x, y, steps2[i][0], steps2[i][1], 6);
            if(piece2 == mBLK){ continue; }

            if(piece1 != mBLK && piece2 != mBLK && PIECES_COLORS[piece1] != PIECES_COLORS[piece2]){
                if(PIECES_COLORS[piece1] == PIECES_COLORS[piece]){
                    if(PIECES_RANKS[piece1] > PIECES_RANKS[piece]){
                        if(piece2 == mWQU || piece2 == mBQU){
                            if(PIECES_RANKS[piece1] == PIECES_RANKS[mWKG]){
                                return PINNED_HARD;
                            }
                            else{
                                state = PINNED_SOFT;
                                continue;
                            }
                        }

                        if(i < 2 && (piece2 == mWRK || piece2 == mBRK)){
                            if(PIECES_RANKS[piece1] == PIECES_RANKS[mWKG]){
                                return PINNED_HARD;
                            }
                            else{
                                state = PINNED_SOFT;
                                continue;
                            }
                        }

                        if(i >= 2 && (piece2 == mWBP || piece2 == mBBP)){
                            if(PIECES_RANKS[piece1] == PIECES_RANKS[mWKG]){
                                return PINNED_HARD;
                            }
                            else{
                                state = PINNED_SOFT;
                                continue;
                            }
                        }
                    }
                }
                else{
                    if(PIECES_RANKS[piece2] > PIECES_RANKS[piece]){
                        if(piece1 == mWQU || piece1 == mBQU){
                            if(PIECES_RANKS[piece1] == PIECES_RANKS[mWKG]){
                                return PINNED_HARD;
                            }
                            else{
                                state = PINNED_SOFT;
                                continue;
                            }
                        }

                        if(i < 2 && (piece1 == mWRK || piece1 == mBRK)){
                            if(PIECES_RANKS[piece1] == PIECES_RANKS[mWKG]){
                                return PINNED_HARD;
                            }
                            else{
                                state = PINNED_SOFT;
                                continue;
                            }
                        }

                        if(i >= 2 && (piece1 == mWBP || piece1 == mBBP)){
                            if(PIECES_RANKS[piece1] == PIECES_RANKS[mWKG]){
                                return PINNED_HARD;
                            }
                            else{
                                state = PINNED_SOFT;
                                continue;
                            }
                        }
                    }
                }
            }
        }

        return state;

    }


    int8_t cBoard::eval_field_state(uint8_t x, uint8_t y) const{
        uint8_t piece = getfield(x, y);
        uint8_t wtouchcnt = 0;
        uint8_t btouchcnt = 0;
        uint8_t wsoftpincnt = 0;
        uint8_t bsoftpincnt = 0;
        uint8_t wlowesttouch = mWKG;
        uint8_t blowesttouch = mBKG;

        vector<cPiece> wpieces, bpieces;
        search_for_all_touching_pieces(wpieces, bpieces, x, y);

        for(const cPiece &wpiece : wpieces){
            uint8_t state = eval_pin_state(wpiece.m_piece, wpiece.m_xpos, wpiece.m_ypos);
            if(state == cBoard::PINNED_NO){
                wtouchcnt++;
            }
            else if(state == cBoard::PINNED_SOFT){
                wsoftpincnt++;
            }
            else{
                continue;
            }
            if(PIECES_RANKS[wpiece.m_piece] < PIECES_RANKS[wlowesttouch]){
                wlowesttouch = wpiece.m_piece;
            }
        }

        uint8_t wadvant = (wtouchcnt * 2) + wsoftpincnt;

        for(const cPiece &bpiece : bpieces){
            uint8_t state = eval_pin_state(bpiece.m_piece, bpiece.m_xpos, bpiece.m_ypos);
            if(state == cBoard::PINNED_NO){
                btouchcnt++;
            }
            else if(state == cBoard::PINNED_SOFT){
                bsoftpincnt++;
            }
            else{
                continue;
            }
            if(PIECES_RANKS[bpiece.m_piece] < PIECES_RANKS[blowesttouch]){
                blowesttouch = bpiece.m_piece;
            }
        }

        uint8_t badvant = (btouchcnt * 2) + bsoftpincnt;

        if(PIECES_COLORS[piece] == mWHITE && badvant > 0 && PIECES_RANKS[blowesttouch] < PIECES_RANKS[piece]){
            return wadvant - badvant - (PIECES_RANKS[piece]- PIECES_RANKS[blowesttouch]);
        }
        else if(PIECES_COLORS[piece] == mBLACK && wadvant > 0 && PIECES_RANKS[wlowesttouch] < PIECES_RANKS[piece]){
            return wadvant - badvant + (PIECES_RANKS[piece]- PIECES_RANKS[wlowesttouch]);
        }
        else{
            return wadvant - badvant;
        }

    }


    /*int8_t cBoard::eval_field_state(uint8_t x, uint8_t y) const{

        cBoard board(*this);

        uint8_t piece = board.getfield(x, y);

        board.setfield(x, y, mBLK);

        vector<cPiece> wpieces, bpieces;
        board.search_for_all_touching_pieces(wpieces, bpieces, x, y);

        vector <cPiece>::iterator it;

        uint8_t lowestwhite = mWKG;
        uint8_t lowestblack = mBKG;

        // remove pinned white pieces
        int8_t wpincnt = 1;

        for(it = wpieces.begin(); it != wpieces.end();){
            uint8_t state = board.eval_pin_state(it->m_piece, it->m_src_x, it->m_src_y);
            if(state != PINNED_NO){
                if(state == PINNED_SOFT){
                    if(PIECES_RANKS[it->m_piece] < PIECES_RANKS[lowestwhite]){
                        lowestwhite = it->m_piece;
                        wpincnt++;
                    }
                }
                it = wpieces.erase(it);
            }
            else{
                it++;
            }
        }
        wpincnt = (wpincnt / 2);

        // remove pinned black pieces
        int8_t bpincnt = 0;

        for(it = bpieces.begin(); it != bpieces.end();){
            uint8_t state = board.eval_pin_state(it->m_piece, it->m_src_x, it->m_src_y);
            if(state != PINNED_NO){
                if(state == PINNED_SOFT){
                    if(PIECES_RANKS[it->m_piece] < PIECES_RANKS[lowestblack]){
                        lowestblack = it->m_piece;
                        bpincnt++;
                    }
                }
                it = bpieces.erase(it);
            }
            else{
                it++;
            }
        }
        bpincnt = (bpincnt / 2);

        // eval lowest touching white piece
        for(const cPiece &wpiece : wpieces){
            if(PIECES_RANKS[wpiece.m_piece] < PIECES_RANKS[lowestwhite]){
                lowestwhite = wpiece.m_piece;
            }
        }

        // eval lowest touching black piece
        for(const cPiece &bpiece : bpieces){
            if(PIECES_RANKS[bpiece.m_piece] < PIECES_RANKS[lowestblack]){
                lowestblack = bpiece.m_piece;
            }
        }

        // eval state

        int8_t adjust = 0;

        if((wpieces.size() + wpincnt) == 0 && (bpieces.size() + bpincnt) == 0){
            adjust = 0;
        }
        else if((wpieces.size() + wpincnt) > 0 && (bpieces.size() + bpincnt) == 0){
            adjust = 1;
        }
        else if((wpieces.size() + wpincnt) == 0 && (bpieces.size() + bpincnt) > 0){
            adjust = -1;
        }
        else if(piece == mBLK){
            if(PIECES_RANKS[lowestwhite] > PIECES_RANKS[lowestblack]){
                adjust = -1;
            }
            if(PIECES_RANKS[lowestwhite] == PIECES_RANKS[lowestblack]){
                adjust = 0;
            }
            else{
                adjust = 1;
            }
        }
        else if(PIECES_COLORS[piece] == mWHITE){
            if(PIECES_RANKS[lowestblack] > PIECES_RANKS[piece]){
                adjust = 1;
            }
            if(PIECES_RANKS[lowestblack] == PIECES_RANKS[piece]){
                adjust = 0;
            }
            else{
                adjust = -1;
            }
        }
        // PIECES_COLORS[piece] == mBLACK
        else{
            if(PIECES_RANKS[lowestwhite] > PIECES_RANKS[piece]){
                adjust = -1;
            }
            if(PIECES_RANKS[lowestwhite] == PIECES_RANKS[piece]){
                adjust = 0;
            }
            else{
                adjust = 1;
            }
        }
        
        if((wpieces.size() + wpincnt) > (bpieces.size() + bpincnt)){
            return 3 + adjust;
        }
        else if((wpieces.size() + wpincnt) == (bpieces.size() + bpincnt)){
            return 0 + adjust;
        }
        else{
            return -3 + adjust;
        }
            
        return 0;

    }*/


    bool cBoard::compare_fields(uint8_t fields[8][8]) const{

        for(uint8_t y = 0; y < 8; ++y){

            for(uint8_t x = 0; x < 8; ++x){

                if(m_fields[y][x] != fields[y][x]){
                    return false;
                }
            }

        }

        return true;

    }


    void cBoard::prnt() const{

        string textcolor, backcolor, strpiece;

        for(int8_t y = 7; y >=0; --y){

            for(uint8_t x = 0; x < 8; ++x){

                uint8_t piece = getfield(x, (uint8_t)y);

                if(piece == mBLK){
                    strpiece = "    ";
                }
                else{
                    strpiece = " " + reverse_lookup(PIECES, piece).substr (1,2) + " ";
                }

                if(PIECES_COLORS[piece] == mWHITE){
                    textcolor = (const string)"\033[97m" + (const string)"\033[1m";
                }
                else{
                    textcolor = (const string)"\033[30m" + (const string)"\033[1m";
                }

                if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
                    backcolor = (const string)"\033[104m";
                }
                else{
                    backcolor = (const string)"\033[42m";
                }

                cout << backcolor + textcolor + strpiece + (const string)"\033[0m"; 
            }
            cout << endl;
            
            for(uint8_t x = 0; x < 8; ++x){

                if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
                    cout << "\033[104m" + textcolor + "    " + (const string)"\033[0m"; 
                }
                else{
                    cout << "\033[42m" + textcolor + "    " + (const string)"\033[0m"; 
                }

            }

            cout << endl;

        }

    }


    void cBoard::copy_fields(uint8_t fields[8][8]) const{

        for(uint8_t y = 0; y < 8; ++y){

            for(uint8_t x = 0; x < 8; ++x){
                fields[y][x] = m_fields[y][x];
            }

        }

    }


    bool cBoard::debug_check_flags() const{

        if(getfield(m_wKg_x, m_wKg_y) != mWKG){ return false; }

        if(getfield(m_bKg_x, m_bKg_y) != mBKG){ return false; }

        for(uint8_t y = 0; y < 8; ++y){

            for(uint8_t x = 0; x < 8; ++x){

                if(m_fields[y][x] == mWKG && (x != m_wKg_x || y != m_wKg_y)){
                    return false;
                }

                if(m_fields[y][x] == mBKG && (x != m_bKg_x || y != m_bKg_y)){
                    return false;
                }

            }

        }

        return true;

    }

