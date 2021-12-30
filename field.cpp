

    #include "./field.hpp"


    cField::cField(const cBoard *board, uint8_t pos_x, uint8_t pos_y){ //  : m_board(board)
        m_board = board;
        m_pos_x = pos_x;
        m_pos_y = pos_y;

        fill();

    }


    void cField::fill(){
        m_piece = m_board->getfield(m_pos_x, m_pos_y);
        m_wtouchcnt = 0;
        m_btouchcnt = 0;
        m_wsoftpincnt = 0;
        m_bsoftpincnt = 0;
        m_wlowesttouch = mWKG;
        m_blowesttouch = mBKG;

        vector<cPiece> wpieces, bpieces;
        m_board->search_for_all_touching_pieces(wpieces, bpieces, m_pos_x, m_pos_y);

        for(const cPiece &wpiece : wpieces){
            uint8_t state = m_board->eval_pin_state(wpiece.m_piece, wpiece.m_src_x, wpiece.m_src_y);
            if(state == cBoard::PINNED_NO){
                m_wtouchcnt++;
            }
            else if(state == cBoard::PINNED_SOFT){
                m_wsoftpincnt++;
            }
            else{
                continue;
            }
            if(PIECES_RANKS[wpiece.m_piece] < PIECES_RANKS[m_wlowesttouch]){
                m_wlowesttouch = wpiece.m_piece;
            }
        }

        for(const cPiece &bpiece : bpieces){
            uint8_t state = m_board->eval_pin_state(bpiece.m_piece, bpiece.m_src_x, bpiece.m_src_y);
            if(state == cBoard::PINNED_NO){
                m_btouchcnt++;
            }
            else if(state == cBoard::PINNED_SOFT){
                m_bsoftpincnt++;
            }
            else{
                continue;
            }
            if(PIECES_RANKS[bpiece.m_piece] < PIECES_RANKS[m_blowesttouch]){
                m_blowesttouch = bpiece.m_piece;
            }
        }

    }


    bool cField::is_clear(){
        return (m_wtouchcnt + m_wsoftpincnt + m_btouchcnt + m_bsoftpincnt) == 0;

    }


    bool cField::is_only_wtouched(){
        return (m_wtouchcnt + m_wsoftpincnt > 0) && (m_btouchcnt + m_bsoftpincnt == 0);
    }


    bool cField::is_only_btouched(){
        return (m_wtouchcnt + m_wsoftpincnt == 0) && (m_btouchcnt + m_bsoftpincnt > 0);

    }


    /*uint8_t cField::get_advant_color(){
        uint8_t wadvant = (m_wtouchcnt * 2) + m_wsoftpincnt;
        uint8_t badvant = (m_btouchcnt * 2) + m_bsoftpincnt;

        if(PIECES_COLORS[m_piece] == mWHITE && badvant > 0 && PIECES_RANKS[m_blowesttouch] < PIECES_RANKS[m_piece]){
            return mBLACK;
        }
        else if(PIECES_COLORS[m_piece] == mBLACK && wadvant > 0 && PIECES_RANKS[m_wlowesttouch] < PIECES_RANKS[m_piece]){
            return mWHITE;
        }
        else{
            if(wadvant > badvant){
                return mWHITE;
            }
            else if(wadvant < badvant){
                return mBLACK;
            }
            else{
                return mBLANK;
            }
        }

    }*/


    int8_t cField::eval_advant_score(){
        uint8_t wadvant = (m_wtouchcnt * 2) + m_wsoftpincnt;
        uint8_t badvant = (m_btouchcnt * 2) + m_bsoftpincnt;

        if(PIECES_COLORS[m_piece] == mWHITE && badvant > 0 && PIECES_RANKS[m_blowesttouch] < PIECES_RANKS[m_piece]){
            return wadvant - badvant - (PIECES_RANKS[m_piece]- PIECES_RANKS[m_blowesttouch]);
        }
        else if(PIECES_COLORS[m_piece] == mBLACK && wadvant > 0 && PIECES_RANKS[m_wlowesttouch] < PIECES_RANKS[m_piece]){
            return wadvant - badvant + (PIECES_RANKS[m_piece]- PIECES_RANKS[m_wlowesttouch]);
        }
        else{
            return wadvant - badvant;
        }

    }
