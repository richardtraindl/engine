

    #include "./daemon.hpp"
    #include "./evaluator.hpp"


    cDaemon::cDaemon(cMatch &match){

        m_match = &match;

        m_deep_search_cnt = 0;

        m_capture_move_cnt = 0;

        m_other_move_cnt = 0;

        if(m_match->next_color() == mWHITE){
            m_last_move_was_check = m_match->m_board.is_field_touched(m_match->m_board.m_wKg_x, m_match->m_board.m_wKg_y, mBLACK);
        }
        else{
            m_last_move_was_check = m_match->m_board.is_field_touched(m_match->m_board.m_bKg_x, m_match->m_board.m_bKg_y, mWHITE);
        }

        m_last_move_was_capture = false;

        if(! m_match->m_minutes.empty()){
            cMove move = m_match->m_minutes.back();
            if(move.m_dstpiece != mBLK || move.is_en_passant()){
                m_last_move_was_capture = true;
            }
        }

    }


    void cDaemon::reset(){

        m_deep_search_cnt = 0;

        m_capture_move_cnt = 0;

        m_other_move_cnt = 0;

    }


    bool cDaemon::is_continue(const cMove &move, const uint8_t depth){

        if(depth <= 5){
            return true;
        }

        if(m_last_move_was_check && depth < 10){
            m_deep_search_cnt++;

            if(move.m_dstpiece != mBLK || move.is_en_passant()){
                m_capture_move_cnt++;
            }
            else{
                m_other_move_cnt++;
            }

            return true;
        }

        if(m_last_move_was_capture && depth < 10){
            if(move.m_dstpiece != mBLK || move.is_en_passant()){
                if(depth > 8){
                    if(m_capture_move_cnt == 0){
                        m_deep_search_cnt++;
                        m_capture_move_cnt++;
                        return true;
                    }
                }
                else{
                    m_deep_search_cnt++;
                    m_capture_move_cnt++;
                    return true;
                }
            }
            else{
                if(depth > 8){
                    if(m_other_move_cnt == 0){
                        m_deep_search_cnt++;
                        m_other_move_cnt++;
                        return true;
                    }
                }
                else{
                    m_deep_search_cnt++;
                    m_other_move_cnt++;
                    return true;
                }
            }
        }

        if(cEvaluator::is_hotspot(*m_match, move.m_dst_x, move.m_dst_y) && depth < 10){
            if(move.m_dstpiece != mBLK || move.is_en_passant()){
                m_deep_search_cnt++;
                m_capture_move_cnt++;
                return true;
            }
            else{
                m_deep_search_cnt++;
                m_other_move_cnt++;
                return true;
            }
        }

        /*if(depth < 10 && move.m_prio <= cMove::P_HIGH){ 
            return true;
        }*/

        return false;

    }

