

  #ifndef DAEMON_HPP

      #define DAEMON_HPP


      #include <vector>
      #include "./match.hpp"
      #include "./values.hpp"


      using namespace std;


      class cDaemon{

        public:
          cDaemon(cMatch &match);

          bool is_continue(cMatch &match, const cMove &move, const uint8_t depth);

        private:
          uint8_t m_move_cnt;

          uint8_t m_capture_move_cnt;

          uint8_t m_other_move_cnt;

          vector<cMove> m_escape_moves;

          bool m_last_move_was_check;

          bool m_last_move_was_capture;

          bool m_last_move_was_promotion;

          cMove *m_postponed_moveptr;

      };

  #endif