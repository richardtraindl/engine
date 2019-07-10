#ifndef CALC_HPP
    #define CALC_HPP

    #include <iostream>
    #include <list> 
    #include <string>
    #include <ctime>
    #include "./board.hpp"
    #include "./move.hpp"
    #include "./match.hpp"

    using namespace std;

    void prnt_priomoves(cMatch *match, list<cPrioMove> *priomoves);

    void prnt_search(cMatch *match, string label, int score, cMove *move, int candidates[]);

    string concat_fmtmoves(cMatch *match, list<cMove> *moves);

    list<cPrioMove> generate_moves(cMatch *match, int candidate, cMove *dbggmove, bool search_for_mate, int mode);

    void append_newmove(cMove *move, list<cMove> candidates, list<cMove> newcandidates);

    alphabeta(cMatch *match, int depth, cSlimits *slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, cPrioMove *candidate, int *score, list<cPrioMove> *candidates);

    class SearchLimitsLevelBlitz{
        public:
            static int add_mvcnt;
            static int dpth_max;
            static int dpth_stage1;
            static int dpth_stage2;
            static int mvcnt_stage1;
            static int mvcnt_stage2;
    };

    class SearchLimitsLevelBlitz : public SearchLimitsLevelLow{
    };

    class SearchLimitsLevelBlitz : public SearchLimitsLevelMedium{
    };

    class SearchLimitsLevelBlitz : public SearchLimitsLevelHigh{
    };


def count_up_to_prio(priomoves, priolimit):
    count = 0
    for priomove in priomoves:
        if(priomove.prio <= priolimit or priomove.is_tactic_stormy()):
            count += 1
        else:
            break
    return count


def count_up_within_stormy(priomoves):
    count = 0
    for priomove in priomoves:
        if(priomove.is_tactic_stormy()):
            count += 1
        else:
            break
    return count


def resort_exchange_or_stormy_moves(priomoves, new_prio, last_pmove, only_exchange):
    if(only_exchange and last_pmove is not None and 
       last_pmove.has_domain(cTactic.DOMAINS['captures']) == False):
        return False
    if(last_pmove is not None and 
       last_pmove.has_tactic_ext(cTactic(cTactic.DOMAINS['captures'], cTactic.WEIGHTS['bad-deal']))):
        last_pmove_capture_bad_deal = True
    else:
        last_pmove_capture_bad_deal = False
    count_of_stormy = 0
    count_of_good_captures = 0
    first_silent = None
    bad_captures = []
    for priomove in priomoves:
        if(only_exchange == False and priomove.is_tactic_stormy()):
            count_of_stormy += 1
            priomove.prio = min(priomove.prio, (new_prio + priomove.prio % 10) - 13)
        elif(priomove.has_domain(cTactic.DOMAINS['captures'])):
            weight = priomove.fetch_weight(cTactic.DOMAINS['captures'])
            if(weight > cTactic.WEIGHTS['bad-deal']):
                count_of_good_captures += 1
                priomove.prio = min(priomove.prio, (new_prio  + priomove.prio % 10) - 12)
            elif(last_pmove_capture_bad_deal):
                bad_captures.append(priomove)
                #count_of_bad_captures += 1
                #priomove.prio = min(priomove.prio, new_prio)
        elif(first_silent is None):
            first_silent = priomove
    if(len(bad_captures) > 0 and count_of_good_captures == 0 and count_of_stormy == 0):
        if(first_silent):
            first_silent.prio = min(first_silent.prio, (new_prio + first_silent.prio % 10) - 10)
        for capture in bad_captures:
            capture.prio = min(capture.prio, new_prio + capture.prio % 10)
    priomoves.sort(key=attrgetter('prio'))
    return True


def select_movecnt(match, priomoves, depth, slimits, last_pmove):
    if(len(priomoves) == 0 or depth > slimits.dpth_max):
        return 0
    if(depth <= slimits.dpth_stage1 and priomoves[0].has_domain(cTactic.DOMAINS['defends-check'])):
        return len(priomoves)

    stormycnt = count_up_within_stormy(priomoves)
    if(depth <= slimits.dpth_stage1):
        resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS['prio1'], last_pmove, False)
        count = count_up_to_prio(priomoves, cPrioMove.PRIOS['prio2'])
        if(count == 0):
            count = slimits.mvcnt_stage1
        else:
            count = min(count, slimits.mvcnt_stage1)
        return max(stormycnt, count)
    elif(depth <= slimits.dpth_stage2):
        resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS['prio1'], last_pmove, False)
        count = count_up_to_prio(priomoves, cPrioMove.PRIOS['prio2'])
        if(count == 0):
            count = slimits.mvcnt_stage2
        else:
            count = min(count, slimits.mvcnt_stage2)
        return max(stormycnt, count)
        """elif(depth <= slimits.dpth_stage3):
        resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS['prio0'], last_pmove, False)
        count = count_up_to_prio(priomoves, cPrioMove.PRIOS['prio0'])
        if(count == 0):
            count = slimits.mvcnt_stage3
        else:
            count = min(count, slimits.mvcnt_stage3)
        return max(stormycnt, count)"""
    else:
        if(resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS['prio0'], last_pmove, True)):
            return count_up_to_prio(priomoves, cPrioMove.PRIOS['prio0'])
            #return min(slimits.mvcnt_stage3, count)
            #return min(2, count)
        else:
            return 0


def concat_fmtmoves(match, moves):
    str_moves = ""
    for move in moves:
        if(move):
            str_moves += " [" + move.format() + "] "
    return str_moves

def prnt_fmttime(msg, seconds):
    minute, sec = divmod(seconds, 60)
    hour, minute = divmod(minute, 60)
    print( msg + "%02d:%02d:%02d" % (hour, minute, sec))


def calc_move(match, candidate):
    time_start = time.time()
    move = retrieve_move(match)
    candidates = []

    if(move):
        candidates.append(move)
        score = match.score
    else:
        slimits = SearchLimits(match)
        maximizing = match.next_color() == COLORS['white']
        alpha = SCORES[PIECES['wKg']] * 10
        beta = SCORES[PIECES['bKg']] * 10
        score, candidates = mpcalc(match, 1, slimits, alpha, beta, maximizing, None, candidate)

    msg = "result: " + str(score) + " match: " + str(match.created_at) + " "
    print(msg + concat_fmtmoves(match, candidates))
    prnt_fmttime("\ncalc-time: ", time.time() - time_start)
    return candidates
