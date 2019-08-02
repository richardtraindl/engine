
    #include <sstream>
    #include "./move.hpp"
    #include "./helper.hpp"
    #include "./values.hpp"

    cMove::cMove(uint256_t _prevfields, int _src, int _dst, int _prompiece){ 
        prevfields = _prevfields;
        src = _src;
        dst = _dst;
        prompiece = _prompiece;
    }
    cMove::cMove(){
    }

    int cMove::getprevfield(int idx){
        if(idx < 0 || idx > 63){
            cout << "getprevfield: " << dec << idx << endl;
            return 0;
        }
        return (int)((prevfields >> ((63 - idx) * 4)) & 0xF);
    }

    string cMove::format(){
        int piece = getprevfield(src);
        if(piece == mWKG || piece == mBKG){
            if((int)(src % 8) - (int)((int)(dst % 8) == -2)){
                return string("0-0");
            }
            if(src % 8 - dst % 8 == 2){
                return string("0-0-0");
            }
        }

        int dstpiece = getprevfield(dst);
        string hyphen = "";
        string trailing = "";
        stringstream out;
        if(dstpiece == mBLK){
            hyphen = "-";
        }
        else{
            hyphen = "x";
        }
        if(piece == mWPW || piece == mBPW){
            if(prompiece != mBLK){
                out << prompiece;
                trailing = ", " + out.str();
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
            {"undefined",             220}
        };

        map<string, int> cTactic::WEIGHTS = {
            {"stormy", 1},
            {"better-deal", 2},
            {"good-deal", 3},
            {"downgraded", 4},
            {"upgraded", 5},
            {"bad-deal", 6}
        };

        map<int, int> cTactic::DOMAINS_TO_PRIOS = {
            // ### level 1 ###
            {DOMAINS["promotes"],               90},
            {DOMAINS["captures"],               91},
            {DOMAINS["is-running-pawn"],        92},
            {DOMAINS["is-tactical-draw"],       93},
            {DOMAINS["defends-check"] ,         94},
            {DOMAINS["prev-candidate"] ,        95},
            // ### level 2 ###
            {DOMAINS["castles"],                200},
            {DOMAINS["attacks-king"],           201},
            {DOMAINS["forks"],                  202}, 
            {DOMAINS["threatens-fork"],         203},
            {DOMAINS["defends-fork"],           204}, 
            {DOMAINS["unpins"],                 205}, 
            {DOMAINS["supports-running-pawn"],  206}, 
            {DOMAINS["flees"],                  207}, 
            {DOMAINS["blocks"],                 208},
            {DOMAINS["controles-file"],         209}, 
            {DOMAINS["is-progress"],            210},
            {DOMAINS["supports"],               211},
            {DOMAINS["attacks"],                212},
            {DOMAINS["opposition"],             213},
            {DOMAINS["approach-opp-king"],      214},
            // ### level 3 ###
            {DOMAINS["undefined"],           500}
        };

        map<int, int> cTactic::WEIGHTS_TO_ADJUST = {
            {WEIGHTS["stormy"],      -70},
            {WEIGHTS["better-deal"], -10},
            {WEIGHTS["good-deal"],     0},
            {WEIGHTS["upgraded"],      0},
            {WEIGHTS["undefined"],     0},
            {WEIGHTS["downgraded"],   60},
            {WEIGHTS["bad-deal"],    130}
        };


        map<string, int> cPrioMove::PRIOS = {
            {"prio0", 100},
            {"prio1", 200},
            {"prio2", 250},
            {"prio3", 300} 
        };

    cPrioMove::cPrioMove(uint256_t prevfields, int src, int dst, int prompiece, int _prio) :  cMove(prevfields, src, dst, prompiece){
        prio = _prio;
    }

    void cPrioMove::evaluate_priority(){
        int count = 0;
        int prio = PRIOS["prio3"];
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            int newprio = it->DOMAINS_TO_PRIOS[it->domain] + it->WEIGHTS_TO_ADJUST[it->weight];
            prio = min(prio, newprio);
            if(it->weight <= cTactic::WEIGHTS["downgraded"]){
                count += 2;
            }
        }
        prio -= count;
    }

    void cPrioMove::downgrade(int domain){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->domain == domain){
                if(it->weight == cTactic::WEIGHTS["stormy"] ||
                   it->weight == cTactic::WEIGHTS["better-deal"] ||
                   it->weight == cTactic::WEIGHTS["good-deal"]){
                    it->weight = cTactic::WEIGHTS["downgraded"];
                    return;
                }
            }
        }
    }
                    
    void cPrioMove::upgrade(int domain){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->domain == domain){
                if(it->weight != cTactic::WEIGHTS["stormy"] && 
                   it->weight != cTactic::WEIGHTS["better-deal"] && 
                   it->weight != cTactic::WEIGHTS["good-deal"]){
                    it->weight = cTactic::WEIGHTS["upgraded"];
                    return;
                }
            }
        }
    }

    int cPrioMove::fetch_weight(int domain){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->domain == domain){
                return it->weight;
            }
        }
        return cTactic::WEIGHTS["undefined"];
    }

    bool cPrioMove::has_domain(int domain){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->domain == domain){
                return true;
            }
        }
        return false;
    }

    bool cPrioMove::has_weight(int weight){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->weight == weight){
                return true;
            }
        }
        return false;
    }

    bool cPrioMove::has_tactic_ext(int domain, int weight){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->domain == domain and it->weight == weight){
                return true;
            }
        }
        return false;
    }

    bool cPrioMove::is_tactic_stormy(){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->weight == cTactic::WEIGHTS["stormy"] || 
               ((it->domain == cTactic::DOMAINS["promotes"] || 
                 it->domain == cTactic::DOMAINS["captures"]) &&  
                (it->weight == cTactic::WEIGHTS["better-deal"] || 
                 it->weight == cTactic::WEIGHTS["good-deal"]))){
                return true;
            }
        }
        return false;
    }

    string cPrioMove::fmt_tactics(){
        string str_tactics = "";
        stringstream out;
        int i = 1;
        string str_end;
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            out << reverse_lookup(cTactic::DOMAINS, it->domain) << " * " << reverse_lookup(cTactic::WEIGHTS, it->weight) << " | " ;
            str_tactics +=  out.str();
            i += 1;
        }
        return str_tactics;
    }

