//  ----------------------------------------------------------------------------
//  Implementation file for the utility classes.                     utility.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "utility.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>

void utility::banner() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::cout << "\n" << separator();
    std::cout << "\t"
              << "Ferhat Erata"
              << "\n\t"
              << "CPSC 427/527"
              << "\n\t" << std::put_time(&tm, "%c %Z");
    std::cout << "\n" << separator() << std::endl;
}

void utility::bye() {
    std::cout << "\n" << separator() << "Normal termination.\n";
}

std::string utility::separator() {
    return "---------------------------------------------------------------\n";
}

std::string utility::print_stream_state(const std::ios& stream) {
    std::stringstream ss;
    ss << " good()=" << stream.good();
    ss << " eof()=" << stream.eof();
    ss << " fail()=" << stream.fail();
    ss << " bad()=" << stream.bad() << std::endl;
    return ss.str();
}