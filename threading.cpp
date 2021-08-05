
    #include "./threading.hpp"


    cThreading::cThreading(cMatch *match, vector<cMove> &moves, int32_t alpha, int32_t beta){
            m_match = match;

            m_pool_moves = moves;
            
            if(m_match->next_color() == mWHITE){
                m_candidate_score = SCORES[mWKG] * 10;
            }
            else{
                m_candidate_score = SCORES[mBKG] * 10;
            }

            m_alpha = alpha;

            m_beta = beta;

    }


    cThreading::~cThreading(){
    }


    void cThreading::start(uint8_t depth, uint8_t maxdepth){

        for(uint8_t i = 0; i < MAXTHREADS; ++i){

            if(m_threads[i].joinable() == false && m_pool_idx + 1 < (uint8_t)m_pool_moves.size()){
                
                m_threading_mutex.lock();

                m_start_time[i] = time(0);

                cMove move = m_pool_moves[m_pool_idx];

                m_pool_idx++;

                cout << "thread started for: " + move.format(true) << endl;

                m_thmatches[i] = new cMatch(*m_match);

                m_thmatches[i]->do_move(move);

                m_threads[i] = thread(&cMatch::calc_alphabeta, ref(m_thmatches[i]), ref(m_thscores[i]), ref(m_thmoves[i]), depth + 1, maxdepth, m_alpha, m_beta);

                m_threading_mutex.unlock();
            }
        }

    }


    bool cThreading::fetch_candidates(int32_t &newscore, vector<cMove> &newmoves){
        
        if(has_finished()){

            m_threading_mutex.lock();

            newscore = m_candidate_score;

            for(cMove move : m_candidate_moves){
                newmoves.push_back(move);
            }

            m_threading_mutex.unlock();

            cout << "___fetch candidates - score: " << newscore << " " + cMatch::fmt_moves(newmoves) << endl;

            return true;

        }
        else{
        
            return false;

        }

    }


    void cThreading::update_candidates(){

        //bool cutoff = false;

        for(uint8_t i = 0; i < MAXTHREADS; ++i){

            if(m_threads[i].joinable() && m_thmatches[i]->m_minutes.size() - 1 == m_match->m_minutes.size()){

                m_threading_mutex.lock();

                m_threads[i].join();

                cMove move = m_thmatches[i]->m_minutes.back();
                
                cout << "...............thread finished for: " + move.format(false);
                cMatch::prnt_fmttime(" time: ", time(0) - m_start_time[i]);

                if(m_match->next_color() == mWHITE){
                    if(m_thscores[i] > m_candidate_score){
                        m_candidate_score = m_thscores[i];

                        m_alpha = max(m_candidate_score, m_alpha);

                        cout << "*****************\nm_candidate_score: "  << m_candidate_score  << " alpha: " << m_alpha << " beta: " << m_beta << "\n*****************" << endl;

                        //if(m_alpha >= m_beta){
                        //    cutoff = true;
                        //}

                        m_candidate_moves.clear();
                        m_candidate_moves.assign(m_thmoves[i].begin(), m_thmoves[i].end());
                        m_candidate_moves.insert(m_candidate_moves.begin(), move);
                    }
                }
                else{
                    if(m_thscores[i] < m_candidate_score){
                        m_candidate_score = m_thscores[i];

                        m_beta = min(m_candidate_score, m_beta);

                        cout << "*****************\nm_candidate_score: "  << m_candidate_score  << " alpha: " << m_alpha << " beta: " << m_beta << "\n*****************" << endl;

                        //if(m_beta <= m_alpha){
                        //    cutoff = true;
                        //}

                        m_candidate_moves.clear();
                        m_candidate_moves.assign(m_thmoves[i].begin(), m_thmoves[i].end());
                        m_candidate_moves.insert(m_candidate_moves.begin(), move);
                    }
                }

                delete m_thmatches[i];

                m_thmoves[i].clear();

                m_threading_mutex.unlock();

                //if(cutoff){
                //    break;
                //}
            }
        }

        //return cutoff;

    }


    bool cThreading::has_finished(){
        
        m_threading_mutex.lock();

        if(m_pool_idx == 0 || m_pool_idx + 1 < (uint8_t)m_pool_moves.size()){

            m_threading_mutex.unlock();

            return false;
        }

        for(uint8_t i = 0; i < MAXTHREADS; ++i){

            if(m_threads[i].joinable()){

                m_threading_mutex.unlock();

                return false;
            }
        }

        m_threading_mutex.unlock();

        return true;

    }


    void cThreading::term(){

        terminate();

        //g_threading_mutex.lock();

        for(uint8_t i = 0; i < MAXTHREADS; ++i){
            //if(m_threads[i].joinable()){

                //m_threads[i].join();

                delete m_thmatches[i];

                m_thmoves[i].clear();

            //}
        }

        //g_threading_mutex.unlock();

    }

