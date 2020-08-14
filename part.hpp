//  ----------------------------------------------------------------------------
//  Header file for the Part class.                                     part.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef REPAIR_PART_HPP
#define REPAIR_PART_HPP

#include <iostream>
#include <string>

// -----------------------------------------------------------------------------
// A company sells many different kinds of parts
struct Part {
    std::string name;   // part's name
    int quantity = 0;   // the quantity on hand
    double price = 0.0; // the selling price

    std::istream& read(std::istream& in);
    std::ostream& write(std::ostream& out) const;
};

// deserialize a part from input stream
inline std::istream& operator>>(std::istream& in, Part& part) {
    return part.read(in);
}

// serialize a part to output stream
inline std::ostream& operator<<(std::ostream& out, const Part& part) {
    return part.write(out);
}

#endif // REPAIR_PART_HPP
