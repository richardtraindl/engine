

    #include "./piece.hpp"
    #include "./board.hpp"



    cDir::cDir(uint16_t dir, uint16_t cardinale, int8_t xstep, int8_t ystep){

        m_dir = dir;

        m_cardinale = cardinale;

        m_xstep = xstep;

        m_ystep = ystep;

    }



    cPiece::cPiece(uint8_t piece, uint8_t xpos, uint8_t ypos){

        m_piece = piece;

        m_xpos = xpos;

        m_ypos = ypos;

    }


    cPiece::cPiece(const cPiece &piece){

        m_piece = piece.m_piece;

        m_xpos = piece.m_xpos;

        m_ypos = piece.m_ypos;

    }


    cDir cPiece::rk_steps[4] = { 
      cDir(mEST_WST, mHORIZONTAL, 1, 0), cDir(mWST_EST, mHORIZONTAL, -1, 0), 
      cDir(mSTH_NTH, mVERTICAL, 0, 1), cDir(mNTH_STH, mVERTICAL, 0, -1) 
    };


    cDir cPiece::bp_steps[4] = { 
      cDir(mSTHWST_NTHEST, mDIA_LEFT_BOTTOM, 1, 1), cDir(mNTHEST_STHWST, mDIA_LEFT_BOTTOM, -1, -1), 
      cDir(mSTHEST_NTHWST, mDIA_RIGHT_BOTTOM, -1, 1), cDir(mNTHWST_STHEST, mDIA_RIGHT_BOTTOM, 1, -1) 
    };


    cDir cPiece::qu_steps[8] = { 
      cDir(mEST_WST, mHORIZONTAL, 1, 0), cDir(mWST_EST, mHORIZONTAL, -1, 0), 
      cDir(mSTH_NTH, mVERTICAL, 0, 1), cDir(mNTH_STH, mVERTICAL, 0, -1),
      cDir(mSTHWST_NTHEST, mDIA_LEFT_BOTTOM, 1, 1), cDir(mNTHEST_STHWST, mDIA_LEFT_BOTTOM, -1, -1), 
      cDir(mSTHEST_NTHWST, mDIA_RIGHT_BOTTOM, -1, 1), cDir(mNTHWST_STHEST, mDIA_RIGHT_BOTTOM, 1, -1) 
    };


    cDir cPiece::kn_steps[8] = { 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, 1, 2), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, 2, 1), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, 2, -1), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, 1, -2), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, -1, -2), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, -2, -1), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, -2, 1), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, -1, 2)
    };


    cDir cPiece::wpw_steps[2] = { 
      cDir(mSTH_NTH, mVERTICAL, 0, 1),
      cDir(mSTH_NTH, mVERTICAL, 0, 2) 
    };


    cDir cPiece::wpw_capture_steps[2] = { 
      cDir(mSTHWST_NTHEST, mDIA_LEFT_BOTTOM, 1, 1), 
      cDir(mSTHEST_NTHWST, mDIA_RIGHT_BOTTOM, -1, 1) 
    };


    cDir cPiece::bpw_steps[2] = { 
      cDir(mNTH_STH, mVERTICAL, 0, -1) ,
      cDir(mNTH_STH, mVERTICAL, 0, -2) 
    };


    cDir cPiece::bpw_capture_steps[2] = { 
      cDir(mNTHEST_STHWST, mDIA_LEFT_BOTTOM, -1, -1), cDir(mNTHWST_STHEST, mDIA_RIGHT_BOTTOM, 1, -1) 
    };


    cDir cPiece::all_steps[20] = { 
      cDir(mEST_WST, mHORIZONTAL, 1, 0), 
      cDir(mWST_EST, mHORIZONTAL, -1, 0), 
      cDir(mSTH_NTH, mVERTICAL, 0, 1), 
      cDir(mNTH_STH, mVERTICAL, -1, 0),
      cDir(mSTHWST_NTHEST, mDIA_LEFT_BOTTOM, 1, 1), 
      cDir(mNTHEST_STHWST, mDIA_LEFT_BOTTOM, -1, -1), 
      cDir(mSTHEST_NTHWST, mDIA_RIGHT_BOTTOM, -1, 1), 
      cDir(mNTHWST_STHEST, mDIA_RIGHT_BOTTOM, 1, -1),
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, 1, 2), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, 2, 1), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, 2, -1), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, 1, -2), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, -1, -2), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, -2, -1), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, -2, 1), 
      cDir(mKNIGHT_DIR, mKNIGHT_DIR, -1, 2),
      cDir(mSTHWST_NTHEST, mDIA_LEFT_BOTTOM, 1, 1), 
      cDir(mSTHEST_NTHWST, mDIA_RIGHT_BOTTOM, -1, 1),
      cDir(mNTHEST_STHWST, mDIA_LEFT_BOTTOM, -1, -1), 
      cDir(mNTHWST_STHEST, mDIA_RIGHT_BOTTOM, 1, -1) 
    };


    void cPiece::get_range(uint8_t range[3], uint8_t piece){

        if(piece == mWPW){
          range[0] = 16;
          range[1] = 17;
          range[2] = 1;
        }
        else if(piece == mBPW){
          range[0] = 18;
          range[1] = 19;
          range[2] = 1;
      }
      else if(piece == mWKG || piece == mBKG){
          range[0] = 0;
          range[1] = 7;
          range[2] = 1;
      }
      else if(piece == mWQU || piece == mBQU){
          range[0] = 0;
          range[1] = 7;
          range[2] = 7;
      }
      else if(piece == mWRK || piece == mBRK){
          range[0] = 0;
          range[1] = 3;
          range[2] = 7;
      }
      else if(piece == mWBP || piece == mBBP){
          range[0] = 4;
          range[1] = 7;
          range[2] = 7;
      }
      else if(piece == mWKN || piece == mBKN){
          range[0] = 8;
          range[1] = 15;
          range[2] = 1;
      }
      else{
          range[0] = 0;
          range[1] = 1;
          range[2] = 0;
      }

    }


    bool cPiece::is_touching_dir(uint8_t piece, uint16_t dir, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y){

        if(piece == mWQU || piece == mBQU){
            return (dir == mEST_WST || dir == mWST_EST || 
                    dir == mSTH_NTH || dir == mNTH_STH || 
                    dir == mSTHWST_NTHEST || dir == mNTHEST_STHWST ||
                    dir == mSTHEST_NTHWST || dir == mNTHWST_STHEST);
        }
        else if(piece == mWRK || piece == mBRK){
            return (dir == mEST_WST || dir == mWST_EST || 
                    dir == mSTH_NTH || dir == mNTH_STH);
        }
        else if(piece == mWBP || piece == mBBP){
            return (dir == mSTHWST_NTHEST || dir == mNTHEST_STHWST ||
                    dir == mSTHEST_NTHWST || dir == mNTHWST_STHEST);
        }
        else if(piece == mWKG || piece == mBKG){
            if(abs(src_x - dst_x) <= 1 && abs(src_y - dst_y) <= 1){
                return (dir == mEST_WST || dir == mWST_EST || 
                        dir == mSTH_NTH || dir == mNTH_STH || 
                        dir == mSTHWST_NTHEST || dir == mNTHEST_STHWST ||
                        dir == mSTHEST_NTHWST || dir == mNTHWST_STHEST);
            }
            else{
                return false;
            }
        }
        else if(piece == mWKN || piece == mBKN){
            return (dir == mKNIGHT_DIR);
        }
        else if(piece == mWPW){
            if((abs(src_x - dst_x) == 1) && (src_y - dst_y == -1)){
                return (dir == mSTHWST_NTHEST || dir == mSTHEST_NTHWST);
            }
            else{
                return false;
            }
        }
        else if(piece == mBPW){
            if((abs(src_x - dst_x) == 1) && (src_y - dst_y == 1)){
                return (dir == mNTHEST_STHWST || dir == mNTHWST_STHEST);
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }

    }
