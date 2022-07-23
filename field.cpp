

    #include "./field.hpp"


    cField::cField(const cBoard *board, uint8_t xpos, uint8_t ypos){ //  : m_board(board)
        m_board = board;
        m_xpos = xpos;
        m_ypos = ypos;

        fill();

    }


    void cField::fill(){
        m_piece = m_board->getfield(m_xpos, m_ypos);
        m_wtouchcnt = 0;
        m_btouchcnt = 0;
        m_wsoftpincnt = 0;
        m_bsoftpincnt = 0;
        m_wlowesttouch = mWKG;
        m_blowesttouch = mBKG;

        vector<cPiece> wpieces, bpieces;
        m_board->search_for_all_touching_pieces(wpieces, bpieces, m_xpos, m_ypos);

        for(const cPiece &piece : wpieces){
            uint8_t state = m_board->eval_pin_state(piece.m_piece, piece.m_xpos, piece.m_ypos);

            if(state == cBoard::PINNED_NO){
                m_wtouchcnt++;
            }
            else if(state == cBoard::PINNED_SOFT){
                m_wsoftpincnt++;
            }
            else{
                continue;
            }

            if(PIECES_RANKS[piece.m_piece] < PIECES_RANKS[m_wlowesttouch]){
                m_wlowesttouch = piece.m_piece;
            }
        }

        for(const cPiece &piece : bpieces){
            uint8_t state = m_board->eval_pin_state(piece.m_piece, piece.m_xpos, piece.m_ypos);

            if(state == cBoard::PINNED_NO){
                m_btouchcnt++;
            }
            else if(state == cBoard::PINNED_SOFT){
                m_bsoftpincnt++;
            }
            else{
                continue;
            }

            if(PIECES_RANKS[piece.m_piece] < PIECES_RANKS[m_blowesttouch]){
                m_blowesttouch = piece.m_piece;
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
        return (m_btouchcnt + m_bsoftpincnt > 0) && (m_wtouchcnt + m_wsoftpincnt == 0);

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


    int8_t cField::eval_score(){
        uint8_t wscore = (m_wtouchcnt * 2) + m_wsoftpincnt;
        uint8_t bscore = (m_btouchcnt * 2) + m_bsoftpincnt;

        if(PIECES_COLORS[m_piece] == mWHITE && bscore > 0 && PIECES_RANKS[m_blowesttouch] < PIECES_RANKS[m_piece]){
            return (wscore - bscore) - (PIECES_RANKS[m_piece]- PIECES_RANKS[m_blowesttouch]);
        }
        else if(PIECES_COLORS[m_piece] == mBLACK && wscore > 0 && PIECES_RANKS[m_wlowesttouch] < PIECES_RANKS[m_piece]){
            return (wscore - bscore) + (PIECES_RANKS[m_piece]- PIECES_RANKS[m_wlowesttouch]);
        }
        else{
            return wscore - bscore;
        }

    }
