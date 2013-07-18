#include <iostream>
#include <tssplitter/filename.hpp>
extern "C" {
  #include <cspec/cspec.h>
  #include <cspec/cspec_output_verbose.h>
}

DESCRIBE(filename, "tssplitter::filename")
  using namespace tssplitter;
  std::string basename = "tmp/out/とある科学の超電磁砲S";
  std::string name = "tmp/out/とある科学の超電磁砲S.ts";
  
  IT("標準入力のときはファイル名を置換できない")
    filename fname("/dev/stdin");
    SHOULD_EQUAL(fname.is_replacable(), false);
  END_IT
  
  IT("標準出力のときはファイル名を置換できない")
    filename fname("/dev/stdout");
    SHOULD_EQUAL(fname.is_replacable(), false);
  END_IT
  
  IT("/dev/nullのときはファイル名を置換できない")
    filename fname("/dev/null");
    SHOULD_EQUAL(fname.is_replacable(), false);
  END_IT
  
  
  IT("名前を deep copy して取得できること")
    filename fname(name);
    std::string str = fname.get_name();
    str += "a";
    SHOULD_EQUAL(fname.name == str, false);
  END_IT
  
  
  IT("置換できること")
    filename fname(name);
    SHOULD_EQUAL(fname.replace(".ts","_1.ts"), basename + "_1.ts");
  END_IT
  
  IT("置換が破壊的で無いこと")
    filename fname(name);
    fname.replace(".ts","_1.ts");
    SHOULD_EQUAL(fname.name, name);
  END_IT
  
  IT("置換できないときは元の文字列が帰ってくること")
    filename fname(name);
    SHOULD_EQUAL(fname.replace("fuga","bar"), name);
  END_IT
END_DESCRIBE

int main(void) {
  return CSpec_Run(DESCRIPTION(filename), CSpec_NewOutputVerbose());
}
