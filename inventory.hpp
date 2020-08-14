//  ----------------------------------------------------------------------------
//  Header file for the Inventory class.                           inventory.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef REPAIR_INVENTORY_HPP
#define REPAIR_INVENTORY_HPP

#include "part.hpp"
#include <vector>

// -----------------------------------------------------------------------------
// Contains a list of parts, represented by a vector, as well as functions for
// reading and writing inventory files and for repairing the parts list.
class Inventory {
  private:
    std::vector<Part> parts; // initializes an empty vector of parts.

  public:
    Inventory() = default;

    std::ostream& write(std::ostream& out) const;
    std::istream& read(std::istream& in); // extracts parts and fills the vector

    Inventory repair(); // repair the parts and returns a new inventory object
};

// deserialize an inventory from input stream
inline std::istream& operator>>(std::istream& in, Inventory& inventory) {
    return inventory.read(in);
}

// serialize an inventory to output stream
inline std::ostream& operator<<(std::ostream& out, const Inventory& inventory) {
    return inventory.write(out);
}

// -----------------------------------------------------------------------------
// a functor that orders two Part objects alphabetically based on a string
// comparison of their names.
class CompareByName {
  public:
    bool operator()(const Part& lhs, const Part& rhs) {
        return (lhs.name < rhs.name);
    }
};

#endif // REPAIR_INVENTORY_HPP
