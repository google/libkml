// Copyright 2009, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the Prompt class.

#ifndef EXAMPLES_HELLONET_PROMPT_H__
#define EXAMPLES_HELLONET_PROMPT_H__

#include <termios.h>
#include <iostream>
#include <string>

// This class provides some static methods useful for prompting on a terminal.
class Prompt {
 public:
  static std::string PromptAndInputWithNoEcho(const std::string& prompt) {
    std::cin.get();
    std::cout << prompt;
    std::cout << std::flush;
    // Get the current state of the input terminal.
    struct termios t;
    tcgetattr(0, &t);
    // Flip off the ECHO bit.
    t.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &t);
    // User can now type password safely.
    std::string input;
    std::getline(std::cin, input);
    // Flip ECHO back on.
    t.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &t);
    std::cout << std::endl;
    return input;
  }
  
  static bool PromptAgain(const std::string& prompt) {
    std::cout << prompt;
    std::string again;
    std::getline(std::cin, again);
    return !again.empty() && again[0] == 'y';
  }
  
};
  
#endif  // EXAMPLES_HELLONET_PROMPT_H__
