

    #include "./bitboard.hpp"


    cBitBoard::cBitBoard(){ 
    }


    const uint64_t cBitBoard::m_tstmask[] = { 
        0x1000000000000000, 0x0100000000000000, 0x0010000000000000, 0x0001000000000000, 0x0000100000000000, 0x0000010000000000, 0x0000001000000000, 0x0000000100000000, 0x0000000010000000, 0x0000000001000000, 0x0000000000100000, 0x0000000000010000, 0x0000000000001000, 0x0000000000000100, 0x0000000000000010, 0x0000000000000001 
    };


    const uint64_t cBitBoard::m_clearmask[] = { 
        0x0FFFFFFFFFFFFFFF, 0xF0FFFFFFFFFFFFFF, 0xFF0FFFFFFFFFFFFF, 0xFFF0FFFFFFFFFFFF, 0xFFFF0FFFFFFFFFFF, 0xFFFFF0FFFFFFFFFF, 0xFFFFFF0FFFFFFFFF, 0xFFFFFFF0FFFFFFFF, 0xFFFFFFFF0FFFFFFF, 0xFFFFFFFFF0FFFFFF, 0xFFFFFFFFFF0FFFFF, 0xFFFFFFFFFFF0FFFF, 0xFFFFFFFFFFFF0FFF, 0xFFFFFFFFFFFFF0FF, 0xFFFFFFFFFFFFFF0F, 0xFFFFFFFFFFFFFFF0 
    };


    uint8_t cBitBoard::get(uint8_t x, uint8_t y){

        uint8_t piece = 0;

        uint8_t test = 0b00000001;

        uint8_t idx = x + ((y % 2) * 8);

        uint64_t tstmask = m_tstmask[idx];

        uint64_t row = m_bitfields[(y / 2)];

        for(uint8_t i = 0; i < 4; ++i){
            if((row & tstmask) > 0){
                piece = (piece | test);
            }

            test = (test << 1);

            tstmask = (tstmask << 1);
        }

        return piece;

    }


    void cBitBoard::set(uint8_t x, uint8_t y, uint8_t piece){

        uint8_t test = 0b00000001;

        uint8_t idx = x + ((y % 2) * 8);

        uint64_t tstmask = m_tstmask[idx];

        uint64_t clearmask = m_clearmask[idx];

        m_bitfields[(y / 2)] = m_bitfields[(y / 2)] & clearmask;

        for(uint8_t i = 0; i < 4; ++i){
            if((piece & test) > 0){
                m_bitfields[(y / 2)] = m_bitfields[(y / 2)] | tstmask;
            }

            test = (test << 1);

            tstmask = (tstmask << 1);
        }

    }


    void cBitBoard::import_fields(uint8_t fields[8][8]){
        
        for(uint8_t y = 0; y < 8; ++y){

            for(uint8_t x = 0; x < 8; ++x){

                uint8_t piece = fields[y][x];

                set(x, y, piece);

            }

        }

    }


    bool cBitBoard::compare(const uint64_t bitfields[4]){

        for(int i = 0; i < 4; ++i){

            if(m_bitfields[i] != bitfields[i]){
                return false;
            }

        }

        return true;

    }


    void cBitBoard::prnt(){

        string textcolor, backcolor, strpiece;

        for(int y = 7; y >=0; --y){

            for(int x = 0; x < 8; ++x){

                uint8_t piece = get(x, y);

                if(piece == mBLK){
                    strpiece = "   ";
                }
                else{
                    strpiece = reverse_lookup(PIECES, piece);
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
        }

    }


