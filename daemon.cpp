

    #include "./daemon.hpp"
    #include "./evaluator.hpp"
    #include "./util/debug.hpp"
    

    cDaemon::cDaemon(cMatch &match){

        m_move_cnt = 0;

        m_capture_move_cnt = 0;

        m_other_move_cnt = 0;

        m_last_move_was_check = match.is_king_attacked();

        m_last_move_was_capture = false;

        m_last_move_was_promotion = false;

        if(! match.m_minutes.empty()){
            cMove move = match.m_minutes.back();
            if(move.is_capture()){
                m_last_move_was_capture = true;
            }

            if(move.m_prompiece != mBLK){
                m_last_move_was_promotion = true;
            }
        }

        m_postponed_moveptr = nullptr;

    }



    bool cDaemon::is_continue(cMatch &match, const cMove &move, const uint8_t depth){

        if(depth <= 3){
            return true;
        }


        uint8_t middepth = 6;
        uint8_t maxdepth = 12;


        // promotion move
        if(move.m_prompiece != mBLK && depth <= maxdepth){
            //incr_tracer(depth, 1);
            if(move.m_dstpiece != mBLK){
                m_capture_move_cnt++;
            }
            else{
                m_other_move_cnt++;
            }

            return true;
        }


        // move after check
        if(m_last_move_was_check && depth <= maxdepth){
            //incr_tracer(depth, 2); // !!
            if(move.is_capture()){
                m_capture_move_cnt++;
            }
            else{
                m_other_move_cnt++;
            }

            return true;
        }


        // king attacking move
        if(cEvaluator::does_move_do_check(match, move) && depth <= maxdepth){
            if(depth <= maxdepth){
                //incr_tracer(depth, 3);
                if(move.is_capture()){
                    m_capture_move_cnt++;
                }
                else{
                    m_other_move_cnt++;
                }

                return true;
            }

            /*if(depth == 9 || depth == 10){
                // search for forced mate
                if(cEvaluator::find_mate(match, move, 3)){
                    return true;
                }
            }*/
        }


        // capture move
        if(move.is_capture()){
            if(depth <= maxdepth){ //middepth
                //incr_tracer(depth, 4);
                m_capture_move_cnt++;

                return true;
            }
            /*else if(depth <= maxdepth){
                if(m_last_move_was_capture && (m_capture_move_cnt + 1 <= 2)){ 
                    //incr_tracer(depth, 5); // !!
                    m_capture_move_cnt++;

                    return true;
                }
            }*/
        }
        // silent move if capture move has occured
        else if(m_capture_move_cnt > 0 && m_other_move_cnt == 0 && depth <= maxdepth){
            //incr_tracer(depth, 6);
            m_other_move_cnt++;

            return true;
        }


        cPiece *wlowest = nullptr;
        cPiece *blowest = nullptr;
        uint8_t dstfield_state = cEvaluator::eval_field_state(wlowest, blowest, match, move.m_srcpiece, move.m_dst_x, move.m_dst_y);

        bool is_dstfield_save = cEvaluator::is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece]);


        // moves attacking/supporting a hotspot
        if(cEvaluator::does_move_touch_weak_piece(match, move) && depth <= middepth){ 
            if(is_dstfield_save){
                //incr_tracer(depth, 7);
                if(move.is_capture()){
                    m_capture_move_cnt++;

                    return true;
                }
                else{
                    m_other_move_cnt++;

                    return true;
                }
            }
        }


        // escape move from a soft pin
        if(cEvaluator::does_move_escape_soft_pin(match, move) && depth <= middepth){
            if(is_dstfield_save){
                //incr_tracer(depth, 8);
                if(move.is_capture()){
                    m_capture_move_cnt++;

                    return true;
                }
                else{
                    m_other_move_cnt++;

                    return true;
                }
            }
        }


        // escape move from attack(s)
        if(cEvaluator::does_move_escape_attacks(match, move) && depth <= middepth){
            bool hasmoved = false;

            for(const cMove &emove : m_escape_moves){
                if(emove.m_src_x == move.m_src_x && emove.m_src_y == move.m_src_y){
                    hasmoved = true;
                    break;
                }
            }

            if( ! hasmoved ){
                //incr_tracer(depth, 9);
                m_escape_moves.push_back(move);

                if(move.is_capture()){
                    m_capture_move_cnt++;

                    return true;
                }
                else{
                    m_other_move_cnt++;

                    return true;
                }
            }
        }


/*
        // take only one move after promotion, no limit
        if(m_last_move_was_promotion){
            if(m_capture_move_cnt == 0 && m_other_move_cnt == 0){
                if(move.m_dstpiece != mBLK || move.is_en_passant()){
                    m_capture_move_cnt++;
                }
                else{
                    m_other_move_cnt++;
                }

                return true;
            }
        }

        // take only one or two move(s) after capture, no limit
        if(m_last_move_was_capture){
            if(move.m_dstpiece != mBLK || move.is_en_passant()){
                if(m_capture_move_cnt == 0){
                    m_capture_move_cnt++;

                    m_single_capture_move_prio = move.m_prio;

                    return true;
                }
            }
            else{
                if(m_other_move_cnt == 0 && m_capture_move_cnt == 1 && m_single_capture_move_prio > cMove::P_MEDIUM){
                    m_other_move_cnt++;

                    return true;
                }
            }
        }
*/
        return false;

    }

