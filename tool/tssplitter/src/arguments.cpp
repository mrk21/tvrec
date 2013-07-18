#include <tssplitter/arguments.hpp>

namespace tssplitter {
  arguments::arguments(int argc, char ** argv) {
    for (int i=0; i < argc; ++i) {
      this->holder.push_back(argv[i]);
    }
  }
  
  bool arguments::verify(void) {
    struct stat sb;
    this->errors.empty();
    
    if (this->holder.size() < 3) {
      this->errors.push_back("引数の数が不正です");
      return false;
    }
    
    // input file
    if (this->holder[1] == "-") {
      this->input_file = "/dev/stdin";
    }
    else if (stat(this->holder[1].c_str(), &sb) == -1) {
      this->errors.push_back("入力ファイルが存在しません!");
    }
    else {
      this->input_file = this->holder[1];
    }
    
    // output file
    if (this->holder[2] == "-" || this->holder[2] == "/dev/stdout") {
      this->output_file = "/dev/stdout";
    }
    else if (this->holder[2] == "/dev/null" || stat(this->holder[2].c_str(), &sb) == -1 || (this->holder.size() == 4 && this->holder[3] == "-f")) {
      this->output_file = this->holder[2];
    }
    else {
      this->errors.push_back("出力先にファイルが存在します!");
    }
    
    return this->errors.size() == 0;
  }
}
