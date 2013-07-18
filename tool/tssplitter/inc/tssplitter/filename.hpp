#pragma once
#include <iostream>
#include <string>
#include <tssplitter/util.hpp>

namespace tssplitter {
  struct filename {
    std::string name;
    
    filename(std::string name);
    bool is_replacable(void) const;
    std::string get_name(void) const;
    std::string replace(std::string from, std::string to) const;
  };
}
