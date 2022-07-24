

  #include "./match.hpp"
  #include "./daemon.hpp"
  #include "./threading.hpp"
  #include "./evaluator.hpp"
  #include "./generator.hpp"


  //uint32_t g_filter_depth_cnt[100][20];


  cMatch::cMatch() : m_board(){ 
  }


  // copy constructor
  cMatch::cMatch(const cMatch &match) : m_board(match.m_board){

      m_score = match.m_score;

      for(const cMove &move : match.m_minutes){
          m_minutes.push_back(move);
      }

  }


  cMatch::~cMatch(){
  }


  
  cMove *cMatch::get_last_move(){

      if(m_minutes.empty()){
          return nullptr;
      }
      else{
          return &(m_minutes.back());
      }

  }



  void cMatch::do_move(const cMove &move){

      // promotion
      if(move.is_promotion()){
          m_board.setfield(move.m_src_x, move.m_src_y, mBLK);
          m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_prompiece);

          m_score += SCORES[move.m_dstpiece];
          if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
              m_score += REVERSED_SCORES[move.m_prompiece] - REVERSED_SCORES[mWPW];
          }
          else{
              m_score += REVERSED_SCORES[move.m_prompiece] - REVERSED_SCORES[mBPW];
          }
          
          m_minutes.push_back(move);
          return;
      }

      // standard move
      m_board.setfield(move.m_src_x, move.m_src_y, mBLK);
      m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);

      m_score += SCORES[move.m_dstpiece];

      // add on for en passant
      if(move.is_en_passant()){
          uint8_t enpiece = m_board.getfield(move.m_dst_x, move.m_src_y);
          m_board.setfield(move.m_dst_x, move.m_src_y, mBLK);

          m_score += SCORES[enpiece];

          m_minutes.push_back(move);
          return;
      }

      if(move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){
          // castlings
          if(move.is_short_castling()){
              uint8_t rook = m_board.getfield(7, move.m_dst_y); // h == 7
              m_board.setfield(7, move.m_dst_y, mBLK);
              m_board.setfield(5, move.m_dst_y, rook); // f == 5
          }
          else if(move.is_long_castling()){
              uint8_t rook = m_board.getfield(0, move.m_dst_y); // a == 0
              m_board.setfield(0, move.m_dst_y, mBLK);
              m_board.setfield(3, move.m_dst_y, rook); // c == 3
          }

          // update flags in board
          if(move.m_srcpiece == mWKG){
              if(m_board.m_wKg_has_moved_at == 0){
                  m_board.m_wKg_has_moved_at = m_minutes.size() + 1;
              }
              m_board.m_wKg_x = move.m_dst_x;
              m_board.m_wKg_y = move.m_dst_y;
          }
          else{
              if(m_board.m_bKg_has_moved_at == 0){
                  m_board.m_bKg_has_moved_at = m_minutes.size() + 1;
              }
              m_board.m_bKg_x = move.m_dst_x;
              m_board.m_bKg_y = move.m_dst_y;
          }
      }
      else if(move.m_srcpiece == mWRK){
          // update flags in board
          if(move.m_src_x == 0 && move.m_src_y == 0 && 
             m_board.m_wRkA_has_moved_at == 0){
              m_board.m_wRkA_has_moved_at = m_minutes.size() + 1;
          }
          if(move.m_src_x == 7 && move.m_src_y == 0 && 
              m_board.m_wRkH_has_moved_at == 0){
              m_board.m_wRkH_has_moved_at = m_minutes.size() + 1;
          }
      }
      else if(move.m_srcpiece == mBRK){
          // update flags in board
          if(move.m_src_x == 0 && move.m_src_y == 7 && 
             m_board.m_bRkA_has_moved_at == 0){
              m_board.m_bRkA_has_moved_at = m_minutes.size() + 1;
          }
          if(move.m_src_x == 7 && move.m_src_y == 7 && 
               m_board.m_bRkH_has_moved_at == 0){
              m_board.m_bRkH_has_moved_at = m_minutes.size() + 1;
          }
      }

      m_minutes.push_back(move);

  }


  bool cMatch::undo_move(){

      if(m_minutes.empty()){
          return false;
      }

      cMove move = m_minutes.back();
      m_minutes.pop_back();

      // undo promotion
      if(move.is_promotion() && PIECES_COLORS[move.m_srcpiece] == mWHITE){
          m_board.setfield(move.m_src_x, move.m_src_y, mWPW);
          m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);
      
          m_score -= SCORES[move.m_dstpiece];
          m_score += SCORES[move.m_prompiece] - SCORES[mWPW];

          return true;
      }
      else if(move.is_promotion() && PIECES_COLORS[move.m_srcpiece] == mBLACK){
          m_board.setfield(move.m_src_x, move.m_src_y, mBPW);
          m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);

          m_score -= SCORES[move.m_dstpiece];
          m_score += SCORES[move.m_prompiece] - SCORES[mBPW];

          return true;
      }
      else{
          // undo standard move
          m_board.setfield(move.m_src_x, move.m_src_y, move.m_srcpiece);
          m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);

          m_score -= SCORES[move.m_dstpiece];
      }

      // undo en passant
      if(move.m_srcpiece == mWPW && move.is_en_passant()){
          m_board.setfield(move.m_dst_x, move.m_src_y, mBPW);

          m_score -= SCORES[mBPW];

          return true;
      }
      else if(move.m_srcpiece == mBPW && move.is_en_passant()){
          m_board.setfield(move.m_dst_x, move.m_src_y, mWPW);

          m_score -= SCORES[mWPW];

          return true;
      }

      // undo castling
      if(move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){
          if(move.is_short_castling()){
              uint8_t rook = m_board.getfield(5, move.m_src_y); // f == 5
              m_board.setfield(5, move.m_src_y, mBLK);
              m_board.setfield(7, move.m_src_y, rook); // h == 7
          }
          else if(move.is_long_castling()){
              uint8_t rook = m_board.getfield(3, move.m_src_y); // c == 3
              m_board.setfield(3, move.m_src_y, mBLK);
              m_board.setfield(0, move.m_src_y, rook); // a == 0
          }

          // update flags in board
          if(move.m_srcpiece == mWKG){
              if(m_board.m_wKg_has_moved_at != 0 && m_board.m_wKg_has_moved_at == m_minutes.size()){
                  m_board.m_wKg_has_moved_at = 0;
              }
              m_board.m_wKg_x = move.m_src_x;
              m_board.m_wKg_y = move.m_src_y;
          }
          else{
              if(m_board.m_bKg_has_moved_at != 0 && m_board.m_bKg_has_moved_at == m_minutes.size()){
                  m_board.m_bKg_has_moved_at = 0;
              }
              m_board.m_bKg_x = move.m_src_x;
              m_board.m_bKg_y = move.m_src_y;
          }
      }
      else if(move.m_srcpiece == mWRK){
          // update flags in board
          if(m_board.m_wRkA_has_moved_at != 0 && m_board.m_wRkA_has_moved_at == m_minutes.size()){
              m_board.m_wRkA_has_moved_at = 0;
          }
          if(m_board.m_wRkH_has_moved_at != 0 && m_board.m_wRkH_has_moved_at == m_minutes.size()){
              m_board.m_wRkH_has_moved_at = 0;
          }
      }
      else if(move.m_srcpiece == mBRK){
          // update flags in board
          if(m_board.m_bRkA_has_moved_at != 0 && m_board.m_bRkA_has_moved_at == m_minutes.size()){
              m_board.m_bRkA_has_moved_at = 0;
          }
          if(m_board.m_bRkH_has_moved_at != 0 && m_board.m_bRkH_has_moved_at == m_minutes.size()){
              m_board.m_bRkH_has_moved_at = 0;
          }
      }

      return true;

  }


  uint8_t cMatch::next_color() const{

    if(m_minutes.size() % 2 == 0){
        return mWHITE;
    }
    else{
        return mBLACK;
    }

  }



  bool cMatch::is_king_attacked() const{

      vector <cPiece> attacking, others;

      if(next_color() == mWHITE){
          m_board.search_all_dirs_for_touching_pieces(others, attacking, m_board.m_wKg_x, m_board.m_wKg_y, nullptr, false);
      }
      else{
          m_board.search_all_dirs_for_touching_pieces(attacking, others, m_board.m_bKg_x, m_board.m_bKg_y, nullptr, false);
      }

      return (attacking.size() > 0);

  }



  uint8_t cMatch::eval_status(){

        vector<cMove> moves;
        
        cGenerator::gen_moves(*this, moves, next_color());

        if(moves.size() > 0){
            return STATUS_OPEN;
        }
        else{
            if(next_color() == mWHITE){
                if(m_board.is_field_touched(m_board.m_wKg_x,m_board.m_wKg_y, mBLACK)){
                    return STATUS_WINNER_BLACK;
                }
            }
            else{
                if(m_board.is_field_touched(m_board.m_bKg_x,m_board.m_bKg_y, mWHITE)){
                    return STATUS_WINNER_WHITE;
                }
            }
        }

        return STATUS_DRAW;

    }


  string cMatch::format_moves(const vector<cMove> &moves, const bool ext){

      string str_moves = "";

      for(const cMove &move : moves){
          str_moves += " [" + move.format(ext) + "] ";
      }

      return str_moves;

  }


  void cMatch::prnt_fmttime(string msg, int seconds){

    uint32_t sec, minute, hour, remaining;

    hour = seconds / (60 * 60);
    remaining = seconds % (60 * 60);
    minute = remaining / 60;
    sec = remaining % 60;

    cout << msg << setw(2) << setfill('0') << hour << ":";
    cout << setw(2) << setfill('0') <<  minute << ":";
    cout << setw(2) << setfill('0') << sec << endl;

  }
  


  void cMatch::calc_move(int32_t &rscore, vector<cMove> &rmoves){

    time_t time_start = time(0);

    int32_t alpha = SCORES[mWKG] * 10;

    int32_t beta = SCORES[mBKG] * 10;

    uint8_t depth = 1;

    uint8_t maxdepth = 3;

    uint8_t stage = eval_stage();
    cout << "stage: " << to_string(stage) << endl;
    calc_alphabeta(rscore, rmoves, depth, maxdepth, alpha, beta, stage);

    cout << "\nmatch m_score: " << m_score;

    cout << "\nresult: " << rscore;

    cout << "\n" << format_moves(rmoves, true) << endl;

    prnt_fmttime("\ncalc-time: ", time(0) - time_start);

  }


  bool cMatch::is_move_valid(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece){

        vector<cMove> moves;
        
        cGenerator::gen_moves(*this, moves, next_color());

        bool found = false;

        for(const cMove &move : moves){

            if(move.m_src_x == src_x && move.m_src_y == src_y &&
            move.m_dst_x == dst_x && move.m_dst_y == dst_y){
                found = true;

                break;
            }

        }

        return found;

    }


