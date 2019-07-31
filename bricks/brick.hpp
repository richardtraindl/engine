	
#ifndef BRICK_HPP
    #define BRICK_HPP    

    #include <boost/multiprecision/cpp_int.hpp>

    using namespace boost::multiprecision;
    using namespace boost::multiprecision::literals;
    using namespace std;

	  class cBrick{
		    public:
			      static const uint256_t BITS1111 = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_cppui;
			      static const uint256_t BITS1000 = 0x8888888888888888888888888888888888888888888888888888888888888888_cppui;
			      uint256_t shape;
			      uint256_t bits1000;

			      cBrick(uint256_t shape);

			      bool tst_whites(uint256_t fields);

			      bool tst_blacks(uint256_t fields);

			      bool tst_blanks(uint256_t fields);
    };

#endif
