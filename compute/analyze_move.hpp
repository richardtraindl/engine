#ifndef ANALYZE_MOVE_HPP
    #define ANALYZE_MOVE_HPP

    #include <list>
    #include "../match.hpp"
    #include "../move.hpp"
    #include "../pieces/piece.hpp"

    using namespace std;

    void add_tactics(cPrioMove &priomove, cMatch &match, cMove *candidate, cMove *dbggmove, list<cExclude> &excludes);

    bool find_excluded(list<cExclude> excludes, int pos, int touch_pos, int tactic_domain);

#endif
