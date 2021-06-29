#ifndef ANALYZE_POSITION_HPP
    #define ANALYZE_POSITION_HPP

    #include <map>
    #include <string>
    #include <cstdint>
    #include <list> 
    #include "./helper.hpp"
    #include "./generator.hpp"
    #include "./match.hpp"
    #include "./move.hpp"
    #include "./values.hpp"


    class cCandidate_new{
        public:
            uint8_t min_matches_cnt;

            uint8_t max_matches_cnt;

            uint64_t field[8];

            list <uint64_t> blanks;

            cCandidate_new(uint8_t min_matches_cnt, 
                           uint8_t max_matches_cnt, 
                           uint64_t *field, 
                           list<uint64_t> &blanks);

            cCandidate_new();

            uint8_t calc_matches_cnt(uint64_t *field);
    };


    class cCandidate{
        public:
            uint64_t field[8];

            list <uint64_t> blanks;

            cCandidate(uint64_t *field, list<uint64_t> &blanks);

            cCandidate();

            bool find(cMatch &match, list<cGMove *> &rcandidates);

            bool _find(cMatch &match, list<uint64_t *> &dstpositions, uint8_t depth, list<cGMove *> &rcandidates);
    };


    class cCandidate_ori{
        public:
            list<uint64_t *> srcpositions;
            list<uint64_t *> dstpositions;

            cCandidate_ori(list<uint64_t *> &srcpositions, list<uint64_t *> &dstpositions);

            cCandidate_ori();

            bool find(cMatch &match, list<cGMove *> &rcandidates);

            bool _find(cMatch &match, list<uint64_t *> &dstpositions, uint8_t depth, list<cGMove *> &rcandidates);
    };


    class cSearchCandidates_new{
        public:
            list<cCandidate_new> white_search_candidates;

            list<cCandidate_new> black_search_candidates;

            cSearchCandidates_new();

            bool search(cMatch &match, uint8_t color, list<cGMove *> &rcandidates);
    };


    class cSearchCandidates{
        public:
            list<cCandidate_ori> white_search_candidates;

            list<cCandidate_ori> black_search_candidates;

            cSearchCandidates();

            bool search(cMatch &match, uint8_t color, list<cGMove *> &rcandidates);
    };


#endif