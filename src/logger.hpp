#pragma once
#include <iostream>
#include <vector>
#include <string>

// comma
class HelperLogger {
    public:
            template <typename T>
            HelperLogger & operator,(const T & val) {
                    std::cerr << val;
                    return *this;
            }
 };

#define Log(level,...)  \
    do { (HelperLogger(),level, ":", __VA_ARGS__, '\n'); } while (0)

namespace html {
    extern std::ostream &io;
    void set_highlights( const std::vector< std::string > &hl );
    void readme();
}
