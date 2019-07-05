
#ifndef MOVE_HPP
    #define MOVE_HPP

    #include <map>
    #include <list> 
    #include <string>

    using namespace std;

    class cMove{
        public:
            unsigned long long int prevfields[4];
            int src;
            int dst;
            int prompiece;

            cMove(int *prevfields, int src, int dst, int prompiece);
            cMove();

            int getPrevfield(int idx);

            string format();
    };


    class cTactic{
        // private:
        public:
            int domain;
            int weight;
            int addition;

            map<string, int> DOMAINS = {
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
                {"is-undefined",          220}
            };

            map<string, int> WEIGHTS = {
                {"stormy", 1},
                {"better-deal", 2},
                {"good-deal", 3},
                {"downgraded", 4},
                {"upgraded", 5},
                {"bad-deal", 6}
            };

            map<int, int> DOMAINS_TO_PRIOS = {
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
                {DOMAINS["is-undefined"],           500}
            };

            map<int, int> WEIGHTS_TO_ADJUST = {
                {WEIGHTS["stormy"],      -70},
                {WEIGHTS["better-deal"], -10},
                {WEIGHTS["good-deal"],     0},
                {WEIGHTS["upgraded"],      0},
                {WEIGHTS["downgraded"],   60},
                {WEIGHTS["bad-deal"],    130}
            };

            cTactic(int domain, int weight, int addition);
    };


    class cPrioMove : public cMove{
        // private:
        public:
            list<cTactic> tactics;
            int prio;
            
            map<string, int> PRIOS = {
                {"prio0", 100},
                {"prio1", 200},
                {"prio2", 250},
                {"prio3", 300} 
            };

            cPrioMove(cMove *move);

            void evaluate_priority();

            void downgrade(int domain);

            void upgrade(int domain);

            int fetch_weight(int domain);

            bool has_domain(int domain);

            bool has_weight(int weight);

            bool has_tactic_ext(cTactic tactic);

            bool is_tactic_stormy();

            string concat_fmttactics();
    };

#endif
