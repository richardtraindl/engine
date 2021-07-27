
    #include "./threading.hpp"


    mutex g_threading_mutex;


    cThreading::cThreading(cMatch *match, vector<cMove> &moves){
            m_match = match;

            m_pool_moves = moves;

            m_pool_idx = 0;

            if(m_match->next_color() == mWHITE){
                m_candidate_score = SCORES[mWKG] * 10;
            }
            else{
                m_candidate_score = SCORES[mBKG] * 10;
            }

    }


    cThreading::~cThreading(){
    }


    void cThreading::start_calc(uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta){

        for(uint8_t i = 0; i < MAXTHREADS; ++i){

            if(m_threads[i].joinable() == false && m_pool_idx + 1 < (uint8_t)m_pool_moves.size()){

                g_threading_mutex.lock();

                cMove move = m_pool_moves[m_pool_idx];

                m_pool_idx++;

                cout << "thread started for: " + move.format() << endl;

                m_thmatches[i] = new cMatch(*m_match);

                m_thmatches[i]->do_move(move);

                m_threads[i] = thread(&cMatch::calc_alphabeta, ref(m_thmatches[i]), ref(m_thscores[i]), ref(m_thmoves[i]), depth + 1, maxdepth, alpha, beta);

                g_threading_mutex.unlock();
            }
        }

    }


    bool cThreading::fetch_candidates(int32_t &newscore, vector<cMove> &newmoves){
        
        if(has_finished()){
            
            newscore = m_candidate_score;

            for(cMove move : m_candidate_moves){
                newmoves.push_back(move);
            }

            return true;

        }
        else{
        
            return false;

        }

    }


    bool cThreading::update_candidates(int32_t &alpha, int32_t &beta){

        bool cutoff = false;

        for(uint8_t i = 0; i < MAXTHREADS; ++i){

            if(m_threads[i].joinable() && m_thmatches[i]->m_minutes.size() - 1 == m_match->m_minutes.size()){
                g_threading_mutex.lock();

                m_threads[i].join();

                cMove move = m_thmatches[i]->m_minutes.back();

                if(m_match->next_color() == mWHITE){
                    if(m_thscores[i] > m_candidate_score){
                        m_candidate_score = m_thscores[i];

                        alpha = max(m_candidate_score, alpha);

                        if(alpha >= beta){
                            cutoff = true;
                        }

                        m_candidate_moves.clear();
                        m_candidate_moves.assign(m_thmoves[i].begin(), m_thmoves[i].end());
                        m_candidate_moves.insert(m_candidate_moves.begin(), move);
                    }
                }
                else{
                    if(m_thscores[i] < m_candidate_score){
                        m_candidate_score = m_thscores[i];

                        beta = min(m_candidate_score, beta);

                        if(beta <= alpha){
                            cutoff = true;
                        }

                        m_candidate_moves.clear();
                        m_candidate_moves.assign(m_thmoves[i].begin(), m_thmoves[i].end());
                        m_candidate_moves.insert(m_candidate_moves.begin(), move);
                    }
                }

                delete m_thmatches[i];

                m_thmoves[i].clear();

                g_threading_mutex.unlock();

                if(cutoff){
                    break;
                }
            }
        }

        return cutoff;

    }


    bool cThreading::has_finished(){

        if(m_pool_idx + 1 < (uint8_t)m_pool_moves.size()){
            return false;
        }

        for(uint8_t i = 0; i < MAXTHREADS; ++i){
            if(m_threads[i].joinable()){
                return false;
            }
        }
        
        return true;

    }
