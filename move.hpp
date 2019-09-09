
#ifndef MOVE_HPP
    #define MOVE_HPP

    #include <map>
    #include <list> 
    #include <string>
    #include <cstdint>

    using namespace std;

    class cMove{
        public:
            uint64_t prevfields[4];
            int src;
            int dst;
            int prompiece;

            cMove(uint64_t *prevfields, int src, int dst, int prompiece);
            cMove();

            int getprevfield(int idx);

            string format();
    };


    class cTactic{
        public:
            static map<string, int> DOMAINS;
            static map<string, int> WEIGHTS;
            static map<int, int> DOMAINS_TO_PRIOS;
            static map<int, int> WEIGHTS_TO_ADJUST;

            int domain;
            int weight;
            int addition;

            cTactic(int domain, int weight, int addition);
    };


    class cPrioMove : public cMove{
        public:
            list<cTactic *> tactics;
            int prio;
            static map<string, int> PRIOS;

            cPrioMove(uint64_t *prevfields, int src, int dst, int prompiece, int prio);

            void evaluate_priority();

            void downgrade(int domain);

            void upgrade(int domain);

            int fetch_weight(int domain);

            bool has_domain(int domain);

            bool has_weight(int weight);

            bool has_tactic_ext(int domain, int weight);

            bool is_tactic_stormy();

            string fmt_tactics();
    };

#endif
