#ifndef THREADING_HPP
    #define THREADING_HPP


    #include <array>
    #include <thread>
    #include <mutex>
    #include "./match.hpp"


    const uint8_t MAXTHREADS = 4;


    using namespace std;


    class cThreading{
        private:
            mutex m_threading_mutex;

            cMatch *m_match;

            vector<cMove> m_pool_moves;

            uint8_t m_pool_idx = 0;

            int8_t m_pool_status[MAXTHREADS][2];

            thread m_threads[MAXTHREADS];

            array<vector<cMove>, MAXTHREADS> m_thmoves;

            array<cMatch*, MAXTHREADS> m_thmatches;

            int32_t m_thscores[MAXTHREADS];

            time_t m_start_time[MAXTHREADS];

            int32_t m_candidate_score;

            int32_t m_alpha;

            int32_t m_beta;

            vector<cMove> m_candidate_moves;

        public:
            cThreading(cMatch *match, vector<cMove> &moves, int32_t alpha, int32_t beta);

            ~cThreading();

            void start(const uint8_t depth, const uint8_t maxdepth, const uint8_t stage);

            bool fetch_candidates(int32_t &newscore, vector<cMove> &newmoves);

            void update_candidates();

            bool has_finished();

            void terminate();

    };

#endif
