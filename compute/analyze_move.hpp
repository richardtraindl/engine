#ifndef ANALYZE_MOVE_HPP
    #define ANALYZE_MOVE_HPP

    #include <list>
    #include "../match.hpp"
    #include "../move.hpp"

    using namespace std;

    void add_tactics(cPrioMove &priomove, cMatch &match, cMove *candidate, cMove *dbggmove, list<cExclude> &excludes);

#endif
