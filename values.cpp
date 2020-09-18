
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


    map<uint8_t, string> PIECES_STR = {
      {PIECES["blk"], "  "}, 
      {PIECES["wPw"], "wPw"}, 
      {PIECES["bPw"], "bPw"}, 
      {PIECES["wKn"], "wKn"}, 
      {PIECES["bKn"], "bKn"}, 
      {PIECES["wBp"], "wBp"}, 
      {PIECES["bBp"], "bBp"}, 
      {PIECES["wRk"], "wRk"}, 
      {PIECES["bRk"], "bRk"}, 
      {PIECES["wQu"], "wQu"}, 
      {PIECES["bQu"], "bQu"}, 
      {PIECES["wKg"], "wKg"}, 
      {PIECES["bKg"], "bKg"}
    };


    map<uint8_t, uint8_t> PIECES_RANKS = {
        {PIECES["blk"], 0}, 
        {PIECES["wPw"], 1}, 
        {PIECES["bPw"], 1}, 
        {PIECES["wKn"], 3}, 
        {PIECES["bKn"], 3}, 
        {PIECES["wBp"], 3}, 
        {PIECES["bBp"], 3}, 
        {PIECES["wRk"], 5}, 
        {PIECES["bRk"], 5}, 
        {PIECES["wQu"], 9}, 
        {PIECES["bQu"], 9}, 
        {PIECES["wKg"], 20}, 
        {PIECES["bKg"], 20}
    };


    map<uint8_t, int16_t> SCORES = {
        {PIECES["blk"], 0},
        {PIECES["wPw"], -10},
        {PIECES["wKn"], -34},
        {PIECES["wBp"], -34},
        {PIECES["wRk"], -45},
        {PIECES["wQu"], -90},
        {PIECES["wKg"], -10000},
        {PIECES["bPw"], 10},
        {PIECES["bKn"], 34},
        {PIECES["bBp"], 34},
        {PIECES["bRk"], 45},
        {PIECES["bQu"], 90},
        {PIECES["bKg"], 10000}
    };


    map<uint8_t, int16_t> REVERSED_SCORES = {
        {PIECES["blk"], 0},
        {PIECES["wPw"], 10},
        {PIECES["wKn"], 34},
        {PIECES["wBp"], 34},
        {PIECES["wRk"], 45},
        {PIECES["wQu"], 90},
        {PIECES["wKg"], 10000},
        {PIECES["bPw"], -10},
        {PIECES["bKn"], -34},
        {PIECES["bBp"], -34},
        {PIECES["bRk"], -45},
        {PIECES["bQu"], -90},
        {PIECES["bKg"], -10000}
    };


    map<string, uint8_t> STEP_OWNER = {
        {"rook", 1}, 
        {"bishop", 2}, 
        {"king", 3}, 
        {"knight", 4},
        {"wpawn", 5},
        {"bpawn", 6}
    };


    map<string, uint8_t> MOVE_TYPE = {
        {"standard", 1}, 
        {"capture", 2},
        {"en-passant", 3},
        {"short-castling", 4}, 
        {"long-castling", 5}
    };

