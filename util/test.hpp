


  #ifndef TEST_HPP

    #define TEST_HPP


    #include <cstdint>
    #include <map>
    #include <string>


    using namespace std;


    void do_usr_move(cMatch &match, const uint8_t src_x, const uint8_t src_y, const uint8_t dst_x, const uint8_t dst_y, const uint8_t prompiece);

    bool import_minutes(cMatch &match, string filename);
    
    void test_eval_piece_state(cMatch &match);

    void test_search_all_dirs_for_touching_pieces(cMatch &match);

    void test_eval_field_state(cMatch &match, const uint8_t src_x, const uint8_t src_y);

    void test_gen2(cMatch &match);

    void test_gen2_king(cMatch &match, const uint8_t color);

    void test_does_move_escape_soft_pin(cMatch &match, const cMove &move);

    void test_is_continue(cMatch &match, const uint8_t depth);

    void test_does_move_touch_weak_piece(cMatch &match, const cMove &move);

    void test_is_piece_weak(cMatch &match);

    void test_find_mate(cMatch &match, const cMove &move, const uint8_t maxdepth);

    void test_pins(cMatch &match, const uint8_t src_x, const uint8_t src_y);

  #endif
