

  #ifndef DAEMON_HPP

      #define DAEMON_HPP


      #include "./match.hpp"
      #include "./values.hpp"


      using namespace std;


      class cDaemon{

        public:
          uint8_t m_move_cnt;

          uint8_t m_capture_move_cnt;

          uint8_t m_single_capture_move_prio;

          uint8_t m_other_move_cnt;

          bool m_last_move_was_check;

          bool m_last_move_was_capture;

          bool m_last_move_was_promotion;

          cDaemon(cMatch &match);

          bool is_continue(cMatch &match, const cMove &move, const uint8_t depth, const uint8_t count);

      };

  #endif