
#ifndef GENERATOR_HPP
    #define GENERATOR_HPP

    #include <map>
    #include <string>
    #include <cstdint>
    #include <list> 
    #include "./helper.hpp"
    #include "./match.hpp"
    #include "./move.hpp"
    #include "./values.hpp"


    class cGenerator{
        public:
            cMatch *match;

            cGenerator(cMatch *match);

            void gen_moves(list<cGMove> &moves);

        private:
            void gen_kg_moves(uint8_t color, list<cGMove> &moves);

            void gen_kg_support_moves(list<cLink *> &attackers, list<cGMove> &moves);

            void add_pw_moves(uint64_t src, uint64_t dst, list<cGMove> &moves);
    };

#endif