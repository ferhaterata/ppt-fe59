//  ----------------------------------------------------------------------------
//  Header file for the Controller class.                         controller.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef REPAIR_CONTROLLER_HPP
#define REPAIR_CONTROLLER_HPP

#include "inventory.hpp"

// -----------------------------------------------------------------------------
// Controls the repair process
class Controller {
  private:
    Inventory inventory;          // creates an empty Inventory object.
    const std::string inputFile;  // holds input filename
    const std::string outputFile; // holds output filename

    void readFile();  // reads the old inventory file into the inventory object.
    void writeFile(); // the repaired inventory list is written back to disk.

  public:
    Controller(std::string inputFile, std::string outputFile);

    void go(); // orchestrates the repair actions.
};

#endif // REPAIR_CONTROLLER_HPP
