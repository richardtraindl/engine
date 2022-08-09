

    #include "./board.hpp"
    #include <unistd.h>



    //*****************************************
    cBoard::cBoard(){ 
    }
    //*****************************************



    //*****************************************
    // copy constructor
    cBoard::cBoard(const cBoard &board){

        for(uint8_t y = 0; y < 8; ++y){

            for(uint8_t x = 0; x < 8; ++x){

                m_fields[y][x] = board.m_fields[y][x];

            }

        }

        m_wKg_x = board.m_wKg_x;

        m_wKg_y = board.m_wKg_y;

        m_bKg_x = board.m_bKg_x;

        m_bKg_y = board.m_bKg_y;

        m_wKg_has_moved_at = board.m_wKg_has_moved_at;

        m_bKg_has_moved_at = board.m_bKg_has_moved_at;

        m_wRkA_has_moved_at = board.m_wRkA_has_moved_at;

        m_wRkH_has_moved_at = board.m_wRkH_has_moved_at;

        m_bRkA_has_moved_at = board.m_bRkA_has_moved_at;

        m_bRkH_has_moved_at = board.m_bRkH_has_moved_at;

    }
    //*****************************************



    //*****************************************
    uint8_t cBoard::getfield(uint8_t x, uint8_t y) const{

        return m_fields[y][x];

    }
    //*****************************************



    //*****************************************
    void cBoard::setfield(uint8_t x, uint8_t y, uint8_t value){

        m_fields[y][x] = (uint8_t)value;

    }
    //*****************************************



    //*****************************************
    void cBoard::copy_fields(uint8_t fields[8][8]) const{

        for(uint8_t y = 0; y < 8; ++y){

            for(uint8_t x = 0; x < 8; ++x){

                fields[y][x] = m_fields[y][x];

            }

        }

    }
    //*****************************************



    //*****************************************
    bool cBoard::compare_fields(uint8_t fields[8][8]) const{

        for(uint8_t y = 0; y < 8; ++y){

            for(uint8_t x = 0; x < 8; ++x){

                if(m_fields[y][x] != fields[y][x]){
                    return false;
                }
            }

        }

        return true;

    }
    //*****************************************



    //*****************************************
    bool cBoard::is_inbounds(const uint8_t x, const uint8_t y){

        return (x >= 0 && x <= 7 && y >= 0 && y <= 7);

    }
    //*****************************************



    //*****************************************
    uint16_t cBoard::eval_dir(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2){

        if(y1 == y2){ 
            if(x1 < x2){
                return mEST_WST; 
            }
            else{
                return mWST_EST;
            }
        }

        if(x1 == x2){ 
            if(y1 < y2){
                return mSTH_NTH; 
            }
            else{
                return mNTH_STH;
            }
        }

        if(x1 - y1 == x2 - y2){ 
            if(y1 < y2){
                return mSTHWST_NTHEST; 
            }
            else{
                return mNTHEST_STHWST;
            }
        }

        if(x1 + y1 == x2 + y2){
            if(y1 < y2){
                return mSTHEST_NTHWST; 
            }
            else{
                return mNTHWST_STHEST;
            }
        }

        if( (abs(x1 + y1) == 2) && (abs(x2 + y2) == 1) ){ 
            return mKNIGHT_DIR; 
        }

        if( (abs(x1 + y1) == 1) && (abs(x2 + y2) == 2) ){ 
            return mKNIGHT_DIR; 
        }

        return mNO_DIR;

    }
    //*****************************************



    //*****************************************
    uint16_t cBoard::eval_cardinale(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2){

        if(y1 == y2){ 
            return mHORIZONTAL;
        }

        if(x1 == x2){ 
            return mVERTICAL;
        }

        if(x1 - y1 == x2 - y2){ 
            return mDIA_LEFT_BOTTOM;
        }

        if(x1 + y1 == x2 + y2){
            return mDIA_RIGHT_BOTTOM;
        }

        if( (abs(x1 + y1) == 2) && (abs(x2 + y2) == 1) ){ 
            return mKNIGHT_DIR; 
        }

        if( (abs(x1 + y1) == 1) && (abs(x2 + y2) == 2) ){ 
            return mKNIGHT_DIR; 
        }

        return mNO_DIR;

    }
    //*****************************************



    //*****************************************
    bool cBoard::eval_steps(int8_t &step_x, int8_t &step_y, const uint16_t dir){

        if(dir == mEST_WST){
            step_x = 1;
            step_y = 0;
            return true;
        }
        if(dir == mWST_EST){
            step_x = -1;
            step_y = 0;
            return true;
        }
        else if(dir == mSTH_NTH){
            step_x = 0;
            step_y = 1;
            return true;
        }
        else if(dir == mNTH_STH){
            step_x = 0;
            step_y = -1;
            return true;
        }
        else if(dir ==  mSTHWST_NTHEST){
            step_x = 1;
            step_y = 1;
            return true;
        }
        else if(dir ==  mNTHEST_STHWST){
            step_x = -1;
            step_y = -1;
            return true;
        }
        else if(dir == mSTHEST_NTHWST){
            step_x = -1;
            step_y = 1;
            return true;
        }
        else if(dir == mNTHWST_STHEST){
            step_x = 1;
            step_y = -1;
            return true;
        }
        else{
            return false;
        }

    }
    //*****************************************



   //*****************************************
    uint16_t cBoard::eval_pindir(const uint8_t src_x, const uint8_t src_y) const{

        uint8_t piece = getfield(src_x, src_y);

        if(piece == mBLK){
            return mNO_DIR;
        }

        uint16_t cardinale;

        if(PIECES_COLORS[piece] == mWHITE){
            cardinale = eval_cardinale(src_x, src_y, m_wKg_x, m_wKg_y);
        }
        else{
            cardinale = eval_cardinale(src_x, src_y, m_bKg_x, m_bKg_y);
        }

        if(cardinale == mNO_DIR || cardinale == mKNIGHT_DIR){ 
            return mNO_DIR;
        }

        vector <cPiece> targets;

        search_cardinale(targets, src_x, src_y, cardinale);

        if(targets.size() != 2){
            return mNO_DIR;
        }

        // create dummy pieces - is a hack but what else? pointers???
        cPiece king(mBLK, 0, 0);
        cPiece enemy(mBLK, 0, 0);

        if(PIECES_COLORS[targets.at(0).m_piece] == PIECES_COLORS[piece]){
            king = targets.at(0);
            enemy = targets.at(1);
        }
        else{
            king = targets.at(1);
            enemy = targets.at(0);
        }

        if(PIECES_COLORS[king.m_piece] == PIECES_COLORS[enemy.m_piece]){
            return mNO_DIR;
        }

        if(PIECES_COLORS[piece] == mWHITE){
            if(! (king.m_xpos == m_wKg_x && king.m_ypos == m_wKg_y)){
                return mNO_DIR;
            }
        }
        else{
            if(! (king.m_xpos == m_bKg_x && king.m_ypos == m_bKg_y)){
                return mNO_DIR;
            }
        }

        if(enemy.m_piece == mWQU || enemy.m_piece == mBQU){
            return cardinale;
        }

        if((cardinale == mHORIZONTAL || cardinale == mVERTICAL) && 
           (enemy.m_piece == mWRK || enemy.m_piece == mBRK)){
            return cardinale;
        }

        if((cardinale == mDIA_LEFT_BOTTOM || cardinale == mDIA_RIGHT_BOTTOM) && (enemy.m_piece == mWBP || enemy.m_piece == mBBP)){
            return cardinale;
        }

        return mNO_DIR;

    }
    //*****************************************



    //*****************************************
    bool cBoard::is_soft_pinned(const uint8_t src_x, const uint8_t src_y) const{

        uint8_t piece = getfield(src_x, src_y);

        if(piece == mBLK){
            return false;
        }
        
        for(uint8_t i = 0; i < size(cPiece::qu_steps); i += 2){
          
            vector <cPiece> targets;

            search_cardinale(targets, src_x, src_y, cPiece::qu_steps[i].m_cardinale);

            if(targets.size() != 2){
                continue;
            }

            cPiece *friendptr, *enemyptr;

            if(PIECES_COLORS[targets.front().m_piece] == PIECES_COLORS[piece]){
                friendptr = &(targets.front());
                enemyptr = &(targets.back());
            }
            else{
                friendptr = &(targets.back());
                enemyptr = &(targets.front());
            }

            if(PIECES_COLORS[friendptr->m_piece] == PIECES_COLORS[enemyptr->m_piece]){
                continue;
            }

            if(PIECES_RANKS[friendptr->m_piece] > PIECES_RANKS[piece]){
                if(enemyptr->m_piece == mWQU || enemyptr->m_piece == mBQU){
                    return true;
                }
                else if(cPiece::qu_steps[i].m_cardinale == mVERTICAL || cPiece::qu_steps[i].m_cardinale == mHORIZONTAL){
                    if(enemyptr->m_piece == mWRK || enemyptr->m_piece == mBRK){
                        return true;
                    }
                }
                else if(cPiece::qu_steps[i].m_cardinale == mDIA_LEFT_BOTTOM || cPiece::qu_steps[i].m_cardinale == mDIA_RIGHT_BOTTOM){
                    if(enemyptr->m_piece == mWBP || enemyptr->m_piece == mBBP){
                        return true;
                    }
                }
            }

        }

        return false;

    }
  //*****************************************



   //*****************************************
    bool cBoard::is_piece_behind_soft_pinned(const uint8_t src_x, const uint8_t src_y) const{

        uint8_t behind_piece = getfield(src_x, src_y);

        if(behind_piece == mBLK){
            return false;
        }

        for(uint8_t i = 0; i < size(cPiece::qu_steps); ++i){
          
            uint8_t dst_x, dst_y;

            uint8_t piece = search_dir(dst_x, dst_y, src_x, src_y, cPiece::qu_steps[i].m_xstep, cPiece::qu_steps[i].m_ystep, 6);

            if(piece == mBLK || PIECES_COLORS[piece] != PIECES_COLORS[behind_piece] || PIECES_RANKS[piece] > PIECES_RANKS[behind_piece]){
                continue;
            }

            uint8_t enemy = search_dir(dst_x, dst_y, dst_x, dst_y, cPiece::qu_steps[i].m_xstep, cPiece::qu_steps[i].m_ystep, 6);

            if(enemy == mBLK || PIECES_COLORS[enemy] == PIECES_COLORS[behind_piece]){
                continue;
            }

            if(enemy == mWQU || enemy == mBQU){
                return true;
            }
            else if((enemy == mWRK || enemy == mBRK) && (cPiece::qu_steps[i].m_cardinale == mHORIZONTAL || cPiece::qu_steps[i].m_cardinale == mVERTICAL)){
                return true;
            }
            else if((enemy == mWBP || enemy == mBBP) && (cPiece::qu_steps[i].m_cardinale == mDIA_LEFT_BOTTOM || cPiece::qu_steps[i].m_cardinale == mDIA_RIGHT_BOTTOM)){
                return true;
            }

        }

        return false;

    }
  //*****************************************



  //*****************************************
  bool cBoard::is_field_touched(const uint8_t src_x, const uint8_t src_y, const uint8_t color) const{

      vector<cPiece> wpieces, bpieces;

      search_all_dirs_for_touching_pieces(wpieces, bpieces, src_x, src_y, NULL, false);

      if(color == mWHITE){
          return wpieces.size() > 0;
      }
      else{
          return bpieces.size() > 0;
      }

  }
  //*****************************************



  //*****************************************
  // search from FIELD in steps to find a piece (white or black)
  uint8_t cBoard::search_dir(uint8_t &dst_x, uint8_t &dst_y, const uint8_t src_x, const uint8_t src_y, const int8_t step_x, const int8_t step_y, const uint8_t maxcnt) const{

      dst_x = src_x;
      dst_y = src_y;

      for(uint8_t cnt = 0; cnt < maxcnt; ++cnt){

          if(is_inbounds((dst_x + step_x), (dst_y + step_y)) == false){
              return mBLK;
          }

          dst_x += step_x;
          dst_y += step_y;

          uint8_t piece = getfield(dst_x, dst_y);

          if(piece != mBLK){ 
              return piece; 
          }

      }

    return mBLK;

  }
  //*****************************************



  //*****************************************
  void cBoard::search_cardinale(vector <cPiece> &targets, const uint8_t src_x, const uint8_t src_y, const uint16_t cardinale) const{

      for(uint8_t i = 0; i < size(cPiece::qu_steps); ++i){
  
          if(cardinale == cPiece::qu_steps[i].m_cardinale){
              uint8_t dst_x, dst_y;

              uint8_t piece = search_dir(dst_x, dst_y, src_x, src_y, cPiece::qu_steps[i].m_xstep, cPiece::qu_steps[i].m_ystep, 7);
  
              if(piece != mBLK){ 
                  targets.push_back(cPiece(piece, dst_x, dst_y));

                  if(targets.size() == 2){
                      return;
                  }
              }
          }

      }

  }
  //*****************************************



  //*****************************************
  void cBoard::search_vertical_for_straight_pawns(vector<cPiece> &wpieces, vector<cPiece> &bpieces, const uint8_t src_x, const uint8_t src_y) const{

      int8_t xstep = 0;

      // search reversed vertical search
      int8_t ysteps[2] = { -1, 1 };

      uint8_t pawns[2] = { mWPW, mBPW };

      for(uint8_t i = 0; i < 2; ++i){

          uint8_t piece, dst_x, dst_y;

          piece = search_dir(dst_x, dst_y, src_x, src_y, xstep, ysteps[i], 1);

          if(piece == mBLK){
              if(pawns[i] == mWPW && src_y == 3){
                  dst_x = src_x;
                  dst_y = (src_y - 2);
                  piece = getfield(dst_x, dst_y);
              }
              else if(pawns[i] == mBPW && src_y == 4){
                  dst_x = src_x;
                  dst_y = (src_y + 2);
                  piece = getfield(dst_x, dst_y);
              }
              else{
                  continue;
              }
          }

          if(piece == pawns[i]){
              uint16_t pindir = eval_pindir(dst_x, dst_y);

              if(pindir == mNO_DIR || pindir == mVERTICAL){
                  if(pawns[i] == mWPW){
                      wpieces.push_back(cPiece(piece, dst_x, dst_y));
                  }
                  else{
                      bpieces.push_back(cPiece(piece, dst_x, dst_y));
                  }
              }
          }

      }

  }
  //*****************************************



  //*****************************************
  void cBoard::search_all_dirs_for_touching_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, const uint8_t src_x, const uint8_t src_y, const cMove *last_move, const bool observe_pins) const{

      // search for qu, rk, bp, kg and pw
      for(uint8_t i = 0; i < size(cPiece::qu_steps); ++i){

          uint8_t piece, dst_x, dst_y;

          piece = search_dir(dst_x, dst_y, src_x, src_y, cPiece::qu_steps[i].m_xstep, cPiece::qu_steps[i].m_ystep, 7);

          if(piece == mBLK || piece == mWKN || piece == mBKN){
              continue;
          }

          if(observe_pins){
          // skip if piece is pinned
              uint16_t pindir = eval_pindir(dst_x, dst_y);

              if(pindir != mNO_DIR && pindir != cPiece::qu_steps[i].m_cardinale){
                  continue;
              }
          }

          uint16_t dir = eval_dir(dst_x, dst_y, src_x, src_y);
          if(cPiece::is_touching_dir(piece, dir, dst_x, dst_y, src_x, src_y)){
              if(PIECES_COLORS[piece] == mWHITE){
                  wpieces.push_back(cPiece(piece, dst_x, dst_y));
                  continue;
              }
              else{
                  bpieces.push_back(cPiece(piece, dst_x, dst_y));
                  continue;
              }
          }

      }

      // search for kn
      for(uint8_t i = 0; i < size(cPiece::kn_steps); ++i){

          uint8_t piece, dst_x, dst_y;

          piece = search_dir(dst_x, dst_y, src_x, src_y, cPiece::kn_steps[i].m_xstep, cPiece::kn_steps[i].m_ystep, 1);
          
          if(piece != mWKN && piece != mBKN){
              continue;
          }

          // skip if piece is pinned
          if(observe_pins){
              uint16_t pindir = eval_pindir(dst_x, dst_y);

              if(pindir != mNO_DIR){
                  continue;
              }
          }

          if(PIECES_COLORS[piece] == mWHITE){
              wpieces.push_back(cPiece(piece, dst_x, dst_y));
              continue;
          }
          else{
              bpieces.push_back(cPiece(piece, dst_x, dst_y));
              continue;
          }

      }

      // search for en passant
      if(last_move != nullptr){

        int8_t xsteps[] = { 1, -1 -1, 1 };

        uint16_t dirs[] = { mDIA_LEFT_BOTTOM, mDIA_RIGHT_BOTTOM, mDIA_LEFT_BOTTOM, mDIA_RIGHT_BOTTOM };

        uint8_t attackers[] = { mBPW, mBPW, mWPW, mWPW };

        uint8_t prev_src_y[] = { 6, 6, 1, 1 };
        uint8_t prev_dst_y[] = { 4, 4, 3, 3 };

        for(uint8_t i = 0; i < 4; ++i){

            uint8_t attacker = getfield(src_x, src_y);

            if(attacker == attackers[i] && last_move->m_srcpiece == attackers[i] && src_x == last_move->m_dst_x && src_y == last_move->m_dst_y){
                uint8_t pwn_x, pwn_y;

                if(last_move->m_src_y == prev_src_y[i] && last_move->m_dst_y == prev_dst_y[i]){
                    if(is_inbounds((src_x + xsteps[i]), src_y)){
                        pwn_x = (src_x + xsteps[i]);
                        pwn_y = src_y;

                        uint16_t pindir = eval_pindir(pwn_x, pwn_y);
                        if(pindir == mNO_DIR || pindir == dirs[i]){
                            if(getfield(pwn_x, pwn_y) == mWPW){
                                wpieces.push_back(cPiece(mWPW, pwn_x, pwn_y));
                            }
                            else if(getfield(pwn_x, pwn_y) == mBPW){
                                bpieces.push_back(cPiece(mBPW, pwn_x, pwn_y));
                            }
                        }
                    }
                }
            }

        }

      }

  }
  //*****************************************



  //*****************************************
  void cBoard::search_from_piece(vector<cPiece> &wpieces, vector<cPiece> &bpieces, const uint8_t piece, const uint8_t src_x, const uint8_t src_y, const bool observe_pins) const{
    
      if(piece == mBLK){
          return;
      }

      uint8_t range[3];
      cPiece::get_range(range, piece);

      uint16_t pindir = mNO_DIR;
      if(observe_pins){
          pindir = eval_pindir(src_x, src_y);
      }

      // search
      for(uint8_t i = range[0]; i <= range[1]; ++i){ 

          if(pindir != mNO_DIR && pindir != cPiece::all_steps[i].m_cardinale){
              continue;
          }

          uint8_t dst_x, dst_y;

          uint8_t newpiece = search_dir(dst_x, dst_y, src_x, src_y, cPiece::all_steps[i].m_xstep, cPiece::all_steps[i].m_ystep, range[2]);

          if(newpiece == mBLK){
              continue;
          }
          else if(PIECES_COLORS[newpiece] == mWHITE){
              wpieces.push_back(cPiece(newpiece, dst_x, dst_y));
              continue;
          }
          else{
              bpieces.push_back(cPiece(newpiece, dst_x, dst_y));
              continue;
          }

      }

  }
  //*****************************************



  //*****************************************
  bool cBoard::is_passed_pawn(const uint8_t piece, const uint8_t src_x, const uint8_t src_y) const{

      if(piece == mWPW){
          for(uint8_t y = (src_y + 1); y <= 7; ++y){

              for(int8_t i = -1; i <= 1; ++i){

                    if(is_inbounds((src_x + i), y)){
                        if(getfield((src_x + i), y) == mBPW){
                            return false;
                        }
                    }

                }

            }

            return true;
        }
        else if(piece == mBPW){
            for(uint8_t y = (src_y - 1); y >= 0; --y){

                for(int8_t i = -1; i <= 1; ++i){

                    if(is_inbounds((src_x + i), y)){
                        if(getfield((src_x + i), y) == mWPW){
                            return false;
                        }
                    }

                }

            }

            return true;
        }

        return false;

    }
  //*****************************************



  //*****************************************
  void cBoard::prnt() const{

      string textcolor, backcolor, strpiece;

      for(int8_t y = 7; y >= 0; --y){

          for(uint8_t x = 0; x <= 7; ++x){

              uint8_t piece = getfield(x, (uint8_t)y);

              if(piece == mBLK){
                  strpiece = "    ";
              }
              else{
                  strpiece = " " + reverse_lookup8(PIECES, piece).substr (1,2) + " ";
              }

              if(PIECES_COLORS[piece] == mWHITE){
                  textcolor = (const string)"\033[97m" + (const string)"\033[1m";
              }
              else{
                  textcolor = (const string)"\033[30m" + (const string)"\033[1m";
              }

              if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
                  backcolor = (const string)"\033[104m";
              }
              else{
                  backcolor = (const string)"\033[42m";
              }

              cout << backcolor + textcolor + strpiece + (const string)"\033[0m"; 
          }
          cout << endl;
          
          for(uint8_t x = 0; x < 8; ++x){

              if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
                  cout << "\033[104m" + textcolor + "    " + (const string)"\033[0m"; 
              }
              else{
                  cout << "\033[42m" + textcolor + "    " + (const string)"\033[0m"; 
              }

          }

          cout << endl;

      }

  }
  //*****************************************



  //*****************************************
  void cBoard::do_move_on_board_only(const cMove &move){

      if(move.is_promotion()){
          setfield(move.m_src_x, move.m_src_y, mBLK);
          setfield(move.m_dst_x, move.m_dst_y, move.m_prompiece);
      }
      else{
          setfield(move.m_src_x, move.m_src_y, mBLK);
          setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);

          if(move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){
              if(move.m_srcpiece == mWKG){
                  m_wKg_x = move.m_dst_x;
                  m_wKg_y = move.m_dst_y;

                  if(m_wKg_has_moved_at == 0){
                      m_wKg_has_moved_at = 1; // Fake number due...
                  }
              }
              else{
                  m_bKg_x = move.m_dst_x;
                  m_bKg_y = move.m_dst_y;
                  
                  if(m_bKg_has_moved_at == 0){
                      m_bKg_has_moved_at = 1; // Fake number due...
                  }
              }

              if(move.is_short_castling()){
                  uint8_t rook = getfield(7, move.m_dst_y); // h == 7
                  setfield(7, move.m_dst_y, mBLK);
                  setfield(5, move.m_dst_y, rook); // f == 5
              }
              else if(move.is_long_castling()){
                  uint8_t rook = getfield(0, move.m_dst_y); // a == 0
                  setfield(0, move.m_dst_y, mBLK);
                  setfield(3, move.m_dst_y, rook); // c == 3
              }
          }
          else if(move.is_en_passant()){
              setfield(move.m_dst_x, move.m_src_y, mBLK);
          }
      }

  }
  //*****************************************
