


  #ifndef TEST_HPP

    #define TEST_HPP


    #include <cstdint>
    #include <map>
    #include <string>


    using namespace std;


    void do_usr_move(cMatch &match, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece);

    bool import_minutes(cMatch &match, string filename);
    
    void test_eval_piece_state(cMatch &match);

    void test_search_all_dirs_for_touching_pieces(cMatch &match);

    void test_eval_field_state(cMatch &match, const uint8_t src_x, const uint8_t src_y);

    void test_gen2(cMatch &match);
    
    void test_does_move_escape_soft_pin(cMatch &match, const cMove &move);

    void test_is_continue(cMatch &match, const uint8_t depth);

    void test_is_continue_sequence(cMatch &match);

    void test_does_move_touch_soft_pinned(cMatch &match, cMove &move);

    void test_does_move_sac_for_supply(cMatch &match, cMove &move);

  #endif
