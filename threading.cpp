
    #include "./threading.hpp"


    cThreading::cThreading(cMatch *match, vector<cMove> &moves, int32_t alpha, int32_t beta){
            m_match = match;

            m_pool_moves = moves;

            for(uint8_t i = 0; i < MAXTHREADS; ++i){
                m_pool_status[i][0] = -1;
                m_pool_status[i][1] = 0;
            }

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

            if(m_threads[i].joinable() == false && m_pool_idx < (uint8_t)m_pool_moves.size() && m_pool_status[i][1] != 1){

                m_threading_mutex.lock();

                m_start_time[i] = time(0);

                cMove move = m_pool_moves[m_pool_idx];

                m_pool_status[i][0] = m_pool_idx;
                m_pool_status[i][1] = 1;

                m_pool_idx++;

                m_thmatches[i] = new cMatch(*m_match);

                m_thmatches[i]->do_move(move);

                m_threads[i] = thread(&cMatch::calc_alphabeta, ref(m_thmatches[i]), ref(m_thscores[i]), ref(m_thmoves[i]), depth + 1, maxdepth, m_alpha, m_beta);

                cout << to_string(m_pool_idx) + "(" + to_string(m_pool_moves.size()) + ") thread started for: " + move.format(true) << endl;

                m_threading_mutex.unlock();
            }
        }

    }


    void cThreading::start_endgame(uint8_t depth, uint8_t maxdepth, uint8_t status){

        for(uint8_t i = 0; i < MAXTHREADS; ++i){

            if(m_threads[i].joinable() == false && m_pool_idx < (uint8_t)m_pool_moves.size() && m_pool_status[i][1] != 1){

                m_threading_mutex.lock();

                m_start_time[i] = time(0);

                cMove move = m_pool_moves[m_pool_idx];

                m_pool_status[i][0] = m_pool_idx;
                m_pool_status[i][1] = 1;

                m_pool_idx++;

                m_thmatches[i] = new cMatch(*m_match);

                m_thmatches[i]->do_move(move);

                m_threads[i] = thread(&cMatch::calc_alphabeta_endgame, ref(m_thmatches[i]), ref(m_thscores[i]), ref(m_thmoves[i]), depth + 1, maxdepth, m_alpha, m_beta, status);

                cout << to_string(m_pool_idx) + "(" + to_string(m_pool_moves.size()) + ") thread started for: " + move.format(true) << endl;

                m_threading_mutex.unlock();
            }
        }

    }


    void cThreading::start_endgame3(uint8_t depth, uint8_t maxdepth, uint8_t status){

        for(uint8_t i = 0; i < MAXTHREADS; ++i){

            if(m_threads[i].joinable() == false && m_pool_idx < (uint8_t)m_pool_moves.size() && m_pool_status[i][1] != 1){
                
                m_threading_mutex.lock();

                m_start_time[i] = time(0);

                cMove move = m_pool_moves[m_pool_idx];

                m_pool_status[i][0] = m_pool_idx;
                m_pool_status[i][1] = 1;

                m_pool_idx++;

                m_thmatches[i] = new cMatch(*m_match);

                m_thmatches[i]->do_move(move);

                //*********************
                /*
                if(status == 100 && PIECES_COLORS[move.m_srcpiece] == mWHITE){

                    //int32_t score = m_thmatches[i]->eval_pos_for_1xx(&move, depth, status);
                    int32_t score = m_thmatches[i]->eval_move_100(move);

                    if(score != 0){
                        
                        cout << "score: " << to_string(score) << endl;
                        
                        cout << move.format(false) << endl;

                        m_thscores[i] = m_thmatches[i]->m_score + score;

                        if(PIECES_COLORS[move.m_srcpiece] == mWHITE){
                            if(m_thscores[i] > m_candidate_score){
                                
                                cout << move.format(false) << endl;

                                m_candidate_score = m_thscores[i];

                                cout << "\nm_CANDIDATE_score: "  << m_candidate_score  << " alpha: " << m_alpha << " beta: " << m_beta << "\n" << endl;

                                m_candidate_moves.clear();
                                m_candidate_moves.push_back(move);
                            }

                            m_alpha = max(m_candidate_score, m_alpha);
                        }
                        else{
                            if(m_thscores[i] < m_candidate_score){
                                m_candidate_score = m_thscores[i];

                                cout << "\nm_CANDIDATE_score: "  << m_candidate_score  << " alpha: " << m_alpha << " beta: " << m_beta << "\n" << endl;

                                m_candidate_moves.clear();
                                m_candidate_moves.push_back(move);
                            }

                            m_beta = min(m_candidate_score, m_beta);
                        }
                    
                        delete m_thmatches[i];

                        m_thmoves[i].clear();

                        m_threading_mutex.unlock();

                        continue;
                    }

                } */
                
                /*if(status == 100){
                    int32_t score = m_thmatches[i]->eval_move_100(move);

                    m_thmatches[i]->m_score += score;

                    if(move.m_srcpiece == mBKG && score < 0){
                        cout << to_string(m_pool_idx) + "(" + to_string(m_pool_moves.size()) + ") !!!100!!! thread started and finished for: " + move.format(true) << endl;

                        m_thscores[i] = m_thmatches[i]->m_score;

                        if(m_thscores[i] < m_candidate_score){
                            m_candidate_score = m_thscores[i];

                            cout << "\nm_CANDIDATE_score: "  << m_candidate_score  << " alpha: " << m_alpha << " beta: " << m_beta << "\n" << endl;

                            m_candidate_moves.clear();
                            m_candidate_moves.push_back(move);
                        }

                        m_beta = min(m_candidate_score, m_beta);

                        delete m_thmatches[i];

                        m_thmoves[i].clear();

                        m_threading_mutex.unlock();

                        continue;
                    }
                }*/

                //*********************   

                m_threads[i] = thread(&cMatch::calc_alphabeta_endgame, ref(m_thmatches[i]), ref(m_thscores[i]), ref(m_thmoves[i]), depth + 1, maxdepth, m_alpha, m_beta, status);

                cout << to_string(m_pool_idx) + "(" + to_string(m_pool_moves.size()) + ") thread started for: " + move.format(true) << endl;

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

            if(m_threads[i].joinable() && m_thmatches[i]->m_minutes.size() - 1 == m_match->m_minutes.size() && m_pool_status[i][1] == 1){

                m_threading_mutex.lock();

                m_threads[i].join();

                cMove move = m_thmatches[i]->m_minutes.back();

                //***
                //m_thscores[i] += m_thmatches[i]->post_eval_first_move(move);
                //***

                cout << "...............thread finished for: " + move.format(false);
                cMatch::prnt_fmttime(" time: ", time(0) - m_start_time[i]);

                if(m_match->next_color() == mWHITE){
                    if(m_thscores[i] > m_candidate_score){
                        m_candidate_score = m_thscores[i];

                        cout << "\nm_CANDIDATE_score: "  << m_candidate_score  << " alpha: " << m_alpha << " beta: " << m_beta << "\n" << endl;

                        m_candidate_moves.clear();
                        m_candidate_moves.assign(m_thmoves[i].begin(), m_thmoves[i].end());
                        m_candidate_moves.insert(m_candidate_moves.begin(), move);
                    }

                    m_alpha = max(m_candidate_score, m_alpha);

                    //if(m_alpha >= m_beta){
                    //    cutoff = true;
                    //}
                }
                else{
                    if(m_thscores[i] < m_candidate_score){
                        m_candidate_score = m_thscores[i];

                        cout << "\nm_CANDIDATE_score: "  << m_candidate_score  << " alpha: " << m_alpha << " beta: " << m_beta << "\n" << endl;

                        m_candidate_moves.clear();
                        m_candidate_moves.assign(m_thmoves[i].begin(), m_thmoves[i].end());
                        m_candidate_moves.insert(m_candidate_moves.begin(), move);
                    }

                    m_beta = min(m_candidate_score, m_beta);

                    //if(m_beta <= m_alpha){
                    //    cutoff = true;
                    //}
                }

                m_pool_status[i][1] = 2;

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

        for(uint8_t i = 0; i < MAXTHREADS; ++i){

            if(m_threads[i].joinable() && m_thmatches[i]->m_minutes.size() - 1 != m_match->m_minutes.size() && m_pool_status[i][1] == 1){
            //if(m_threads[i].joinable()){

                m_threading_mutex.unlock();

                return false;
            }
        }

        if(m_pool_idx == 0 || m_pool_idx + 1 < (uint8_t)m_pool_moves.size()){
        //if(m_pool_idx <= (uint8_t)m_pool_moves.size()){

            m_threading_mutex.unlock();

            return false;
        }

        m_threading_mutex.unlock();

        return true;

    }


    void cThreading::terminate(){

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

