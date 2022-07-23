


  #include "./values.hpp"

  
  map<string, uint8_t> PIECES = {
      {"blk", mBLK}, 
      {"wPw", mWPW}, 
      {"wKn", mWKN}, 
      {"wBp", mWBP}, 
      {"wRk", mWRK}, 
      {"wQu", mWQU}, 
      {"wKg", mWKG},
      {"bPw", mBPW}, 
      {"bKn", mBKN}, 
      {"bBp", mBBP}, 
      {"bRk", mBRK}, 
      {"bQu", mBQU}, 
      {"bKg", mBKG}
  };


  map<string, uint8_t> COLORS = {
      {"blank", mBLANK},
      {"white", mWHITE},
      {"black", mBLACK}
  };


  map<uint8_t, uint8_t> REVERSED_COLORS = {
      {mBLANK, mBLANK},
      {mWHITE, mBLACK},
      {mBLACK, mWHITE} 
  };


  map<uint8_t, uint8_t> PIECES_COLORS = {
      {mBLK, mBLANK},
      {mWPW, mWHITE},
      {mWKN, mWHITE},
      {mWBP, mWHITE},
      {mWRK, mWHITE},
      {mWQU, mWHITE},
      {mWKG, mWHITE},
      {mBPW, mBLACK},
      {mBKN, mBLACK},
      {mBBP, mBLACK},
      {mBRK, mBLACK},
      {mBQU, mBLACK},
      {mBKG, mBLACK}
  };


  map<string, uint16_t> DIRS = {
      {"no_dir", mNO_DIR},
      {"est-wst", mEST_WST},
      {"wst-est", mWST_EST},
      {"sth-nth", mSTH_NTH},
      {"nth-sth", mNTH_STH},
      {"sth_wst-nth_est", mSTHWST_NTHEST}, 
      {"nth_est-sth_wst", mNTHEST_STHWST}, 
      {"sth_est-nth_wst", mSTHEST_NTHWST},
      {"nth_wst-sth_est", mNTHWST_STHEST},
      {"knight_dir", mKNIGHT_DIR},
      {"horizontal", mHORIZONTAL},
      {"vertical", mVERTICAL},
      {"dia_left_bottom", mDIA_LEFT_BOTTOM},
      {"dia_right_bottom", mDIA_RIGHT_BOTTOM}
  };


  map<uint8_t, int32_t> SCORES = {
      {mBLK, 0},
      {mWPW, -100},
      {mWKN, -340},
      {mWBP, -340},
      {mWRK, -450},
      {mWQU, -900},
      {mWKG, -20000},
      {mBPW, 100},
      {mBKN, 340},
      {mBBP, 340},
      {mBRK, 450},
      {mBQU, 900},
      {mBKG, 20000},
      {mWPLUS1, 30},
      {mWPLUS2, 50},
      {mWPLUS3, 70},
      {mWPLUS4, 90},
      {mBPLUS1, -30},
      {mBPLUS2, -50},
      {mBPLUS3, -70},
      {mBPLUS4, -90}
  };


  map<uint8_t, int32_t> REVERSED_SCORES = {
      {mBLK, 0},
      {mWPW, 100},
      {mWKN, 340},
      {mWBP, 340},
      {mWRK, 450},
      {mWQU, 900},
      {mWKG, 20000},
      {mBPW, -100},
      {mBKN, -340},
      {mBBP, -340},
      {mBRK, -450},
      {mBQU, -900},
      {mBKG, -20000}
  };


  map<uint8_t, uint8_t> PIECES_RANKS = {
      {mBLK, 0}, 
      {mWPW, 10}, 
      {mBPW, 10}, 
      {mWKN, 34}, 
      {mBKN, 34}, 
      {mWBP, 34}, 
      {mBBP, 34}, 
      {mWRK, 45}, 
      {mBRK, 45}, 
      {mWQU, 90}, 
      {mBQU, 90}, 
      {mWKG, 200}, 
      {mBKG, 200}
  };


  string reverse_lookup8(const map<string, uint8_t> &dict, int value){

      for(auto &x: dict){
          if(x.second == value){
              return x.first;
          }
      }
      return "";

  }


  string reverse_lookup16(const map<string, uint16_t> &dict, int value){

      for(auto &x: dict){
          if(x.second == value){
              return x.first;
          }
      }
      return "";

  }

