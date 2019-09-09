
    #include <iostream>
    #include "./move.hpp"
    #include "./board.hpp"
    #include "./helper.hpp"
    #include "./values.hpp"

    cMove::cMove(uint64_t *_prevfields, int _src, int _dst, int _prompiece){ 
        prevfields[0] = *_prevfields;
        prevfields[1] = *(_prevfields + 1);
        prevfields[2] = *(_prevfields + 2);
        prevfields[3] = *(_prevfields + 3);
        src = _src;
        dst = _dst;
        prompiece = _prompiece;
    }
    cMove::cMove(){
    }

    int cMove::getprevfield(int idx){
        if(idx < 0 || idx > 63){
            cout << "getprevfield: " << idx << endl;
            return -1;
        }
        int piece = 0;
        int rank = idx >> 4;
        int index = idx & 0xF;
        uint64_t testmask = cBoard::BITPOSMASK[index];
        uint64_t writemask = 0x1;
        for(int i = 0; i < 4; ++i){
            if(prevfields[rank] & testmask){
                piece = piece | writemask;
            }
            testmask = testmask << 1;
            writemask = writemask << 1;
        }
        return piece;
    }

    string cMove::format(){
        int piece = getprevfield(src);
        if(piece == mWKG || piece == mBKG){
            if(src % 8 - dst % 8 == -2){
                return string("0-0");
            }
            if(src % 8 - dst % 8 == 2){
                return string("0-0-0");
            }
        }

        int dstpiece = getprevfield(dst);
        string hyphen = "";
        string trailing = "";
        if(dstpiece == mBLK){
            hyphen = "-";
        }
        else{
            hyphen = "x";
        }
        if(piece == mWPW || piece == mBPW){
            if(prompiece != mBLK){
                trailing = ", " + reverse_lookup(PIECES, prompiece);
            }
            else{
                if(dstpiece == mBLK && src % 8 != dst % 8){
                    trailing = " e.p.";
                }
            }
        }
        return index_to_coord(src) + hyphen + index_to_coord(dst) + trailing;
    }


    cTactic::cTactic(int _domain, int _weight, int _addition){ 
        domain = _domain;
        weight = _weight;
        addition = _addition;
    }

    map<string, int> cTactic::DOMAINS = {
            {"defends-check",         10},
            {"captures",              20},
            {"attacks-king",          30},
            {"attacks",               40},
            {"supports",              50},
            {"supports-running-pawn", 60},
            {"flees",                 70},
            {"forks",                 80},
            {"threatens-fork",        90},
            {"defends-fork",          100},
            {"unpins",                110},
            {"blocks",                120},
            {"promotes",              130}, 
            {"is-tactical-draw",      140},
            {"prev-candidate",        150},
            {"is-running-pawn",       160}, 
            {"controles-file",        170},
            {"castles",               180},
            {"is-progress",           190},
            {"opposition",            200},
            {"approach-opp-king",     210},
            {"undef",                 220}
        };

        map<string, int> cTactic::WEIGHTS = {
            {"stormy",                  1},
            {"better-deal",             2},
            {"good-deal",               3},
            {"vague-deal",              4},
            {"downgraded",              5},
            {"upgraded",                6},
            {"bad-deal",                7},
            {"undef",                   8}
        };

        map<int, int> cTactic::DOMAINS_TO_PRIOS = {
            // ### level 1 ###
            {DOMAINS["promotes"],               100},
            {DOMAINS["captures"],               101},
            {DOMAINS["is-running-pawn"],        102},
            {DOMAINS["is-tactical-draw"],       103},
            {DOMAINS["defends-check"] ,         104},
            {DOMAINS["prev-candidate"] ,        105},
            // ### level 2 ###
            {DOMAINS["castles"],                120},
            {DOMAINS["attacks-king"],           121},
            {DOMAINS["forks"],                  122}, 
            {DOMAINS["threatens-fork"],         123},
            {DOMAINS["defends-fork"],           124}, 
            {DOMAINS["unpins"],                 125}, 
            {DOMAINS["supports-running-pawn"],  126}, 
            {DOMAINS["flees"],                  127}, 
            {DOMAINS["blocks"],                 128},
            {DOMAINS["controles-file"],         129}, 
            {DOMAINS["is-progress"],            130},
            {DOMAINS["supports"],               131},
            {DOMAINS["attacks"],                132},
            {DOMAINS["opposition"],             133},
            {DOMAINS["approach-opp-king"],      134},
            // ### level 3 ###
            {DOMAINS["undef"],                  150}
        };

        map<int, int> cTactic::WEIGHTS_TO_ADJUST = {
            {WEIGHTS["stormy"],                 -40},
            {WEIGHTS["better-deal"],            -30},
            {WEIGHTS["good-deal"],                0},
            {WEIGHTS["vague-deal"],              10},
            {WEIGHTS["upgraded"],                 0},
            {WEIGHTS["undef"],                    0},
            {WEIGHTS["downgraded"],              40},
            {WEIGHTS["bad-deal"],                50}
        };

        map<string, int> cPrioMove::PRIOS = {
            {"prio0",                            50},
            {"prio1",                           100},
            {"prio2",                           150},
            {"prio3",                           200} 
        };

    cPrioMove::cPrioMove(uint64_t *prevfields, int src, int dst, int prompiece, int _prio) :  cMove(prevfields, src, dst, prompiece){
        prio = _prio;
    }

    void cPrioMove::evaluate_priority(){
        int count = 0;
        prio = PRIOS["prio3"];
        for(cTactic *tactic : tactics){
            int newprio = cTactic::DOMAINS_TO_PRIOS[tactic->domain] + cTactic::WEIGHTS_TO_ADJUST[tactic->weight] + tactic->addition;
            prio = min(prio, newprio);
            if(tactic->weight <= cTactic::WEIGHTS["downgraded"]){
                count += 2;
            }
        }
        prio -= count;
    }

    void cPrioMove::downgrade(int domain){
        for(cTactic *tactic : tactics){
            if(tactic->domain == domain){
                if(tactic->weight == cTactic::WEIGHTS["stormy"] ||
                   tactic->weight == cTactic::WEIGHTS["better-deal"] ||
                   tactic->weight == cTactic::WEIGHTS["good-deal"]){
                    tactic->weight = cTactic::WEIGHTS["downgraded"];
                    return;
                }
            }
        }
    }
                    
    void cPrioMove::upgrade(int domain){
        for(cTactic *tactic : tactics){
            if(tactic->domain == domain){
                if(tactic->weight != cTactic::WEIGHTS["stormy"] && 
                   tactic->weight != cTactic::WEIGHTS["better-deal"] && 
                   tactic->weight != cTactic::WEIGHTS["good-deal"]){
                    tactic->weight = cTactic::WEIGHTS["upgraded"];
                    return;
                }
            }
        }
    }

    int cPrioMove::fetch_weight(int domain){
        for(cTactic *tactic : tactics){
            if(tactic->domain == domain){
                return tactic->weight;
            }
        }
        return cTactic::WEIGHTS["undef"];
    }

    bool cPrioMove::has_domain(int domain){
        for(cTactic *tactic : tactics){
            if(tactic->domain == domain){
                return true;
            }
        }
        return false;
    }

    bool cPrioMove::has_weight(int weight){
        for(cTactic *tactic : tactics){
            if(tactic->weight == weight){
                return true;
            }
        }
        return false;
    }

    bool cPrioMove::has_tactic_ext(int domain, int weight){
        for(cTactic *tactic : tactics){
            if(tactic->domain == domain and tactic->weight == weight){
                return true;
            }
        }
        return false;
    }

    bool cPrioMove::is_tactic_stormy(){
        for(cTactic *tactic : tactics){
            if(tactic->weight == cTactic::WEIGHTS["stormy"] || 
               ((tactic->domain == cTactic::DOMAINS["promotes"] || 
                 tactic->domain == cTactic::DOMAINS["captures"]) &&  
                (tactic->weight == cTactic::WEIGHTS["better-deal"] || 
                 tactic->weight == cTactic::WEIGHTS["good-deal"]))){
                return true;
            }
        }
        return false;
    }

    string cPrioMove::fmt_tactics(){
        string str_tactics = "";
        for(cTactic *tactic : tactics){
            str_tactics += reverse_lookup(cTactic::DOMAINS, tactic->domain) + " * " + reverse_lookup(cTactic::WEIGHTS, tactic->weight) + " | " ;
        }
        return str_tactics;
    }

