//  ----------------------------------------------------------------------------
//  Implementation file for the Part class.                             part.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "part.hpp"
#include "exception.hpp"
#include <iomanip>

// -----------------------------------------------------------------------------
// Part class should be an expert at extracting the part record from the file
std::istream& Part::read(std::istream& in) {
    in >> name >> quantity >> price;
    if (in.good() and quantity < 0) {
        std::stringstream ss;
        ss << "(" << *this << "):"
           << " quantity cannot be negative!";
        throw IOerror(ss.str());
    }
    return in;
}

// -----------------------------------------------------------------------------
// Part is an expert at serializing its fields. The fields comprising a part
// should be separated by a single space. The price should be printed as a
// decimal number with two digits after the decimal point
std::ostream& Part::write(std::ostream& out) const {
    return out << std::fixed << name << " " << quantity << " "
               << std::setprecision(2) << price;
}
