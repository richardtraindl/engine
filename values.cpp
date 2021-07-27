

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


    map<string, uint8_t> DIRS = {
        {"no_dir", mNO_DIR},
        {"est-wst", mEST_WST},
        {"sth-nth", mSTH_NTH},
        {"sth_wst-nth_est", mSTHWST_NTHEST}, 
        {"sth_est-nth_wst", mSTHEST_NTHWST}
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
        {mWPLUS, 10},
        {mBPLUS, -10}
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
        {mWPW, 1}, 
        {mBPW, 1}, 
        {mWKN, 3}, 
        {mBKN, 3}, 
        {mWBP, 3}, 
        {mBBP, 3}, 
        {mWRK, 5}, 
        {mBRK, 5}, 
        {mWQU, 9}, 
        {mBQU, 9}, 
        {mWKG, 20}, 
        {mBKG, 20}
    };


    string reverse_lookup(const map<string, uint8_t> &dict, int value){

        for(auto &x: dict){
            if(x.second == value){
                return x.first;
            }
        }
        return "";

    }

