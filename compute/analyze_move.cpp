
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
        //from_castl_rk_supported = []
        //from_castl_rk_attacked = []
        //crook = None
        int piece = match.board.getfield(priomove.src);
        int dstpiece = match.board.getfield(priomove.dst);
        /*
        list<cTouch>from_dstfield_supported, from_dstfield_attacked;
        find_touches_after_move(match, move, from_dstfield_supported, from_dstfield_attacked);
        */ 
        /*
        list<cTouch> supported, attacked;
        find_touches_after_move(match, move, &supported, &attacked);
        */
        /*
        list<cTouch> discl_supported, list<cTouch> discl_attacked;
        find_disclosures(match, move, &discl_supported, &discl_attacked);
        */
        int weight = weight_for_standard(match, piece, priomove);


    /*if(candidate):
        if(candidate.src == move.src and candidate.dst == move.dst and candidate.prompiece == move.prompiece):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["prev-candidate"], cTactic.WEIGHTS["good-deal"]))
    */
    if(defends_check(match)){
        priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["defends-check"], weight, 0)));
    }

    if(castles(match, piece, priomove)){
        priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["castles"], weight, 0)));
        //crook, from_castl_rk_supported, from_castl_rk_attacked = find_rook_touches_after_castling(match, move)
    }
    /*
    if(is_tactical_draw(match, move)):
        priomove.tactics.append(cTactic(cTactic.DOMAINS["is-tactical-draw"], cTactic.WEIGHTS["good-deal"]))
    */    
    if(promotes(priomove)){
        priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["promotes"], weight, 0)));
    }

    if(captures(match, piece, priomove)){
        int capture_weight = weight_for_capture(match, piece, priomove, weight);
        priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["captures"], capture_weight, 0)));
    }

    /*
    if(does_unpin(match, piece, move)):
        priomove.tactics.append(cTactic(cTactic.DOMAINS["unpins"], weight))

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

    if(flees(match, piece, move)):
        flee_weight = weight_for_flee(match, piece, move, weight)
        tactic = cTactic(cTactic.DOMAINS["flees"], flee_weight, move.src)
        priomove.tactics.append(tactic)
        excludes.append(cExcluded(priomove, tactic))

    if(len(from_dstfield_attacked) > 0 or len(from_castl_rk_attacked) > 0):
        for i in range(2):
            if(i == 0):
                tmp_list_attacked = from_dstfield_attacked
                tmp_piece = piece
            else:
                if(crook is None):
                    continue
                else:
                    tmp_list_attacked = from_castl_rk_attacked
                    tmp_piece = crook.piece
            for attacked in tmp_list_attacked:
                if(attacked.piece == PIECES["wKg"] or attacked.piece == PIECES["bKg"]):
                    if(search_for_mate):
                        attacking_weight = weight_for_attacking_king(match, move, weight)
                    else:
                        attacking_weight = weight
                    tactic = cTactic(cTactic.DOMAINS["attacks-king"], attacking_weight, attacked.field)
                    priomove.tactics.append(tactic)
                    excludes.append(cExcluded(priomove, tactic))
                else:
                    attacking_weight = weight_for_attacking(match, tmp_piece, move, attacked, weight)
                    tactic = cTactic(cTactic.DOMAINS["attacks"], attacking_weight, attacked.field)
                    priomove.tactics.append(tactic)
                    excludes.append(cExcluded(priomove, tactic))

    if(len(from_dstfield_supported) > 0 or len(from_castl_rk_supported) > 0):
        for i in range(2):
            if(i == 0):
                tmp_list_supported = from_dstfield_supported
                tmp_piece = piece
            else:
                if(crook is None):
                    continue
                else:
                    tmp_list_attacked = from_castl_rk_supported
                    tmp_piece = crook.piece
            for supported in tmp_list_supported:
                if(is_supported_running_pawn(match, supported)):
                    supporting_tactic = cTactic.DOMAINS["supports-running-pawn"]
                elif(len(supported.attacker_beyond) > 0):
                    supporting_tactic = cTactic.DOMAINS["supports"]
                else:
                    continue
                ###
                supporting_weight = weight_for_supporting(match, tmp_piece, move, supported, weight)
                tactic = cTactic(supporting_tactic, supporting_weight, supported.field)
                priomove.tactics.append(tactic)
                excludes.append(cExcluded(priomove, tactic))

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

    if(running_pawn(match, piece, move)):
        running_weight = weight_for_running_pawn(match, piece, move, weight)
        priomove.tactics.append(cTactic(cTactic.DOMAINS["is-running-pawn"], running_weight))

    if(controles_file(match, move)):
        priomove.tactics.append(cTactic(cTactic.DOMAINS["controles-file"], weight))

    if(is_progress(match, move)):
        priomove.tactics.append(cTactic(cTactic.DOMAINS["is-progress"], weight))

    if(match.is_endgame() and is_approach_of_opp_king(match, piece, move)):
        priomove.tactics.append(cTactic(cTactic.DOMAINS["approach-opp-king"], weight))

    if(dbggmove and dbggmove.src == move.src and dbggmove.dst == move.dst):
        priomove.prio = 1
        excludes.clear()
        return excludes

    priomove.evaluate_priority()

    return excludes
    */
    }