	
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

	cBrick::cBrick(uint256_t newshape){
		shape = newshape;
		bits1000 = shape & BITS1000;
		tst_blank;
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
