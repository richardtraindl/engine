
#ifndef HELPER_HPP
    #define HELPER_HPP

    #include <string>
    #include <map>

    using namespace std;

    int coord_to_index(string coord);

    string index_to_coord(int idx);

    string reverse_lookup(const map<string, int>& dict, int value);
#endif
