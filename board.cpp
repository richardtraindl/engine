

    #include "./board.hpp"


    cBoard::cBoard(){ 
        wKg_x = 4;
        wKg_y = 0;
        bKg_x = 4;
        bKg_y = 7;
        wKg_has_moved_at = 0;
        bKg_has_moved_at = 0;
        wRkA_has_moved_at = 0;
        wRkH_has_moved_at = 0;
        bRkA_has_moved_at = 0;
        bRkH_has_moved_at = 0;
    }


    cBoard::cBoard(const cBoard &board){
    } // copy constructor


    uint8_t cBoard::getfield(uint8_t x, uint8_t y){
        return fields[y][x];
    }

    
    void cBoard::setfield(uint8_t x, uint8_t y, uint8_t value){
        fields[y][x] = (uint8_t)value;
    }


    bool cBoard::is_inbounds(uint8_t x, uint8_t y){
        return (x >= 0 && x <= 7 && y >= 0 && y <= 7);
    }


    uint8_t cBoard::search_dir_for_piece(uint8_t &dst_x, uint8_t &dst_y, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y, uint8_t maxcnt){

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


    void cBoard::search_dir_for_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y){

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


    bool cBoard::search_for_touching_piece(uint8_t src_x, uint8_t src_y, uint8_t color){

        uint8_t piece, dst_x, dst_y;

        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(uint8_t i = 0; i < 8; ++i){

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

        for(uint8_t i = 0; i < 8; ++i){

            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 1);

            if(PIECES_COLORS[piece] == color){
                if(piece == mWKG || piece == mBKG){
                    return true;
                }
            }
        }

        int8_t kn_steps[][2] = { { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

        for(uint8_t i = 0; i < 8; ++i){

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


    void cBoard::search_for_touching_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, uint8_t color, bool touching_only){

        uint8_t piece, dst_x, dst_y;

        // qu, rk and bp
        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(uint8_t i = 0; i < 8; ++i){
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
        for(uint8_t i = 0; i < 8; ++i){
            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 1);

            if(PIECES_COLORS[piece] == color){
                if(piece == mWKG || piece == mBKG){
                    pieces.push_back(cPiece(piece, dst_x, dst_y));
                }
            }
        }

        // kn
        int8_t kn_steps[][2] = { { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };

        for(uint8_t i = 0; i < 8; ++i){
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


    void cBoard::search_for_all_touching_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, uint8_t src_x, uint8_t src_y){

        uint8_t piece, dst_x, dst_y;

        // qu, rk and bp
        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };

        for(uint8_t i = 0; i < 8; ++i){
            piece = search_dir_for_piece(dst_x, dst_y, src_x, src_y, steps[i][0], steps[i][1], 7);

            if(piece == mBLK){
                continue;
            }

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
        for(uint8_t i = 0; i < 8; ++i){
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

        for(uint8_t i = 0; i < 8; ++i){
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


    void cBoard::piece_search_for_touched_pieces(vector<cPiece> &pieces, uint8_t piece, uint8_t piece_x, uint8_t piece_y, uint8_t color, uint8_t excl_dir){

        uint8_t dstpiece, dst_x, dst_y;

        int8_t steps[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };

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

                for(uint8_t i = 0; i < 8; ++i){
                    if(excl_dir != mNO_DIR && excl_dir == eval_dir(piece_x, piece_y, steps[i][0], steps[i][1])){
                        continue;
                    }

                    dstpiece = search_dir_for_piece(dst_x, dst_y, piece_x, piece_y, steps[i][0], steps[i][1], 1);

                    if(PIECES_COLORS[dstpiece] == color){
                        pieces.push_back(cPiece(dstpiece, dst_x, dst_y));
                    }
                }
            }
            return;
        }

        for(; idx < maxidx; ++idx){
            dstpiece = search_dir_for_piece(dst_x, dst_y, piece_x, piece_y, steps[idx][0], steps[idx][1], cnt);

            if(PIECES_COLORS[dstpiece] == color){
                pieces.push_back(cPiece(dstpiece, dst_x, dst_y));
            }
        }

    }


    uint8_t cBoard::eval_dir(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){

        if(y1 == y2){ return mEST_WST; }
        if(x1 == x2){ return mSTH_NTH; }
        if(x1 - y1 == x2 - y2){ return mSTHWST_NTHEST; }
        if(x1 + y1 == x2 + y2){ return mSTHEST_NTHWST; }
        else{ return mNO_DIR; }

    }


    uint8_t cBoard::eval_pindir(uint8_t src_x, uint8_t src_y, uint8_t color){

        uint8_t king, kg_x, kg_y, enmycolor;

        if(color == mWHITE){
            king = mWKG;
            kg_x = wKg_x;
            kg_y = wKg_y;
            enmycolor = mBLACK;
        }
        else{
            king = mBKG;
            kg_x = bKg_x;
            kg_y = bKg_y;
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


    void cBoard::prnt(){

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


    void cBoard::debug_copy_fields(uint8_t newfields[8][8]){

        for(uint8_t y = 0; y < 8; ++y){
            for(uint8_t x = 0; x < 8; ++x){
                newfields[y][x] = fields[y][x];
            }
        }

    }


    bool cBoard::debug_compare_fields(uint8_t newfields[8][8]){

        for(uint8_t y = 0; y < 8; ++y){
            for(uint8_t x = 0; x < 8; ++x){
                if(fields[y][x] != newfields[y][x]){
                    return false;
                }
            }
        }
        
        return true;

    }


    bool cBoard::debug_check_flags(){

        if(getfield(wKg_x, wKg_y) != mWKG){ return false; }

        if(getfield(bKg_x, bKg_y) != mBKG){ return false; }

        for(uint8_t y = 0; y < 8; ++y){
            for(uint8_t x = 0; x < 8; ++x){
                if(fields[y][x] == mWKG && (x != wKg_x || y != wKg_y)){
                    return false;
                }
                if(fields[y][x] == mBKG && (x != bKg_x || y != bKg_y)){
                    return false;
                }
            }
        }
        
        return true;

    }

