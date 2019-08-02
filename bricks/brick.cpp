
    #include "./brick.hpp"

    const uint256_t cBrick::BITS1111 = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_cppui;
    const uint256_t cBrick::BITS1000 = 0x8888888888888888888888888888888888888888888888888888888888888888_cppui;


      cBrick::cBrick(uint256_t newshape){
            shape = newshape;
            bits1000 = shape & BITS1000;
      }


    bool cBrick::tst_whites(uint256_t fields){
        uint256_t cuts = fields & shape;
        return cuts > 0 && (cuts & bits1000) == 0;
    }


    bool cBrick::tst_blacks(uint256_t fields){
            uint256_t cuts = fields & shape;
            return (cuts & bits1000) > 0;
     }


      bool cBrick::tst_blanks(uint256_t fields){
            uint256_t cuts = fields & shape;
            return cuts == 0;
      }

