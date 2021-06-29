
#ifndef HELPER_HPP
    #define HELPER_HPP

    #include <cstdint>
    #include <iomanip>
    #include <iostream>
    #include <map>
    #include <sstream>
    #include <string>

    using namespace std;


    uint64_t coord_to_pos(string coord);

    string pos_to_coord(uint64_t pos);

    string reverse_lookup(const map<string, uint8_t>& dict, uint8_t value);

    void prnt_fmttime(string msg, int seconds);

    void prnt_16hex(uint64_t pos);
#endif
