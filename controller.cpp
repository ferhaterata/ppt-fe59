//  ----------------------------------------------------------------------------
//  Implementation file for the Controller class.                 controller.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "controller.hpp"
#include "exception.hpp"
#include "utility.hpp"
#include <fstream>
#include <utility>

// -----------------------------------------------------------------------------
// Constructor that builds
Controller::Controller(std::string inputFile, std::string outputFile)
    : inputFile(std::move(inputFile)), outputFile(std::move(outputFile)) {}

// -----------------------------------------------------------------------------
// Initiates the repair procedure
void Controller::go() {
    readFile();
    inventory = inventory.repair();
    writeFile();
}

// -----------------------------------------------------------------------------
// Reads the old inventory file into the inventory object.
void Controller::readFile() {
    std::ifstream in(inputFile);
    if (in) {            // checks if the file is open
        in >> inventory; // extract an inventory object from the file
    } else {
        throw IOerror("can't open file for reading!");
    }
    utility::print_stream_state(in);
    if (!in.eof()) // ????
        throw IOerror("I/O error or bad data during extraction");
    in.close();
}

// -----------------------------------------------------------------------------
// writes the repaired inventory list back to disk.
void Controller::writeFile() {
    std::ofstream fout(outputFile);
    if (fout)
        fout << inventory;
    else
        throw IOerror("can't open file for writing!");
    fout.close();
}
