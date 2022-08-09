

  #include "./generator.hpp"
  #include "./evaluator.hpp"


  cGenerator::cGenerator(cMatch &match){

    m_matchptr = &match;

  }


  cGenerator::~cGenerator(){
  }


  void cGenerator::gen_moves(vector<cMove> &moves, uint8_t color){

        uint8_t king, kg_x, kg_y;

        if(color == mWHITE){
            king = mWKG;
            kg_x = m_matchptr->m_board.m_wKg_x;
            kg_y = m_matchptr->m_board.m_wKg_y;
        }
        else{
            king = mBKG;
            kg_x = m_matchptr->m_board.m_bKg_x;
            kg_y = m_matchptr->m_board.m_bKg_y;
        }

        vector<cPiece> attacking_pieces, supporting_pieces;
        if(color == mWHITE){
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(supporting_pieces, attacking_pieces, kg_x, kg_y, m_matchptr->get_last_move(), false);
        }
        else{
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(attacking_pieces, supporting_pieces, kg_x, kg_y, m_matchptr->get_last_move(), false);
        }

        if(attacking_pieces.size() >= 2){
            gen_kg_moves(moves, kg_x, kg_y, king);
        }
        else if(attacking_pieces.size() == 1){
            gen_kg_moves(moves, kg_x, kg_y, king);

            cPiece attacking_piece = attacking_pieces.front();

            gen_capture_checking_piece_moves(moves, attacking_piece); 

            gen_blocking_checking_piece_moves(moves, attacking_piece); 
        }
        else{
            for(uint8_t y = 0; y < 8; ++y){

                for(uint8_t x = 0; x < 8; ++x){

                    uint8_t piece = m_matchptr->m_board.getfield(x, y);

                    if(PIECES_COLORS[piece] == color){
                        if(piece == mWPW){ 
                            gen_wpw_moves(moves, x, y); 
                            continue; 
                        }
                        else if(piece == mBPW){ 
                            gen_bpw_moves(moves, x, y); 
                            continue; 
                        }
                        else if(piece == mWKG){ 
                            gen_kg_moves(moves, x, y, mWKG);
                            gen_wkg_castling_moves(moves, x, y);
                            continue; 
                        }
                        else if(piece == mBKG){ 
                            gen_kg_moves(moves, x, y, mBKG);
                            gen_bkg_castling_moves(moves, x, y);
                            continue; 
                        }
                        else if(piece == mWRK || piece == mBRK){ 
                            gen_qrb_moves(moves, x, y, piece, 'r');
                            continue; 
                        }
                        else if(piece == mWBP || piece == mBBP){ 
                            gen_qrb_moves(moves, x, y, piece, 'b');
                            continue; 
                        }
                        else if(piece == mWKN || piece == mBKN){ 
                            gen_kn_moves(moves, x, y, piece); 
                            continue;
                        }
                        else if(piece == mWQU || piece == mBQU){
                            gen_qrb_moves(moves, x, y, piece, 'q');
                            continue; 
                        }
                    }

                }

            }

            attacking_pieces.clear();
            supporting_pieces.clear();
        }

    }


    void cGenerator::add_wpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece){

        if(src_y == 6 && dst_y == 7){
            uint8_t prom_pieces[4] = { mWQU, mWRK, mWBP, mWKN };

            for(uint8_t i = 0; i < 4; ++i){

                cMove move(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, prom_pieces[i], cMove::P_LOW);

                cEvaluator::priorize_move(*m_matchptr, move);

                moves.push_back(move);

            }

        }
        else{
            cMove move(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mBLK, cMove::P_LOW);

            cEvaluator::priorize_move(*m_matchptr, move);

            moves.push_back(move);
        }
    }


    void cGenerator::gen_wpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        uint16_t pindir = m_matchptr->m_board.eval_pindir(x, y);

        for(uint8_t i = 0; i < 2; ++i){

            if(pindir == mNO_DIR || pindir == cPiece::wpw_capture_steps[i].m_cardinale){
                if(cBoard::is_inbounds(x + cPiece::wpw_capture_steps[i].m_xstep, y + cPiece::wpw_capture_steps[i].m_ystep)){
                    uint8_t dst_x = x + cPiece::wpw_capture_steps[i].m_xstep;
                    uint8_t dst_y = y + cPiece::wpw_capture_steps[i].m_ystep;

                    uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                    if(PIECES_COLORS[dstpiece] == mBLACK){
                        add_wpw_moves(moves, x, y, dst_x, dst_y, dstpiece);
                    }
                    else if(dstpiece == mBLK && y == 4){ // en passant
                        cMove prevmove = m_matchptr->m_minutes.back();
                        uint8_t enpiece = m_matchptr->m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                        if(enpiece == mBPW && prevmove.m_src_y == 6 && prevmove.m_dst_y == 4 && prevmove.m_src_x == dst_x){
                            cMove move(x, y, dst_x, dst_y, mWPW, dstpiece, mBLK, cMove::P_LOW);

                            cEvaluator::priorize_move(*m_matchptr, move);

                            moves.push_back(move);
                        }
                    }
                }
            }

        }

        if(pindir == mNO_DIR || pindir == mVERTICAL){
            if(cBoard::is_inbounds(x, y + cPiece::wpw_steps[0].m_ystep)){
                uint8_t dst_x = x;
                uint8_t dst_y = y + cPiece::wpw_steps[0].m_ystep;

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(dstpiece == mBLK){
                    add_wpw_moves(moves, x, y, dst_x, dst_y, dstpiece);

                    // double step
                    if(cBoard::is_inbounds(x, y + cPiece::wpw_steps[1].m_ystep)){
                        uint8_t dst_x = x;
                        uint8_t dst_y = y + cPiece::wpw_steps[1].m_ystep;

                        uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                        if(dstpiece == mBLK && y == 1){
                            add_wpw_moves(moves, x, y, dst_x, dst_y, dstpiece);
                        }
                    }
                }
            }
        }

    }


    void cGenerator::add_bpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece){

        if(src_y == 1 && dst_y == 0){
            uint8_t prom_pieces[4] = { mBQU, mBRK, mBBP, mBKN };

            for(uint8_t i = 0; i < 4; ++i){

                cMove move(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, prom_pieces[i], cMove::P_LOW);

                cEvaluator::priorize_move(*m_matchptr, move);

                moves.push_back(move);

            }

        }
        else{
            cMove move(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBLK, cMove::P_LOW);

            cEvaluator::priorize_move(*m_matchptr, move);

            moves.push_back(move);
        }

    }


    void cGenerator::gen_bpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y){

        uint16_t pindir = m_matchptr->m_board.eval_pindir(x, y);

        for(uint8_t i = 0; i < 2; ++i){

            if(pindir == mNO_DIR || pindir == cPiece::bpw_capture_steps[i].m_cardinale){
                if(cBoard::is_inbounds(x + cPiece::bpw_capture_steps[i].m_xstep, y + cPiece::bpw_capture_steps[i].m_ystep)){
                    uint8_t dst_x = x + cPiece::bpw_capture_steps[i].m_xstep;
                    uint8_t dst_y = y + cPiece::bpw_capture_steps[i].m_ystep;

                    uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                    if(PIECES_COLORS[dstpiece] == mWHITE){
                        add_bpw_moves(moves, x, y, dst_x, dst_y, dstpiece);
                    }
                    else if(dstpiece == mBLK && y == 3){ // en passant
                        cMove prevmove = m_matchptr->m_minutes.back();
                        uint8_t enpiece = m_matchptr->m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                        if(enpiece == mWPW && prevmove.m_src_y == 1 && prevmove.m_dst_y == 3 && prevmove.m_src_x == dst_x){
                            cMove move(x, y, dst_x, dst_y, mBPW, dstpiece, mBLK, cMove::P_LOW);

                            cEvaluator::priorize_move(*m_matchptr, move);

                            moves.push_back(move);
                        }

                    }
                }
            }

        }

        if(pindir == mNO_DIR || pindir == mVERTICAL){
            if(cBoard::is_inbounds(x, y + cPiece::bpw_steps[0].m_ystep)){
                uint8_t dst_x = x;
                uint8_t dst_y = y + cPiece::bpw_steps[0].m_ystep;

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(dstpiece == mBLK){
                    add_bpw_moves(moves, x, y, dst_x, dst_y, dstpiece);

                    // double step
                    if(cBoard::is_inbounds(x, y + cPiece::bpw_steps[1].m_ystep)){
                        uint8_t dst_x = x;
                        uint8_t dst_y = y + cPiece::bpw_steps[1].m_ystep;

                        uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                        if(dstpiece == mBLK && y == 6){
                            add_bpw_moves(moves, x, y, dst_x, dst_y, dstpiece);
                        }
                    }
                }
            }
        }

    }


    void cGenerator::gen_kn_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t knight){

        uint16_t pindir = m_matchptr->m_board.eval_pindir(x, y);
        if(pindir != mNO_DIR){ 
            return; 
        }

        for(uint8_t i = 0; i < size(cPiece::kn_steps); ++i){

            if(cBoard::is_inbounds(x + cPiece::kn_steps[i].m_xstep, y + cPiece::kn_steps[i].m_ystep)){

                uint8_t dst_x = x + cPiece::kn_steps[i].m_xstep;
                uint8_t dst_y = y + cPiece::kn_steps[i].m_ystep;

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] != PIECES_COLORS[knight]){
                    cMove move(x, y, dst_x, dst_y, knight, dstpiece, mBLK, cMove::P_LOW);

                    cEvaluator::priorize_move(*m_matchptr, move);

                    moves.push_back(move);
                }
            }
        }

    }


    void cGenerator::gen_qrb_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t piece, char _switch){

        uint16_t pindir = m_matchptr->m_board.eval_pindir(x, y);
        uint8_t idx, maxidx;

        if(_switch == 'q'){ idx = 0; maxidx = 8; }
        else if(_switch == 'r'){ idx = 0; maxidx = 4; }
        else if(_switch == 'b'){ idx = 4; maxidx = 8; }
        else{ return; }

        for(; idx < maxidx; ++idx){
            uint8_t dst_x = x;
            uint8_t dst_y = y;

            for(uint8_t j = 0; j < 7; ++j){
                if(cBoard::is_inbounds(dst_x + cPiece::qu_steps[idx].m_xstep, dst_y + cPiece::qu_steps[idx].m_ystep) == false){
                    break;
                }

                dst_x += cPiece::qu_steps[idx].m_xstep;
                dst_y += cPiece::qu_steps[idx].m_ystep;

                if(pindir != mNO_DIR && (pindir & cBoard::eval_dir(x, y, dst_x, dst_y)) == 0){ 
                    break; 
                }

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[piece]){
                    break;
                }

                cMove move(x, y, dst_x, dst_y, piece, dstpiece, mBLK, cMove::P_LOW);

                cEvaluator::priorize_move(*m_matchptr, move);

                moves.push_back(move);

                if(dstpiece != mBLK){
                    break;
                }
            }
        }

    }


    void cGenerator::gen_kg_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t king){

        uint8_t enmycolor;

        cBoard board(m_matchptr->m_board);

        if(PIECES_COLORS[king] == mWHITE){
            enmycolor = mBLACK;
            board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);
        }
        else{
            enmycolor = mWHITE;
            board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);
        }

        for(uint8_t i = 0; i < size(cPiece::qu_steps); ++i){

            if(cBoard::is_inbounds((x + cPiece::qu_steps[i].m_xstep), (y + cPiece::qu_steps[i].m_ystep))){

                uint8_t dst_x = (x + cPiece::qu_steps[i].m_xstep);
                uint8_t dst_y = (y + cPiece::qu_steps[i].m_ystep);

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[king]){
                    continue;
                }

                if(board.is_field_touched(dst_x, dst_y, enmycolor)){
                    continue;
                }

                cMove move(x, y, dst_x, dst_y, king, dstpiece, mBLK, cMove::P_LOW);

                cEvaluator::priorize_move(*m_matchptr, move);

                moves.push_back(move);
            }
        }


    }


    void cGenerator::gen_wkg_castling_moves(vector<cMove> &moves,  uint8_t kg_x, uint8_t kg_y){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);

        // short white castling
        if(m_matchptr->m_board.m_wKg_has_moved_at == 0 && m_matchptr->m_board.m_wRkH_has_moved_at == 0 && cBoard::is_inbounds(kg_x + 3, kg_y)){

            if(kg_y == 0 && m_matchptr->m_board.getfield(kg_x + 1, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x + 2, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x + 3, kg_y) == mWRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(kg_x + i, kg_y, mBLACK)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove move(kg_x, kg_y, kg_x + 2, kg_y, mWKG, mBLK, mBLK, cMove::P_LOW);

                    cEvaluator::priorize_move(*m_matchptr, move);

                    moves.push_back(move);
                }
            }
        }

        // long white castling
        if(m_matchptr->m_board.m_wKg_has_moved_at == 0 && m_matchptr->m_board.m_wRkA_has_moved_at == 0 && cBoard::is_inbounds(kg_x - 4, kg_y)){

            if(kg_y == 0 && m_matchptr->m_board.getfield(kg_x - 1, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x - 2, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x - 3, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x - 4, kg_y) == mWRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(kg_x - i, kg_y, mBLACK)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove move(kg_x, kg_y, kg_x - 2, kg_y, mWKG, mBLK, mBLK, cMove::P_LOW);

                    cEvaluator::priorize_move(*m_matchptr, move);

                    moves.push_back(move);
                }
            }
        }

    }


    void cGenerator::gen_bkg_castling_moves(vector<cMove> &moves, uint8_t kg_x, uint8_t kg_y){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);

        // short black castling
        if(m_matchptr->m_board.m_bKg_has_moved_at == 0 && m_matchptr->m_board.m_bRkH_has_moved_at == 0 && cBoard::is_inbounds(kg_x + 3, kg_y)){

            if(kg_y == 7 && m_matchptr->m_board.getfield(kg_x + 1, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x + 2, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x + 3, kg_y) == mBRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(kg_x + i, kg_y, mWHITE)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove move(kg_x, kg_y, kg_x + 2, kg_y, mBKG, mBLK, mBLK, cMove::P_LOW);

                    cEvaluator::priorize_move(*m_matchptr, move);

                    moves.push_back(move);
                }
            }
        }

        // long black castling
        if(m_matchptr->m_board.m_bKg_has_moved_at == 0 && m_matchptr->m_board.m_bRkA_has_moved_at == 0 && cBoard::is_inbounds(kg_x - 4, kg_y)){

            if(kg_y == 7 && m_matchptr->m_board.getfield(kg_x - 1, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x - 2, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x - 3, kg_y) == mBLK && m_matchptr->m_board.getfield(kg_x - 4, kg_y) == mBRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(kg_x - i, kg_y, mWHITE)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove move(kg_x, kg_y, kg_x - 2, kg_y, mBKG, mBLK, mBLK, cMove::P_LOW);

                    cEvaluator::priorize_move(*m_matchptr, move);

                    moves.push_back(move);
                }
            }
        }

    }

 
    void cGenerator::gen_capture_checking_piece_moves(vector<cMove> &moves, const cPiece &attacker){

        // search capturing pieces
        vector<cPiece> defending, others;

        if(PIECES_COLORS[attacker.m_piece] == mBLACK){
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(defending, others, attacker.m_xpos, attacker.m_ypos, m_matchptr->get_last_move(), true);
        }
        else{
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(others, defending, attacker.m_xpos, attacker.m_ypos, m_matchptr->get_last_move(), true);
        }

        for(const cPiece &defender : defending){
            
            if(defender.m_piece == mWKG || defender.m_piece == mBKG){
                // king moves are covered by gen_kg_moves()
                continue;
            }
            else if(defender.m_piece == mWPW){
                add_wpw_moves(moves, defender.m_xpos, defender.m_ypos, attacker.m_xpos, attacker.m_ypos, attacker.m_piece);
            }
            else if(defender.m_piece == mBPW){
                add_bpw_moves(moves, defender.m_xpos, defender.m_ypos, attacker.m_xpos, attacker.m_ypos, attacker.m_piece);
            }
            else{
                cMove move(defender.m_xpos, defender.m_ypos, attacker.m_xpos, attacker.m_ypos, defender.m_piece, attacker.m_piece, mBLK, cMove::P_LOW);

                cEvaluator::priorize_move(*m_matchptr, move);

                moves.push_back(move);
            }

        }

    }


    void cGenerator::gen_blocking_checking_piece_moves(vector<cMove> &moves, const cPiece &attacker){

        // search blocking pieces
        if(attacker.m_piece == mWPW || attacker.m_piece == mBPW || attacker.m_piece == mWKN || attacker.m_piece == mBKN){
            // for pawns and knights no blocking moves exist
            return;
        }

        uint8_t kg_x, kg_y;
        if(PIECES_COLORS[attacker.m_piece] == mWHITE){
            kg_x = m_matchptr->m_board.m_bKg_x;
            kg_y = m_matchptr->m_board.m_bKg_y;
        }
        else{
            kg_x = m_matchptr->m_board.m_wKg_x;
            kg_y = m_matchptr->m_board.m_wKg_y;
        }

        uint16_t dir = cBoard::eval_dir(kg_x, kg_y, attacker.m_xpos, attacker.m_ypos);

        int8_t step_x, step_y;
        if(cBoard::eval_steps(step_x, step_y, dir) == false){
            return;
        }

        uint8_t dst_x, dst_y;
        if(m_matchptr->m_board.is_inbounds((kg_x + step_x), kg_y + step_y)){
            dst_x = (kg_x + step_x);
            dst_y = (kg_y + step_y);
        }
        else{
            return;
        }

        while(!(dst_x == attacker.m_xpos && dst_y == attacker.m_ypos)){

            vector<cPiece> blocking, others;
      
            if(PIECES_COLORS[attacker.m_piece] == mBLACK){
                m_matchptr->m_board.search_all_dirs_for_touching_pieces(blocking, others, dst_x, dst_y, m_matchptr->get_last_move(), true);
            }
            else{
                m_matchptr->m_board.search_all_dirs_for_touching_pieces(others, blocking, dst_x, dst_y, m_matchptr->get_last_move(), true);
            }

            // remove touching pawns on blank fields
            vector<cPiece>::iterator it = blocking.begin();
            while(it != blocking.end()){
                if(it->m_piece == mWPW || it->m_piece == mBPW){
                    if(m_matchptr->m_board.getfield(dst_x, dst_y) == mBLK){
                        it = blocking.erase(it);
                    }
                }
                else{
                    ++it;
                }
            }

            // add blocking vertical pawns
            if(PIECES_COLORS[attacker.m_piece] == mBLACK){
                m_matchptr->m_board.search_vertical_for_straight_pawns(blocking, others, dst_x, dst_y);
            }
            else{
                m_matchptr->m_board.search_vertical_for_straight_pawns(others, blocking, dst_x, dst_y);
            }

            for(const cPiece &blocker : blocking){

                if(blocker.m_piece == mWKG || blocker.m_piece == mBKG){
                    continue;
                }

                if(blocker.m_piece == mWPW){
                    add_wpw_moves(moves, blocker.m_xpos, blocker.m_ypos, dst_x, dst_y, mBLK);
                }
                else if(blocker.m_piece == mBPW){
                    add_bpw_moves(moves, blocker.m_xpos, blocker.m_ypos, dst_x, dst_y, mBLK);
                }
                else{
                    cMove move(blocker.m_xpos, blocker.m_ypos, dst_x, dst_y, blocker.m_piece, mBLK, mBLK, cMove::P_LOW);

                    cEvaluator::priorize_move(*m_matchptr, move);

                    moves.push_back(move);
                }
            }

            blocking.clear();
            others.clear();

            if(m_matchptr->m_board.is_inbounds((dst_x + step_x), dst_y + step_y)){
                dst_x += step_x;
                dst_y += step_y;
            }
            else{
                return;
            }

        }

    }

