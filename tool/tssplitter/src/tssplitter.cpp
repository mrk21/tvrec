#include <iostream>
#include <tssplitter/arguments.hpp>
#include <tssplitter/ts/splitter.hpp>

int main(int argc, char ** argv) {
  using namespace tssplitter;
  
  arguments args(argc, argv);
  
  if (!args.verify()) {
    for (auto error: args.errors) {
      std::cerr << error << std::endl;
    }
    return 1;
  }
  
  ts::splitter splitter(args.input_file, args.output_file);
  return splitter.execute();
}
