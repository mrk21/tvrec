#include <iostream>
#include <tssplitter/arguments.hpp>
extern "C" {
  #include <cspec/cspec.h>
  #include <cspec/cspec_output_verbose.h>
}

#define ARGUMENTS_FIXTURES(...) \
  char _argv[][256] = {__VA_ARGS__};                \
  int argc = sizeof(_argv) / sizeof(*_argv);        \
  char * argv[256];                                 \
  for (int i=0; i < argc; ++i) argv[i] = _argv[i];  \


DESCRIBE(arguments, "tsspliter::arguments")
  using namespace tssplitter;
  
  IT("引数の数が不正だとエラーがでること - 1個")
    ARGUMENTS_FIXTURES(__FILE__);
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), false);
  END_IT
  
  IT("引数の数が不正だとエラーがでること - 2個")
    ARGUMENTS_FIXTURES(__FILE__,"-");
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), false);
  END_IT
  
  IT("引数の数が適正だとエラーが出ないこと - 3個")
    ARGUMENTS_FIXTURES(__FILE__,"-","-");
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), true);
  END_IT
  
  
  IT("入力ファイルが存在しない時はエラーがでること")
    ARGUMENTS_FIXTURES(__FILE__,"hoge","-");
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), false);
  END_IT
  
  IT("入力ファイルがきちんと指定できること")
    ARGUMENTS_FIXTURES(__FILE__,"Makefile","-");
    arguments args(argc, argv);
    args.verify();
    SHOULD_EQUAL(args.input_file, "Makefile");
  END_IT
  
  IT("入力ファイルに'-'が指定されたときは標準入力")
    ARGUMENTS_FIXTURES(__FILE__,"-","-");
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), true);
    SHOULD_EQUAL(args.input_file, "/dev/stdin");
  END_IT
  
  
  IT("出力先にファイルが存在する時はエラーがでること")
    ARGUMENTS_FIXTURES(__FILE__,"-","Makefile");
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), false);
  END_IT
  
  IT("'-f'オプションが指定されているときは出力先にファイルが存在してもエラーにならないこと")
    ARGUMENTS_FIXTURES(__FILE__,"-","Makefile","-f");
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), true);
  END_IT
  
  IT("出力ファイルがきちんと指定できること")
    ARGUMENTS_FIXTURES(__FILE__,"-","a");
    arguments args(argc, argv);
    args.verify();
    SHOULD_EQUAL(args.output_file, "a");
  END_IT
  
  IT("出力ファイルに'-'が指定されたときは標準出力")
    ARGUMENTS_FIXTURES(__FILE__,"-","-");
    arguments args(argc, argv);
    args.verify();
    SHOULD_EQUAL(args.output_file, "/dev/stdout");
  END_IT
  
  IT("出力ファイルに標準出力'/dev/stdout'が指定されたときはファイルが存在してもエラーにならない")
    ARGUMENTS_FIXTURES(__FILE__,"-","/dev/stdout");
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), true);
  END_IT
  
  IT("出力ファイルに'/dev/null'が指定されたときはファイルが存在してもエラーにならない")
    ARGUMENTS_FIXTURES(__FILE__,"-","/dev/null");
    arguments args(argc, argv);
    SHOULD_EQUAL(args.verify(), true);
  END_IT
END_DESCRIBE


int main(void) {
  return CSpec_Run(DESCRIPTION(arguments), CSpec_NewOutputVerbose());
}
