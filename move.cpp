

    #include "./move.hpp"


    cMove::cMove(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t srcpiece, uint8_t dstpiece, uint8_t prompiece, uint8_t prio){ 
        m_src_x = src_x;
        m_src_y = src_y;
        m_dst_x = dst_x;
        m_dst_y = dst_y;
        m_srcpiece = srcpiece;
        m_dstpiece = dstpiece;
        m_prompiece = prompiece;
        m_prio = prio;
    }


    cMove::cMove(){
    }

    cMove::cMove(const cMove &move){
        m_src_x = move.m_src_x;
        m_src_y = move.m_src_y;
        m_dst_x = move.m_dst_x;
        m_dst_y = move.m_dst_y;
        m_srcpiece = move.m_srcpiece;
        m_dstpiece = move.m_dstpiece;
        m_prompiece = move.m_prompiece;  
        m_prio = move.m_prio; 
    }


    bool cMove::is_en_passant(){

        if(m_srcpiece == mWPW){
            if(m_dstpiece == mBLK && m_src_y == 4 && m_dst_y == 5){
                if((m_src_x + 1 <= 7 && m_src_x + 1 == m_dst_x) || (m_src_x - 1 >= 0 && m_src_x - 1 == m_dst_x)){
                    return true;
                }
            }
        }
        else if(m_srcpiece == mBPW){
            if(m_dstpiece == mBLK && m_src_y == 3 && m_dst_y == 2){
                if((m_src_x + 1 <= 7 && m_src_x + 1 == m_dst_x) || (m_src_x - 1 >= 0 && m_src_x - 1 == m_dst_x)){
                    return true;
                }
            }
        }

        return false;

    }


    bool cMove::is_promotion(){

        if(m_srcpiece == mWPW){
            if(m_prompiece != mBLK && m_src_y == 6 && m_dst_y == 7){
                return true;
            }
        }
        else if(m_srcpiece == mBPW){
            if(m_prompiece != mBLK && m_src_y == 1 && m_dst_y == 0){
                return true;
            }
        }

        return false;

    }


   bool cMove::is_short_castling(){

        if(m_srcpiece == mWKG){
            if(m_src_y == 0 && m_dst_y == 0 && m_src_x == 4 && m_dst_x == 6){
                return true;
            }
        }
        else if(m_srcpiece == mBKG){
            if(m_src_y == 7 && m_dst_y == 7 && m_src_x == 4 && m_dst_x == 6){
                return true;
            }
        }

        return false;

    }


    bool cMove::is_long_castling(){

        if(m_srcpiece == mWKG){
            if(m_src_y == 0 && m_dst_y == 0 && m_src_x == 4 && m_dst_x == 2){
                return true;
            }
        }
        else if(m_srcpiece == mBKG){
            if(m_src_y == 7 && m_dst_y == 7 && m_src_x == 4 && m_dst_x == 2){
                return true;
            }
        }

        return false;

    }


    string cMove::format(){

        string hyphen = "";
        string trailing = "";

        if(is_en_passant()){
            return indices_to_coord(m_src_x, m_src_y) + "x" + indices_to_coord(m_dst_x, m_dst_y) + ", e.p.";
        }

        (m_dstpiece == mBLK)? hyphen = "-" : hyphen = "x";

        if(m_prompiece != mBLK){ 
            trailing = ", " + reverse_lookup(PIECES, m_prompiece); 
        }

        return indices_to_coord(m_src_x, m_src_y) + hyphen + indices_to_coord(m_dst_x, m_dst_y) + trailing;

    }


    void cMove::coord_to_indices(uint8_t &x, uint8_t &y, string coord){

        //if(coord.length() < 4){
        //    return;
        //}

        uint8_t x1 = (uint8_t)(coord[0]);
        if(x1 > (uint8_t)'H'){
            x = x1 - (uint8_t)'a';
        }
        else{
            x = x1 - (uint8_t)'A';
        }
        uint8_t y1 = (uint8_t)(coord[1]);
        y = y1 - (uint8_t)'1';

    }


    string cMove::indices_to_coord(uint8_t x, uint8_t y){

        ostringstream outstream;
        outstream << (char)(x + (uint8_t)'a' ) << (char)(y + (uint8_t)'1');
        return outstream.str();

    }

