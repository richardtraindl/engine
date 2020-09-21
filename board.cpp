
    #include "./board.hpp"


    cStep::cStep(uint8_t newowner, 
                 uint8_t newdir, 
                 bool newrightshift, 
                 uint8_t newshiftcnt, 
                 uint8_t newstepcnt, 
                 uint64_t newborder){ 
        owner = newowner;
        dir = newdir;
        rightshift = newrightshift;
        shiftcnt = newshiftcnt;
        stepcnt = newstepcnt;
        border = newborder;
    }


    cStep::cStep(){
    }


    cLink::cLink(uint64_t newposA, 
                 uint8_t newpieceA, 
                 uint64_t newposB, 
                 uint8_t newpieceB,
                 uint8_t newdirAB){
        posA = newposA;
        pieceA = newpieceA;
        posB = newposB;
        pieceB = newpieceB;
        dirAB = newdirAB;
    }


    cLink::cLink(){
    }


    const cStep cLink::step_for_dir(uint8_t dir){
        switch(dir){
            case mEST: return cBoard::rk_steps[0];
            
            case mWST: return cBoard::rk_steps[1];
            
            case mNTH: return cBoard::rk_steps[2];
            
            case mSTH: return cBoard::rk_steps[3];
            
            case mNTH_EST: return cBoard::bp_steps[0];
            
            case mSTH_WST: return cBoard::bp_steps[1];

            case mNTH_WST: return cBoard::bp_steps[2];
            
            case mSTH_EST: return cBoard::bp_steps[3];
            
            case mNTH2_EST1: return cBoard::kn_steps[0];

            case mSTH2_WST1: return cBoard::kn_steps[1];

            case mNTH1_EST2: return cBoard::kn_steps[2];

            case mSTH1_WST2: return cBoard::kn_steps[3];

            case mNTH2_WST1: return cBoard::kn_steps[4];

            case mSTH2_EST1: return cBoard::kn_steps[5];

            case mNTH1_WST2: return cBoard::kn_steps[6];

            case mSTH1_EST2: return cBoard::kn_steps[7];
        }
        return cStep();
    }


    uint8_t cLink::reverse_dir(uint8_t dir){
        switch(dir){
            case mEST: return mWST;
            
            case mWST: return mEST;
            
            case mNTH: return mSTH;
            
            case mSTH: return mNTH;
            
            case mNTH_EST: return mSTH_WST;
            
            case mSTH_WST: return mNTH_EST;

            case mNTH_WST: return mSTH_EST;
            
            case mSTH_EST: return mNTH_WST;

            case mNTH2_EST1: return mSTH2_WST1;

            case mSTH2_WST1: return mNTH2_EST1;

            case mNTH1_EST2: return mSTH1_WST2;

            case mSTH1_WST2: return mNTH1_EST2;

            case mNTH2_WST1: return mSTH2_EST1;

            case mSTH2_EST1: return mNTH2_WST1;

            case mNTH1_WST2: return mSTH1_EST2;

            case mSTH1_EST2: return mNTH1_WST2;
        }
        return mUNDEF;
    }


    cPin::cPin(){
    }


    bool cPin::is_pinned(uint64_t pos, uint8_t dir){
        if((pins[mIDX_PIN] & pos) == 0){
            return false;
        }

        if((pins[mIDX_WST_EST] & pos) > 0 &&
           (dir == mEST || dir == mWST)){
            return true;
        }

        if((pins[mIDX_STH_NTH] & pos) > 0 &&
           (dir == mNTH || dir == mSTH)){
            return true;
        }

        if((pins[mIDX_STH_WST_NTH_EST] & pos) > 0 &&
           (dir == mNTH_EST || dir == mSTH_WST)){
            return true;
        }

        if((pins[mIDX_STH_EST_NTH_WST] & pos) > 0 &&
           (dir == mNTH_WST || dir == mSTH_EST)){
            return true;
        }

        return false;
    }


    cBoard::cBoard(){ 
    }


    cBoard::cBoard(const cBoard &board){
    } // copy constructor


    const cStep cBoard::rk_steps[] = { 
        cStep(STEP_OWNER["rook"], mEST, true, 1, 7, mEST_BORDER),
        cStep(STEP_OWNER["rook"], mWST, false, 1, 7, mWST_BORDER),
        cStep(STEP_OWNER["rook"], mNTH, true, 8, 7, mNTH_BORDER),
        cStep(STEP_OWNER["rook"], mSTH, false, 8, 7, mSTH_BORDER) 
    };


    const cStep cBoard::bp_steps[] = { 
        cStep(STEP_OWNER["bishop"], mNTH_EST, true, 9, 7, mNTH_EST_BORDER),
        cStep(STEP_OWNER["bishop"], mSTH_WST, false, 9, 7, mSTH_WST_BORDER),
        cStep(STEP_OWNER["bishop"], mNTH_WST, true, 7, 7, mNTH_WST_BORDER),
        cStep(STEP_OWNER["bishop"], mSTH_EST, false, 7, 7, mSTH_EST_BORDER)
    };


    const cStep cBoard::qu_steps[] = { 
        rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3], 
        bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3] 
    };


    const cStep cBoard::kg_steps_generic[] = { 
        cStep(STEP_OWNER["king"], mEST, true, 1, 1, mEST_BORDER),
        cStep(STEP_OWNER["king"], mWST, false, 1, 1, mWST_BORDER),
        cStep(STEP_OWNER["king"], mNTH, true, 8, 1, mNTH_BORDER),
        cStep(STEP_OWNER["king"], mSTH, false, 8, 1, mSTH_BORDER), 
        cStep(STEP_OWNER["king"], mNTH_EST, true, 9, 1, mNTH_EST_BORDER),
        cStep(STEP_OWNER["king"], mSTH_WST, false, 9, 1, mSTH_WST_BORDER),
        cStep(STEP_OWNER["king"], mNTH_WST, true, 7, 1, mNTH_WST_BORDER),
        cStep(STEP_OWNER["king"], mSTH_EST, false, 7, 1, mSTH_EST_BORDER) 
    };


    const cStep cBoard::wkg_steps_and_castl[] = { 
        kg_steps_generic[0], kg_steps_generic[1], 
        kg_steps_generic[2], kg_steps_generic[3], 
        kg_steps_generic[4], kg_steps_generic[5], 
        kg_steps_generic[6], kg_steps_generic[7],
        cStep(STEP_OWNER["king"], mEST2, true, 2, 1, mWHITE_CASTL_BORDER),
        cStep(STEP_OWNER["king"], mWST2, false, 2, 1, mWHITE_CASTL_BORDER) 
    };

    const cStep cBoard::bkg_steps_and_castl[] = { 
        kg_steps_generic[0], kg_steps_generic[1], 
        kg_steps_generic[2], kg_steps_generic[3], 
        kg_steps_generic[4], kg_steps_generic[5], 
        kg_steps_generic[6], kg_steps_generic[7],
        cStep(STEP_OWNER["king"], mEST2, true, 2, 1, mBLACK_CASTL_BORDER),
        cStep(STEP_OWNER["king"], mWST2, false, 2, 1, mBLACK_CASTL_BORDER) 
    };


    const cStep cBoard::kn_steps[] = { 
        cStep(STEP_OWNER["knight"], mNTH2_EST1, true, 17, 1, mNTH2_EST1_BORDER),
        cStep(STEP_OWNER["knight"], mSTH2_WST1, false, 17, 1, mSTH2_WST1_BORDER),
        cStep(STEP_OWNER["knight"], mNTH1_EST2, true, 10, 1, mNTH1_EST2_BORDER), 
        cStep(STEP_OWNER["knight"], mSTH1_WST2, false, 10, 1, mSTH1_WST2_BORDER),
        cStep(STEP_OWNER["knight"], mNTH2_WST1, true, 15, 1, mNTH2_WST1_BORDER),
        cStep(STEP_OWNER["knight"], mSTH2_EST1, false, 15, 1, mSTH2_EST1_BORDER),
        cStep(STEP_OWNER["knight"], mNTH1_WST2, true, 6, 1, mNTH1_WST2_BORDER),
        cStep(STEP_OWNER["knight"], mSTH1_EST2, false, 6, 1, mSTH1_EST2_BORDER)
    };


    const cStep cBoard::wpw_steps_attack[] = { 
        cStep(STEP_OWNER["wpawn"], mNTH_EST, true, 9, 1, mNTH_EST_BORDER),
        cStep(STEP_OWNER["wpawn"], mNTH_WST, true, 7, 1, mNTH_WST_BORDER) 
    };


    const cStep cBoard::wpw_steps_attack_search[] = { 
        cStep(STEP_OWNER["wpawn"], mSTH_WST, false, 9, 1, mSTH_WST_BORDER),
        cStep(STEP_OWNER["wpawn"], mSTH_EST, false, 7, 1, mSTH_EST_BORDER) 
    };


    const cStep cBoard::wpw_steps_support_search[] = { 
        wpw_steps_attack_search[0],
        wpw_steps_attack_search[1],
        cStep(STEP_OWNER["wpawn"], mSTH, false, 8, 2, mSTH_BORDER) 
    };


    const cStep cBoard::wpw_steps[] = { 
        cStep(wpw_steps_attack[0]),
        cStep(wpw_steps_attack[1]),
        cStep(STEP_OWNER["wpawn"], mNTH, true, 8, 2, mNTH_BORDER) 
    };


    const cStep cBoard::bpw_steps_attack[] = {
        cStep(STEP_OWNER["bpawn"], mSTH_WST, false, 9, 1, mSTH_WST_BORDER),
        cStep(STEP_OWNER["bpawn"], mSTH_EST, false, 7, 1, mSTH_EST_BORDER) 
    };


    const cStep cBoard::bpw_steps_attack_search[] = {
        cStep(STEP_OWNER["bpawn"], mNTH_EST, true, 9, 1, mNTH_EST_BORDER),
        cStep(STEP_OWNER["bpawn"], mNTH_WST, true, 7, 1, mNTH_WST_BORDER) 
    };


    const cStep cBoard::bpw_steps_support_search[] = { 
        bpw_steps_attack_search[0],
        bpw_steps_attack_search[1],
        cStep(STEP_OWNER["bpawn"], mNTH, true, 8, 2, mNTH_BORDER) 
    };


    const cStep cBoard::bpw_steps[] = { 
        cStep(bpw_steps_attack[0]),
        cStep(bpw_steps_attack[1]),
        cStep(STEP_OWNER["bpawn"], mSTH, false, 8, 2, mSTH_BORDER) 
    };


    const cStep cBoard::steps_for_pin_search[] = { 
        rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
        bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3]
    };


    const cStep cBoard::steps_for_white_enemies_search[] = { 
        rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
        bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
        kg_steps_generic[0], kg_steps_generic[1],
        kg_steps_generic[2], kg_steps_generic[3],
        kg_steps_generic[4], kg_steps_generic[5],
        kg_steps_generic[6], kg_steps_generic[7],
        kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
        kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
        wpw_steps_attack_search[0], wpw_steps_attack_search[1]
    };


    const cStep cBoard::steps_for_black_enemies_search[] = { 
        rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
        bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
        kg_steps_generic[0], kg_steps_generic[1],
        kg_steps_generic[2], kg_steps_generic[3],
        kg_steps_generic[4], kg_steps_generic[5],
        kg_steps_generic[6], kg_steps_generic[7],
        kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
        kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
        bpw_steps_attack_search[0], bpw_steps_attack_search[1]
    };


    const cStep cBoard::steps_for_white_checks_search[] = { 
        rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
        bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
        kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
        kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
        wpw_steps_attack_search[0], wpw_steps_attack_search[1]
    };


    const cStep cBoard::steps_for_black_checks_search[] = { 
        rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
        bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
        kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
        kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
        bpw_steps_attack_search[0], bpw_steps_attack_search[1]
    };


    const cStep cBoard::steps_for_wkg_support_search[] = { 
        rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
        bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
        kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
        kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
        wpw_steps_support_search[0], wpw_steps_support_search[1], 
        wpw_steps_support_search[2]
    };

    const cStep cBoard::steps_for_bkg_support_search[] = { 
        rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
        bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
        kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
        kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
        bpw_steps_support_search[0], bpw_steps_support_search[1], 
        bpw_steps_support_search[2]
    };


    uint8_t cBoard::read(uint64_t pos){
        uint8_t piece = 0;
        uint8_t test = 0b10000000;
        for(uint8_t i = 0; i < 8; ++i){
            if((field[i] & pos) > 0){
                piece = (piece | test);
            }
            test = (test >> 1);
        }
        return piece;
    }


    void cBoard::write(uint64_t pos, uint8_t piece){
        uint8_t test = 0b10000000;
        uint64_t negmask = (0xFFFFFFFFFFFFFFFF ^ pos);
        for(uint8_t i = 0; i < 8; ++i){
            field[i] = (field[i] & negmask);
            if((piece & test) > 0){
                field[i] = (field[i] | pos);
            }
            test = (test >> 1);
        }
    }


    uint64_t cBoard::read_wkg_pos(){
        return (field[mIDX_WHITE] & field[mIDX_KING]);
    }


    uint64_t cBoard::read_bkg_pos(){
        return (field[mIDX_BLACK] & field[mIDX_KING]);
    }


    bool cBoard::is_square_blank(uint64_t pos){
        return (pos & field[mIDX_WHITE]) == 0 && (pos & field[mIDX_BLACK]) == 0;
    }


    bool cBoard::is_square_white_occupied(uint64_t pos){
        return (pos & field[mIDX_WHITE]) > 0;
    }


    bool cBoard::is_square_black_occupied(uint64_t pos){
        return (pos & field[mIDX_BLACK]) > 0;
    }


    bool cBoard::is_piece_blank(uint8_t piece){
        return (piece & mWHITE) == 0 && (piece & mBLACK) == 0;
    }


    bool cBoard::is_piece_white(uint8_t piece){
        return (piece & mWHITE) > 0;
    }


    bool cBoard::is_piece_black(uint8_t piece){
        return (piece & mBLACK) > 0;
    }


    bool cBoard::verify(){
        int wKg_cnt = 0;
        int bKg_cnt = 0;
        int wPw_cnt = 0;
        int bPw_cnt = 0;
        int wOfficer_cnt = 0;
        int bOfficer_cnt = 0;
        uint64_t pos = mPOS_A1; 

        while(pos > 0){
            uint8_t piece = read(pos);
            if(piece == mWKG){
                wKg_cnt += 1; 
                continue;
            }
            if(piece == mBKG){
                bKg_cnt += 1;
                continue;
            }
            if(piece == mWPW){
                wPw_cnt += 1;
                continue;
            }
            if(piece == mBPW){
                bPw_cnt += 1;
                continue; 
            }
            if(piece == mWRK || piece == mWBP || piece == mWKN || piece == mWQU){
                wOfficer_cnt += 1;
                continue;
            }
            if(piece == mBRK || piece == mBBP || piece == mBKN || piece == mBQU){
                bOfficer_cnt += 1;
                continue;
            }
            pos = (pos >> 1);
        }

        if(wKg_cnt != 1 || bKg_cnt != 1){
            return false;
        }
        if(wPw_cnt > 8 || bPw_cnt > 8){
            return false;
        }
        if(wPw_cnt + wOfficer_cnt > 15){
            return false;
        }
        if(bPw_cnt + bOfficer_cnt > 15){
            return false;
        }
        if((field[mIDX_WHITE] & field[mIDX_KING]) == 0 || 
            (field[mIDX_BLACK] & field[mIDX_KING]) == 0 ){
            return false;
        }
        
        uint64_t wkg_pos = read_wkg_pos();
        uint64_t bkg_pos = read_bkg_pos();
        for(cStep step : kg_steps_generic){
            if((wkg_pos & step.border) > 0){
                continue;
            }
            if(step.rightshift){
                if((wkg_pos >> step.shiftcnt) == bkg_pos){
                    return false;
                }
                else{
                    if((wkg_pos << step.shiftcnt) == bkg_pos){
                        return false;
                    }
                }
            }
        }

        return true;
    }


    void cBoard::eval_count_of_officers(int &wofficers, int &bofficers){
        wofficers = 0;
        bofficers = 0;
        for(uint64_t pos = mPOS_A1; pos != 0; pos = (pos >> 1)){
            int piece = read(pos);
            if(piece == mWRK || piece == mWBP || piece == mWKN || piece == mWQU){
                wofficers += 1;
                continue;
            }
            if(piece == mBRK || piece == mBBP || piece == mBKN || piece == mBQU){
                bofficers += 1;
                continue;
            }
        }
    }


    bool cBoard::compare(cBoard &newboard){
        for(int i = 0; i < 8; ++i){
            if(field[i] != newboard.field[i]){
                return false;
            }
        }
        return true;
    }


    void cBoard::gen_moves(list<cMove> &minutes, list<cGMove> &moves){
        uint64_t pos = mPOS_A1;
        uint8_t color;
        const cStep *steps;

        if((minutes.size() % 2) == 0){
            color = mWHITE;
        }
        else{
            color = mBLACK;
        }
        
        cPin *cpin = determine_pins(color);

        list <cLink *> attackers;
        determine_checks(color, attackers);
        cout << "determine_checks: " << endl;
        for(cLink *attacker : attackers){
            cout << "1" << endl;
            prnt_16hex(attacker->posA);
            cout << "2" << endl;
            prnt_16hex(attacker->posB);
            cout << "3" << endl;
            prnt_16hex(attacker->dirAB);
        }

        cout << "att size " << attackers.size() << endl;

        if(attackers.size() > 1){
            gen_kg_moves(color, moves);
            return;
        }
        else if(attackers.size() == 1){
            gen_kg_moves(color, moves);
            gen_kg_support_moves(attackers, moves);
            return;
        }

        while(pos > 0){
            uint8_t piece = read(pos);
            int maxidx;
            
            if((piece & color) > 0){
                if(piece == mWRK || piece == mBRK){ steps = rk_steps; maxidx = 4; }
                else if(piece == mWBP || piece == mBBP){ steps = bp_steps; maxidx = 4; }
                else if(piece == mWQU || piece == mBQU){ steps = qu_steps; maxidx = 8; }
                else if(piece == mWKG){ steps = wkg_steps_and_castl; maxidx = 10; }
                else if(piece == mBKG){ steps = bkg_steps_and_castl; maxidx = 10; }
                else if(piece == mWKN || piece == mBKN){ steps = kn_steps; maxidx = 8; }
                else if(piece == mWPW){ steps = wpw_steps; maxidx = 3; }
                else if(piece == mBPW){ steps = bpw_steps; maxidx = 3; }

                cout << "-----" << endl;
                cout << PIECES_STR[piece] << endl;

                for(int i = 0; i < maxidx; ++i){
                    cStep step = steps[i];
                    uint64_t newpos = pos;

                    for(int k = 0; k < step.stepcnt; ++k){
                        if(cpin->is_pinned(pos, step.dir)){
                            break;
                        }

                        if((newpos & step.border) > 0){
                            break;
                        }

                        if(step.rightshift){
                            newpos = (newpos >> step.shiftcnt);
                        }
                        else{
                            newpos = (newpos << step.shiftcnt);
                        }

                        if(is_piece_white(piece) && (field[mIDX_WHITE] & newpos) > 0){
                            break;
                        }
                        else if(is_piece_black(piece) && (field[mIDX_BLACK] & newpos) > 0){
                            break;
                        }
                        else{
                            if(piece == mWPW){
                                if(tst_wpw_move(pos, newpos) ||
                                   tst_en_passant(pos, newpos, minutes)){
                                    gen_pw_moves(pos, newpos, moves);
                                }
                            }
                            else if(piece == mBPW){
                                if(tst_bpw_move(pos, newpos) ||
                                   tst_en_passant(pos, newpos, minutes)){
                                    gen_pw_moves(pos, newpos, moves);
                                }
                            }
                            else if(piece == mWKG || piece == mBKG){
                                if(tst_kg_move(pos, newpos, minutes)){
                                    moves.push_back(cGMove(pos, newpos, mBLK));
                                    cout << pos_to_coord(pos) << "-" << pos_to_coord(newpos) << endl;
                                }
                            }
                            else{
                                moves.push_back(cGMove(pos, newpos, mBLK));
                                cout << pos_to_coord(pos) << "-" << pos_to_coord(newpos) << endl;
                            }
                            if((field[mIDX_WHITE] & newpos) > 0 || 
                               (field[mIDX_BLACK] & newpos) > 0){
                                break;
                            }
                        }
                    }
                }
            }

            pos = (pos >> 1);
        }
    }


    void cBoard::prnt(){
        string textcolor, backcolor, strpiece;
        uint64_t startpos = 0x0000000000000080;
        for(int y = 7; y >=0; --y){
            uint64_t pos = startpos;
            for(int x = 0; x < 8; ++x){
                uint8_t piece = read(pos);
                if(piece == mBLK){
                    strpiece = "   ";
                }
                else{
                    strpiece = reverse_lookup(PIECES, piece);
                }
                if(is_piece_white(piece)){
                    textcolor = mWHITE_TEXT + mBOLD_ON;
                }
                else{
                    textcolor = mBLACK_TEXT + mBOLD_ON;
                }
                if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
                    backcolor = mBLUE_BACK;
                }
                else{
                    backcolor = mGREEN_BACK;
                }
                // "\e#3" \e#4"
                cout << backcolor + textcolor + strpiece + mRESET_ALL;

                pos = (pos >> 1);
            }
            cout << endl;

            startpos = (startpos << 8);
        }
    }


    void cBoard::prnt_16hex(uint64_t pos){
        cout << "0x" << hex << setfill('0') << setw(16);
        cout << pos << endl;
    }


    bool cBoard::is_square_enemy_touched(uint64_t pos){
        const cStep *steps;
        uint8_t piece = read(pos);
        uint8_t enemy;
        uint8_t enemycolor;        
        if(is_piece_white(piece)){
            enemycolor = mBLACK;
            steps = steps_for_black_enemies_search;
        }
        else{
            enemycolor = mWHITE;
            steps = steps_for_white_enemies_search;
        }

        for(uint8_t i = 0; i < 26; ++i){
            cStep step = steps[i];

            uint64_t newpos = pos;

            for(int k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }

                if(is_square_blank(newpos)){
                    continue;
                }

                enemy = read(newpos);
                if((field[mIDX_WHITE] & newpos) > 0 && enemycolor == mWHITE){
                    if((step.owner == STEP_OWNER["rook"] && (enemy == mWRK || enemy == mWQU)) ||
                       (step.owner == STEP_OWNER["bishop"] && (enemy == mWBP || enemy == mWQU)) ||
                       (step.owner == STEP_OWNER["king"] && enemy == mWKG) ||
                       (step.owner == STEP_OWNER["knight"] && enemy == mWKN) ||
                       (step.owner == STEP_OWNER["wpawn"] && enemy == mWPW)){
                        return true;
                    }
                    else{
                        break;
                    }
                }
                else if((field[mIDX_BLACK] & newpos) > 0 && enemycolor == mBLACK){
                    if((step.owner == STEP_OWNER["rook"] && (enemy == mBRK || enemy == mBQU)) ||
                       (step.owner == STEP_OWNER["bishop"] && (enemy == mBBP || enemy == mBQU)) ||
                       (step.owner == STEP_OWNER["king"] && enemy == mBKG) ||
                       (step.owner == STEP_OWNER["knight"] && enemy == mBKN) ||
                       (step.owner == STEP_OWNER["bpawn"] && enemy == mBPW)){
                        return true;
                    }
                    else{
                        break;
                    }
                }
                else{
                    break;
                }
            }
        }
        return false;
    }


    bool cBoard::tst_en_passant(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        if(minutes.size() > 0){
            cMove move = minutes.back();

            if(is_square_white_occupied(pos)){
                uint64_t opp_pawn_src = (newpos >> 8);
                uint64_t opp_pawn_dst = (newpos << 8);
                return (move.src == opp_pawn_src && 
                        move.dst == opp_pawn_dst &&
                        read(opp_pawn_dst) == mBPW);
            }
            if(is_square_black_occupied(pos)){
                uint64_t opp_pawn_src = (newpos << 8);
                uint64_t opp_pawn_dst = (newpos >> 8);
                return (move.src == opp_pawn_src && 
                        move.dst == opp_pawn_dst &&
                        read(opp_pawn_dst) == mWPW);
            }
        }
        return false;
    }


    bool cBoard::tst_wpw_move(uint64_t pos, uint64_t newpos){
        // check, if field after one step forward is blank
        if((pos >> 8) == newpos){
            return (field[mIDX_WHITE] & newpos) == 0 && (field[mIDX_BLACK] & newpos) == 0;
        }
        // check, if fields after two step forward are blank
        else if((pos >> 16) == newpos){
            return (field[mIDX_WHITE] & newpos) == 0 && (field[mIDX_BLACK] & newpos) == 0 && 
                   (field[mIDX_WHITE] & (pos >> 8)) == 0 && (field[mIDX_BLACK] & (pos >> 8)) == 0;
        }
        else if((pos >> 9) == newpos || (pos >> 7) == newpos){
            return (field[mIDX_BLACK] & newpos) > 0;
        }
        else{
            return false;
        }
    }


    bool cBoard::tst_bpw_move(uint64_t pos, uint64_t newpos){
        // check, if field after one step forward is blank
        if((pos << 8) == newpos){
            return (field[mIDX_WHITE] & newpos) == 0 && (field[mIDX_BLACK] & newpos) == 0;
        }
        // check, if fields after two step forward are blank
        else if((pos << 16) == newpos){
            return (field[mIDX_WHITE] & newpos) == 0 && (field[mIDX_BLACK] & newpos) == 0 && 
                   (field[mIDX_WHITE] & (pos << 8)) == 0 && (field[mIDX_BLACK] & (pos << 8)) == 0;
        }
        else if((pos << 9) == newpos || (pos << 7) == newpos){
            return (field[mIDX_WHITE] & newpos) > 0;
        }
        else{
            return false;
        }
    }


    bool cBoard::tst_castling(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        uint64_t mask;
        uint8_t color_idx;
        
        if(is_square_white_occupied(pos)){
            color_idx = mIDX_WHITE;
            if((pos >> 2) == newpos){
                mask = 0x0900000000000000;
            }
            else{
                mask = 0x8800000000000000;
            }
        }
        else{
            color_idx = mIDX_BLACK;
            if((pos >> 2) == newpos){
                mask = 0x0000000000000009;
            }
            else{
                mask = 0x0000000000000088;
            }
        }

        for(list<cMove>::reverse_iterator it = minutes.rbegin(); it != minutes.rend(); ++it){
            if((it->prev_field[color_idx] & mask) != mask){
                return false;
            }
        }
        return true;
    }


    bool cBoard::tst_kg_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes){
        uint8_t idx_color;

        if(is_square_white_occupied(pos)){
            idx_color = mIDX_WHITE;
        }
        else{
            idx_color = mIDX_BLACK;
        }

        if((pos >> 2) == newpos){
            if(is_square_blank((pos >> 1)) && is_square_blank((pos >> 2)) &&
               (field[idx_color] & (pos >> 3)) > 0 && (field[mIDX_ROOK] & (pos >> 3)) > 0){
                for(int i = 0; i < 3; ++i){
                    if(is_square_enemy_touched((pos >> i))){
                        return false;
                    }
                }
                return tst_castling(pos, newpos, minutes);
            }
            else{
                return false;
            }
        }
        if((pos << 2) == newpos){
            if(is_square_blank((pos << 1)) && is_square_blank((pos << 2)) &&
               is_square_blank((pos << 3)) && (field[idx_color] & (pos << 4)) > 0 &&
               (field[mIDX_ROOK] & (pos << 4)) > 0){
                for(int i = 0; i < 3; ++i){
                    if(is_square_enemy_touched((pos << i))){
                        return false;
                    }
                }
                return tst_castling(pos, newpos, minutes);
            }
            else{
                return false;
            }
        }
        else{
            return (is_square_enemy_touched(newpos) == false);
        }
    }


    cPin *cBoard::determine_pins(uint8_t color){
        uint64_t kg_pos;
        cPin *cpin = new cPin();

        if(color == mWHITE){
            kg_pos = read_wkg_pos();
        }
        else{
            kg_pos = read_bkg_pos();
        }

        for(uint8_t i = 0; i < 8; ++i){
            const cStep step = steps_for_pin_search[i];

            uint64_t newpos = kg_pos;
            uint64_t friend_pos = 0;

            for(int k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }

                if(is_square_blank(newpos)){
                    continue;
                }

                if((is_square_white_occupied(newpos) && color == mWHITE) ||
                   (is_square_black_occupied(newpos) && color == mBLACK)){
                    if(friend_pos > 0){
                        break;
                    }
                    else{
                        friend_pos = newpos;
                        continue;
                    }
                }

                // enemy found because of checks above
                uint8_t enemy = read(newpos);

                if(step.owner == STEP_OWNER["rook"] &&
                   (enemy == mWRK || enemy == mBRK || enemy == mWQU || enemy == mBQU) && 
                   friend_pos > 0){
                    cpin->pins[mIDX_PIN] = (cpin->pins[mIDX_PIN] | friend_pos);

                    if(step.shiftcnt == 1){
                        cpin->pins[mIDX_WST_EST] = (cpin->pins[mIDX_WST_EST] | friend_pos);
                    }
                    else{
                       cpin->pins[mIDX_STH_NTH] = (cpin->pins[mIDX_STH_NTH] | friend_pos);
                    }
                }
                else if(step.owner == STEP_OWNER["bishop"] &&
                        (enemy == mWBP || enemy == mBBP || enemy == mWQU || enemy == mBQU) && 
                        friend_pos > 0){
                    cpin->pins[mIDX_PIN] = (cpin->pins[mIDX_PIN] | friend_pos);

                    if(step.shiftcnt == 9){
                        cpin->pins[mIDX_STH_WST_NTH_EST] = (cpin->pins[mIDX_STH_WST_NTH_EST] | friend_pos);
                    }
                    else{
                       cpin->pins[mIDX_STH_EST_NTH_WST] = (cpin->pins[mIDX_STH_EST_NTH_WST] | friend_pos);
                    }
                }
                break;
            }
        }
        return cpin;
    }


    void cBoard::determine_checks(uint8_t color, list<cLink *> &attackers){
        const cStep *steps;
        uint64_t kg_pos;
        uint8_t king, enemycolor;

        if(color == mWHITE){
            kg_pos = read_wkg_pos();
            enemycolor = mBLACK;
            steps = steps_for_black_checks_search;
        }
        else{
            kg_pos = read_bkg_pos();
            enemycolor = mWHITE;
            steps = steps_for_white_checks_search;
        }
        king = read(kg_pos);

        for(uint8_t i = 0; i < 18; ++i){
            cStep step = steps[i];

            uint64_t newpos = kg_pos;

            for(int k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                uint64_t prevpos = newpos;

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }

                if(is_square_blank(newpos)){
                    continue;
                }

                uint8_t enemy = read(newpos);
                if(is_piece_white(enemy) && enemycolor == mWHITE){
                    if(step.owner == STEP_OWNER["wpawn"] && enemy == mWPW){
                        if(tst_wpw_move(newpos, prevpos)){
                            attackers.push_back(new cLink(kg_pos, king, newpos, enemy, step.dir));
                        }
                        break;
                    }
                    if((step.owner == STEP_OWNER["rook"] && (enemy == mWRK || enemy == mWQU)) ||
                       (step.owner == STEP_OWNER["bishop"] && (enemy == mWBP || enemy == mWQU)) ||
                       (step.owner == STEP_OWNER["knight"] && enemy == mWKN)){
                        attackers.push_back(new cLink(kg_pos, king, newpos, enemy, step.dir));
                        break;
                    }
                    break;
                }
                else if(is_piece_black(enemy) && enemycolor == mBLACK){
                    if(step.owner == STEP_OWNER["bpawn"] && enemy == mBPW){
                        if(tst_bpw_move(newpos, prevpos)){
                            attackers.push_back(new cLink(kg_pos, king, newpos, enemy, step.dir));
                        }
                        break;
                    }
                    if((step.owner == STEP_OWNER["rook"] && (enemy == mBRK || enemy == mBQU)) ||
                       (step.owner == STEP_OWNER["bishop"] && (enemy == mBBP || enemy == mBQU)) ||
                       (step.owner == STEP_OWNER["knight"] && enemy == mBKN)){
                        attackers.push_back(new cLink(kg_pos, king, newpos, enemy, step.dir));
                        break;
                    }
                    break;
                }
                else{
                    break;
                }
            }
        }
    }


    void cBoard::gen_kg_moves(uint8_t color, list <cGMove> &moves){
        uint64_t kg_pos;
        uint8_t king;
        bool tstsquare;

        if(color == mWHITE){
            kg_pos = read_wkg_pos();
        }
        else{
            kg_pos = read_bkg_pos();
        }
        king = read(kg_pos);

        for(int i = 0; i < 8; ++i){
            uint64_t newpos = kg_pos;
            cStep step = kg_steps_generic[i];

            for(int k = 0; k < step.stepcnt; ++k){
                if((newpos & step.border) > 0){
                    break;
                }

                if(step.rightshift){
                    newpos = (newpos >> step.shiftcnt);
                }
                else{
                    newpos = (newpos << step.shiftcnt);
                }

                if(is_piece_white(king) && (field[mIDX_WHITE] & newpos) > 0){
                    break;
                }
                else if(is_piece_black(king) && (field[mIDX_BLACK] & newpos) > 0){
                    break;
                }
                else{
                    write(kg_pos, mBLK);
                    tstsquare = is_square_enemy_touched(newpos);
                    write(kg_pos, king);

                    if(tstsquare){
                        moves.push_back(cGMove(kg_pos, newpos, mBLK));
                    }
                }
            }
        }
    }


    void cBoard::gen_kg_support_moves(list <cLink *> &attackers, list <cGMove> &moves){
        if(attackers.size() != 1){
            return;
        }

        uint64_t kg_pos, enemy_pos;
        uint8_t king, color;

        cLink *clink = attackers.front();
        kg_pos = clink->posA;
        king = clink->pieceA;
        enemy_pos = clink->posB;
        const cStep *steps;

        if(is_piece_white(king)){
            color = mWHITE;
            steps = steps_for_wkg_support_search;
        }
        else{
            color = mBLACK;
            steps = steps_for_bkg_support_search;
        }

        cPin *cpin = determine_pins(color);

        uint64_t pos = enemy_pos;
        cStep outerstep;
        if(clink->pieceA == mWKG || clink->pieceA == mBKG){
            uint8_t reversed_dir = clink->reverse_dir(clink->dirAB);
            outerstep = clink->step_for_dir(reversed_dir);
        }
        else{
            outerstep = clink->step_for_dir(clink->dirAB);
        }

        while(pos != kg_pos){
            for(uint8_t i = 0; i < 19; ++i){
                cStep step = steps[i];

                uint64_t newpos = pos;

                for(int k = 0; k < step.stepcnt; ++k){
                    if((newpos & step.border) > 0){
                        break;
                    }

                    if(step.rightshift){
                        newpos = (newpos >> step.shiftcnt);
                    }
                    else{
                        newpos = (newpos << step.shiftcnt);
                    }

                    if(is_square_blank(newpos)){
                        continue;
                    }

                    uint8_t piece = read(newpos);

                    if((field[mIDX_WHITE] & newpos) > 0 && color == mWHITE){
                        if(cpin->is_pinned(pos, step.dir)){
                            break;
                        }
                        if(step.owner == STEP_OWNER["wpawn"] && piece == mWPW){
                            if(tst_wpw_move(newpos, pos)){
                                add_pw_moves(newpos, pos, moves);
                            }
                            break;
                        }
                        if((step.owner == STEP_OWNER["rook"] && (piece == mWRK || piece == mWQU)) ||
                           (step.owner == STEP_OWNER["bishop"] && (piece == mWBP || piece == mWQU)) ||
                           (step.owner == STEP_OWNER["knight"] && piece == mWKN)){
                            moves.push_back(cGMove(newpos, pos, mBLK));
                            break;
                        }
                    }
                    else if((field[mIDX_BLACK] & newpos) > 0 && color == mBLACK){
                        if(cpin->is_pinned(pos, step.dir)){
                            break;
                        }
                        if(step.owner == STEP_OWNER["bpawn"] && piece == mBPW){
                            if(tst_bpw_move(newpos, pos)){
                                add_pw_moves(newpos, pos, moves);
                            }
                            break;
                        }
                        if((step.owner == STEP_OWNER["rook"] && (piece == mBRK || piece == mBQU)) ||
                           (step.owner == STEP_OWNER["bishop"] && (piece == mBBP || piece == mBQU)) ||
                           (step.owner == STEP_OWNER["knight"] && piece == mBKN)){
                            moves.push_back(cGMove(newpos, pos, mBLK));
                            break;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            if((outerstep.border & pos) > 0){
                return;
            }

            if(outerstep.rightshift){
                pos = (pos >> outerstep.shiftcnt);
            }
            else{
                pos = (pos << outerstep.shiftcnt);
            }
        }
    }
  

    void cBoard::add_pw_moves(uint64_t src, uint64_t dst, list <cGMove> &moves){
        if((dst & 0xFF00000000000000) > 0){
            uint8_t pieces[] = { mWQU, mWRK, mWBP, mWKN };

            for(uint8_t i = 0; i < 4; ++i){
                moves.push_back(cGMove(src, dst, pieces[i]));
            }
        }
        else if((dst & 0x00000000000000FF) > 0){
            uint8_t pieces[] = { mBQU, mBRK, mBBP, mBKN };

            for(uint8_t i = 0; i < 4; ++i){
                moves.push_back(cGMove(src, dst, pieces[i]));
            }
        }
        else{
            moves.push_back(cGMove(src, dst, mBLK));
        }
    }

