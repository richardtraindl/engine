

  #ifndef DAEMON_HPP

      #define DAEMON_HPP


      #include "./match.hpp"
      #include "./values.hpp"


      using namespace std;


      class cDaemon{

        public:
          cMatch *m_match;

          uint8_t m_deep_search_cnt;

          uint8_t m_capture_move_cnt;

          uint8_t m_other_move_cnt;

          bool m_last_move_was_check;

          bool m_last_move_was_capture;

          cDaemon(cMatch &match);

          void reset();

          bool is_continue(const cMove &move, const uint8_t depth);

      };

  #endif
