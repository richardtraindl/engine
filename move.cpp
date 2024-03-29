

    #include <sstream>
    #include "./move.hpp"
    #include "./board.hpp"


  //*****************************************
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
  //*****************************************



  //*****************************************
  cMove::cMove(){
  }
  //*****************************************



  //*****************************************
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
  //*****************************************



  //*****************************************
  cMove::~cMove(){
  }
  //*****************************************



  //*****************************************
  bool cMove::is_capture() const{

    if(m_dstpiece != mBLK){
        return true;
    }

    return is_en_passant();

  }
  //*****************************************



  //*****************************************
  bool cMove::is_en_passant() const{

      if(m_srcpiece == mWPW){
          if(m_dstpiece == mBLK && m_src_y == 4 && m_dst_y == 5 && m_src_x != m_dst_x){
              return true;
          }
      }
      else if(m_srcpiece == mBPW){
          if(m_dstpiece == mBLK && m_src_y == 3 && m_dst_y == 2 && m_src_x != m_dst_x){
              return true;
          }
      }

      return false;

  }
  //*****************************************



  //*****************************************
  bool cMove::is_promotion() const{

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
  //*****************************************



  //*****************************************
  bool cMove::is_short_castling() const{

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
  //*****************************************



  //*****************************************
  bool cMove::is_long_castling() const{

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
  //*****************************************



  //*****************************************
  void cMove::create_fake_rook_move(cMove &rookmove) const{

      if(is_short_castling()){
          uint8_t rook;

          PIECES_COLORS[m_srcpiece] == mWHITE ? rook = mWRK : rook = mBRK;

          rookmove.m_src_x = (m_dst_x + 1);
          rookmove.m_src_y = m_src_y;
          rookmove.m_dst_x = (m_src_x + 1);
          rookmove.m_dst_y = m_src_y;
          rookmove.m_srcpiece = rook;
          rookmove.m_dstpiece = mBLK;
          rookmove.m_prompiece = mBLK;
          rookmove.m_prio = cMove::P_BOTTOM;
      }
      else if(is_long_castling()){
          uint8_t rook;

          PIECES_COLORS[m_srcpiece] == mWHITE ? rook = mWRK : rook = mBRK;

          rookmove.m_src_x = (m_dst_x - 2);
          rookmove.m_src_y = m_src_y;
          rookmove.m_dst_x = (m_dst_x + 1);
          rookmove.m_dst_y = m_src_y;
          rookmove.m_srcpiece = rook;
          rookmove.m_dstpiece = mBLK;
          rookmove.m_prompiece = mBLK;
          rookmove.m_prio = cMove::P_BOTTOM;
      }

  }
  //*****************************************



  //*****************************************
  bool cMove::compare(const cMove &move) const{

      return (m_src_x == move.m_src_x && m_src_y == move.m_src_y && m_dst_x == move.m_dst_x && m_dst_y == move.m_dst_y);

  }
  //*****************************************



  //*****************************************
  string cMove::format(bool ext) const{

      string hyphen = "";

      string trailing = "";

      if(is_en_passant()){

          return cBoard::indices_to_coord(m_src_x, m_src_y) + "x" + cBoard::indices_to_coord(m_dst_x, m_dst_y) + ", e.p.";

      }

      (m_dstpiece == mBLK)? hyphen = "-" : hyphen = "x";

      if(m_prompiece != mBLK){ 

          trailing = ", " + reverse_lookup8(PIECES, m_prompiece); 

      }

      string out = cBoard::indices_to_coord(m_src_x, m_src_y) + hyphen + cBoard::indices_to_coord(m_dst_x, m_dst_y) + trailing;

      if(ext){

          return out + " prio: " + to_string(m_prio);

      }
      else{

          return out;

      }

  }
  //*****************************************


