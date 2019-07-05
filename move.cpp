
#include <sstream>
#include "./move.hpp"
#include "./helper.hpp"
#include "./values.hpp"
    
    using namespace std;

    cMove::cMove(int **_prevfields, int _srcx, int _srcy, int _dstx, int _dsty, int _prompiece){ 
        if(_prevfields != NULL){
            for(int y = 0; y < 8; ++y){
                for(int x = 0; x < 8; ++x){
                    prevfields[y][x] = *(*_prevfields + y) + x;
                }
            }
        }
    } 
    cMove::cMove(){
    }

    int cMove::getPrevfield(int x, int y){
        return prevfields[y][x];
    }

    string cMove::format(){
        int piece = prevfields[srcy][srcx];
        if(piece == PIECES["wKg"] || piece == PIECES["bKg"]){
            if(srcx - dstx == -2){
                return "0-0";
            }
            if(srcx - dstx == 2){
                return "0-0-0";
            }
        }

        int dstpiece = prevfields[dsty][dstx];
        string hyphen = "";
        string trailing = "";
        stringstream out;
        if(dstpiece == PIECES["blk"]){
            hyphen = "-";
        }
        else{
            hyphen = "x";
        }
        if(piece == PIECES["wPw"] || piece == PIECES["bPw"]){
            if(prompiece != PIECES["blk"]){ // prompiece != NULL && 
                out << prompiece;
                trailing = ", " + out.str(); // STR_PIECES(prompiece);
            }
            else{
                if(dstpiece == PIECES["blk"] && srcx != dstx){
                    trailing = " e.p.";
                }
            }
        }
        return index_to_coord(srcx, srcy) + hyphen + index_to_coord(dstx, dsty) + trailing;
    }


    cTactic::cTactic(int _domain, int _weight, int _addition){ 
        domain = _domain;
        weight = _weight;
        addition = _addition;
    }


    cPrioMove::cPrioMove(cMove *_move){
        // prevfields = _move->prevfields;
        srcx = _move->srcx;
        srcy = _move->srcy;
        dstx = _move->dstx;
        dsty = _move->dsty;
        prompiece = _move->prompiece;
    }

    void cPrioMove::evaluate_priority(){
        int count = 0;
        int prio = PRIOS["prio3"];
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            int newprio = it->DOMAINS_TO_PRIOS[it->domain] + it->WEIGHTS_TO_ADJUST[it->weight];
            prio = min(prio, newprio);
            if(it->weight <= it->WEIGHTS["downgraded"]){
                count += 2;
            }
        }
        prio -= count;
    }

    void cPrioMove::downgrade(int domain){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->domain == domain){
                if(it->weight == it->WEIGHTS["stormy"] ||
                   it->weight == it->WEIGHTS["better-deal"] ||
                   it->weight == it->WEIGHTS["good-deal"]){
                    it->weight = it->WEIGHTS["downgraded"];
                    return;
                }
            }
        }
    }
                    
    void cPrioMove::upgrade(int domain){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->domain == domain){
                if(it->weight != it->WEIGHTS["stormy"] && 
                   it->weight != it->WEIGHTS["better-deal"] && 
                   it->weight != it->WEIGHTS["good-deal"]){
                    it->weight = it->WEIGHTS["upgraded"];
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
        return NULL;
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

    bool cPrioMove::has_tactic_ext(cTactic tactic){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->domain == tactic.domain and it->weight == tactic.weight){
                return true;
            }
        }
        return false;
    }

    bool cPrioMove::is_tactic_stormy(){
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(it->weight == it->WEIGHTS["stormy"] || 
               ((it->domain == it->DOMAINS["promotes"] || 
                 it->domain == it->DOMAINS["captures"]) &&  
                (it->weight == it->WEIGHTS["better-deal"] || 
                 it->weight == it->WEIGHTS["good-deal"]))){
                return true;
            }
        }
        return false;
    }

    string cPrioMove::concat_fmttactics(){
        string str_tactics = "";
        int length = tactics.size();
        stringstream out;
        int i = 1;
        string str_end;
        for(list<cTactic>::iterator it = tactics.begin(); it != tactics.end(); ++it){
            if(i < length){
                str_end = " | ";
            }
            else{
                str_end = "\n";
            }
            out << it->domain << " * " << it->weight;
            // reverse_lookup(WEIGHTS, it->weight)
            // reverse_lookup(DOMAINS, it->domain) + weight_str  + str_end
            str_tactics +=  out.str() + str_end;
            i += 1;
        }
        return str_tactics;
    }


