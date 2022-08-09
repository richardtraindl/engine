

    #include "./daemon.hpp"
    #include "./evaluator.hpp"
    #include "./util/debug.hpp"
    

    cDaemon::cDaemon(cMatch &match){

        m_move_cnt = 0;

        m_capture_move_cnt = 0;

        m_single_capture_move_prio = cMove::P_HIGH_UP;

        m_other_move_cnt = 0;

        m_last_move_was_check = match.is_king_attacked();

        m_last_move_was_capture = false;

        m_last_move_was_promotion = false;

        if(! match.m_minutes.empty()){
            cMove move = match.m_minutes.back();
            if(move.m_dstpiece != mBLK || move.is_en_passant()){
                m_last_move_was_capture = true;
            }

            if(move.m_prompiece != mBLK){
                m_last_move_was_promotion = true;
            }
        }

    }



    bool cDaemon::is_continue(cMatch &match, const cMove &move, const uint8_t depth, const uint8_t count){

        if(depth <= 3){
            return true;
            /*if(count > 24){
                return false;
            }
            else{
                return true;
            }*/
        }
        /*else{
            if(count > 16){
                return false;
            }
            if(move.m_prio == cMove::P_MEDIUM){
                return false;
            }
        }*/


        // take all moves after check
        if(m_last_move_was_check && depth <= 12){
            if(move.m_dstpiece != mBLK || move.is_en_passant()){
                m_capture_move_cnt++;
            }
            else{
                m_other_move_cnt++;
            }

            return true;
        }


        // take all promotion moves
        if(move.m_prompiece != mBLK && depth <= 12){
            if(move.m_dstpiece != mBLK){
                m_capture_move_cnt++;
            }
            else{
                m_other_move_cnt++;
            }

            return true;
        }


        // take all king attacking moves
        if(cEvaluator::does_move_attack_king(match, move) && depth <= 12){
            if(move.m_dstpiece != mBLK || move.is_en_passant()){
                m_capture_move_cnt++;
            }
            else{
                m_other_move_cnt++;
            }

            return true;
        }


        // take all capture moves and one silent move
        if((move.m_dstpiece != mBLK || move.is_en_passant()) && depth <= 12){
            m_capture_move_cnt++;

            return true;
        }
        else{
            if(m_capture_move_cnt > 0 && m_other_move_cnt == 0){
                m_other_move_cnt++;

                return true;
            }
        }


        uint8_t dstfield_state = cEvaluator::eval_field_state(match, move.m_dst_x, move.m_dst_y);

        bool is_dstfield_save = cEvaluator::is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece]);


        // take all moves attacking/supporting a hotspot
        if(cEvaluator::does_move_touch_soft_pinned(match, move) && depth <= 12){
            if(is_dstfield_save){
                if(move.m_dstpiece != mBLK || move.is_en_passant()){
                    m_capture_move_cnt++;

                    return true;
                }
                else{
                    m_other_move_cnt++;

                    return true;
                }
            }
        }


        // take all escape move from a soft pin
        if(cEvaluator::does_move_escape_soft_pin(match, move) && depth <= 12){
            if(is_dstfield_save){
                if(move.m_dstpiece != mBLK || move.is_en_passant()){
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
