//  ----------------------------------------------------------------------------
//  Main file for the inventory program.                                main.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "controller.hpp"
#include "exception.hpp"
#include "utility.hpp"

// -----------------------------------------------------------------------------
// runs the repair program
void run(int argc, char* argv[]);

// -----------------------------------------------------------------------------
// prints instructions
static void instructions();

// -----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    utility::banner();
    try {
        run(argc, argv);
    } catch (IOerror& e) {
        std::cerr << "\nCatching IO error...\n\n\t" << e.what() << "\n";
    } catch (std::runtime_error& e) {
        std::cerr << "\nCatching Runtime Error...\n\n\t" << e.what() << "\n";
    } catch (std::bad_alloc& e) { // handle memory exhaustion
        std::cerr << "\nCatching Allocation error...\n\n\t" << e.what() << "\n";
        return 1; // exit main
    } catch (...) {
        std::cerr << "Uncaught exception..." << std::endl;
        throw;
    }
    utility::bye();
}

// -----------------------------------------------------------------------------
// initiates the controller
void run(int argc, char* argv[]) {
    if (argc == 3) {
        //  instantiate the controller class with the command line arguments.
        Controller controller(argv[1], argv[2]);
        controller.go();
        std::cout << "\tThe inventory file is successfully repaired.\n";
    } else {
        std::cout << "Invalid number of arguments: " << argc - 1 << "\n";
        instructions();
    }
}

// -----------------------------------------------------------------------------
// prints the instructions
static void instructions() {
    std::cout << "\n\tUsage: repair [INPUT FILE] [OUTPUT FILE]" << std::endl;
}