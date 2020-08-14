//  ----------------------------------------------------------------------------
//  Header file for the IOerror class.                             exception.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 12, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef REPAIR_EXCEPTION_HPP
#define REPAIR_EXCEPTION_HPP

#include <exception>
#include <string>
#include <utility>

// -----------------------------------------------------------------------------
// IOerror class is derived from std::exception; std::exception has virtual
// destructor and virtual "exception::what()" method that returns a C-style
// character string describing the general cause of the current error.
class IOerror : public std::exception {
  private:
    const std::string message;

  public:
    explicit IOerror(std::string message) : message(std::move(message)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return message.data();
    }
};

#endif // REPAIR_EXCEPTION_HPP
