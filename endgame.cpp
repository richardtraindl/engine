

    #include "./endgame.hpp"


    cEndGame100_base::cEndGame100_base(){
    }


    int32_t cEndGame100_base::eval_push_kg_to_margin(uint8_t status, uint8_t depth, const uint8_t fields[8][8], uint8_t wkg_x, uint8_t wkg_y, uint8_t bkg_x, uint8_t bkg_y){

        int32_t score = 0;

        if(status == cBoard::ENDGAME_STAT_100 || status == cBoard::ENDGAME_STAT_110){

            score = 10 - depth;

            uint8_t diffkg = cBoard::max_diff(wkg_x, wkg_y, bkg_x, bkg_y);
            score += (3 - diffkg) * 2;

            uint8_t diff_to_margin = cBoard::diff_to_margin(bkg_x, bkg_y);
            score += (2 - diff_to_margin) * 2 ;

            if(status == cBoard::ENDGAME_STAT_100){
                score += cEndGame100::m_single_KG[bkg_y][bkg_x];
            }
            else{
                score += cEndGame110::m_single_KG[bkg_y][bkg_x];
            }
        }
        else{

            score = -10 + depth;

            uint8_t diffkg = cBoard::max_diff(wkg_x, wkg_y, bkg_x, bkg_y);
            score -= (3 - diffkg) * 2;

            uint8_t diff_to_margin = cBoard::diff_to_margin(wkg_x, wkg_y);
            score -= (2 - diff_to_margin) * 2 ;
            
            if(status == cBoard::ENDGAME_STAT_120){
                score -= cEndGame100::m_single_KG[wkg_y][wkg_x];
            }
            else{
                score -= cEndGame110::m_single_KG[wkg_y][wkg_x];
            }
        }

        return score;

    }


    uint8_t cEndGame100_base::eval_path_to_mate(uint8_t status, const uint8_t fields[8][8]){

        uint8_t lib = 0;

        const uint64_t (*tstfields)[5][4];

        switch(status){
            case cBoard::ENDGAME_STAT_100: 
                tstfields = cEndGame100::m_path_to_mate; 
                break;
            
            case cBoard::ENDGAME_STAT_110: 
                tstfields = cEndGame110::m_path_to_mate; 
                break;
            
            case cBoard::ENDGAME_STAT_120: 
                tstfields = cEndGame120::m_path_to_mate; 
                break;
        
            case cBoard::ENDGAME_STAT_130: 
                tstfields = cEndGame130::m_path_to_mate; 
                break;
                
            default:
                return lib;
        }

        cBitBoard bitboard;
        bitboard.import_fields(fields);

        uint64_t tmpfields[4];

        for(uint8_t i = 0; i < 4; ++i){

            for(uint8_t j = 0; j < 5; ++j){

                for(uint8_t k = 0; k < 4; ++k){
                    tmpfields[k] = (bitboard.m_bitfields[k] & tstfields[i][j][k]);
                }

                if(bitboard.compare(tmpfields)){
                    lib = j + 1;

                    break;
                }
            }

        }
        return lib;

    }


    // 100 series
    cEndGame100::cEndGame100(){
    }


    const int32_t cEndGame100::m_single_KG[8][8] = {
        { 0,  0,  1,  2,  3,  4,  5,  6 },  // first row
        { 0,  0,  0,  1,  2,  3,  5,  5 },
        { 1,  0,  0,  0,  1,  2,  3,  4 },
        { 2,  1,  0,  0,  0,  1,  2,  3 },
        { 3,  2,  1,  0,  0,  0,  1,  2 },
        { 4,  3,  2,  1,  0,  0,  0,  1 },
        { 5,  5,  3,  2,  1,  0,  0,  0 },
        { 6,  5,  4,  3,  2,  1,  0,  0 }
    };

    const uint64_t cEndGame100::m_path_to_mate[4][5][4] = {
        // wKg_y == 2
        { { 0x00E0000030200000,
            0x0060000000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x000EEE0030002E00,
            0x0006000000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x0000E00000302000,
            0x0000600000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x00000E0000300020,
            0x0300060030000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x000000E000003200,
            0x0003006000300200,
            0x0300002030000000,
            0x0000000000000000 } },

        // wKg_x == 3
        { { 0x0300000000000000,
            0xE260000000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x0300000000000000,
            0x00000000E0600000,
            0xE2000000EE000000,
            0x0000000000000000 },

          { 0x0000000000000000,
            0x0300000000000000,
            0xE260000000000000,
            0x0000000000000000 },

          { 0x0003000000300000,
            0x0300000000000000,
            0x02000000E0600000,
            0x0000000000000000 },

          { 0x0000030000003000,
            0x0003000000300000,
            0x0300000002020000,
            0xE060200000000000 } },

        // wKg_y == 5
        { { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000000600,
            0x0000020300000E00 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000006000,
            0x00E2000300EEE000 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000060000,
            0x00020300000E0000 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x0000000300600030,
            0x0200030000E00000 },

          { 0x0000000000000000,
            0x0000000302000030,
            0x0020030006003000,
            0x002300000E000000 } },

        // wKg_x == 5
        { { 0x0000000000000000,
            0x0000000000000000,
            0x000000000000062E,
            0x0000000000000030 },

          { 0x0000000000000000,
            0x000000EE0000002E,
            0x0000060E00000000,
            0x0000000000000030 },

          { 0x0000000000000000,
            0x000000000000062E,
            0x0000000000000030,
            0x0000000000000000 },

          { 0x0000000000000000,
            0x0000060E00000020,
            0x0000000000000030,
            0x0000030000003000 },

          { 0x000000000002060E,
            0x0000202000000030,
            0x0000030000003000,
            0x0003000000300000 } }
    };


    // 110 series
    cEndGame110::cEndGame110(){
    }


    const int32_t cEndGame110::m_single_KG[8][8] = {
        { 6,  5,  4,  3,  2,  1,  0,  0 }, // first row
        { 5,  5,  3,  2,  1,  0,  0,  0 },
        { 4,  3,  2,  1,  0,  0,  0,  1 },
        { 3,  2,  1,  0,  0,  0,  1,  2 },
        { 2,  1,  0,  0,  0,  1,  2,  3 },
        { 1,  0,  0,  0,  1,  2,  3,  4 },
        { 0,  0,  0,  1,  2,  3,  5,  5 },
        { 0,  0,  1,  2,  3,  4,  5,  6 }
    };


    const uint64_t cEndGame110::m_path_to_mate[4][5][4] = {
        // wKg_y == 2
        { { 0x00000E0000000203,
            0x0000060000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x00EEE00000E20003,
            0x0000600000000000,
            0x0000000000000000,
            0x0000000000000000 },
            
          { 0x000E000000020300,
            0x0006000000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x00E0000002000300,
            0x0060003000000003,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x0E00000000230000,
            0x0600300000200300,
            0x0200003000000003,
            0x0000000000000000 } },

        // wKg_x == 2
        { { 0x0000003000000000,
            0x0000062E00000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x0000003000000000,
            0x000000000000060E,
            0x0000002E000000EE,
            0x0000000000000000 },

          { 0x0000000000000000,
            0x0000003000000000,
            0x0000062E00000000,
            0x0000000000000000 },

          { 0x0000300000000300,
            0x0000003000000000,
            0x000000200000060E,
            0x0000000000000000 },

          { 0x0030000000030000,
            0x0000300000000300,
            0x0000003000002020,
            0x0002060E00000000 } },

        // wKg_y == 5
        { { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000600000,
            0x3020000000E00000 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000060000,
            0x30002E00000EEE00 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000006000,
            0x003020000000E000 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x3000000003000600,
            0x0030002000000E00 },

          { 0x0000000000000000,
            0x3000000003000020,
            0x0030020000030060,
            0x00003200000000E0 } },

        // wKg_x == 5
        { { 0x0000000000000000,
            0x0000000000000000,
            0x00000000E2600000,
            0x0000000003000000 },

          { 0x0000000000000000,
            0xEE000000E2000000,
            0xE060000000000000,
            0x0000000003000000 },

          { 0x0000000000000000,
            0x00000000E2600000,
            0x0000000003000000,
            0x0000000000000000 },

          { 0x0000000000000000,
            0xE060000002000000,
            0x0000000003000000,
            0x0030000000030000 },

          { 0x00000000E0602000,
            0x0202000003000000,
            0x0030000000030000,
            0x0000300000000300 } }
    };


    // 120 series
    cEndGame120::cEndGame120(){
    }


    const uint64_t cEndGame120::m_path_to_mate[4][5][4] = {
        // bKg_y == 2
        { { 0x00600000B0A00000,
            0x00E0000000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x00066600B000A600,
            0x000E000000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x0000600000B0A000,
            0x0000E00000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x0000060000B000A0,
            0x0B000E00B0000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x000000600000BA00,
            0x000B00E000B00A00,
            0x0B0000A0B0000000,
            0x0000000000000000  } },

        // bKg_x == 2
        { { 0x0B00000000000000,
            0x6AE0000000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x0B00000000000000,
            0x0000000060E00000,
            0x6A00000066000000,
            0x0000000000000000 },

          { 0x0000000000000000,
            0x0B00000000000000,
            0x6AE0000000000000,
            0x0000000000000000 },

          { 0x000B000000B00000,
            0x0B00000000000000,
            0x0A00000060E00000,
            0x0000000000000000 },

          { 0x00000B000000B000,
            0x000B000000B00000,
            0x0B0000000A0A0000,
            0x60E0A00000000000 } },

        // bKg_y == 5
        { { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000000E00,
            0x00000A0B00000600 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x000000000000E000,
            0x006A000B00666000 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x00000000000E0000,
            0x000A0B0000060000 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x0000000B00E000B0,
            0x0A000B0000600000 },

          { 0x0000000000000000,
            0x0000000B0A0000B0,
            0x00A00B000E00B000,
            0x00AB000006000000 } },

        // bKg_x == 5
        { { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000000EA6,
            0x00000000000000B0 },

          { 0x0000000000000000,
            0x00000066000000A6,
            0x00000E0600000000,
            0x00000000000000B0 },

          { 0x0000000000000000,
            0x0000000000000EA6,
            0x00000000000000B0,
            0x0000000000000000 },

          { 0x0000000000000000,
            0x00000E06000000A0,
            0x00000000000000B0,
            0x00000B000000B000 },

          { 0x00000000000A0E06,
            0x0000A0A0000000B0,
            0x00000B000000B000,
            0x000B000000B00000 } }
    };


     // 130 series
    cEndGame130::cEndGame130(){
    }


    const uint64_t cEndGame130::m_path_to_mate[4][5][4] = {
        // bKg_y == 2
        { { 0x0000060000000A0B,
            0x00000E0000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x00666000006A000B,
            0x0000E00000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x00060000000A0B00,
            0x000E000000000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x006000000A000B00,
            0x00E000B00000000B,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x0600000000AB0000,
            0x0E00B00000A00B00,
            0x0A0000B00000000B,
            0x0000000000000000 } },

        // bKg_x == 2
        { { 0x000000B000000000,
            0x00000EA600000000,
            0x0000000000000000,
            0x0000000000000000 },

          { 0x000000B000000000,
            0x0000000000000E06,
            0x000000A600000066,
            0x0000000000000000 },

          { 0x0000000000000000,
            0x000000B000000000,
            0x00000EA600000000,
            0x0000000000000000 },

          { 0x0000B00000000B00,
            0x000000B000000000,
            0x000000A000000E06,
            0x0000000000000000 },

          { 0x00B00000000B0000,
            0x0000B00000000B00,
            0x000000B00000A0A0,
            0x000A0E0600000000 } },

        // bKg_y == 5
        { { 0x0000000000000000,
            0x0000000000000000,
            0x0000000000E00000,
            0xB0A0000000600000 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x00000000000E0000,
            0xB000A60000066600 },

          { 0x0000000000000000,
            0x0000000000000000,
            0x000000000000E000,
            0x00B0A00000006000 },

          { 0x0000000000000000,
            0x0000000000000000,
            0xB00000000B000E00,
            0x00B000A000000600 },

          { 0x0000000000000000,
            0xB00000000B0000A0,
            0x00B00A00000B00E0,
            0x0000BA0000000060 } },

        // bKg_x == 5
        { { 0x0000000000000000,
            0x0000000000000000,
            0x000000006AE00000,
            0x000000000B000000 },

          { 0x0000000000000000,
            0x660000006A000000,
            0x60E0000000000000,
            0x000000000B000000 },

          { 0x0000000000000000,
            0x000000006AE00000,
            0x000000000B000000,
            0x0000000000000000 },

          { 0x0000000000000000,
            0x60E000000A000000,
            0x000000000B000000,
            0x00B00000000B0000 },

          { 0x0000000060E0A000,
            0x0A0A00000B000000,
            0x00B00000000B0000,
            0x0000B00000000B00 } }
    };

