#ifndef THREADING_HPP
    #define THREADING_HPP


    #include <thread>
    #include <mutex>
    #include "./match.hpp"


    const uint8_t MAXTHREADS = 4;


    using namespace std;


    class cThreading{
        private:
            cMatch *m_match;

            vector<cMove> m_pool_moves;

            uint8_t m_pool_idx = 0;

            thread m_threads[MAXTHREADS];

            array<vector<cMove>, MAXTHREADS> m_thmoves;

            array<cMatch*, MAXTHREADS> m_thmatches;

            int32_t m_thscores[MAXTHREADS];

            int32_t m_candidate_score;

            vector<cMove> m_candidate_moves;

        public:
            cThreading(cMatch *match, vector<cMove> &moves);

            ~cThreading();

            void start(uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta);

            bool fetch_candidates(int32_t &newscore, vector<cMove> &newmoves);

            bool update_candidates(int32_t &alpha, int32_t &beta);

            bool has_finished();

            void term();

    };

#endif
