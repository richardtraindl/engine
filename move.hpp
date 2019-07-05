
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

            cMove(unsigned long long int prevfields[], int src, int dst, int prompiece);
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

            static map<string, int> DOMAINS;
            static map<string, int> WEIGHTS;
            static map<int, int> DOMAINS_TO_PRIOS;
            static map<int, int> WEIGHTS_TO_ADJUST;

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

            cPrioMove(cMove *move, int prio);

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
