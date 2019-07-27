#ifndef ANALYZE_MOVE_HPP
    #define ANALYZE_MOVE_HPP

    #include <list>
    #include "../match.hpp"
    #include "../move.hpp"

    using namespace std;


    class cExcluded{
        public:
            cPrioMove *priomove;
            cTactic *tactic;
            int prio;
            cExcluded(cPrioMove *priomove, cTactic *tactic, int prio=0);
    };

    void add_tactics(cPrioMove &priomove, cMatch &match, cMove *candidate, cMove *dbggmove, bool search_for_mate, list<cPrioMove> &excludes);

#endif
