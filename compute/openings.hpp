#ifndef OPENINGS_HPP
    #define OPENINGS_HPP

    #include <ctime>
    #include <cstdint>
    #include <vector>
    #include <string>
    #include "../match.hpp"
    #include "../move.hpp"

    using namespace std;

    class cNode{
        public:
            uint64_t fields[4];
            vector<string> candidates;

            cNode(uint64_t fields[4], vector<string> &candidates);
    };


    class cOpenings{
        public:
            static const int MAXDEPTH = 4;
            array<vector<cNode>, 4> stages;
            //array<array<int, 3>, 3> arr = {{{5, 8, 2}, {8, 3, 1}, {5, 3, 9}}};

            cOpenings();
            void add_node(cNode &node, int depth);
    };


    cMove *retrieve_move(cMatch &match);

    cOpenings *populate_openings();

#endif
