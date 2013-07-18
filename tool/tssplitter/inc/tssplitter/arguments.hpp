#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <tssplitter/util.hpp>

namespace tssplitter {
  struct arguments {
    std::vector<std::string> holder;
    std::vector<std::string> errors;
    
    std::string input_file;
    std::string output_file;
    
    arguments(int argc, char ** argv);
    bool verify(void);
  };
}
