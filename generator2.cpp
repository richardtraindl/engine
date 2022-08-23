

  #include "./generator2.hpp"
  #include "./evaluator.hpp"


    //*****************************************
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

        if(m_check_cnt == 2){
            m_king_attackerptr = nullptr;

            if(m_matchptr->next_color() == mWHITE){
                m_gen_x = m_matchptr->m_board.m_wKg_x;

                m_gen_y = m_matchptr->m_board.m_wKg_y;
            }
            else{
                m_gen_x = m_matchptr->m_board.m_bKg_x;

                m_gen_y = m_matchptr->m_board.m_bKg_y;
            }

            m_king_move_remaining = true;

            m_king_move_has_started = false;

            m_capture_of_checking_remaining = false;

            m_blocking_of_checking_remaining = false;
        }
        else if(attacking_pieces.size() == 1){
            m_king_attackerptr = new cPiece(attacking_pieces.front());

            if(m_matchptr->next_color() == mWHITE){
                m_gen_x = m_matchptr->m_board.m_wKg_x;

                m_gen_y = m_matchptr->m_board.m_wKg_y;
            }
            else{
                m_gen_x = m_matchptr->m_board.m_bKg_x;

                m_gen_y = m_matchptr->m_board.m_bKg_y;
            }

            m_king_move_remaining = true;

            m_king_move_has_started = false;

            m_capture_of_checking_remaining = true;

            m_capture_of_checking_has_startet = false;

            m_blocking_of_checking_remaining = true;

            m_blocking_of_checking_has_started = false;
        }
        else{
            m_king_attackerptr = nullptr;

            m_gen_x = 0;

            m_gen_y = 0;

            m_king_move_remaining = false;

            m_capture_of_checking_remaining = false;

            m_blocking_of_checking_remaining = false;
        }

        m_prom_idx = 0;

        m_step_idx = 0;

        m_dir_idx = 0;

        m_gen_end = false;

        m_postponedptr = nullptr;

    }
    //*****************************************



    //*****************************************
    cGenerator2::~cGenerator2(){

        if(m_king_attackerptr != nullptr){
            delete m_king_attackerptr;
        }

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::gen_move(){
      
        cMove *moveptr = _gen_move();

        if(m_postponedptr == nullptr && moveptr == nullptr){
            return moveptr;
        }

        if(m_postponedptr != nullptr && moveptr == nullptr){
            moveptr = m_postponedptr;
            
            m_postponedptr = nullptr;

            return moveptr;
        }

        if(m_postponedptr == nullptr && moveptr != nullptr){
            cMove *tmpmoveptr = _gen_move();

            if(tmpmoveptr == nullptr){
                return moveptr;
            }
            else{
                if(moveptr->m_prio <= tmpmoveptr->m_prio){
                    return moveptr;
                }
                else{
                    m_postponedptr = moveptr;

                    return tmpmoveptr;
                }
            }
        }

        if(m_postponedptr != nullptr && moveptr != nullptr){
            if(moveptr->m_prio <= m_postponedptr->m_prio){
                return moveptr;
            }
            else{
                cMove *tmpmoveptr = moveptr;

                moveptr = m_postponedptr;

                m_postponedptr = tmpmoveptr;

                return moveptr;
            }
        }

        return moveptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::_gen_move(){

        uint8_t color = m_matchptr->next_color();

        if(m_check_cnt == 2){
            if(m_king_move_remaining){
                if( ! m_king_move_has_started){
                    m_king_move_has_started = true;

                    m_dir_idx = 0;
                    m_step_idx = 0;
                }

                cMove *moveptr = nullptr;

                if(color == mWHITE){
                    moveptr = gen_kg_move(mWKG, true);
                }
                else{
                    moveptr = gen_kg_move(mBKG, true);
                }

                if(moveptr != nullptr){
                    return moveptr;
                }
                else{
                    m_king_move_remaining = false;
                }
            }

            return nullptr;
        }
        else if(m_check_cnt == 1){
            if( m_king_move_remaining){
                if( ! m_king_move_has_started){
                    m_king_move_has_started = true;

                    m_step_idx = 0;
                    m_dir_idx = 0;
                }

                cMove *moveptr = nullptr;

                if(color == mWHITE){
                    moveptr = gen_kg_move(mWKG, true);
                }
                else{
                    moveptr = gen_kg_move(mBKG, true);
                }

                if(moveptr != nullptr){
                    return moveptr;
                }

                m_king_move_remaining = false;
            }

            if(m_capture_of_checking_remaining){
                if( ! m_capture_of_checking_has_startet ){
                    m_capture_of_checking_has_startet = true;

                    m_step_idx = 0;
                    m_dir_idx = 0;
                }

                cMove *moveptr = nullptr;

                moveptr = gen_capture_move_checking_piece();

                if(moveptr != nullptr){
                    return moveptr;
                }

                m_capture_of_checking_remaining = false;
            }

            if(m_blocking_of_checking_remaining){
                if( ! m_blocking_of_checking_has_started ){
                    m_blocking_of_checking_has_started = true;

                    m_step_idx = 0;
                    m_dir_idx = 0;
                }

                cMove *moveptr = nullptr;

                moveptr = gen_blocking_move_checking_piece();

                if(moveptr != nullptr){
                    return moveptr;
                }

                m_blocking_of_checking_remaining = false;
            }

            return nullptr;
        }
        else{
            while(m_gen_end == false){

                uint8_t piece = m_matchptr->m_board.getfield(m_gen_x, m_gen_y);

                if(PIECES_COLORS[piece] != color){
                    incr_gen(0, 0, 0);

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
                else if(piece == mWKG || piece == mBKG){ 
                    cMove *moveptr = gen_kg_move(piece, false);

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
                else if(piece == mWRK || piece == mBRK || piece == mWBP || piece == mBBP || piece == mWQU || piece == mBQU){ 
                    cMove *moveptr = gen_long_range_move(piece);

                    if(moveptr != nullptr){
                        return moveptr;
                    }
                }
                else{
                    cout << "NOOOOOOOOOOOOOOOOOOOOOOO";
                    incr_gen(0, 0, 0);
                }

            }

            return nullptr;
       }

    }
    //*****************************************



    //*****************************************
    void cGenerator2::incr_gen(const uint8_t max_prom_cnt, const uint8_t max_step_cnt, const uint8_t max_dir_cnt){

        if((m_prom_idx + 1) < max_prom_cnt){
            m_prom_idx++;
        }
        else if((m_step_idx + 1) < max_step_cnt){
            m_prom_idx = 0;
            m_step_idx++;
        }
        else if((m_dir_idx + 1) < max_dir_cnt){
            m_prom_idx = 0;
            m_step_idx = 0;
            m_dir_idx++;
        }
        else{
            m_prom_idx = 0;
            m_step_idx = 0;
            m_dir_idx = 0;

            if(cBoard::is_inbounds(m_gen_x + 1, m_gen_y)){
                m_gen_x++;
            }
            else if(cBoard::is_inbounds(0, m_gen_y + 1)){
                m_gen_x = 0;
                m_gen_y++;
            }
            else{
                m_gen_end = true;
            }
        }

    }
    //*****************************************



    //*****************************************
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

                            if(moveptr != nullptr && moveptr->is_promotion()){
                                incr_gen(4, 0, 4);
                            }
                            else{
                                incr_gen(0, 0, 4);
                            }

                            return moveptr;

                        }
                        else if(dstpiece == mBLK && m_gen_y == 4){ // en passant
                            cMove prevmove = m_matchptr->m_minutes.back();
                            uint8_t enpiece = m_matchptr->m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                            if(enpiece == mBPW && prevmove.m_src_y == 6 && prevmove.m_dst_y == 4 && prevmove.m_src_x == dst_x){
                                cMove *moveptr = new cMove(m_gen_x, m_gen_y, dst_x, dst_y, pawn, dstpiece, mBLK, cMove::P_BOTTOM);

                                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                                m_dir_idx = idx;

                                incr_gen(0, 0, 4);

                                return moveptr;
                            }
                        }
                    }
                }

            }

            m_dir_idx = 1;

            incr_gen(0, 0, 4);

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

                        if(moveptr != nullptr && moveptr->is_promotion()){
                            incr_gen(4, 0, 4);
                        }
                        else{
                            incr_gen(0, 0, 4);
                        }

                        return moveptr;
                    }
                }
            }

            m_dir_idx = 2;

            incr_gen(0, 0, 4);
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

                        incr_gen(0, 0, 4);

                        return moveptr;
                    }
                }
            }
            m_dir_idx = 3;

            incr_gen(0, 0, 4);
        }

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::_create_wpw_move(const uint8_t src_x, const uint8_t src_y, const uint8_t dst_x, const uint8_t dst_y, const uint8_t dstpiece){

        if(src_y == 6 && dst_y == 7){
            uint8_t prom_pieces[4] = { mWQU, mWRK, mWBP, mWKN };

            cMove *moveptr = new cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, prom_pieces[m_prom_idx], cMove::P_PROMOTION + m_prom_idx);

            cEvaluator::priorize_move(*m_matchptr, *moveptr);

            return moveptr;
        }
        else{
            cMove *moveptr = new cMove(src_x, src_y, dst_x, dst_y, mWPW, dstpiece, mBLK, cMove::P_BOTTOM);

            cEvaluator::priorize_move(*m_matchptr, *moveptr);

            return moveptr;
        }
        
        return nullptr;

    }
    //*****************************************



    //*****************************************
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

                            if(moveptr != nullptr && moveptr->is_promotion()){
                                incr_gen(4, 0, 4);
                            }
                            else{
                                incr_gen(0, 0, 4);
                            }

                            return moveptr;

                        }
                        else if(dstpiece == mBLK && m_gen_y == 3){ // en passant
                            cMove prevmove = m_matchptr->m_minutes.back();
                            uint8_t enpiece = m_matchptr->m_board.getfield(prevmove.m_dst_x, prevmove.m_dst_y);

                            if(enpiece == mWPW && prevmove.m_src_y == 1 && prevmove.m_dst_y == 3 && prevmove.m_src_x == dst_x){
                                cMove *moveptr = new cMove(m_gen_x, m_gen_y, dst_x, dst_y, pawn, dstpiece, mBLK, cMove::P_BOTTOM);

                                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                                m_dir_idx = idx;

                                incr_gen(0, 0, 4);

                                return moveptr;
                            }
                        }
                    }
                }

            }

            m_dir_idx = 1;

            incr_gen(0, 0, 4);

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

                        if(moveptr != nullptr && moveptr->is_promotion()){
                            incr_gen(4, 0, 4);
                        }
                        else{
                            incr_gen(0, 0, 4);
                        }

                        return moveptr;
                    }
                }
            }

            m_dir_idx = 2;

            incr_gen(0, 0, 4);
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

                        incr_gen(0, 0, 4);

                        return moveptr;
                    }
                }
            }
            m_dir_idx = 3;

            incr_gen(0, 0, 4);
        }

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::_create_bpw_move(const uint8_t src_x, const uint8_t src_y, const uint8_t dst_x, const uint8_t dst_y, const uint8_t dstpiece){

        if(src_y == 1 && dst_y == 0){
            uint8_t prom_pieces[4] = { mBQU, mBRK, mBBP, mBKN };

            cMove *moveptr = new cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, prom_pieces[m_prom_idx], cMove::P_PROMOTION + m_prom_idx);

            cEvaluator::priorize_move(*m_matchptr, *moveptr);

            return moveptr;
        }
        else{
            cMove *moveptr = new cMove(src_x, src_y, dst_x, dst_y, mBPW, dstpiece, mBLK, cMove::P_BOTTOM);

            cEvaluator::priorize_move(*m_matchptr, *moveptr);

            return moveptr;
        }
        
        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::gen_kn_move(const uint8_t knight){

        uint16_t pindir = m_matchptr->m_board.eval_pindir(m_gen_x, m_gen_y);
        if(pindir != mNO_DIR){ 
            incr_gen(0, 0, 0);

            return nullptr; 
        }

        for(uint8_t idx = m_dir_idx; idx < size(cPiece::kn_steps); ++idx){

            if(cBoard::is_inbounds(m_gen_x + cPiece::kn_steps[idx].m_xstep, m_gen_y + cPiece::kn_steps[idx].m_ystep)){

                uint8_t dst_x = m_gen_x + cPiece::kn_steps[idx].m_xstep;
                uint8_t dst_y = m_gen_y + cPiece::kn_steps[idx].m_ystep;

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] != PIECES_COLORS[knight]){
                    cMove *moveptr =  new cMove(m_gen_x, m_gen_y, dst_x, dst_y, knight, dstpiece, mBLK, cMove::P_BOTTOM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    m_dir_idx = idx;

                    incr_gen(0, 0, 8);

                    return moveptr;
                }
            }

        }

        incr_gen(0, 0, 0);

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::gen_long_range_move(const uint8_t piece){

        int8_t steps[][7][2] = {
            { { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 }, { 6, 0 }, { 7, 0 } },
            { { -1, 0 }, { -2, 0 }, { -3, 0 }, { -4, 0 }, { -5, 0 }, { -6, 0 }, { -7, 0 } },
            { { 0, 1 }, { 0 , 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 0, 7 } },
            { { 0, -1 }, { 0 , -2 }, { 0, -3 }, { 0, -4 }, { 0, -5 }, { 0, -6 }, { 0, -7 } }, 
            { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, { 6, 6 }, { 7, 7 } },
            { { -1, -1 }, { -2, -2 }, { -3, -3 }, { -4, -4 }, { -5, -5 }, { -6, -6 }, { -7, -7 } },
            { { -1, 1 }, { -2, 2 }, { -3, 3 }, { -4, 4 }, { -5, 5 }, { -6, 6 }, { -7, 7 } },
            { { 1, -1 }, { 2, -2 }, { 3, -3 }, { 4, -4 }, { 5, -5 }, { 6, -6 }, { 7, -7 } } 
            };

        uint8_t offset = 0;
        uint8_t maxidx = 4;

        if(piece == mWBP || piece == mBBP){
            offset = 4;
        }
        else if(piece == mWQU || piece == mBQU){
            maxidx = 8;
        }

        uint16_t pindir = m_matchptr->m_board.eval_pindir(m_gen_x, m_gen_y);

        for(uint8_t idx = m_dir_idx; idx < maxidx; ++idx){
            
            uint8_t dst_x = m_gen_x;
            uint8_t dst_y = m_gen_y;

            for(uint8_t j = m_step_idx; j < 7; ++j){

                if( ! cBoard::is_inbounds(dst_x + steps[idx+offset][j][0], dst_y + steps[idx+offset][j][1]) ){
                    break;
                }

                dst_x += steps[idx+offset][j][0];
                dst_y += steps[idx+offset][j][1];

                if(pindir != mNO_DIR && (pindir & cBoard::eval_dir(m_gen_x, m_gen_y, dst_x, dst_y)) == 0){ 
                  break;
                }

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] == PIECES_COLORS[piece]){
                    break;
                }

                cMove *moveptr = new cMove(m_gen_x, m_gen_y, dst_x, dst_y, piece, dstpiece, mBLK, cMove::P_BOTTOM);

                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                m_step_idx = j;

                m_dir_idx = idx;

                if(dstpiece == mBLK){
                    incr_gen(0, 7, maxidx);
                }
                else{
                    incr_gen(0, 0, maxidx);
                }

                return moveptr;

            }

            m_dir_idx = idx;

            incr_gen(0, 0, maxidx);

        }

        incr_gen(0, 0, maxidx);

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::gen_kg_move(const uint8_t king, const bool isattacked){

        uint8_t kg_x, kg_y, maxidx;

        uint8_t idx = m_dir_idx;

        maxidx = 10; //include castlings, makes also sense for isattacked, because of incr_gen...

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

        for(;idx < size(cPiece::qu_steps); ++idx){

            if(cBoard::is_inbounds((kg_x + cPiece::qu_steps[idx].m_xstep), (kg_y + cPiece::qu_steps[idx].m_ystep))){

                uint8_t dst_x = (kg_x + cPiece::qu_steps[idx].m_xstep);
                uint8_t dst_y = (kg_y + cPiece::qu_steps[idx].m_ystep);

                uint8_t dstpiece = m_matchptr->m_board.getfield(dst_x, dst_y);

                if(PIECES_COLORS[dstpiece] != PIECES_COLORS[king]){

                    if( !(board.is_field_touched(dst_x, dst_y, REVERSED_COLORS[PIECES_COLORS[king]])) ){

                        cMove *moveptr = new cMove(kg_x, kg_y, dst_x, dst_y, king, dstpiece, mBLK, cMove::P_BOTTOM);

                        cEvaluator::priorize_move(*m_matchptr, *moveptr);

                        m_dir_idx = idx;
                        incr_gen(0, 0, maxidx);

                        return moveptr;
                    }
                }
            }

            m_dir_idx = idx;
            incr_gen(0, 0, maxidx);

        }

        if(isattacked == false && m_dir_idx == 8){
            cMove *moveptr = nullptr;

            if(PIECES_COLORS[king] == mWHITE){
                moveptr = _gen_wkg_sh_castl_move();
            }
            else{
                moveptr = _gen_bkg_sh_castl_move();
            }

            incr_gen(0, 0, maxidx);

            if(moveptr != nullptr){
                return moveptr;
            }
        }


        if(isattacked == false && m_dir_idx == 9){
            cMove *moveptr = nullptr;

            if(PIECES_COLORS[king] == mWHITE){
                moveptr = _gen_wkg_lg_castl_move();
            }
            else{
                moveptr = _gen_bkg_lg_castl_move();
            }

            incr_gen(0, 0, maxidx);

            if(moveptr != nullptr){
                return moveptr;
            }
        }

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::_gen_wkg_sh_castl_move(){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);

        if(board.m_wKg_has_moved_at == 0 && board.m_wRkH_has_moved_at == 0 && cBoard::is_inbounds(board.m_wKg_x + 3, board.m_wKg_y)){

            if(board.m_wKg_y == 0 && board.getfield(board.m_wKg_x + 1, board.m_wKg_y) == mBLK && board.getfield(board.m_wKg_x + 2, board.m_wKg_y) == mBLK && board.getfield(board.m_wKg_x + 3, board.m_wKg_y) == mWRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(board.m_wKg_x + i, board.m_wKg_y, mBLACK)){
                        is_touched = true;

                        break;
                    }

                }

                if(is_touched == false){
                    cMove *moveptr = new cMove(board.m_wKg_x, board.m_wKg_y, board.m_wKg_x + 2, board.m_wKg_y, mWKG, mBLK, mBLK, cMove::P_BOTTOM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    return moveptr;
                }
            }
        }

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::_gen_wkg_lg_castl_move(){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_wKg_x, board.m_wKg_y, mBLK);

        if(board.m_wKg_has_moved_at == 0 && board.m_wRkA_has_moved_at == 0 && cBoard::is_inbounds(board.m_wKg_x - 4, board.m_wKg_y)){

            if(m_gen_y == 0 && board.getfield(board.m_wKg_x - 1, board.m_wKg_y) == mBLK && board.getfield(board.m_wKg_x - 2, board.m_wKg_y) == mBLK && board.getfield(board.m_wKg_x - 3, board.m_wKg_y) == mBLK && board.getfield(board.m_wKg_x - 4, board.m_wKg_y) == mWRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(board.m_wKg_x - i, board.m_wKg_y, mBLACK)){
                        is_touched = true;

                        break;
                    }

                }

                if(is_touched == false){
                    cMove *moveptr = new cMove(board.m_wKg_x, board.m_wKg_y, board.m_wKg_x - 2, board.m_wKg_y, mWKG, mBLK, mBLK, cMove::P_BOTTOM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    return moveptr;
                }
            }
        }

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::_gen_bkg_sh_castl_move(){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);

        if(board.m_bKg_has_moved_at == 0 && board.m_bRkH_has_moved_at == 0 && cBoard::is_inbounds(board.m_bKg_x + 3, board.m_bKg_y)){

            if(m_gen_y == 7 && board.getfield(board.m_bKg_x + 1, board.m_bKg_y) == mBLK && board.getfield(board.m_bKg_x + 2, board.m_bKg_y) == mBLK && board.getfield(board.m_bKg_x + 3, board.m_bKg_y) == mBRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(board.m_bKg_x + i, board.m_bKg_y, mWHITE)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove *moveptr = new cMove(board.m_bKg_x, board.m_bKg_y, board.m_bKg_x + 2, board.m_bKg_y, mBKG, mBLK, mBLK, cMove::P_BOTTOM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    return moveptr;
                }
            }
        }

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::_gen_bkg_lg_castl_move(){

        cBoard board(m_matchptr->m_board);
        board.setfield(board.m_bKg_x, board.m_bKg_y, mBLK);

        if(board.m_bKg_has_moved_at == 0 && board.m_bRkA_has_moved_at == 0 && cBoard::is_inbounds(board.m_bKg_x - 4, board.m_bKg_y)){

            if(m_gen_y == 7 && board.getfield(board.m_bKg_x - 1, board.m_bKg_y) == mBLK && board.getfield(board.m_bKg_x - 2, board.m_bKg_y) == mBLK && board.getfield(board.m_bKg_x - 3, board.m_bKg_y) == mBLK && board.getfield(board.m_bKg_x - 4, board.m_bKg_y) == mBRK){

                bool is_touched = false;

                for(uint8_t i = 0; i < 3; ++i){

                    if(board.is_field_touched(board.m_bKg_x - i, board.m_bKg_y, mWHITE)){
                        is_touched = true;
                        break;
                    }

                }

                if(is_touched == false){
                    cMove *moveptr = new cMove(board.m_bKg_x, board.m_bKg_y, board.m_bKg_x - 2, board.m_bKg_y, mBKG, mBLK, mBLK, cMove::P_BOTTOM);

                    cEvaluator::priorize_move(*m_matchptr, *moveptr);

                    return moveptr;
                }
            }
        }

        return nullptr;

    }
    //*****************************************



    //*****************************************
    cMove *cGenerator2::gen_capture_move_checking_piece(){

        // search capturing pieces
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
                cMove *moveptr = new cMove(defending.at(j).m_xpos, defending.at(j).m_ypos, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, defending.at(j).m_piece, m_king_attackerptr->m_piece, mBLK, cMove::P_BOTTOM);

                cEvaluator::priorize_move(*m_matchptr, *moveptr);

                m_step_idx = (j + 1);

                return moveptr;
            }

        }

        m_prom_idx = 0;
        
        m_step_idx = 0;
        
        m_dir_idx = 0;

        return nullptr;

    }
    //*****************************************
 
 
 
    //*****************************************
    cMove *cGenerator2::gen_blocking_move_checking_piece(){

        if(m_king_attackerptr == nullptr){
            return nullptr;
        }

        // search blocking pieces
        vector<cPiece> defending, others;
        
        if(PIECES_COLORS[m_king_attackerptr->m_piece] == mBLACK){
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(defending, others, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, m_matchptr->get_last_move(), true);
        }
        else{
            m_matchptr->m_board.search_all_dirs_for_touching_pieces(others, defending, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos, m_matchptr->get_last_move(), true);
        }

        // for pawns and knights no blocking moves exist
        if(m_king_attackerptr->m_piece == mWPW || m_king_attackerptr->m_piece == mBPW || m_king_attackerptr->m_piece == mWKN || m_king_attackerptr->m_piece == mBKN){
            return nullptr;
        }

        uint8_t kg_x, kg_y;
        if(PIECES_COLORS[m_king_attackerptr->m_piece] == mBLACK){
            kg_x = m_matchptr->m_board.m_wKg_x;
            kg_y = m_matchptr->m_board.m_wKg_y;
        }
        else{
            kg_x = m_matchptr->m_board.m_bKg_x;
            kg_y = m_matchptr->m_board.m_bKg_y;
        }

        uint16_t dir = cBoard::eval_dir(kg_x, kg_y, m_king_attackerptr->m_xpos, m_king_attackerptr->m_ypos);

        int8_t step_x, step_y;
        if(cBoard::eval_steps(step_x, step_y, dir) == false){
            cout << "should never occur" << endl;
            return nullptr;
        }
        
        uint8_t dst_x, dst_y;
        if(m_matchptr->m_board.is_inbounds((kg_x + (step_x * (m_dir_idx + 1))), kg_y + (step_y * (m_dir_idx + 1)))){
            dst_x = (kg_x + (step_x * (m_dir_idx + 1)));
            dst_y = (kg_y + (step_y * (m_dir_idx + 1)));
        }
        else{
            return nullptr;
        }

        while( !(dst_x == m_king_attackerptr->m_xpos && dst_y == m_king_attackerptr->m_ypos) ){

            vector<cPiece> blocking, others;
      
            if(PIECES_COLORS[m_king_attackerptr->m_piece] == mBLACK){
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
            if(PIECES_COLORS[m_king_attackerptr->m_piece] == mBLACK){
                m_matchptr->m_board.search_vertical_for_straight_pawns(blocking, others, dst_x, dst_y);
            }
            else{
                m_matchptr->m_board.search_vertical_for_straight_pawns(others, blocking, dst_x, dst_y);
            }

            for(uint8_t j = m_step_idx; j < blocking.size(); ++j){

                if(blocking.at(j).m_piece == mWKG || blocking.at(j).m_piece == mBKG){
                    m_step_idx = (j + 1);

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
                    cMove *moveptr = new cMove(blocking.at(j).m_xpos, blocking.at(j).m_ypos, dst_x, dst_y, blocking.at(j).m_piece, mBLK, mBLK, cMove::P_BOTTOM);

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

        m_dir_idx = 0;

        return nullptr;

    }
  //*****************************************
 


