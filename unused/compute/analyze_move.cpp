
    #include "./analyze_move.hpp"
    #include "./analyze_helper.hpp"
    #include "./analyze_tactics.hpp"
    #include "../values.hpp"
    #include "../helper.hpp"


    void add_tactics(cPrioMove &priomove, cMatch &match, cMove *dbggmove, list<cExclude *> &excludes){
        int castlrook = mBLK;
        list<cTouch> from_castl_rk_supported, from_castl_rk_attacked;

        int piece = match.board.getfield(priomove.src);
        int dstpiece = match.board.getfield(priomove.dst);

        list<cTouch>from_dstfield_supported, from_dstfield_attacked;
        find_touches_after_move(match, priomove, from_dstfield_supported, from_dstfield_attacked);

        list<cTouch> discl_supported, discl_attacked;
        find_disclosures(match, piece, priomove, discl_supported, discl_attacked);

        cAnalyzeField analyzesrc(match.board, PIECES_COLORS[piece], priomove.src);

        match.do_move(priomove.src, priomove.dst, priomove.prompiece);
        cAnalyzeField analyzedst(match.board, PIECES_COLORS[piece], priomove.dst);
        match.undo_move();

        int standard_weight = weight_for_standard(match, piece, priomove, analyzedst);

        bool is_endgame = match.is_endgame();


        if(defends_check(match)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["defends-check"], standard_weight, PIECES_RANKS[piece]));
        }


        if(castles(match, piece, priomove)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["castles"], standard_weight, PIECES_RANKS[piece]));
            find_rook_touches_after_castling(match, priomove, castlrook, from_castl_rk_supported, from_castl_rk_attacked);
        }


        if(is_tactical_draw(match, priomove)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["is-tactical-draw"], cTactic::WEIGHTS["good-deal"], PIECES_RANKS[piece]));
        }


        if(promotes(priomove)){
            int promote_weight;
            if(analyzedst.is_field_ok){
                promote_weight = cTactic::WEIGHTS["stormy"];
            }
            else{
                promote_weight = standard_weight;
            }
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["promotes"], promote_weight, PIECES_RANKS[piece]));
        }


        if(captures(match, priomove)){
            int capture_weight = weight_for_capture(match, piece, dstpiece, priomove, analyzedst);
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["captures"], capture_weight, PIECES_RANKS[dstpiece]));
        }
        if(captures_enpassant(match, piece, priomove)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["good-deal"], PIECES_RANKS[piece]));
        }


        if(does_unpin(match, piece, priomove)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["unpins"], standard_weight, PIECES_RANKS[piece]));
        }


        if(forks(piece, from_dstfield_attacked)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["forks"], standard_weight, PIECES_RANKS[piece]));
        }


        if(defends_fork(match, piece, dstpiece, priomove)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["defends-fork"], standard_weight, PIECES_RANKS[piece]));
        }


        if(threatens_fork(match, piece, priomove)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["threatens-fork"], standard_weight, PIECES_RANKS[piece]));
        }


        if(flees(match, piece, priomove)){
            int flee_weight = weight_for_flee(match, piece, priomove, analyzesrc, analyzedst);
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["flees"], flee_weight, PIECES_RANKS[piece]));
            int mvdir = cPiece::dir_for_move(piece, priomove.src, priomove.dst);
            if(find_excluded(excludes, priomove.src, cBoard::SNOK, mvdir, cTactic::DOMAINS["flees"])){
                priomove.downgrade(cTactic::DOMAINS["flees"]);
            }
            else{
                if(flee_weight >= cTactic::WEIGHTS["vague-deal"]){
                    excludes.push_back(new cExclude(priomove.src, cBoard::SNOK, mvdir, cTactic::DOMAINS["flees"]));
                }
            }
        }


        if(from_dstfield_supported.size() > 0){
            fill_supported(match, piece, priomove, NULL, from_dstfield_supported, analyzedst, excludes);
        }
        if(castlrook != mBLK){
            int src, dst;
            if((priomove.src % 8) + 2 == priomove.dst % 8){
                src = priomove.dst + 1;
                dst = priomove.dst - 1;
            }
            else{
                src = priomove.dst - 2;
                dst = priomove.dst + 1;
            }
            cPrioMove *pmove_for_castlrook = new cPrioMove(src, dst, mBLK, mBLK, 0);
            fill_supported(match, castlrook, priomove, pmove_for_castlrook, from_castl_rk_supported, analyzedst, excludes);
            delete pmove_for_castlrook;
        }


        if(from_dstfield_attacked.size() > 0){
            fill_attacked(match, piece, priomove, NULL, from_dstfield_attacked, analyzedst, excludes);
        }
        if(castlrook != mBLK){
            int src, dst;
            if((priomove.src % 8) + 2 == priomove.dst % 8){
                src = priomove.dst + 1;
                dst = priomove.dst - 1;
            }
            else{
                src = priomove.dst - 2;
                dst = priomove.dst + 1;
            }
            cPrioMove *pmove_for_castlrook = new cPrioMove(src, dst, mBLK, mBLK, 0);
            fill_attacked(match, castlrook, priomove, pmove_for_castlrook, from_castl_rk_attacked, analyzedst, excludes);
            delete pmove_for_castlrook;
        }


        if(discl_supported.size() > 0){
            int weight_for_discl_supporting_piece = cTactic::WEIGHTS["undef"];
            int touchpos_for_piece;
            for(cTouch touch : discl_supported){
                int new_weight_for_discl_supporting_piece = weight_for_discl_support(match, piece, priomove, touch, analyzedst);
                if(new_weight_for_discl_supporting_piece < weight_for_discl_supporting_piece){
                    weight_for_discl_supporting_piece = new_weight_for_discl_supporting_piece;
                    touchpos_for_piece = touch.pos;
                }
            }
            if(weight_for_discl_supporting_piece < cTactic::WEIGHTS["undef"]){
                priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["supports"], weight_for_discl_supporting_piece, PIECES_RANKS[piece]));
                if(find_excluded(excludes, priomove.src, touchpos_for_piece, DIRS["undef"], cTactic::DOMAINS["supports"])){
                    priomove.downgrade(cTactic::DOMAINS["supports"]);
                }
                else{
                    if(weight_for_discl_supporting_piece >= cTactic::WEIGHTS["vague-deal"]){
                        excludes.push_back(new cExclude(priomove.src, touchpos_for_piece, DIRS["undef"], cTactic::DOMAINS["supports"]));
                    }
                }
            }
        }


        if(discl_attacked.size() > 0){
            int weight_for_discl_attacking_piece = cTactic::WEIGHTS["undef"];
            int touchpos_for_piece;
            for(cTouch touch : discl_attacked){
                int new_weight_for_discl_attacking_piece = weight_for_discl_attack(match, piece, priomove, touch, analyzedst);
                if(new_weight_for_discl_attacking_piece < weight_for_discl_attacking_piece){
                    weight_for_discl_attacking_piece = new_weight_for_discl_attacking_piece;
                    touchpos_for_piece = touch.pos;
                }
            }
            if(weight_for_discl_attacking_piece < cTactic::WEIGHTS["undef"]){
                priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["attacks"], weight_for_discl_attacking_piece, PIECES_RANKS[piece]));
                if(find_excluded(excludes, priomove.src, touchpos_for_piece, DIRS["undef"], cTactic::DOMAINS["attacks"])){
                    priomove.downgrade(cTactic::DOMAINS["attacks"]);
                }
                else{
                    if(weight_for_discl_attacking_piece >= cTactic::WEIGHTS["vague-deal"]){
                        excludes.push_back(new cExclude(priomove.src, touchpos_for_piece, DIRS["undef"], cTactic::DOMAINS["attacks"]));
                    }
                }
            }
        }


        if(blocks(match, piece, priomove)){
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["blocks"], standard_weight, PIECES_RANKS[piece]));
        }


        /*
        if(controles_file(match, move)):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["controles-file"], weight))

        if(is_progress(match, move)):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["is-progress"], weight))
        */


        if(running_pawn(match, priomove)){
            int running_weight = max(standard_weight, cTactic::WEIGHTS["undef"]);
            int running_addition = 0;
            if(is_endgame){
                if(PIECES_COLORS[piece] == COLORS["white"]){
                    running_addition = priomove.dst;
                }
                else{
                    running_addition = 8 - priomove.dst;
                }
                running_weight = standard_weight;
            }
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["is-running-pawn"], running_weight, running_addition));
        }


        if(is_approach_to_opp_king(match, piece, priomove)){
            int approach_weight = max(standard_weight, cTactic::WEIGHTS["undef"]);
            if(is_endgame){
                 approach_weight = standard_weight;
            }
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["approach-opp-king"], approach_weight, PIECES_RANKS[piece]));
        }


        if(leads_pawn_to_promotion(match, piece, priomove)){
            int leading_weight = max(standard_weight, cTactic::WEIGHTS["undef"]);
            if(is_endgame){
                 leading_weight = standard_weight;
            }
            priomove.tactics.push_back(new cTactic(cTactic::DOMAINS["guards-pawn-to-promotion"], leading_weight, PIECES_RANKS[piece]));
        }


        if(dbggmove != NULL && dbggmove->src == priomove.src && dbggmove->dst == priomove.dst){
            priomove.prio = 1;
        }
        else{
            priomove.evaluate_priority();
        }
    }