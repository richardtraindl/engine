

  #include "./generator2.hpp"
  #include "./evaluator.hpp"


    cGenerator2::cGenerator2(cMatch &match){

        m_matchptr = &match;

        vector<cPiece> attacking_pieces, supporting_pieces;

        if(m_matchptr->next_color() == mWHITE){
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(supporting_pieces, attacking_pieces, m_matchptr->m_board.m_wKg_x, m_matchptr->m_board.m_wKg_y, m_matchptr->get_last_move(), false);
        }
        else{
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(attacking_pieces, supporting_pieces, m_matchptr->m_board.m_bKg_x, m_matchptr->m_board.m_bKg_y, m_matchptr->get_last_move(), false);
        }

        m_check_cnt = attacking_pieces.size();

        if(attacking_pieces.size() == 0){
            m_king_attackerptr = nullptr;

            m_attacked_kg_move_remaining = false;

            m_capture_of_checking_remaining = false;

            m_blocking_of_checking_remaining = false;
        }
        else if(attacking_pieces.size() == 1){
            m_king_attackerptr = new cPiece(attacking_pieces.front());

            m_attacked_kg_move_remaining = true;

            m_capture_of_checking_remaining = true;

            m_blocking_of_checking_remaining = true;
        }
        else{
            m_king_attackerptr = nullptr;

            m_attacked_kg_move_remaining = true;

            m_capture_of_checking_remaining = false;

            m_blocking_of_checking_remaining = false;
        }

        m_gen_x = 0;

        m_gen_y = 0;

        m_dir_idx = 0;

        m_step_idx = 0;

        m_sh_castl_remaining = true;

        m_lg_castl_remaining = true;

        m_gen_end = false;

    }


    cGenerator2::~cGenerator2(){

        if(m_king_attackerptr != nullptr){
            delete m_king_attackerptr;
        }

    }


    cMove *cGenerator2::gen_move(){

        uint8_t color = m_matchptr->next_color();

        if(m_check_cnt == 2){
            cMove *moveptr;

            if(m_attacked_kg_move_remaining){
                if(color == mWHITE){
                    moveptr = gen_bare_kg_move(mWKG, false);
                }
                else{
                    moveptr = gen_bare_kg_move(mBKG, false);
                }

                if(moveptr != nullptr){
                    return moveptr;
                }

                m_attacked_kg_move_remaining = false;
            }

            return nullptr;
        }
        else if(m_check_cnt == 1){
            cMove *moveptr;

            if(color == mWHITE){
                moveptr = gen_kg_and_supporting_move(mWKG);
            }
            else{
                moveptr = gen_kg_and_supporting_move(mBKG);
            }

            return moveptr;
        }
        else{
            while(m_gen_end == false){

                uint8_t piece = m_matchptr->m_board.getfield(m_gen_x, m_gen_y);

                if(PIECES_COLORS[piece] != color){
                    incr_gen(0, 0);

                    continue;
                }
                  
                if(piece == mWPW){ 
                    cMove *moveptr = gen_wpw_move(piece);

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                }
                else if(piece == mBPW){ 
                    cMove *moveptr =  gen_bpw_move(piece); 

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                }
                else if(piece == mWKG){ 
                    cMove *moveptr = gen_kg_and_castl_move(piece);

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                }
                else if(piece == mBKG){ 
                    cMove *moveptr = gen_kg_and_castl_move(piece);

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                } 
                else if(piece == mWRK || piece == mBRK){ 
                    cMove *moveptr = gen_qu_rk_bp_move(piece);

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                }
                else if(piece == mWBP || piece == mBBP){ 
                    cMove *moveptr = gen_qu_rk_bp_move(piece);

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                }
                else if(piece == mWKN || piece == mBKN){ 
                    cMove *moveptr = gen_kn_move(piece); 

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                }
                else if(piece == mWQU || piece == mBQU){
                    cMove *moveptr = gen_qu_rk_bp_move(piece);

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                }
                else{
                    incr_gen(0, 0);
                }

            }

            return nullptr;
        }
        
    }


    void cGenerator2::incr_gen(const uint8_t max_step_idx, const uint8_t max_dir_idx){

        if((m_step_idx + 1) < max_step_idx){
            m_step_idx++;
        }
        else{
            m_step_idx = 0;

            incr_dir(max_dir_idx);
        }

    }


    void cGenerator2::incr_dir(const uint8_t max_dir_idx){

        if((m_dir_idx + 1) < max_dir_idx){
            m_dir_idx++;
        }
        else{
            m_dir_idx = 0;

            incr_coord();
        }

    }


    void cGenerator2::incr_coord(){

        if(cBoard::is_inbounds(m_gen_x + 1, m_gen_y)){
            m_gen_x++;
        }
        else{
            if(cBoard::is_inbounds(0, m_gen_y + 1)){
                m_gen_x = 0;
                m_gen_y++;
            }
            else{
                m_gen_end = true;
            }
        }

    }


    cMove *cGenerator2::gen_wpw_move(const uint8_t pawn){

        uint16_t pindir = m_matchptr->m_board.eval_pindir(m_gen_x, m_gen_y);

        if(m_dir_idx < 2){
            for(uint8_t idx = m_dir_idx; idx < 2; ++idx){

                if(pindir == mNO_DIR || pindir == cPiece::wpw_capture_steps[idx].m_cardinale){
                    if(cBoard::is_inbounds(m_gen_x + cPiece::wpw_capture_steps[idx].m_xstep, m_gen_y + cPiece::wpw_capture_steps[idx].m_ystep)){
                        uint8_t dst_x = m_gen_x + cPiece::wpw_capture_steps[idx].m_xstep;
                        uint8_t dst_y = m_gen_y + cPiece::wpw_capture_steps[idx].m_ystep;

                        uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                        if(PIECES_COLORS[dstpiece] == mBLACK){
                            cMove *moveptr = _create_wpw_move(m_gen_x, m_gen_y, dst_x, dst_y, dstpiece);

                            m_dir_idx = idx;

                            incr_gen(0, 4);

                            return moveptr;

                        }
                        else if(dstpiece == mBLK && m_gen_y == 4){ // en passant
                            cMove prevmove = m_matchptr->m_minutes.back();
                            uint8_t enpiece = m_matchptr->m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                            if(enpiece == mBPW && prevmove.m_src_y == 6 && prevmove.m_dst_y == 4 && prevmove.m_src_x == dst_x){
                                cMove *moveptr = new cMove(m_gen_x, m_gen_y, dst_x, dst_y, pawn, dstpiece, mBLK, cMove::P_HIGH_UP);

                                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                                m_dir_idx = idx;

                                incr_gen(0, 4);

                                return moveptr;
                            }
                        }
                    }
                }

            }

            m_dir_idx = 1;

            incr_gen(0, 4);

        }

        if(m_dir_idx == 2){
            if(pindir == mNO_DIR || pindir == mVERTICAL){
                if(cBoard::is_inbounds(m_gen_x, m_gen_y + cPiece::wpw_steps[0].m_ystep)){
                    uint8_t dst_x = m_gen_x;
                    uint8_t dst_y = m_gen_y + cPiece::wpw_steps[0].m_ystep;

                    uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                    if(dstpiece == mBLK){
                        cMove *moveptr = _create_wpw_move(m_gen_x, m_gen_y, dst_x, dst_y, dstpiece);

                        m_dir_idx = 2;

                        incr_gen(0, 4);

                        return moveptr;
                    }
                }
            }

            m_dir_idx = 2;

            incr_gen(0, 4);
        }

        if(m_dir_idx == 3){
            if(pindir == mNO_DIR || pindir == mVERTICAL){
                // double step
                if(cBoard::is_inbounds(m_gen_x, m_gen_y + cPiece::wpw_steps[1].m_ystep)){
                    uint8_t dst_x = m_gen_x;
                    uint8_t dst_y = m_gen_y + cPiece::wpw_steps[1].m_ystep;
                    uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                    uint8_t mid_y = m_gen_y + cPiece::wpw_steps[0].m_ystep;
                    uint8_t midpiece = m_matchptr->m_board.getfield(dst_x, mid_y);

                    if(midpiece == mBLK && dstpiece == mBLK && m_gen_y == 1){
                        cMove *moveptr = _create_wpw_move(m_gen_x, m_gen_y, dst_x, dst_y, dstpiece);

                        m_dir_idx = 3;

                        incr_gen(0, 4);

                        return moveptr;
                    }
                }
            }
            m_dir_idx = 3;

            incr_gen(0, 4);
        }

        return nullptr;

    }


    cMove *cGenerator2::_create_wpw_move(const uint8_t src_x, const uint8_t src_y, const uint8_t dst_x, const uint8_t dst_y, const uint8_t dstpiece){

        if(src_y == 6 && dst_y == 7){
            uint8_t prom_pieces[4] = { mWQU, mWRK, mWBP, mWKN };

            for(uint8_t i = 0; i < 1; ++i){ // Todo 4 times

                cMove *moveptr = new cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, prom_pieces[i], cMove::P_HIGH_UP + i);

                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                return moveptr;

            }

        }
        else{
            cMove *moveptr = new cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mBLK, cMove::P_MEDIUM);

            cEvaluator::priorize_move(*m_matchptr, *moveptr);

            return moveptr;
        }
        
        return nullptr;

    }


   cMove *cGenerator2::gen_bpw_move(const uint8_t pawn){

        uint16_t pindir = m_matchptr->m_board.eval_pindir(m_gen_x, m_gen_y);

        if(m_dir_idx < 2){
            for(uint8_t idx = m_dir_idx; idx < 2; ++idx){

                if(pindir == mNO_DIR || pindir == cPiece::bpw_capture_steps[idx].m_cardinale){
                    if(cBoard::is_inbounds(m_gen_x + cPiece::bpw_capture_steps[idx].m_xstep, m_gen_y + cPiece::bpw_capture_steps[idx].m_ystep)){
                        uint8_t dst_x = m_gen_x + cPiece::bpw_capture_steps[idx].m_xstep;
                        uint8_t dst_y = m_gen_y + cPiece::bpw_capture_steps[idx].m_ystep;

                        uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                        if(PIECES_COLORS[dstpiece] == mWHITE){
                            cMove *moveptr = _create_bpw_move(m_gen_x, m_gen_y, dst_x, dst_y, dstpiece);

                            m_dir_idx = idx;

                            incr_gen(0, 4);

                            return moveptr;

                        }
                        else if(dstpiece == mBLK && m_gen_y == 3){ // en passant
                            cMove prevmove = m_matchptr->m_minutes.back();
                            uint8_t enpiece = m_matchptr->m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                            if(enpiece == mWPW && prevmove.m_src_y == 1 && prevmove.m_dst_y == 3 && prevmove.m_src_x == dst_x){
                                cMove *moveptr = new cMove(m_gen_x, m_gen_y, dst_x, dst_y, pawn, dstpiece, mBLK, cMove::P_HIGH_UP);

                                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                                m_dir_idx = idx;

                                incr_gen(0, 4);

                                return moveptr;
                            }
                        }
                    }
                }

            }

            m_dir_idx = 1;

            incr_gen(0, 4);

        }

        if(m_dir_idx == 2){
            if(pindir == mNO_DIR || pindir == mVERTICAL){
                if(cBoard::is_inbounds(m_gen_x, m_gen_y + cPiece::bpw_steps[0].m_ystep)){
                    uint8_t dst_x = m_gen_x;
                    uint8_t dst_y = m_gen_y + cPiece::bpw_steps[0].m_ystep;

                    uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                    if(dstpiece == mBLK){
                        cMove *moveptr = _create_bpw_move(m_gen_x, m_gen_y, dst_x, dst_y, dstpiece);

                        m_dir_idx = 2;

                        incr_gen(0, 4);

                        return moveptr;
                    }
                }
            }

            m_dir_idx = 2;

            incr_gen(0, 4);
        }

        if(m_dir_idx == 3){
            if(pindir == mNO_DIR || pindir == mVERTICAL){
                // double step
                if(cBoard::is_inbounds(m_gen_x, m_gen_y + cPiece::bpw_steps[1].m_ystep)){
                    uint8_t dst_x = m_gen_x;
                    uint8_t dst_y = m_gen_y + cPiece::bpw_steps[1].m_ystep;
                    uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                    uint8_t mid_y = m_gen_y + cPiece::bpw_steps[0].m_ystep;
                    uint8_t midpiece = m_matchptr->m_board.getfield(dst_x, mid_y);

                    if(midpiece == mBLK && dstpiece == mBLK && m_gen_y == 6){
                        cMove *moveptr = _create_bpw_move(m_gen_x, m_gen_y, dst_x, dst_y, dstpiece);

                        m_dir_idx = 3;

                        incr_gen(0, 4);

                        return moveptr;
                    }
                }
            }
            m_dir_idx = 3;

            incr_gen(0, 4);
        }

        return nullptr;

    }


    cMove *cGenerator2::_create_bpw_move(const uint8_t src_x, const uint8_t src_y, const uint8_t dst_x, const uint8_t dst_y, const uint8_t dstpiece){

        if(src_y == 1 && dst_y == 0){
            uint8_t prom_pieces[4] = { mBQU, mBRK, mBBP, mBKN };

            for(uint8_t i = 0; i < 1; ++i){ // Todo 4 times

                cMove *moveptr = new cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, prom_pieces[i], cMove::P_HIGH_UP + i);

                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                return moveptr;

            }

        }
        else{
            cMove *moveptr = new cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBLK, cMove::P_MEDIUM);

            cEvaluator::priorize_move(*m_matchptr, *moveptr);

            return moveptr;
        }
        
        return nullptr;

    }


    cMove *cGenerator2::gen_kn_move(const uint8_t knight){

        uint16_t pindir = m_matchptr->m_board.eval_pindir(m_gen_x, m_gen_y);
        if(pindir != mNO_DIR){ 
            incr_gen(0, 0);

            return nullptr; 
        }

        for(uint8_t idx = m_dir_idx; idx < size(cPiece::kn_steps); ++idx){

            if(cBoard::is_inbounds(m_gen_x + cPiece::kn_steps[idx].m_xstep, m_gen_y + cPiece::kn_steps[idx].m_ystep)){

                uint8_t dst_x = m_gen_x + cPiece::kn_steps[idx].m_xstep;
                uint8_t dst_y = m_gen_y + cPiece::kn_steps[idx].m_ystep;

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] != PIECES_COLORS[knight]){
                    cMove *moveptr =  new cMove(m_gen_x, m_gen_y, dst_x, dst_y, knight, dstpiece, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    m_dir_idx = idx;

                    incr_gen(0, 8);

                    return moveptr;
                }
            }

        }

        m_dir_idx = 7;

        incr_gen(0, 8);
        
        //incr_gen(0, 0);

        return nullptr;

    }


    cMove *cGenerator2::gen_qu_rk_bp_move(const uint8_t piece){
      
        uint8_t offset = 0;
        uint8_t maxidx = 4;

        if(piece == mWBP || piece == mBBP){
            offset = 4;
        }

        if(piece == mWQU || piece == mBQU){
            maxidx = 8;
        }
      
        uint16_t pindir = m_matchptr->m_board.eval_pindir(m_gen_x, m_gen_y);

        for(uint8_t idx = m_dir_idx; idx < maxidx; ++idx){

            uint8_t dst_x = m_gen_x + (cPiece::qu_steps[idx+offset].m_xstep * m_step_idx);
            uint8_t dst_y = m_gen_y + (cPiece::qu_steps[idx+offset].m_ystep * m_step_idx);

            for(uint8_t j = m_step_idx; j < 7; ++j){

                if(cBoard::is_inbounds(dst_x + cPiece::qu_steps[idx+offset].m_xstep, dst_y + cPiece::qu_steps[idx+offset].m_ystep) == false){
                    m_dir_idx = idx;

                    incr_gen(0, maxidx);

                    break;
                }

                dst_x += cPiece::qu_steps[idx+offset].m_xstep;
                dst_y += cPiece::qu_steps[idx+offset].m_ystep;

                if(pindir != mNO_DIR && (pindir & cBoard::eval_dir(m_gen_x, m_gen_y, dst_x, dst_y)) == 0){ 
                    m_dir_idx = idx;

                    incr_gen(0, maxidx);

                    break;
                }

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[piece]){
                    m_dir_idx = idx;

                    incr_gen(0, maxidx);

                    break;
                }

                cMove *moveptr = new cMove(m_gen_x, m_gen_y, dst_x, dst_y, piece, dstpiece, mBLK, cMove::P_MEDIUM);

                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                if(dstpiece == mBLK){
                    m_step_idx = j;

                    m_dir_idx = idx;

                    incr_gen(7, maxidx);
                }
                else{
                    m_dir_idx = idx;

                    incr_gen(0, maxidx);
                }

                return moveptr;

            }

        }

        m_dir_idx = (maxidx - 1);

        incr_gen(0, maxidx);

        return nullptr;

    }

    cMove *cGenerator2::gen_kg_and_castl_move(const uint8_t king){

        cMove *moveptr;

        if(m_sh_castl_remaining){
            if(king == mWKG){
                moveptr = _gen_wkg_sh_castl_move();
            }
            else{
                moveptr = _gen_bkg_sh_castl_move();
            }

            m_sh_castl_remaining = false;

            if(moveptr != nullptr){
                return moveptr;
            }
        }

        if(m_lg_castl_remaining){
            if(king == mWKG){
                moveptr = _gen_wkg_lg_castl_move();
            }
            else{
                moveptr = _gen_bkg_lg_castl_move();
            }

            m_lg_castl_remaining = false;

            if(moveptr != nullptr){
                return moveptr;
            }
        }

        moveptr = gen_bare_kg_move(king, true);

        return moveptr;

    }


    cMove *cGenerator2::gen_bare_kg_move(const uint8_t king, const bool nonattacked){

        uint8_t kg_x, kg_y, maxidx;

        if(nonattacked){
            maxidx = 8;
        }
        else{
            maxidx = 10; // does not set m_dir_idx after 8 loops to 0
        }

        cBoard board(m_matchptr->m_board);

        if(PIECES_COLORS[king] == mWHITE){
            board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);
            kg_x = m_matchptr->m_board.m_wKg_x;
            kg_y = m_matchptr->m_board.m_wKg_y;
        }
        else{
            board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);
            kg_x = m_matchptr->m_board.m_bKg_x;
            kg_y = m_matchptr->m_board.m_bKg_y;
        }

        for(uint8_t idx = m_dir_idx; idx < size(cPiece::qu_steps); ++idx){

            if(cBoard::is_inbounds((kg_x + cPiece::qu_steps[idx].m_xstep), (kg_y + cPiece::qu_steps[idx].m_ystep)) == false){
                //m_dir_idx = idx;

                //incr_gen(0, maxidx);

                continue;
            }

            uint8_t dst_x = (kg_x + cPiece::qu_steps[idx].m_xstep);
            uint8_t dst_y = (kg_y + cPiece::qu_steps[idx].m_ystep);

            uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

            if(PIECES_COLORS[dstpiece] == PIECES_COLORS[king]){
                //m_dir_idx = idx;

                //incr_gen(0, maxidx);

                continue;
            }

            if(board.is_field_touched(dst_x, dst_y, REVERSED_COLORS[PIECES_COLORS[king]])){
                //m_dir_idx = idx;

                //incr_gen(0, maxidx);

                continue;
            }

            cMove *moveptr = new cMove(kg_x, kg_y, dst_x, dst_y, king, dstpiece, mBLK, cMove::P_MEDIUM);

            cEvaluator::priorize_move(*m_matchptr, *moveptr);

            m_dir_idx = idx;

            incr_gen(0, maxidx);

            return moveptr;

        }

        m_dir_idx = (maxidx - 1); // 7

        incr_gen(0, maxidx);

        return nullptr;

    }


    cMove *cGenerator2::_gen_wkg_sh_castl_move(){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);

        if(board.m_wKg_has_moved_at == 0 && board.m_wRkH_has_moved_at == 0 && cBoard::is_inbounds(m_gen_x + 3, m_gen_y)){

            if(m_gen_y == 0 && board.getfield(m_gen_x + 1, m_gen_y) == mBLK && board.getfield(m_gen_x + 2, m_gen_y) == mBLK && board.getfield(m_gen_x + 3, m_gen_y) == mWRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(m_gen_x + i, m_gen_y, mBLACK)){
                        is_touched = true;

                        break;
                    }

                }

                if(is_touched == false){
                    cMove *moveptr = new cMove(m_gen_x, m_gen_y, m_gen_x + 2, m_gen_y, mWKG, mBLK, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    return moveptr;
                }
            }
        }

        return nullptr;

    }


    cMove *cGenerator2::_gen_wkg_lg_castl_move(){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);

        if(board.m_wKg_has_moved_at == 0 && board.m_wRkA_has_moved_at == 0 && cBoard::is_inbounds(m_gen_x - 4, m_gen_y)){

            if(m_gen_y == 0 && board.getfield(m_gen_x - 1, m_gen_y) == mBLK && board.getfield(m_gen_x - 2, m_gen_y) == mBLK && board.getfield(m_gen_x - 3, m_gen_y) == mBLK && board.getfield(m_gen_x - 4, m_gen_y) == mWRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(m_gen_x - i, m_gen_y, mBLACK)){
                        is_touched = true;

                        break;
                    }

                }

                if(is_touched == false){
                    cMove *moveptr = new cMove(m_gen_x, m_gen_y, m_gen_x - 2, m_gen_y, mWKG, mBLK, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    return moveptr;
                }
            }
        }

        return nullptr;

    }


    cMove *cGenerator2::_gen_bkg_sh_castl_move(){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);

        if(board.m_bKg_has_moved_at == 0 && board.m_bRkH_has_moved_at == 0 && cBoard::is_inbounds(m_gen_x + 3, m_gen_y)){

            if(m_gen_y == 7 && board.getfield(m_gen_x + 1, m_gen_y) == mBLK && board.getfield(m_gen_x + 2, m_gen_y) == mBLK && board.getfield(m_gen_x + 3, m_gen_y) == mBRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(m_gen_x + i, m_gen_y, mWHITE)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove *moveptr = new cMove(m_gen_x, m_gen_y, m_gen_x + 2, m_gen_y, mBKG, mBLK, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    return moveptr;
                }
            }
        }

        return nullptr;

    }


    cMove *cGenerator2::_gen_bkg_lg_castl_move(){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);

        if(board.m_bKg_has_moved_at == 0 && board.m_bRkA_has_moved_at == 0 && cBoard::is_inbounds(m_gen_x - 4, m_gen_y)){

            if(m_gen_y == 7 && board.getfield(m_gen_x - 1, m_gen_y) == mBLK && board.getfield(m_gen_x - 2, m_gen_y) == mBLK && board.getfield(m_gen_x - 3, m_gen_y) == mBLK && board.getfield(m_gen_x - 4, m_gen_y) == mBRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(m_gen_x - i, m_gen_y, mWHITE)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove *moveptr = new cMove(m_gen_x, m_gen_y, m_gen_x - 2, m_gen_y, mBKG, mBLK, mBLK, cMove::P_MEDIUM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    return moveptr;
                }
            }
        }

        return nullptr;

    }


    cMove *cGenerator2::gen_kg_and_supporting_move(const uint8_t king){

        cMove *moveptr;

        if(m_capture_of_checking_remaining){
            moveptr = _gen_capture_move_checking_piece();

            if(moveptr != nullptr){
                return moveptr;
            }

            m_capture_of_checking_remaining = false;
        }

        if(m_blocking_of_checking_remaining){
            moveptr = _gen_blocking_move_checking_piece();

            if(moveptr != nullptr){
                return moveptr;
            }

            m_blocking_of_checking_remaining = false;

        }

        if(m_attacked_kg_move_remaining){
            moveptr = gen_bare_kg_move(king, false);
            
            if(moveptr != nullptr){
                return moveptr;
            }

            m_attacked_kg_move_remaining = false;
        }

        return nullptr;

    }


    cMove *cGenerator2::_gen_capture_move_checking_piece(){

        // search for capturing pieces
        vector<cPiece> defending, others;

        if(PIECES_COLORS[m_king_attackerptr->m_piece] == mBLACK){
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(defending, others, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, m_matchptr->get_last_move(), true);
        }
        else{
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(others, defending, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, m_matchptr->get_last_move(), true);
        }

        for(uint8_t j = m_step_idx; j < defending.size(); ++j){

            if(defending.at(j).m_piece == mWKG || defending.at(j).m_piece == mBKG){
                // king moves are covered by gen_kg_moves()
                continue;
            }

            if(defending.at(j).m_piece == mWPW){
                cMove *moveptr = _create_wpw_move(defending.at(j).m_xpos, defending.at(j).m_ypos, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, m_king_attackerptr->m_piece);

                m_step_idx = (j + 1);

                return moveptr;
            }
            else if(defending.at(j).m_piece == mBPW){
                cMove *moveptr = _create_bpw_move(defending.at(j).m_xpos, defending.at(j).m_ypos, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, m_king_attackerptr->m_piece);

                m_step_idx = (j + 1);

                return moveptr;
            }
            else{
                cMove *moveptr = new cMove(defending.at(j).m_xpos, defending.at(j).m_ypos, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, defending.at(j).m_piece, m_king_attackerptr->m_piece, mBLK, cMove::P_HIGH_UP);

                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                m_step_idx = (j + 1);

                return moveptr;
            }

        }

        m_step_idx = 0;
        
        m_dir_idx = 0;

        return nullptr;

    }

 
    cMove *cGenerator2::_gen_blocking_move_checking_piece(){

        // search blocking pieces
        vector<cPiece> defending, others;
        
        if(PIECES_COLORS[m_king_attackerptr->m_piece] == mBLACK){
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(defending, others, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, m_matchptr->get_last_move(), true);
        }
        else{
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(others, defending, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, m_matchptr->get_last_move(), true);
        }

        if(m_king_attackerptr->m_piece == mWPW || m_king_attackerptr->m_piece == mBPW || m_king_attackerptr->m_piece == mWKN || m_king_attackerptr->m_piece == mBKN){
            // for pawns and knights no blocking moves exist
            return nullptr;
        }

        uint8_t kg_x, kg_y;
        if(PIECES_COLORS[m_king_attackerptr->m_piece] == mWHITE){
            kg_x = m_matchptr->m_board.m_bKg_x;
            kg_y = m_matchptr->m_board.m_bKg_y;
        }
        else{
            kg_x = m_matchptr->m_board.m_wKg_x;
            kg_y = m_matchptr->m_board.m_wKg_y;
        }

        uint16_t dir = cBoard::eval_dir(kg_x, kg_y, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos);

        int8_t step_x, step_y;
        if(cBoard::eval_steps(step_x, step_y, dir) == false){
            cout << "should never occur" << endl;
            return nullptr;
        }
        
        //cout << to_string(step_x) << " " << to_string(step_y) << "should never occur" << endl;

        uint8_t dst_x, dst_y;
        if(m_matchptr->m_board.is_inbounds((kg_x + (step_x * (m_dir_idx + 1))), kg_y + (step_y * (m_dir_idx + 1)))){
            dst_x = (kg_x + (step_x * (m_dir_idx + 1)));
            dst_y = (kg_y + (step_y * (m_dir_idx + 1)));
        }
        else{
            return nullptr;
        }

        while(!(dst_x == m_king_attackerptr->m_xpos && dst_y == m_king_attackerptr->m_ypos)){

            vector<cPiece> blocking, others;
      
            if(PIECES_COLORS[m_king_attackerptr->m_piece] == mBLACK){
                m_matchptr->m_board.search_all_dirs_for_touching_pieces(blocking, others, dst_x, dst_y, m_matchptr->get_last_move(), true);
            }
            else{
                m_matchptr->m_board.search_all_dirs_for_touching_pieces(others, blocking, dst_x, dst_y, m_matchptr->get_last_move(), true);
            }
            
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

            if(PIECES_COLORS[m_king_attackerptr->m_piece] == mBLACK){
                m_matchptr->m_board.search_vertical_for_straight_pawns(blocking, others, dst_x, dst_y);
            }
            else{
                m_matchptr->m_board.search_vertical_for_straight_pawns(others, blocking, dst_x, dst_y);
            }

            for(uint8_t j = m_step_idx; j < blocking.size(); ++j){

                if(blocking.at(j).m_piece == mWKG || blocking.at(j).m_piece == mBKG){
                    continue;
                }

                if(blocking.at(j).m_piece == mWPW){
                    cMove *moveptr = _create_wpw_move(blocking.at(j).m_xpos, blocking.at(j).m_ypos, dst_x, dst_y, mBLK);

                    m_step_idx = (j + 1);

                    return moveptr;
                }
                else if(blocking.at(j).m_piece == mBPW){
                    cMove *moveptr = _create_bpw_move(blocking.at(j).m_xpos, blocking.at(j).m_ypos, dst_x, dst_y, mBLK);

                    m_step_idx = (j + 1);

                    return moveptr;
                }
                else{
                    cMove *moveptr = new cMove(blocking.at(j).m_xpos, blocking.at(j).m_ypos, dst_x, dst_y, blocking.at(j).m_piece, mBLK, mBLK, cMove::P_HIGH_UP);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    m_step_idx = (j + 1);

                    return moveptr;
                }
            }

            blocking.clear();
            others.clear();

            if(m_matchptr->m_board.is_inbounds((dst_x + step_x), dst_y + step_y)){
                dst_x += step_x;
                dst_y += step_y;

                m_step_idx = 0;

                m_dir_idx++;
            }
            else{
                m_step_idx = 0;

                m_dir_idx = 0;

                return nullptr;
            }

        }

        m_step_idx = 0;

        return nullptr;

    }
