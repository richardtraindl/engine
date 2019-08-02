
    #include "./analyze_move.hpp"
    #include "./analyze_helper.hpp"
    #include "./analyze_tactics.hpp"
    #include "../values.hpp"

    using namespace std;


    cExcluded::cExcluded(cPrioMove *_priomove, cTactic *_tactic, int _prio){
        priomove = _priomove;
        tactic = _tactic;
        prio = _prio;
    }


    void add_tactics(cPrioMove &priomove, cMatch &match, cMove *candidate, cMove *dbggmove, bool search_for_mate, list<cPrioMove> &excludes){
        int rook = mBLK;
        list<cTouch> from_castl_rk_supported, from_castl_rk_attacked;
        int piece = match.board.getfield(priomove.src);

        //int dstpiece = match.board.getfield(priomove.dst);

        list<cTouch>from_dstfield_supported, from_dstfield_attacked;
        find_touches_after_move(match, priomove, from_dstfield_supported, from_dstfield_attacked);

        //list<cTouch> discl_supported, list<cTouch> discl_attacked;
        //find_disclosures(match, move, &discl_supported, &discl_attacked);

        int weight = weight_for_standard(match, piece, priomove);


        if(candidate != NULL){
            if(candidate->src == priomove.src && candidate->dst == priomove.dst && candidate->prompiece == priomove.prompiece){
                priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["prev-candidate"], cTactic::WEIGHTS["good-deal"], 0)));
            }
        }

        if(defends_check(match)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["defends-check"], weight, 0)));
        }

        if(castles(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["castles"], weight, 0)));
            find_rook_touches_after_castling(match, priomove, rook, from_castl_rk_supported, from_castl_rk_attacked);
        }

        if(is_tactical_draw(match, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["is-tactical-draw"], cTactic::WEIGHTS["good-deal"], 0)));
        }

        if(promotes(priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["promotes"], weight, 0)));
        }

        if(captures(match, piece, priomove)){
            int capture_weight = weight_for_capture(match, piece, priomove, weight);
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["captures"], capture_weight, 0)));
        }


        if(does_unpin(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["unpins"], weight, 0)));
        }

        /*
        if(forks(piece, from_dstfield_attacked)):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["forks"], weight))

        if(defends_fork(match, piece, move, dstpiece)):
            tactic = cTactic(cTactic.DOMAINS["defends-fork"], weight)
            priomove.tactics.append(tactic)
            excludes.append(cExcluded(priomove, tactic))

        if(threatens_fork(match, piece, move)):
            tactic = cTactic(cTactic.DOMAINS["threatens-fork"], weight)
            priomove.tactics.append(tactic)
            excludes.append(cExcluded(priomove, tactic))
        */


        if(flees(match, piece, priomove)){
            int flee_weight = weight_for_flee(match, piece, priomove, weight);
            cTactic tactic(cTactic::DOMAINS["flees"], flee_weight, priomove.src);
            priomove.tactics.push_back(tactic);
            //excludes.push_back(cExcluded(priomove, tactic));
        }

        fill_attacked(match, piece, priomove, search_for_mate, from_dstfield_attacked);
        
        if(rook != mBLK){
            fill_attacked(match, rook, priomove, search_for_mate, from_castl_rk_attacked);
        }

        fill_supported(match, piece, priomove, from_dstfield_supported);
        
        if(rook != mBLK){
            fill_supported(match, rook, priomove, from_castl_rk_supported);
        }


        /*
        if(len(discl_attacked) > 0):
            for dattd in discl_attacked:
                discl_attacking_weight = weight_for_discl_attacked(dattd, weight)
                tactic = cTactic(cTactic.DOMAINS["attacks"], discl_attacking_weight, dattd.field)
                priomove.tactics.append(tactic)        
                excludes.append(cExcluded(priomove, tactic))

        if(len(discl_supported) > 0):
            for dsuppd in discl_supported:
                discl_supporting_weight = weight_for_discl_supported(dsuppd, weight)
                tactic = cTactic(cTactic.DOMAINS["supports"], discl_supporting_weight, dsuppd.field)
                priomove.tactics.append(tactic)
                excludes.append(cExcluded(priomove, tactic))

        if(blocks(match, piece, move)):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["blocks"], weight))
        */

        if(running_pawn(match, priomove)){
            int running_weight = weight_for_running_pawn(match, piece, priomove, weight);
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["is-running-pawn"], running_weight, 0)));
        }

        /*
        if(controles_file(match, move)):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["controles-file"], weight))

        if(is_progress(match, move)):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["is-progress"], weight))
        */

        if(match.is_endgame() && is_approach_to_opp_king(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["approach-opp-king"], weight, 0)));
        }

        /*
        if(dbggmove and dbggmove.src == move.src and dbggmove.dst == move.dst):
            priomove.prio = 1
            excludes.clear()
            return excludes
        */

        priomove.evaluate_priority();

        //return excludes
    }
