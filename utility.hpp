//  ----------------------------------------------------------------------------
//  Header file for the utility classes.                             utility.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef UTILITY_HPP
#define UTILITY_HPP

#ifdef DEBUG

#define print(a) std::cout << a << std::endl

#else
#define DEBUG 0
#define print(a)
#endif

#include <string>

namespace utility {

// -----------------------------------------------------------------------------
// Prints a neat header on the output.
void banner();

// -----------------------------------------------------------------------------
//  Prints termination message.
void bye();
void ferhat();
// -----------------------------------------------------------------------------
//  Generates a separator string
std::string separator();

// -----------------------------------------------------------------------------
// Prints stream state for debugging purposes.
std::string print_stream_state(const std::ios& stream);

} // namespace utility

#endif // UTILITY_HPP
