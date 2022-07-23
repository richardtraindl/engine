

  #include "./generator.hpp"
  #include "./evaluator.hpp"


  bool hasbug(const cMatch &match){

      if(match.m_board.getfield(match.m_board.m_wKg_x, match.m_board.m_wKg_y) != mWKG || match.m_board.getfield(match.m_board.m_bKg_x, match.m_board.m_bKg_y) != mBKG){
          cout << "BUG******************************" << endl;
          cout << match.format_moves(match.m_minutes, true) << endl;
          cout << "BUG******************************" << endl;
          return true;
      }
      else{
          return false;
      }

  }


  cGenerator::cGenerator(){
  }


  void cGenerator::gen_moves(cMatch &match, vector<cMove> &moves, uint8_t color){

        if(hasbug(match)){
            return;
        }

        uint8_t king, kg_x, kg_y;

        if(color == mWHITE){
            king = mWKG;
            kg_x = match.m_board.m_wKg_x;
            kg_y = match.m_board.m_wKg_y;
        }
        else{
            king = mBKG;
            kg_x = match.m_board.m_bKg_x;
            kg_y = match.m_board.m_bKg_y;
        }

        vector<cPiece> attacking_pieces, supporting_pieces;
        if(color == mWHITE){
            match.m_board.search_all_dirs_for_touching_pieces(supporting_pieces, attacking_pieces, kg_x, kg_y, match.get_last_move(), false);
        }
        else{
            match.m_board.search_all_dirs_for_touching_pieces(attacking_pieces, supporting_pieces, kg_x, kg_y, match.get_last_move(), false);
        }
        hasbug(match);

        if(attacking_pieces.size() >= 2){
            gen_kg_moves(match, moves, kg_x, kg_y, king);
            hasbug(match);
        }
        else if(attacking_pieces.size() == 1){
            gen_kg_moves(match, moves, kg_x, kg_y, king);

            cPiece attacking_piece = attacking_pieces.front();

            gen_kg_supporting_moves(match, moves, attacking_piece); 
            hasbug(match);
        }
        else{
            for(uint8_t y = 0; y < 8; ++y){

                for(uint8_t x = 0; x < 8; ++x){

                    uint8_t piece = match.m_board.getfield(x, y);

                    if(PIECES_COLORS[piece] == color){
                        if(piece == mWPW){ 
                            gen_wpw_moves(match, moves, x, y); 
                            hasbug(match);
                            continue; 
                        }
                        else if(piece == mBPW){ 
                            gen_bpw_moves(match, moves, x, y); 
                            hasbug(match);
                            continue; 
                        }
                        else if(piece == mWKG){ 
                            gen_kg_moves(match, moves, x, y, mWKG);
                            gen_wkg_castling_moves(match, moves, x, y);
                            hasbug(match);
                            continue; 
                        }
                        else if(piece == mBKG){ 
                            gen_kg_moves(match, moves, x, y, mBKG);
                            gen_bkg_castling_moves(match, moves, x, y);
                            hasbug(match);
                            continue; 
                        }
                        else if(piece == mWRK || piece == mBRK){ 
                            gen_qrb_moves(match, moves, x, y, piece, 'r');
                            hasbug(match);
                            continue; 
                        }
                        else if(piece == mWBP || piece == mBBP){ 
                            gen_qrb_moves(match, moves, x, y, piece, 'b');
                            hasbug(match);
                            continue; 
                        }
                        else if(piece == mWKN || piece == mBKN){ 
                            gen_kn_moves(match, moves, x, y, piece); 
                            hasbug(match);
                            continue;
                        }
                        else if(piece == mWQU || piece == mBQU){
                            gen_qrb_moves(match, moves, x, y, piece, 'q');
                            hasbug(match);
                            continue; 
                        }
                    }

                }

            }

            attacking_pieces.clear();
            supporting_pieces.clear();
        }

    }


    void cGenerator::add_wpw_moves(cMatch &match, vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece){

        if(src_y == 6 && dst_y == 7){
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWQU, cMove::P_HIGH_UP));
            
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWRK, cMove::P_HIGH_UP + 1));
            
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWBP, cMove::P_HIGH_UP + 2));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mWKN, cMove::P_HIGH_UP + 3));
        }
        else{
            cMove move(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mBLK, cMove::P_MEDIUM);

            //set_prio_for_move(match, move);
            cEvaluator::priorize_move(match, move);

            moves.push_back(move);
        }
    }


    void cGenerator::gen_wpw_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y){

        uint16_t pindir = match.m_board.eval_pindir(x, y);

        for(uint8_t i = 0; i < 2; ++i){

            if(pindir == mNO_DIR || pindir == cPiece::wpw_capture_steps[i].m_cardinale){
                if(cBoard::is_inbounds(x + cPiece::wpw_capture_steps[i].m_xstep, y + cPiece::wpw_capture_steps[i].m_ystep)){
                    uint8_t dst_x = x + cPiece::wpw_capture_steps[i].m_xstep;
                    uint8_t dst_y = y + cPiece::wpw_capture_steps[i].m_ystep;

                    uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                    if(PIECES_COLORS[dstpiece] == mBLACK){
                        add_wpw_moves(match, moves, x, y, dst_x, dst_y, dstpiece);
                    }
                    else if(dstpiece == mBLK && y == 4){ // en passant
                        cMove prevmove = match.m_minutes.back();
                        uint8_t enpiece = match.m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                        if(enpiece == mBPW && prevmove.m_src_y == 6 && prevmove.m_dst_y == 4 && prevmove.m_src_x == dst_x){
                            moves.push_back(cMove(x, y, dst_x, dst_y, mWPW, dstpiece, mBLK, cMove::P_HIGH_UP));
                        }
                    }
                }
            }

        }

        if(pindir == mNO_DIR || pindir == mVERTICAL){
            if(cBoard::is_inbounds(x, y + cPiece::wpw_steps[0].m_ystep)){
                uint8_t dst_x = x;
                uint8_t dst_y = y + cPiece::wpw_steps[0].m_ystep;

                uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                if(dstpiece == mBLK){
                    add_wpw_moves(match, moves, x, y, dst_x, dst_y, dstpiece);

                    // double step
                    if(cBoard::is_inbounds(x, y + cPiece::wpw_steps[1].m_ystep)){
                        uint8_t dst_x = x;
                        uint8_t dst_y = y + cPiece::wpw_steps[1].m_ystep;

                        uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                        if(dstpiece == mBLK && y == 1){
                            add_wpw_moves(match, moves, x, y, dst_x, dst_y, dstpiece);
                        }
                    }
                }
            }
        }
    }


    void cGenerator::add_bpw_moves(cMatch &match, vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece){

        if(src_y == 1 && dst_y == 0){
            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBQU, cMove::P_HIGH_UP));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBRK, cMove::P_HIGH_UP + 1));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBBP, cMove::P_HIGH_UP + 2));

            moves.push_back(cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBKN, cMove::P_HIGH_UP + 3));
        }
        else{
            cMove move(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBLK, cMove::P_MEDIUM);

            //set_prio_for_move(match, move);
            cEvaluator::priorize_move(match, move);

            moves.push_back(move);
        }

    }


    void cGenerator::gen_bpw_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y){

        uint16_t pindir = match.m_board.eval_pindir(x, y);

        for(uint8_t i = 0; i < 2; ++i){

            if(pindir == mNO_DIR || pindir == cPiece::bpw_capture_steps[i].m_cardinale){
                if(cBoard::is_inbounds(x + cPiece::bpw_capture_steps[i].m_xstep, y + cPiece::bpw_capture_steps[i].m_ystep)){
                    uint8_t dst_x = x + cPiece::bpw_capture_steps[i].m_xstep;
                    uint8_t dst_y = y + cPiece::bpw_capture_steps[i].m_ystep;

                    uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                    if(PIECES_COLORS[dstpiece] == mWHITE){
                        add_bpw_moves(match, moves, x, y, dst_x, dst_y, dstpiece);
                    }
                    else if(dstpiece == mBLK && y == 3){ // en passant
                        cMove prevmove = match.m_minutes.back();
                        uint8_t enpiece = match.m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                        if(enpiece == mWPW && prevmove.m_src_y == 1 && prevmove.m_dst_y == 3 && prevmove.m_src_x == dst_x){
                            moves.push_back(cMove(x, y, dst_x, dst_y, mBPW, dstpiece, mBLK, cMove::P_HIGH_UP));
                        }

                    }
                }
            }

        }

        if(pindir == mNO_DIR || pindir == mVERTICAL){
            if(cBoard::is_inbounds(x, y + cPiece::bpw_steps[0].m_ystep)){
                uint8_t dst_x = x;
                uint8_t dst_y = y + cPiece::bpw_steps[0].m_ystep;

                uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                if(dstpiece == mBLK){
                    add_bpw_moves(match, moves, x, y, dst_x, dst_y, dstpiece);

                    // double step
                    if(cBoard::is_inbounds(x, y + cPiece::bpw_steps[1].m_ystep)){
                        uint8_t dst_x = x;
                        uint8_t dst_y = y + cPiece::bpw_steps[1].m_ystep;

                        uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                        if(dstpiece == mBLK && y == 6){
                            add_bpw_moves(match, moves, x, y, dst_x, dst_y, dstpiece);
                        }
                    }
                }
            }
        }

    }


    void cGenerator::gen_kn_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t knight){

        uint16_t pindir = match.m_board.eval_pindir(x, y);
        if(pindir != mNO_DIR){ 
            return; 
        }

        for(uint8_t i = 0; i < size(cPiece::kn_steps); ++i){

            if(cBoard::is_inbounds(x + cPiece::kn_steps[i].m_xstep, y + cPiece::kn_steps[i].m_ystep)){

                uint8_t dst_x = x + cPiece::kn_steps[i].m_xstep;
                uint8_t dst_y = y + cPiece::kn_steps[i].m_ystep;

                uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] != PIECES_COLORS[knight]){
                    cMove move(x, y, dst_x, dst_y, knight, dstpiece, mBLK, cMove::P_MEDIUM);

                    //set_prio_for_move(match, move);
                    cEvaluator::priorize_move(match, move);

                    moves.push_back(move);
                }
            }
        }

    }


    void cGenerator::gen_qrb_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t piece, char _switch){

        uint16_t pindir = match.m_board.eval_pindir(x, y);
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

                uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[piece]){
                    break;
                }

                cMove move(x, y, dst_x, dst_y, piece, dstpiece, mBLK, cMove::P_MEDIUM);

                //set_prio_for_move(match, move);
                cEvaluator::priorize_move(match, move);

                moves.push_back(move);

                if(dstpiece != mBLK){
                    break;
                }
            }
        }

    }


    void cGenerator::gen_kg_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t king){

        uint8_t enmycolor;

        cBoard board(match.m_board);

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

                uint8_t dstpiece = match.m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[king]){
                    continue;
                }

                if(board.is_field_touched(dst_x, dst_y, enmycolor)){
                    continue;
                }

                cMove move(x, y, dst_x, dst_y, king, dstpiece, mBLK, cMove::P_MEDIUM_UP);

                cEvaluator::priorize_move(match, move);

                moves.push_back(move);
            }
        }


    }


    void cGenerator::gen_wkg_castling_moves(cMatch &match, vector<cMove> &moves,  uint8_t kg_x, uint8_t kg_y){

        cBoard board(match.m_board);
        board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);

        // short white castling
        if(match.m_board.m_wKg_has_moved_at == 0 && match.m_board.m_wRkH_has_moved_at == 0 && cBoard::is_inbounds(kg_x + 3, kg_y)){

            if(kg_y == 0 && match.m_board.getfield(kg_x + 1, kg_y) == mBLK && match.m_board.getfield(kg_x + 2, kg_y) == mBLK && match.m_board.getfield(kg_x + 3, kg_y) == mWRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(kg_x + i, kg_y, mBLACK)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove move(kg_x, kg_y, kg_x + 2, kg_y, mWKG, mBLK, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(match, move);
                    //set_prio_for_castling(match, move);

                    moves.push_back(move);
                }
            }
        }

        // long white castling
        if(match.m_board.m_wKg_has_moved_at == 0 && match.m_board.m_wRkA_has_moved_at == 0 && cBoard::is_inbounds(kg_x - 4, kg_y)){

            if(kg_y == 0 && match.m_board.getfield(kg_x - 1, kg_y) == mBLK && match.m_board.getfield(kg_x - 2, kg_y) == mBLK && match.m_board.getfield(kg_x - 3, kg_y) == mBLK && match.m_board.getfield(kg_x - 4, kg_y) == mWRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(kg_x - i, kg_y, mBLACK)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove move(kg_x, kg_y, kg_x - 2, kg_y, mWKG, mBLK, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(match, move);
                    //set_prio_for_castling(match, move);

                    moves.push_back(move);
                }
            }
        }

    }


    void cGenerator::gen_bkg_castling_moves(cMatch &match, vector<cMove> &moves, uint8_t kg_x, uint8_t kg_y){

        cBoard board(match.m_board);
        board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);

        // short black castling
        if(match.m_board.m_bKg_has_moved_at == 0 && match.m_board.m_bRkH_has_moved_at == 0 && cBoard::is_inbounds(kg_x + 3, kg_y)){

            if(kg_y == 7 && match.m_board.getfield(kg_x + 1, kg_y) == mBLK && match.m_board.getfield(kg_x + 2, kg_y) == mBLK && match.m_board.getfield(kg_x + 3, kg_y) == mBRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(kg_x + i, kg_y, mWHITE)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove move(kg_x, kg_y, kg_x + 2, kg_y, mBKG, mBLK, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(match, move);
                    //set_prio_for_castling(match, move);

                    moves.push_back(move);
                }
            }
        }

        // long black castling
        if(match.m_board.m_bKg_has_moved_at == 0 && match.m_board.m_bRkA_has_moved_at == 0 && cBoard::is_inbounds(kg_x - 4, kg_y)){

            if(kg_y == 7 && match.m_board.getfield(kg_x - 1, kg_y) == mBLK && match.m_board.getfield(kg_x - 2, kg_y) == mBLK && match.m_board.getfield(kg_x - 3, kg_y) == mBLK && match.m_board.getfield(kg_x - 4, kg_y) == mBRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(kg_x - i, kg_y, mWHITE)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove move(kg_x, kg_y, kg_x - 2, kg_y, mBKG, mBLK, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(match, move);
                    //set_prio_for_castling(match, move);

                    moves.push_back(move);
                }
            }
        }

    }

 
    void cGenerator::gen_kg_supporting_moves(cMatch &match, vector<cMove> &moves, const cPiece &attacker){

        // search capturing pieces
        vector<cPiece> defending, others;

        if(PIECES_COLORS[attacker.m_piece] == mBLACK){
            match.m_board.search_all_dirs_for_touching_pieces(defending, others, attacker.m_xpos, attacker.m_ypos, match.get_last_move(), true);
        }
        else{
            match.m_board.search_all_dirs_for_touching_pieces(others, defending, attacker.m_xpos, attacker.m_ypos, match.get_last_move(), true);
        }

        for(const cPiece &defender : defending){
            
            if(defender.m_piece == mWKG || defender.m_piece == mBKG){
                // king moves are covered by gen_kg_moves()
                continue;
            }
            else if(defender.m_piece == mWPW){
                add_wpw_moves(match, moves, defender.m_xpos, defender.m_ypos, attacker.m_xpos, attacker.m_ypos, attacker.m_piece);
            }
            else if(defender.m_piece == mBPW){
                add_bpw_moves(match, moves, defender.m_xpos, defender.m_ypos, attacker.m_xpos, attacker.m_ypos, attacker.m_piece);
            }
            else{
                moves.push_back(cMove(defender.m_xpos, defender.m_ypos, attacker.m_xpos, attacker.m_ypos, defender.m_piece, attacker.m_piece, mBLK, cMove::P_HIGH_UP));
            }

        }

        defending.clear();
        others.clear();
        
        // search blocking pieces
        if(attacker.m_piece == mWPW || attacker.m_piece == mBPW || attacker.m_piece == mWKN || attacker.m_piece == mBKN){
            // for pawns and knights no blocking moves exist
            return;
        }

        uint8_t kg_x, kg_y;
        if(PIECES_COLORS[attacker.m_piece] == mWHITE){
            kg_x = match.m_board.m_bKg_x;
            kg_y = match.m_board.m_bKg_y;
        }
        else{
            kg_x = match.m_board.m_wKg_x;
            kg_y = match.m_board.m_wKg_y;
        }

        uint16_t dir = cBoard::eval_dir(kg_x, kg_y, attacker.m_xpos, attacker.m_ypos);

        int8_t step_x, step_y;
        if(cBoard::eval_steps(step_x, step_y, dir) == false){
            return;
        }

        uint8_t dst_x, dst_y;
        if(match.m_board.is_inbounds((kg_x + step_x), kg_y + step_y)){
            dst_x = (kg_x + step_x);
            dst_y = (kg_y + step_y);
        }
        else{
            return;
        }

        while(!(dst_x == attacker.m_xpos && dst_y == attacker.m_ypos)){

            vector<cPiece> blocking, others;
      
            if(PIECES_COLORS[attacker.m_piece] == mBLACK){
                match.m_board.search_all_dirs_for_touching_pieces(blocking, others, dst_x, dst_y, match.get_last_move(), true);

                match.m_board.search_vertical_for_straight_pawns(blocking, others, dst_x, dst_y);
            }
            else{
                match.m_board.search_all_dirs_for_touching_pieces(others, blocking, dst_x, dst_y, match.get_last_move(), true);

                match.m_board.search_vertical_for_straight_pawns(others, blocking, dst_x, dst_y);
            }

            for(const cPiece &blocker : blocking){

                if(blocker.m_piece == mWKG || blocker.m_piece == mBKG){
                    continue;
                }

                if(blocker.m_piece == mWPW){
                    add_wpw_moves(match, moves, blocker.m_xpos, blocker.m_ypos, dst_x, dst_y, mBLK);
                }
                else if(blocker.m_piece == mBPW){
                    add_bpw_moves(match, moves, blocker.m_xpos, blocker.m_ypos, dst_x, dst_y, mBLK);
                }
                else{
                    moves.push_back(cMove(blocker.m_xpos, blocker.m_ypos, dst_x, dst_y, blocker.m_piece, mBLK, mBLK, cMove::P_HIGH_UP));
                }
            }

            blocking.clear();
            others.clear();

            if(match.m_board.is_inbounds((dst_x + step_x), dst_y + step_y)){
                dst_x += step_x;
                dst_y += step_y;
            }
            else{
                return;
            }

        }

    }


  /*void cGenerator::set_prio_for_move(cMatch &match, cMove &move){

      // eval state for move
      cBoard board(match.m_board);

      board.setfield(move.m_src_x, move.m_src_y, mBLK);
      board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);

      int32_t mscore = cEvaluator::score_move(match, move);

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
              //does_move_touch_vulnerable_piece, cMove::PRIO_URGENT_TOUCH)

              // attack on King
              vector<cPiece> wpieces, bpieces;

              board.search_from_piece(wpieces, bpieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, false);

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

  }


  void cGenerator::set_prio_for_castling(cMatch &match, cMove &move){

        move.m_prio = cMove::PRIO_CASTLING;
        
        return; // !!!

        cBoard board(match.m_board);

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

        int32_t mscore = cEvaluator::score_move(match, move);

        bool is_good_dstfield = (mscore >= 0 && PIECES_COLORS[move.m_srcpiece] == mWHITE) || (mscore <= 0 && PIECES_COLORS[move.m_srcpiece] == mBLACK);

        if(is_good_dstfield){
            // does_move_touch_vulnerable_piece, cMove::PRIO_URGENT_TOUCH

            // attack on King
            vector<cPiece> wpieces, bpieces;

            board.search_from_piece(wpieces, bpieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, false);
            
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
        else{
            // bad dstfield
            if(move.m_prio == cMove::PRIO_STANDARD){
                move.m_prio = 120;
            }
        }

    } */

