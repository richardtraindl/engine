

    #include "./endgame.hpp"


    cEndGame100_base::cEndGame100_base(){
    }


    int32_t cEndGame100_base::m_score[5] = {
        100, 110, 120, 130, 140
    };


    // 100 series
    cEndGame100::cEndGame100(){

        m_100_series[0] = cEndGame100::m_bottom_left_estbnd;

        m_100_series[1] = cEndGame100::m_bottom_left_nthbnd;

        m_100_series[2] = cEndGame100::m_top_right_wstbnd;

        m_100_series[3] = cEndGame100::m_top_right_sthbnd;

        for(uint8_t i = 0; i < 5; ++i){
            
            uint64_t tmpfields[4];

            cBitBoard::flip_horizontal(tmpfields, cEndGame100::m_top_right_wstbnd[i]);
            cBitBoard::flip_vertical(cEndGame100::m_bottom_left_estbnd[i], tmpfields);

            cBitBoard::flip_horizontal(tmpfields, cEndGame100::m_top_right_sthbnd[i]);
            cBitBoard::flip_vertical(cEndGame100::m_bottom_left_nthbnd[i], tmpfields);

        }

    }


    const int32_t cEndGame100::m_single_KG[8][8] = {
        {  0,  4,  8, 12, 18, 24, 30, 30 }, // first row
        {  4,  0,  0,  0, 12, 18, 24, 30 },
        {  8,  0,  0,  0, 12, 18, 18, 24 },
        { 12,  0,  0,  0,  0, 12, 12, 18 },        
        { 18, 12, 12,  0,  0,  0,  0, 12 },        
        { 24, 18, 18, 12,  0,  0,  0,  8 },
        { 30, 24, 18, 12,  0,  0,  0,  4 },
        { 30, 30, 24, 18, 12,  8,  4,  0 }
    };


    array<uint64_t(*)[4], 4> cEndGame100::m_100_series;


    uint64_t cEndGame100::m_bottom_left_estbnd[5][4];


    uint64_t cEndGame100::m_bottom_left_nthbnd[5][4];


    uint64_t cEndGame100::m_top_right_wstbnd[5][4] = {
        { 0x0000000000000000,
          0x0000000000000000,
          0x0000000000000600,
          0x000002030000EE00 },

        { 0x0000000000000000,
          0x0000000000000000,
          0x0000000000E06030,
          0x0EE2000300EEE000 },

        { 0x0000000000003000,
          0x0003000000000000,
          0x0000000000E06000,
          0x0EE2000000EE0000 },

        { 0x0000000000000000,
          0x0000000000000000,
          0x0000000300060030,
          0x02000300000E0000 },

        { 0x0000000000000000,
          0x0000000300000030,
          0x0000030006003000,
          0x020300000E000000 },
      };


    uint64_t cEndGame100::m_top_right_sthbnd[5][4] = {
        { 0x0000000000000000,
          0x0000000000000000,
          0x0000000E0000062E,
          0x0000000000000030 },

        { 0x00000000000000E0,
          0x00000EEE0000002E,
          0x0000060E00000000,
          0x0000030000000030 },

        { 0x00000000000000E0,
          0x00000EEE0030002E,
          0x0300060000000000,
          0x0000000000000000 },

        { 0x0000000000000020,
          0x000000000000060E,
          0x0000000000000030,
          0x0000030000003000 },

        { 0x000000000000062E,
          0x0000000000000030,
          0x0000030000003000,
          0x0003000000300000 },
      };


    // 110 series
    cEndGame110::cEndGame110(){

        m_110_series[0] = cEndGame110::m_bottom_right_wstbnd;

        m_110_series[1] = cEndGame110::m_bottom_right_nthbnd;

        m_110_series[2] = cEndGame110::m_top_left_estbnd;

        m_110_series[3] = cEndGame110::m_top_left_sthbnd;


        for(uint8_t i = 0; i < 5; ++i){
            
            cBitBoard::flip_vertical(cEndGame110::m_top_left_estbnd[i], cEndGame100::m_top_right_wstbnd[i]);

            cBitBoard::flip_horizontal(cEndGame110::m_bottom_right_wstbnd[i], cEndGame100::m_top_right_wstbnd[i]);

            cBitBoard::flip_horizontal(cEndGame110::m_bottom_right_nthbnd[i], cEndGame100::m_top_right_sthbnd[i]);

            cBitBoard::flip_vertical(cEndGame110::m_top_left_sthbnd[i], cEndGame100::m_top_right_sthbnd[i]);

        }

    }


    const int32_t cEndGame110::m_single_KG[8][8] = {
        { 30, 30, 24, 18, 12,  8,  4,  0 }, // first row
        { 30, 24, 18, 12,  0,  0,  0,  4 },
        { 24, 18, 18, 12,  0,  0,  0,  8 },
        { 18, 12, 12,  0,  0,  0,  0, 12 },        
        { 12,  0,  0,  0,  0, 12, 12, 18 },        
        {  8,  0,  0,  0, 12, 18, 18, 24 },
        {  4,  0,  0,  0, 12, 18, 24, 30 },
        {  0,  4,  8, 12, 18, 24, 30, 30 }
    };


    array<uint64_t(*)[4], 4> cEndGame110::m_110_series;


    uint64_t cEndGame110::m_bottom_right_wstbnd[5][4];


    uint64_t cEndGame110::m_bottom_right_nthbnd[5][4];


    uint64_t cEndGame110::m_top_left_estbnd[5][4];


    uint64_t cEndGame110::m_top_left_sthbnd[5][4];


   // 120 series
    cEndGame120::cEndGame120(){

        m_120_series[0] = cEndGame120::m_bottom_left_estbnd;

        m_120_series[1] = cEndGame120::m_bottom_left_nthbnd;

        m_120_series[2] = cEndGame120::m_top_right_wstbnd;

        m_120_series[3] = cEndGame120::m_top_right_sthbnd;


        for(uint8_t i = 0; i < 5; ++i){

            cBitBoard::switch_colors(cEndGame120::m_bottom_left_estbnd[i], cEndGame100::m_bottom_left_estbnd[i]);

            cBitBoard::switch_colors(cEndGame120::m_bottom_left_nthbnd[i], cEndGame100::m_bottom_left_nthbnd[i]);

            cBitBoard::switch_colors(cEndGame120::m_top_right_wstbnd[i], cEndGame100::m_top_right_wstbnd[i]);

            cBitBoard::switch_colors(cEndGame120::m_top_right_sthbnd[i], cEndGame100::m_top_right_sthbnd[i]);

        }

    }


    const int32_t cEndGame120::m_single_KG[8][8] = {
        { 30, 30, 24, 18, 12,  8,  4,  0 }, // first row
        { 30, 24, 18, 12,  0,  0,  0,  4 },
        { 24, 18, 18, 12,  0,  0,  0,  8 },
        { 18, 12, 12,  0,  0,  0,  0, 12 },        
        { 12,  0,  0,  0,  0, 12, 12, 18 },        
        {  8,  0,  0,  0, 12, 18, 18, 24 },
        {  4,  0,  0,  0, 12, 18, 24, 30 },
        {  0,  4,  8, 12, 18, 24, 30, 30 }
    };


    array<uint64_t(*)[4], 4> cEndGame120::m_120_series;


    uint64_t cEndGame120::m_bottom_left_estbnd[5][4];


    uint64_t cEndGame120::m_bottom_left_nthbnd[5][4];


    uint64_t cEndGame120::m_top_right_wstbnd[5][4];


    uint64_t cEndGame120::m_top_right_sthbnd[5][4];


   // 130 series
    cEndGame130::cEndGame130(){

        m_130_series[0] = cEndGame130::m_bottom_right_wstbnd;

        m_130_series[1] = cEndGame130::m_bottom_right_nthbnd;

        m_130_series[2] = cEndGame130::m_top_left_estbnd;

        m_130_series[3] = cEndGame130::m_top_left_sthbnd;


        for(uint8_t i = 0; i < 5; ++i){
            
            cBitBoard::switch_colors(cEndGame130::m_bottom_right_wstbnd[i], cEndGame110::m_bottom_right_wstbnd[i]);

            cBitBoard::switch_colors(cEndGame130::m_bottom_right_nthbnd[i], cEndGame110::m_bottom_right_nthbnd[i]);

            cBitBoard::switch_colors(cEndGame130::m_top_left_estbnd[i], cEndGame110::m_top_left_estbnd[i]);

            cBitBoard::switch_colors(cEndGame130::m_top_left_sthbnd[i], cEndGame110::m_top_left_sthbnd[i]);

        }

    }


    const int32_t cEndGame130::m_single_KG[8][8] = {
        { 30, 30, 24, 18, 12,  8,  4,  0 }, // first row
        { 30, 24, 18, 12,  0,  0,  0,  4 },
        { 24, 18, 18, 12,  0,  0,  0,  8 },
        { 18, 12, 12,  0,  0,  0,  0, 12 },        
        { 12,  0,  0,  0,  0, 12, 12, 18 },        
        {  8,  0,  0,  0, 12, 18, 18, 24 },
        {  4,  0,  0,  0, 12, 18, 24, 30 },
        {  0,  4,  8, 12, 18, 24, 30, 30 }
    };


    array<uint64_t(*)[4], 4> cEndGame130::m_130_series;


    uint64_t cEndGame130::m_bottom_right_wstbnd[5][4];


    uint64_t cEndGame130::m_bottom_right_nthbnd[5][4];


    uint64_t cEndGame130::m_top_left_estbnd[5][4];


    uint64_t cEndGame130::m_top_left_sthbnd[5][4];