//****************************************************************************

// CALCULATION
// CALCULATION
// CALCULATION

//****************************************************************************



  void cMatch::calc_alphabeta(int32_t &rscore, vector<cMove> &rmoves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage){

      vector<cMove> newmoves;

      int32_t newscore;

      u_int8_t count = 0;

      bool maximizing = (next_color() == mWHITE);

      if(maximizing){
          rscore = SCORES[mWKG] * 10;
      }
      else{
          rscore = SCORES[mBKG] * 10;
      }

      vector<cMove> moves;
      cGenerator::gen_moves(*this, moves, next_color());
      
      if(depth == 1){
          cout << format_moves(moves, true);
          cout << "***************************" << endl;
      }

      if(moves.size() == 0){
          rscore = score_terminate(depth);
          return;
      }
      else if(moves.size() == 1 && depth == 1){
          cMove move = moves.back();

          rscore = m_score; // + cEvaluator::score_board(*this, depth, stage);

          rmoves.push_back(move);

          return;
      }
      else{
          sort(moves.begin(), moves.end(), sortByPrio);
      }

      if(depth == 1){
          start_alphabeta_threads(newscore, newmoves, moves, depth, maxdepth, alpha, beta, stage);

          rscore = newscore;

          for(cMove &nmove : newmoves){

              rmoves.push_back(nmove);

          }

          return;
      }

      cDaemon daemon(*this);

      for(const cMove &move : moves){
        
          count++;

          newmoves.clear();

          bool dosearch = true;

          if(is_three_times_repetition(move, depth)){ 
              newscore = 0;

              dosearch = false;
          }
          else if(! daemon.is_continue(move, depth)){
              //if( !(depth < 10 && move.m_prio < cMove::P_HIGH)){
              //if(is_search_continue(move, depth, maxdepth, stage) == false){
              newscore = m_score; // + cEvaluator::score_move(*this, move) + cEvaluator::score_board(*this, depth, stage);

              dosearch = false;
          }

          if(dosearch){
            do_move(move);

            calc_alphabeta(newscore, newmoves, depth + 1, maxdepth, alpha, beta, stage);

            undo_move();
          }

          if(maximizing){
            if(newscore > rscore){
              rscore = newscore;
              append_newmove(rmoves, newmoves, move);
            }

            alpha = max(rscore, alpha);
            if(alpha >= beta){
              break;
            }
          }
          else{
            if(newscore < rscore){
              rscore = newscore;
              append_newmove(rmoves, newmoves, move);
            }
            beta = min(rscore, beta);
            if(beta <= alpha){
              break;
            }
          }
          
      }

      moves.clear();

  }


  uint8_t cMatch::eval_stage() const{

    // opening
    if(m_minutes.size() <= 30){
        return STAGE_OPENING;
    }

    //endgame or middlegame
    uint8_t wpwcnt = 0;
    uint8_t bpwcnt = 0;
    uint8_t wqucnt = 0;
    uint8_t bqucnt = 0;

    vector<cPiece> wofficers, bofficers;

    for(uint8_t y = 0; y < 8; ++y){
        for(uint8_t x = 0; x < 8; ++x){
            uint8_t piece = m_board.getfield(x, y);

            if(piece == mBLK){
                continue;
            }
            else if(piece == mWPW){
                wpwcnt++;
            }
            else if(piece == mBPW){
                bpwcnt++;
            }
            else if(piece == mWKG || piece == mBKG){
                continue;
            }
            else if(PIECES_COLORS[piece] == mWHITE){
                if(piece == mWQU){
                    wqucnt++;
                }
                else{
                    wofficers.push_back(cPiece(piece, x, y));
                }
            }
            else{
                if(piece == mBQU){
                    bqucnt++;
                }
                else{
                    bofficers.push_back(cPiece(piece, x, y));
                }
            }
        }
    }

    if((wqucnt + bqucnt + wofficers.size() + bofficers.size()) > 6){
        return STAGE_MIDDLE;
    }

    return STAGE_ENDGAME;

  }


  void cMatch::append_newmove(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, const cMove &move){

    rcandidates.clear();

    rcandidates.push_back(move);

    for(const cMove &nmove : newcandidates){

        rcandidates.push_back(nmove);

    }

  }


  bool cMatch::is_three_times_repetition(const cMove &move, uint8_t depth) const{

    cMatch match(*this);

    match.do_move(move);

    uint8_t fields[8][8];

    match.m_board.copy_fields(fields);

    uint8_t equalcnt = 0;

    uint8_t maxcnt = min((uint8_t)(8 + depth), (uint8_t)match.m_minutes.size());

    for(uint8_t i = 0; i < maxcnt; ++i){
        match.undo_move();
        //match.undo_move(); 2x???

        if(match.m_board.compare_fields(fields)){
            equalcnt++;
        }
    }

    return equalcnt >= 2;

  }

 
  bool cMatch::is_search_continue(const cMove &move, const uint8_t depth, const uint8_t maxdepth, const uint8_t stage){

    // take capture moves
    if(move.m_dstpiece != mBLK){
        return true;
    }

    // take en passant moves
    if(move.is_en_passant()){ 
        return true;
    }

    // take promotion moves
    if(move.is_promotion()){
        return true;
    }

    // do move on cloned match/board
    cMatch match(*this);
    match.do_move(move);

    // take passed pawn moves
    if(match.m_board.is_passed_pawn(move.m_srcpiece, move.m_dst_x, move.m_dst_y) && depth < 8){
        return true;
    }

    // take king attacking moves
    if(match.is_king_attacked() && depth < 8){
        //if(match.is_series_of_king_attacks(4)){
            return true;
        //}
    }


    // take good capture moves
    /*bool prevmove_was_capture = false;

    if(m_minutes.empty() == false){
        cMove prevmove = m_minutes.back();
        prevmove_was_capture = (prevmove.is_en_passant() || prevmove.m_dstpiece != mBLK);
    }

    if(depth <= maxdepth + 7 || (depth <= maxdepth + 7 && prevmove_was_capture)){ 
        int32_t mscore = cEvaluator::score_move(*this, move);

        if((PIECES_COLORS[move.m_srcpiece] == mWHITE && mscore >= SCORES[mBPW]) || (PIECES_COLORS[move.m_srcpiece] == mBLACK && mscore <= SCORES[mWPW])){
            return true;
        }
    }*/

    /*if(depth <= maxdepth + 7){
        int8_t move_score = cEvaluator::score_move(m_board, move);

        if((PIECES_COLORS[move.m_srcpiece] == mWHITE && move_score > 2) || (PIECES_COLORS[move.m_srcpiece] == mBLACK && move_score < -2)){
            return true;

        // stormy moves
        int32_t score_before = cEvaluator::score_touches_on_all_pieces(m_board);

        int32_t score_after = cEvaluator::score_touches_on_all_pieces(board);
        
        //PIECES_COLORS[move.m_srcpiece] == mWHITE && 
        if(score_after > (score_before + SCORES[mWPLUS2])){
            return true;
        }
        //PIECES_COLORS[move.m_srcpiece] == mBLACK && 
        else if(score_after < (score_before + SCORES[mBPLUS2])){
            return true;
        }
    } */

    

    // every search has to come to an end
    if(stage == STAGE_ENDGAME){
        return (depth < 8); 
    }
    else{
        return (depth < 8);
    }

  }


  void cMatch::start_alphabeta_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage){

      cThreading threading(this, moves, alpha, beta);

      while(threading.has_finished() == false){

          threading.start(depth, maxdepth, stage);

          sleep(0.6);

          threading.update_candidates();

      }

      threading.update_candidates();

      threading.fetch_candidates(rscore, rmoves);

      moves.clear(); 

  } 


  // 100 Züge davor kein Bauernzug und keine Figur geschlagen
  bool cMatch::is_fifty_moves_rule() const{

    if(m_minutes.size() < 100){
        return false;
    }

    int rulecnt = 0;

    int cnt = 0;

    cMatch *match = new cMatch(*this);

    for(vector<cMove>::reverse_iterator it = match->m_minutes.rbegin(); it != match->m_minutes.rend(); ++it){

      cnt++;

      if(cnt > 101){
          delete match;

          return false;
      }

      match->undo_move();

      if(it->m_dstpiece != mBLK || it->m_srcpiece == mWPW || it->m_srcpiece == mBPW){
          rulecnt = 0;

          continue;
      }

      rulecnt++;
      if(rulecnt >= 100){
          delete match;

          return true;
      }

    }

    delete match;

    return false;

  } 


  int32_t cMatch::score_terminate(uint8_t depth) const{

    if(next_color() == mWHITE){
        if(m_board.is_field_touched(m_board.m_wKg_x, m_board.m_wKg_y, mBLACK)){
            return (SCORES[mWKG]) + depth;
        }
    }
    else{
        if(m_board.is_field_touched(m_board.m_bKg_x, m_board.m_bKg_y, mWHITE)){
            return (SCORES[mBKG]) - depth;
        }
    }

    return 0; // draw

  }


  bool cMatch::sortByPrio(const cMove &a, const cMove &b){

    return a.m_prio < b.m_prio;

  }


  bool cMatch::sortByRank(const cPiece &a, const cPiece &b){

    return (PIECES_RANKS[a.m_piece] < PIECES_RANKS[b.m_piece]);

  }

