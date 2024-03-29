

  #include <algorithm> 
  #include <unistd.h>
  #include <iostream>
  #include "./match.hpp"
  #include "./daemon.hpp"
  #include "./threading.hpp"
  #include "./evaluator.hpp"
  #include "./generator.hpp"
  #include "./generator2.hpp"
  #include "./util/debug.hpp"



  //****************************************************************************
  cMatch::cMatch() : m_board(){ 
    
      m_score = 0;

      m_wpawn_cnt = 8;

      m_bpawn_cnt = 8;

      m_wofficer_cnt = 7;

      m_bofficer_cnt = 7;

  }
  //****************************************************************************



  //****************************************************************************
  // copy constructor
  cMatch::cMatch(const cMatch &match) : m_board(match.m_board){

      m_score = match.m_score;

      m_wpawn_cnt = match.m_wpawn_cnt;

      m_bpawn_cnt = match.m_bpawn_cnt;

      m_wofficer_cnt = match.m_wofficer_cnt;

      m_bofficer_cnt = match.m_bofficer_cnt;

      for(const cMove &move : match.m_minutes){
          m_minutes.push_back(move);
      }

  }
  //****************************************************************************



  //****************************************************************************
  cMatch::~cMatch(){
  }
  //****************************************************************************



  //****************************************************************************
  cMove *cMatch::get_last_move(){

      if(m_minutes.empty()){
          return nullptr;
      }
      else{
          return &(m_minutes.back());
      }

  }
  //****************************************************************************



  //****************************************************************************
  void cMatch::do_move(const cMove &move){

      // promotion
      if(move.is_promotion()){
          m_board.setfield(move.m_src_x, move.m_src_y, mBLK);
          m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_prompiece);

          m_score += SCORES[move.m_dstpiece];
          if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
              m_score += REVERSED_SCORES[move.m_prompiece] - REVERSED_SCORES[mWPW];

              m_wpawn_cnt--;
              m_wofficer_cnt++;

          }
          else{
              m_score += REVERSED_SCORES[move.m_prompiece] - REVERSED_SCORES[mBPW];

              m_bpawn_cnt--;
              m_bofficer_cnt++;
          }

          m_minutes.push_back(move);
          return;
      }

      // standard move
      m_board.setfield(move.m_src_x, move.m_src_y, mBLK);
      m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_srcpiece);

      m_score += SCORES[move.m_dstpiece];

      if(move.m_dstpiece != mBLK){
          if(move.m_dstpiece == mWPW){
              m_wpawn_cnt--;
          }
          else if(move.m_dstpiece == mBPW){
              m_bpawn_cnt--;
          }
          else if(PIECES_COLORS[move.m_dstpiece] == mWHITE){
              m_wofficer_cnt--;
          }
          else if(PIECES_COLORS[move.m_dstpiece] == mBLACK){
              m_bofficer_cnt--;
          }
      }

      // add on for en passant
      if(move.is_en_passant()){
          uint8_t enpiece = m_board.getfield(move.m_dst_x, move.m_src_y);
          m_board.setfield(move.m_dst_x, move.m_src_y, mBLK);

          m_score += SCORES[enpiece];

          if(PIECES_COLORS[enpiece] == mWHITE){
              m_wpawn_cnt--;
          }
          else{
              m_bpawn_cnt--;
          }

          m_minutes.push_back(move);
          return;
      }

      // castlings
      if(move.m_srcpiece == mWKG || move.m_srcpiece == mBKG){
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
  //****************************************************************************



  //****************************************************************************
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

          m_wpawn_cnt++;
          m_wofficer_cnt--;

          return true;
      }
      else if(move.is_promotion() && PIECES_COLORS[move.m_srcpiece] == mBLACK){
          m_board.setfield(move.m_src_x, move.m_src_y, mBPW);
          m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);

          m_score -= SCORES[move.m_dstpiece];
          m_score += SCORES[move.m_prompiece] - SCORES[mBPW];

          m_bpawn_cnt++;
          m_bofficer_cnt--;

          return true;
      }
      else{
          // undo standard move
          m_board.setfield(move.m_src_x, move.m_src_y, move.m_srcpiece);
          m_board.setfield(move.m_dst_x, move.m_dst_y, move.m_dstpiece);

          m_score -= SCORES[move.m_dstpiece];

          if(move.m_dstpiece != mBLK){
              if(move.m_dstpiece == mWPW){
                  m_wpawn_cnt++;
              }
              else if(move.m_dstpiece == mBPW){
                  m_bpawn_cnt++;
              }
              else if(PIECES_COLORS[move.m_dstpiece] == mWHITE){
                  m_wofficer_cnt++;
              }
              else if(PIECES_COLORS[move.m_dstpiece] == mBLACK){
                  m_bofficer_cnt++;
              }
          }
      }

      // undo en passant
      if(move.m_srcpiece == mWPW && move.is_en_passant()){
          m_board.setfield(move.m_dst_x, move.m_src_y, mBPW);

          m_score -= SCORES[mBPW];

          m_bpawn_cnt++;

          return true;
      }
      else if(move.m_srcpiece == mBPW && move.is_en_passant()){
          m_board.setfield(move.m_dst_x, move.m_src_y, mWPW);

          m_score -= SCORES[mWPW];

          m_wpawn_cnt++;

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
              if(m_board.m_wKg_has_moved_at != 0 && m_board.m_wKg_has_moved_at == m_minutes.size() + 1){
                  m_board.m_wKg_has_moved_at = 0;
              }
              m_board.m_wKg_x = move.m_src_x;
              m_board.m_wKg_y = move.m_src_y;
          }
          else{
              if(m_board.m_bKg_has_moved_at != 0 && m_board.m_bKg_has_moved_at == m_minutes.size() + 1){
                  m_board.m_bKg_has_moved_at = 0;
              }
              m_board.m_bKg_x = move.m_src_x;
              m_board.m_bKg_y = move.m_src_y;
          }
      }
      else if(move.m_srcpiece == mWRK){
          // update flags in board
          if(m_board.m_wRkA_has_moved_at != 0 && m_board.m_wRkA_has_moved_at == m_minutes.size() + 1){
              m_board.m_wRkA_has_moved_at = 0;
          }
          if(m_board.m_wRkH_has_moved_at != 0 && m_board.m_wRkH_has_moved_at == m_minutes.size() + 1){
              m_board.m_wRkH_has_moved_at = 0;
          }
      }
      else if(move.m_srcpiece == mBRK){
          // update flags in board
          if(m_board.m_bRkA_has_moved_at != 0 && m_board.m_bRkA_has_moved_at == m_minutes.size() + 1){
              m_board.m_bRkA_has_moved_at = 0;
          }
          if(m_board.m_bRkH_has_moved_at != 0 && m_board.m_bRkH_has_moved_at == m_minutes.size() + 1){
              m_board.m_bRkH_has_moved_at = 0;
          }
      }

      return true;

  }
  //****************************************************************************



  //****************************************************************************
  uint8_t cMatch::next_color() const{

    if(m_minutes.size() % 2 == 0){
        return mWHITE;
    }
    else{
        return mBLACK;
    }

  }
  //****************************************************************************



  //****************************************************************************
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
  //****************************************************************************



  //****************************************************************************
  uint8_t cMatch::eval_status(){

        vector<cMove> moves;

        cGenerator gen(*this);
        gen.gen_moves(moves, next_color());

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
  //****************************************************************************



  //****************************************************************************
  string cMatch::format_moves(const vector<cMove> &moves, const bool ext){

      string str_moves = "";

      for(const cMove &move : moves){
          str_moves += " [" + move.format(ext) + "] ";
      }

      return str_moves;

  }
  //****************************************************************************



  //****************************************************************************
  string cMatch::fmttime(time_t seconds){

      string str_hours = to_string(seconds / 3600);
      if(str_hours.length() == 1){ str_hours = "0" + str_hours; };

      time_t remaining = seconds % 3600;

      string str_minutes = to_string(remaining / 60);
      if(str_minutes.length() == 1){ str_minutes = "0" + str_minutes; };

      string str_seconds = to_string(remaining % 60);
      if(str_seconds.length() == 1){ str_seconds = "0" + str_seconds; };

      return str_hours + ":" + str_minutes + ":" + str_seconds;

  }
  //****************************************************************************



  //****************************************************************************
  void cMatch::calc_move(int32_t &rscore, vector<cMove> &rmoves, const uint8_t version){

    time_t time_start = time(0);

    int32_t alpha = SCORES[mWKG] * 10;

    int32_t beta = SCORES[mBKG] * 10;

    uint8_t depth = 1;

    uint8_t maxdepth = 3;

    void init_tracer();

    if(version == 1){
        calc_alphabeta(rscore, rmoves, depth, maxdepth, alpha, beta);
    }
    else{
        calc_alphabeta2(rscore, rmoves, depth, maxdepth, alpha, beta);
    }

    cout << "\nversion: " << to_string(version) << endl;

    cout << " score before calc: " << to_string(m_score) << " score after calc: " << to_string(rscore) << " calc-time: " << fmttime(time(0) - time_start) << endl;

    cout << format_moves(rmoves, true) << endl;

    prnt_tracer();

  }
  //****************************************************************************



  //****************************************************************************
  bool cMatch::is_move_valid(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece){

        vector<cMove> moves;
        
        cGenerator gen(*this);
        gen.gen_moves(moves, next_color());

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
  void cMatch::calc_alphabeta(int32_t &rscore, vector<cMove> &rmoves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta){

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

      cGenerator gen(*this);
      gen.gen_moves(moves, next_color());
      
      if(moves.size() == 0){
          rscore = score_terminate(depth);

          return;
      }
      else if(moves.size() == 1 && depth == 1){
          cMove move = moves.back();

          rscore = m_score + cEvaluator::score_board(*this, depth);

          rmoves.push_back(move);

          return;
      }
      else{
          sort(moves.begin(), moves.end(), sortByPrio);

          if(depth == 1){
              cout << "\n________________________" << endl;
              for(const cMove &move : moves){

                  cout << move.format(true) << endl;

              }
              cout << "________________________" << endl;
          }
      }

      if(depth == 1){
          vector<cMove> newmoves;

          start_alphabeta_threads(newscore, newmoves, moves, depth, maxdepth, alpha, beta, 1);

          rscore = newscore;

          for(cMove &nmove : newmoves){

              rmoves.push_back(nmove);

          }

          return;
      }

      cDaemon daemon(*this);

      for(const cMove &move : moves){

          if(depth == 1){
              cout << "started for: " << to_string(count + 1) << "(" << to_string(moves.size()) << ") " << move.format(true) << endl;
          }

          count++;

          vector<cMove> newmoves;

          bool dosearch = true;

          bool mate = false;

          if(is_three_times_repetition(move, depth)){ 
              // draw
              newscore = 0;

              newmoves.clear();

              dosearch = false;
          }
          else if(mate == false && daemon.is_continue(*this, move, depth) == false){
              newscore = m_score + cEvaluator::score_board(*this, depth);

              newmoves.clear();

              dosearch = false;
          }
          /*else if(depth > 3 && cEvaluator::does_move_do_check(*this, move)){
              // search for forced mate              
              if(cEvaluator::find_mate(*this, move, 3)){
                  dosearch = false;

                  mate = true;

                  newmoves.clear();

                  if(maximizing){
                      newscore = (SCORES[mBKG] + depth);
                  }
                  else{
                      newscore = (SCORES[mWKG] - depth);
                  }
              }
          }*/

          if(dosearch){
              do_move(move);

              calc_alphabeta(newscore, newmoves, depth + 1, maxdepth, alpha, beta);

              undo_move();
          }

          /*if(depth == 1){
              cout << "-------------------------------------------\n" << endl;
              cout << "CANDIDATE score: " << newscore << " ";
              cout << "[" <<move.format(true) << "]";
              cout << cMatch::format_moves(newmoves, true) << endl;
              cout << "\n" << endl;
          }*/

          if(maximizing){
              if(newscore > rscore){
                  rscore = newscore;

                  append_newmove(rmoves, newmoves, move);

                  /*if(depth == 1){
                      cout << "\n!!!CANDIDATE APPROVED!!!" << endl;
                      cout << "-------------------------------------------\n" << endl;
                  }*/
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

                  /*if(depth == 1){
                      cout << "\n!!!CANDIDATE APPROVED!!!" << endl;
                      cout << "-------------------------------------------\n" << endl;
                  }*/
              }
              beta = min(rscore, beta);
              if(beta <= alpha){
                  break;
              }
          }

          newmoves.clear();

      }

      moves.clear();

  }
  //****************************************************************************



  //****************************************************************************
  void cMatch::calc_alphabeta2(int32_t &rscore, vector<cMove> &rmoves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta){

      int32_t newscore;

      u_int8_t count = 0;

      bool maximizing = (next_color() == mWHITE);

      if(maximizing){
          rscore = SCORES[mWKG] * 10;
      }
      else{
          rscore = SCORES[mBKG] * 10;
      }

      if(depth == 1){
          vector<cMove> moves;

          cGenerator gen(*this);
          gen.gen_moves(moves, next_color());
          
          if(moves.size() == 0){
              rscore = score_terminate(depth);

              return;
          }
          else if(moves.size() == 1 && depth == 1){
              cMove move = moves.back();

              rscore = m_score + cEvaluator::score_board(*this, depth);

              rmoves.push_back(move);

              return;
          }

          sort(moves.begin(), moves.end(), sortByPrio);

          vector<cMove> newmoves;

          start_alphabeta_threads(newscore, newmoves, moves, depth, maxdepth, alpha, beta, 2);

          rscore = newscore;

          for(cMove &nmove : newmoves){

              rmoves.push_back(nmove);

          }

          return;
      }

      cGenerator2 gen2(*this);

      while(true){

          cMove *moveptr = gen2.gen_move();

          if(moveptr == nullptr){
              if(count == 0){
                  rscore = score_terminate(depth);

                  return;
              }
              else{
                  //rscore = m_score;

                  return;
              }
          }

          cDaemon daemon(*this);

          count++;

          if(depth == 1){
              cout << moveptr->format(true) << endl;
          }

          vector<cMove> newmoves;
          
          bool dosearch = true;

          bool mate = false;
          
          if(is_three_times_repetition(*moveptr, depth)){ 
              // draw
              newscore = 0;

              newmoves.clear();

              dosearch = false;
          }
          /*else if(depth > 3 && cEvaluator::does_move_do_check(*this, *moveptr)){
              // search for forced mate              
              if(cEvaluator::find_mate(*this, *moveptr, 3)){
                  dosearch = false;

                  mate = true;

                  newmoves.clear();

                  if(maximizing){
                      newscore = (SCORES[mBKG] + depth);
                  }
                  else{
                      newscore = (SCORES[mWKG] - depth);
                  }
              }
          }*/

          if(mate == false && daemon.is_continue(*this, *moveptr, depth) == false){
              newscore = m_score + cEvaluator::score_board(*this, depth);

              newmoves.clear();

              dosearch = false;
          }

          if(dosearch){
              do_move(cMove(*moveptr));

              calc_alphabeta2(newscore, newmoves, depth + 1, maxdepth, alpha, beta);

              undo_move();
          }

          if(maximizing){
              if(newscore > rscore){
                  rscore = newscore;

                  append_newmove2(rmoves, newmoves, moveptr);

                  if(depth == 1){
                      cout << "---------CANDIDATE APPROVED----------------\n" << endl;
                      cout << "score: " << newscore << " ";
                      cout << "[" << moveptr->format(true) << "]";
                      cout << cMatch::format_moves(newmoves, true) << endl;
                      cout << "\n" << endl;
                      cout << "-------------------------------------------\n" << endl;
                  }
              }

              alpha = max(rscore, alpha);
              if(alpha >= beta){
                  delete moveptr;

                  return;
              }
          }
          else{
              if(newscore < rscore){
                  rscore = newscore;

                  append_newmove2(rmoves, newmoves, moveptr);

                  if(depth == 1){
                      cout << "---------CANDIDATE APPROVED----------------\n" << endl;
                      cout << "score: " << newscore << " ";
                      cout << "[" << moveptr->format(true) << "]";
                      cout << cMatch::format_moves(newmoves, true) << endl;
                      cout << "\n" << endl;
                      cout << "-------------------------------------------\n" << endl;
                  }
              }

              beta = min(rscore, beta);
              if(beta <= alpha){
                  delete moveptr;

                  return;
              }
          }
          
          delete moveptr;

      }

  }
  //****************************************************************************



  //****************************************************************************
  uint8_t cMatch::eval_stage() const{

    // opening
    if(m_minutes.size() <= 30){
        return STAGE_OPENING;
    }
    else if((m_wofficer_cnt + m_bofficer_cnt) > 6){
        return STAGE_MIDDLE;
    }
    else{
        return STAGE_ENDGAME;
    }

  }
  //****************************************************************************



  //****************************************************************************
  void cMatch::append_newmove(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, const cMove &move){

    rcandidates.clear();

    rcandidates.push_back(move);

    for(const cMove &nmove : newcandidates){

        rcandidates.push_back(nmove);

    }

  }
  //****************************************************************************



  //****************************************************************************
  void cMatch::append_newmove2(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, const cMove *moveptr){

    rcandidates.clear();

    rcandidates.push_back(cMove(*moveptr));

    for(const cMove &nmove : newcandidates){

        rcandidates.push_back(nmove);

    }

  }
  //****************************************************************************



  //****************************************************************************
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
  //****************************************************************************



  //****************************************************************************
  void cMatch::start_alphabeta_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t version){

      cThreading threading(this, moves, alpha, beta);

      while(threading.has_finished() == false){

          threading.start(depth, maxdepth, version);

          sleep(1); // 0.6

          threading.update_candidates();

      }

      threading.update_candidates();

      threading.fetch_candidates(rscore, rmoves);

      moves.clear(); 

  } 
  //****************************************************************************



  //****************************************************************************
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
  //****************************************************************************



  //****************************************************************************
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
  //****************************************************************************



  //****************************************************************************
  bool cMatch::sortByPrio(const cMove &a, const cMove &b){

    return a.m_prio < b.m_prio;

  }
  //****************************************************************************



  //****************************************************************************
  bool cMatch::sortByRank(const cPiece &a, const cPiece &b){

    return (PIECES_RANKS[a.m_piece] < PIECES_RANKS[b.m_piece]);

  }
  //****************************************************************************
