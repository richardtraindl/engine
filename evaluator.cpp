


  #include "./evaluator.hpp"


  //*****************************************
  cEvaluator::cEvaluator(){
  }
  //*****************************************



  //*****************************************
  int32_t cEvaluator::score_move(cMatch &match, const cMove &move){

      int32_t score = 0;

      if(move.is_promotion()){
          score += SCORES[move.m_dstpiece];

          if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
              score += REVERSED_SCORES[move.m_prompiece] - REVERSED_SCORES[mWPW];
          }
          else{
              score += REVERSED_SCORES[move.m_prompiece] - REVERSED_SCORES[mBPW];
          }
      }
      else{
          // standard move
          score += SCORES[move.m_dstpiece];

          // add on for en passant
          if(move.is_en_passant()){
              uint8_t enpiece = match.m_board.getfield(move.m_dst_x, move.m_src_y);
              score += SCORES[enpiece];
          }
      }

      return score;

  }
  //*****************************************



 //*****************************************
  int32_t cEvaluator::score_board(cMatch &match, const uint8_t depth, const uint8_t stage){

      int32_t score = 0;

      /*
      // opening
      if(stage == STAGE_OPENING){

          // penalty for inactive white pieces
          cPiece white_pieces[4] = { 
              cPiece(mWKN, 1, 0), cPiece(mWKN, 6, 0), cPiece(mWBP, 2, 0), cPiece(mWBP, 5, 0) 
          };

          uint8_t wcnt = 0;

          for(const cPiece &wpiece : white_pieces){

              if(m_board.getfield(wpiece.m_xpos, wpiece.m_ypos) == wpiece.m_piece){
                  wcnt++;
              }

          }

          if(wcnt > 1){
              score += SCORES[mBPLUS1] * wcnt; // penalty
          }


          // penalty for inactive black pieces
          cPiece black_pieces[4] = { 
              cPiece(mBKN, 1, 7), cPiece(mBKN, 6, 7), cPiece(mBBP, 2, 7), cPiece(mBBP, 5, 7) 
          };

          uint8_t bcnt = 0;

          for(const cPiece &bpiece : black_pieces){

              if(m_board.getfield(bpiece.m_xpos, bpiece.m_ypos) == bpiece.m_piece){
                  bcnt++;
              }

          }

          if(bcnt > 1){
              score += SCORES[mWPLUS1] * bcnt; // penalty
          }

          // penalty for inactive white pawns
          wcnt = 0;

          for(uint8_t x = 0; x < 8; ++x){

              if(m_board.getfield(x, 1) == mWPW){
                  wcnt++;
              }

          }

          if(wcnt >= 6){
              score += SCORES[mBPLUS1]; // penalty
          }

          // penalty for inactive black pawns
          bcnt = 0;

          for(uint8_t x = 0; x < 8; ++x){

              if(m_board.getfield(x, 6) == mBPW){
                  bcnt++;
              }

          }

          if(bcnt >= 6){
              score += SCORES[mWPLUS1]; // penalty
          }


          // pawns on central fields
          vector<cPiece> wpieces, bpieces;

          for(uint8_t y = 3; y <= 4; ++y){

              for(uint8_t x = 3; x <= 4; ++x){

                  m_board.search_all_dirs_for_touching_pieces(wpieces, bpieces, x, y, false);

              }
          }

          for(const cPiece &wpiece : wpieces){

              if(wpiece.m_piece == mWPW){
                  score += SCORES[mWPLUS1];

                  if(wpiece.m_ypos == 3){
                      score += SCORES[mWPLUS1]; // addition plus for 4th rank
                  }
              }

          }

          for(const cPiece &bpiece : bpieces){

              if(bpiece.m_piece == mBPW){
                  score += SCORES[mBPLUS1];

                  if(bpiece.m_ypos == 4){
                      score += SCORES[mBPLUS1]; // addition plus for 5th rank
                  }
              }

          }

          // penalty for f2 / f7 moved pawns
          if(m_board.getfield(5, 1) != mWPW){
              score += SCORES[mBPLUS3]; // penalty
          }

          if(m_board.getfield(5, 6) != mBPW){
              score += SCORES[mWPLUS3]; // penalty
          }
      }*/

      // opening and middlegame

      // passed pawns
      if(has_passed_pawn(match.m_board, mWHITE)){
          score += SCORES[mWPLUS4];
      }

      if(has_passed_pawn(match.m_board, mBLACK)){
          score += SCORES[mBPLUS4];
      }

      // penalty for weak kings
      if(is_wking_weak(match)){
          score += SCORES[mBPLUS4]; // penalty
      }

      if(is_bking_weak(match)){
          score += SCORES[mWPLUS4]; // penalty
      }

      // wrook or wqueen on 7th rank
      uint8_t wqu_x, wqu_y;
      if(is_wqu_wrk_on_7th_rank(wqu_x, wqu_y, match.m_board)){
          vector<cPiece> wpieces, bpieces; 

          match.m_board.search_all_dirs_for_touching_pieces(wpieces, bpieces, wqu_x, wqu_y, match.get_last_move(), true);

          if(wpieces.size() >= bpieces.size()){
              score += SCORES[mWPLUS3];
          }
      }

      // brook or bquen on 2nd rank
      uint8_t bqu_x, bqu_y;
      if(is_bqu_brk_on_2th_rank(bqu_x, bqu_y, match.m_board)){
          vector<cPiece> wpieces, bpieces; 

          match.m_board.search_all_dirs_for_touching_pieces(wpieces, bpieces, bqu_x, bqu_y, match.get_last_move(), true);

          if(bpieces.size() >= wpieces.size()){
              score += SCORES[mBPLUS3];
          }
      }

      score += score_touches_on_all_pieces(match);

      return score;

  }
  //*****************************************



  //*****************************************
  int32_t cEvaluator::score_touches_on_all_pieces(cMatch &match){

      int32_t score = 0;

      for(uint8_t y = 0; y < 8; ++y){

          for(uint8_t x = 0; x < 8; ++x){

              uint8_t piece = match.m_board.getfield(x, y);

              if(piece == mBLK){
                  continue;
              }

              int8_t state = eval_piece_state(match, piece, x, y);
              score += score_piece_state(match, piece, x, y, state);

          }
      }

    return score;

  }
  //*****************************************



  //*****************************************
  int8_t cEvaluator::eval_piece_state(cMatch &match, const uint8_t piece, const uint8_t src_x, const uint8_t src_y){

      int8_t state = 0;

      if(match.m_board.eval_pindir(src_x, src_y) != mNO_DIR){
          PIECES_COLORS[piece] == mWHITE ? state = -5 : state = 5;
      }
      else if(match.m_board.is_soft_pinned(src_x, src_y)){
          PIECES_COLORS[piece] == mWHITE ? state = -2 : state = 2;
      }

      vector <cPiece> wpieces, bpieces;
      match.m_board.search_all_dirs_for_touching_pieces(wpieces, bpieces, src_x, src_y, match.get_last_move(), true);

      for(const cPiece &piece : wpieces){

          int8_t adjust = 0;

          if(match.m_board.is_soft_pinned(piece.m_xpos, piece.m_ypos)){
              adjust += -2;
          }

          vector <cPiece> wp, bp;

          match.m_board.search_all_dirs_for_touching_pieces(wp, bp, piece.m_xpos, piece.m_ypos, match.get_last_move(), true);

          if(wp.size() > bp.size() || (wp.size() == 0 && bp.size() == 0)){
              state += 3 + adjust;
          }
          else if(wp.size() == bp.size()){
              state += 2 + adjust;
          }
          else{
              state += 1 + adjust;
          }

      }

      for(const cPiece &piece : bpieces){

          int8_t adjust = 0;

          if(match.m_board.is_soft_pinned(piece.m_xpos, piece.m_ypos)){
              adjust += 2;
          }

          vector <cPiece> wp, bp;

          match.m_board.search_all_dirs_for_touching_pieces(wp, bp, piece.m_xpos, piece.m_ypos, match.get_last_move(), true);

          if(bp.size() > wp.size()  || (wp.size() == 0 && bp.size() == 0)){
              state += -3 + adjust;
          }
          else if(bp.size() == wp.size()){
              state += -2 + adjust;
          }
          else{
              state += -1 + adjust;
          }

      }

      return state;

  }
  //*****************************************



  //*****************************************
  uint8_t cEvaluator::eval_field_state(cMatch &match, const uint8_t src_x, const uint8_t src_y){

      uint8_t wscore = 0;
      uint8_t bscore = 0;

      vector <cPiece> wpieces, bpieces;

      match.m_board.search_all_dirs_for_touching_pieces(wpieces, bpieces, src_x, src_y, match.get_last_move(), true);

      for(const cPiece &piece : wpieces){

          if(match.m_board.is_soft_pinned(piece.m_xpos, piece.m_ypos)){
              wscore += 1;
          }
          else{
              wscore += 2;
          }

      }

      for(const cPiece &piece : bpieces){

          if(match.m_board.is_soft_pinned(piece.m_xpos, piece.m_ypos)){
              bscore += 1;
          }
          else{
              bscore += 2;
          }
      }

      if(wscore == 0 && bscore == 0){
          return mF_CLEAR;
      }
      else if(wscore > 0 && bscore == 0){
          return mF_WDOM;
      }
      else if(wscore == 0 && bscore > 0){
          return mF_BDOM;
      }
      else if(wscore > bscore){
          const cPiece *wlowest = get_lowest(wpieces);
          const cPiece *blowest = get_lowest(bpieces);

          if(wlowest != nullptr && blowest != nullptr){
              if(PIECES_RANKS[wlowest->m_piece] <= PIECES_RANKS[blowest->m_piece]){
                  return mF_WGT;
              }
          }

          return mF_HAZY;
      }
      else if(wscore < bscore){
          const cPiece *wlowest = get_lowest(wpieces);
          const cPiece *blowest = get_lowest(bpieces);

          if(wlowest != nullptr && blowest != nullptr){
              if(PIECES_RANKS[blowest->m_piece] <= PIECES_RANKS[wlowest->m_piece]){
                  return mF_BGT;
              }
          }

          return mF_HAZY;
      }

      return mF_HAZY;

  }
  //*****************************************



  //*****************************************
  bool cEvaluator::is_field_save_for_color(const uint8_t field_state, const uint8_t color){

      if(color == mWHITE){
          return (field_state == mF_CLEAR || field_state == mF_WGT || field_state == mF_WDOM);
      }
      else{
          return (field_state == mF_CLEAR ||field_state == mF_BGT || field_state == mF_BDOM);
      }

  }
  //*****************************************
  


  //*****************************************
  bool cEvaluator::does_move_touch_soft_pinned(cMatch &match, const cMove &move){

      cBoard board(match.m_board);
      board.do_move_on_board_only(move);

      vector<cPiece> wpieces, bpieces;

      board.search_from_piece(wpieces, bpieces, move.m_srcpiece, move.m_dst_x, move.m_dst_y, false);

      uint16_t move_dir = cBoard::eval_cardinale(move.m_src_x, move.m_src_y, move.m_dst_x, move.m_dst_y);

      for(const cPiece &piece : wpieces){

          uint16_t touch_dir = cBoard::eval_cardinale(move.m_dst_x, move.m_dst_y, piece.m_xpos, piece.m_ypos);

          if(move_dir == touch_dir){
              continue;
          }

          if(match.m_board.is_soft_pinned(piece.m_xpos, piece.m_ypos)){
              return true;
          }

      }
            
      for(const cPiece &piece : bpieces){

          uint16_t touch_dir = cBoard::eval_cardinale(move.m_dst_x, move.m_dst_y, piece.m_xpos, piece.m_ypos);

          if(move_dir == touch_dir){
              continue;
          }

          if(match.m_board.is_soft_pinned(piece.m_xpos, piece.m_ypos)){
              return true;
          }

      }

      return false;

  }
  //*****************************************



  //*****************************************
  bool cEvaluator::does_move_escape_soft_pin(cMatch &match, const cMove &move){

      if(match.m_board.is_piece_behind_soft_pinned(move.m_src_x, move.m_src_y)){

          cBoard board(match.m_board);
          board.do_move_on_board_only(move);

          return (! board.is_piece_behind_soft_pinned(move.m_dst_x, move.m_dst_y));
      }

      return false;

  }
  //*****************************************

 
  
  
  //*****************************************
  bool cEvaluator::does_move_sac_for_supply(cMatch &match, cMove &move){

        if(PIECES_COLORS[move.m_dstpiece] == mBLK){
            return false;
        }

        if( ! (match.m_board.is_soft_pinned(move.m_dst_x, move.m_dst_y)) ){
            return false;
        }

        vector<cPiece> supplies, others;

        uint8_t queen;

        if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
            match.m_board.search_all_dirs_for_touching_pieces(supplies, others, move.m_src_x, move.m_src_y, match.get_last_move(), true);
            
            queen = mWQU;
        }
        else{
            match.m_board.search_all_dirs_for_touching_pieces(others, supplies, move.m_src_x, move.m_src_y, match.get_last_move(), true);

            queen = mBQU;
        }

        // remove short range friends and friends with not matching move steps
        vector<cPiece>::iterator it = supplies.begin();

        while(it != supplies.end()){

            if(it->m_piece == mWPW || it->m_piece == mBPW ||
               it->m_piece == mWKG || it->m_piece == mBKG ||
               it->m_piece == mWKN || it->m_piece == mBKN){

                it = supplies.erase(it);

            }
            else if(it->m_piece != move.m_srcpiece && it->m_piece != queen){
                it = supplies.erase(it);
            }
            else{
                ++it;
                return true;
            }

        }

        return false;
    }
  //*****************************************
  


  //*****************************************
  bool cEvaluator::does_move_attack_king(cMatch &match, const cMove &move){

    cMatch nmatch(match);
    nmatch.do_move(move);

    return nmatch.is_king_attacked();

  }
  //*****************************************



  //*****************************************
  int32_t cEvaluator::score_piece_state(cMatch &match, const uint8_t piece, const uint8_t src_x, const uint8_t src_y, const int8_t state){

      if(state > 0){
          return (SCORES[mWPLUS1] * state);
      }

      if(state < 0){
          return (SCORES[mBPLUS1] * state);
      }

      return 0;

  }
  //*****************************************



  //*****************************************
  bool cEvaluator::has_passed_pawn(const cBoard &board, const uint8_t color){

      if(color == mWHITE){
          for(uint8_t y = 4; y < 7; ++y){

              for(uint8_t x = 0; x < 8; ++x){

                  uint8_t pawn = board.getfield(x, y);
                  if(pawn == mWPW && board.is_passed_pawn(pawn, x, y)){
                      return true;
                  }
              }

          }
      }
      else{
          for(uint8_t y = 3; y > 0; --y){

              for(uint8_t x = 0; x < 8; ++x){

                  uint8_t pawn = board.getfield(x, y);
                  if(pawn == mBPW && board.is_passed_pawn(pawn, x, y)){
                      return true;
                  }

              }

          }

      }

      return false;

  }
  //*****************************************



  //*****************************************
  bool cEvaluator::is_wking_weak(cMatch &match){

      for(uint8_t i = 0; i < size(cPiece::qu_steps); ++i){

          vector <cPiece> wpieces, bpieces;

          if(cBoard::is_inbounds(match.m_board.m_wKg_x + cPiece::qu_steps[i].m_xstep, match.m_board.m_wKg_y + cPiece::qu_steps[i].m_ystep)){

              match.m_board.search_all_dirs_for_touching_pieces(wpieces, bpieces, match.m_board.m_wKg_x + cPiece::qu_steps[i].m_xstep, match.m_board.m_wKg_y + cPiece::qu_steps[i].m_ystep, match.get_last_move(), false);

              if((wpieces.size() + 1) < bpieces.size()){
                  return true;
              }
          }

      }

      return false;

  }
  //*****************************************



  //*****************************************
  bool cEvaluator::is_bking_weak(cMatch &match){

      for(uint8_t i = 0; i < size(cPiece::qu_steps); ++i){

          vector <cPiece> wpieces, bpieces;

          if(cBoard::is_inbounds(match.m_board.m_bKg_x + cPiece::qu_steps[i].m_xstep, match.m_board.m_bKg_y + cPiece::qu_steps[i].m_ystep)){

              match.m_board.search_all_dirs_for_touching_pieces(wpieces, bpieces, match.m_board.m_bKg_x + cPiece::qu_steps[i].m_xstep, match.m_board.m_bKg_y + cPiece::qu_steps[i].m_ystep, match.get_last_move(), false);

              if((bpieces.size() + 1) < wpieces.size()){
                  return true;
              }
          }

      }

      return false;

  }
  //*****************************************



  //*****************************************
  bool cEvaluator::is_wqu_wrk_on_7th_rank(uint8_t &x, uint8_t &y, const cBoard &board){

      y = 6;

      for(x = 0; x < 8; ++x){

          uint8_t piece = board.getfield(x, y);

          if(piece == mWRK || piece == mWQU){
              return true;
          }

      }

      return false;

  }
  //*****************************************



  //*****************************************
  bool cEvaluator::is_bqu_brk_on_2th_rank(uint8_t &x, uint8_t &y, const cBoard &board){

      y = 1;

      for(x = 0; x < 8; ++x){

          uint8_t piece = board.getfield(x, y);

          if(piece == mBRK || piece == mBQU){
              return true;
          }

      }

      return false;

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_move(cMatch &match, cMove &move){
  
      cMatch match_after_move(match);
      match_after_move.do_move(move);

      uint8_t dstfield_state = eval_field_state(match_after_move, move.m_dst_x, move.m_dst_y);

      priorize_passed_pawn_supporting_move(match_after_move, move, dstfield_state);

      priorize_capture_move(move, dstfield_state);

      if(move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){
          priorize_castling_move(move);
      }
      else{
          priorize_king_attacking_move(match_after_move, move, dstfield_state);

          priorize_escape_move(match, move, dstfield_state);

          priorize_touch_soft_pinned_move(match, move, dstfield_state);

          priorize_sac_for_supply_move(match, move);

          if(move.m_srcpiece == mWPW || move.m_srcpiece == mBPW){
              priorize_promotion_move(move, dstfield_state);

              priorize_passed_pawn_move(match_after_move, move, dstfield_state);
          }
      }

      if(is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece])){ 
          move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM + 3));
      }
      else{
          move.m_prio = min(move.m_prio, cMove::P_LOW);
      }

  }
  //*****************************************



  //*****************************************
  const cPiece *cEvaluator::get_lowest(vector <cPiece> &pieces){

      const cPiece *lowest = nullptr;

      if(pieces.size() == 0){
          return lowest;
      }

      lowest = &(pieces.front());

      for(const cPiece &piece : pieces){

          if(PIECES_RANKS[piece.m_piece] < PIECES_RANKS[lowest->m_piece]){
              lowest = &(piece);
          }

      }

      return lowest;

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_capture_move(cMove &move, const uint8_t dstfield_state){

      if(move.is_en_passant()){ 
          move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_HIGH_UP - 8));
      }
      else if(move.m_dstpiece != mBLK){
          if(is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece])){ 
              move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_HIGH_UP - 6));
          }
          else{
              if(PIECES_RANKS[move.m_srcpiece] < PIECES_RANKS[move.m_dstpiece]){
                  move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_HIGH_UP - 4));
              }
              else if(PIECES_RANKS[move.m_srcpiece] == PIECES_RANKS[move.m_dstpiece]){
                  move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_UP + 5));
              }
              else{
                  move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_DOWN + 5));
              }
          }
      }

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_promotion_move(cMove &move, const uint8_t dstfield_state){

      if(move.is_promotion()){
          if(is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece])){ 
              move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_HIGH_UP - 10));
          }
          else{
              move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_DOWN - 10));
          }
      }

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_castling_move(cMove &move){

      if(move.is_short_castling() || move.is_long_castling()){
          move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_UP + 6));
      }

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_passed_pawn_move(cMatch &match_after_move, cMove &move, const uint8_t dstfield_state){

      if(match_after_move.m_board.is_passed_pawn(move.m_srcpiece, move.m_dst_x, move.m_dst_y)){
          if(is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece])){ 
              if(match_after_move.eval_stage() == cMatch::STAGE_ENDGAME){ 
                  move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_UP - 3));
              }
              else{
                  move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM - 3));
              }
          }
          else{
              move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_LOW - 3));
          }
      }

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_passed_pawn_supporting_move(cMatch &match_after_move, cMove &move, const uint8_t dstfield_state){

      vector<cPiece> friends, enemies;

      if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
          match_after_move.m_board.search_from_piece(friends, enemies, move.m_srcpiece, move.m_dst_x, move.m_dst_y, false);
      }
      else{
          match_after_move.m_board.search_from_piece(enemies, friends, move.m_srcpiece, move.m_dst_x, move.m_dst_y, false);
      }

      uint16_t move_dir = cBoard::eval_cardinale(move.m_src_x, move.m_src_y, move.m_dst_x, move.m_dst_y);

      for(const cPiece &piece : friends){

          uint16_t touch_dir = cBoard::eval_cardinale(move.m_dst_x, move.m_dst_y, piece.m_xpos, piece.m_ypos);

          if(move_dir == touch_dir){
              continue;
          }

          if(match_after_move.m_board.is_passed_pawn(piece.m_piece, piece.m_xpos, piece.m_ypos)){

              if(is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece])){
                  if(match_after_move.eval_stage() == cMatch::STAGE_ENDGAME){  
                      move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_UP - 2));
                  }
                  else{
                      move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM - 2));
                  }
              }
              else{
                  move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_LOW - 2));
              }

              return;
          }

      }

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_king_attacking_move(cMatch &match_after_move, cMove &move, const uint8_t dstfield_state){

      if(match_after_move.is_king_attacked()){
          if(is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece])){ 
              move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_UP - 10));
          }
          else{
              move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_LOW - 10));
          }
      }

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_escape_move(cMatch &match_after_move, cMove &move, const uint8_t dstfield_state){

      if(does_move_escape_soft_pin(match_after_move, move)){
          if(is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece])){ 
              move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_UP + 2));
          }
          else{
              move.m_prio = min(move.m_prio, cMove::P_LOW);
          }
      }

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_touch_soft_pinned_move(cMatch &match_after_move, cMove &move, const uint8_t dstfield_state){

      if(cEvaluator::does_move_touch_soft_pinned(match_after_move, move)){
          if(is_field_save_for_color(dstfield_state, PIECES_COLORS[move.m_srcpiece])){ 
              move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_UP + 3));
          }
          else{
              move.m_prio = min(move.m_prio, cMove::P_LOW);
          }
      }

  }
  //*****************************************



  //*****************************************
  void cEvaluator::priorize_sac_for_supply_move(cMatch &match, cMove &move){

      if(cEvaluator::does_move_sac_for_supply(match, move)){
          move.m_prio = min(move.m_prio, (uint8_t)(cMove::P_MEDIUM_UP - 7));
      }

  }
  //*****************************************


