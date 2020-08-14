//  ----------------------------------------------------------------------------
//  Implementation file for the Inventory class.                   inventory.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "inventory.hpp"
#include "utility.hpp"
#include <algorithm>

// -----------------------------------------------------------------------------
// Inventory class should be an expert at extracting the all records
std::istream& Inventory::read(std::istream& in) {
    do {
        Part part;
        if (in >> part) {          // !in.fail()
            parts.push_back(part); // got good data
            print(part);
        }
        if (in.fail()) {
            print(utility::print_stream_state(in));
            print("(" << part << ")\n");
        }
    } while (in.good());
    return in;
}

// -----------------------------------------------------------------------------
// Inventory class is an expert at serializing all the part data.
// ensures: The output file should have each part on a separate line.
std::ostream& Inventory::write(std::ostream& out) const {
    for (const auto& part : parts) {
        out << part << "\n";
    }
    return out;
}

// -----------------------------------------------------------------------------
// ensures: If the original file contains multiple entries for the same part,
// that part’s quantity on hand in the new file should be the sum over those
// entries of the quantities on hand in the old file. The price of that part in
// the new file should be the maximum price of that part in the old file. There
// should be only one entry for each part. Records should be sorted
// alphabetically by part name
Inventory Inventory::repair() {
    // sort alphabetically based on a string comparison of their names.
    std::sort(parts.begin(), parts.end(), CompareByName());
    print(utility::separator());
    print(*this);
    Inventory repaired;         // creates a new Inventory object to return
    auto first = parts.begin(); // iterator
    auto last = parts.end();    // mark last
    while (first != last) {     // it makes a pass over the old inventory list
        auto head = first++;
        double max = head->price;
        // merges together each block of entries for the same part,
        while (first != last and head->name == first->name) {
            head->quantity += first->quantity;
            if (first->price > max) {
                max = first->price;
            }
            ++first;
        }
        // The price of that part in the new file should be the maximum price of
        // that part in the old file.
        head->price = max;
        // and pushes the resulting parts onto the back of the new inventory
        // object
        repaired.parts.push_back(*head);
    }
    print(utility::separator());
    print(repaired);
    return repaired;
}
