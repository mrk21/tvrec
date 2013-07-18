#include <tssplitter/filename.hpp>

namespace tssplitter {
  filename::filename(std::string name) : name(name) {}
  
  bool filename::is_replacable(void) const {
    return this->name != "/dev/stdin" && this->name != "/dev/stdout" && this->name != "/dev/null";
  }
  
  std::string filename::get_name(void) const {
    std::string result;
    for (auto c: this->name) result.push_back(c);
    return result;
  }
  
  std::string filename::replace(std::string from, std::string to) const {
    std::string result = this->get_name();
    if (!this->is_replacable()) return result;
    
    auto first = result.rfind(from);
    if (first == std::string::npos) return result;
    
    auto last = result.size();
    return result.replace(first, last, to);
  }
}
