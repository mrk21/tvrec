#include <iostream>
#include <tssplitter/bitfield.hpp>
extern "C" {
  #include <cspec/cspec.h>
  #include <cspec/cspec_output_verbose.h>
}

struct bitfield_test_type {
  using version_type =        tssplitter::bitfield<8>;
  using is1_type     = version_type::next_bitfield<1>;
  using is2_type     =     is1_type::next_bitfield<1>;
  using value_type   =     is2_type::next_bitfield<12>;
  using value2_type  =   value_type::next_bitfield<2>;
  using container_type = value2_type::inclusion_container_type;
  
  union {
    container_type data;
    
    version_type  version;
    is1_type      is1;
    is2_type      is2;
    value_type    value;
    value2_type   value2;
  };
};

DESCRIBE(bitfield, "tsspliter::bitfield")
  bitfield_test_type target;
  uint32_t version;
  uint32_t is1;
  uint32_t is2;
  uint32_t value;
  uint32_t value2;
  
  auto setup = [&](){
    version = 16;
    is1 = 1;
    is2 = 0;
    value = 2048;
    value2 = 1;
    target.data[0] = version;
    target.data[1] = (is1 << 7) | (is2 << 6) | (value >> 6);
    target.data[2] = ((value << 2) & 0xFF) | value2;
  };
  
  auto equals = [&](){
    std::cout << target.data << std::endl;
    std::cout << "version: " << version << std::endl;
    std::cout << "is1: " << is1 << std::endl;
    std::cout << "is2: " << is2 << std::endl;
    std::cout << "value: " << value << std::endl;
    std::cout << "value2: " << value2 << std::endl;
    
    SHOULD_EQUAL(target.version, version);
    SHOULD_EQUAL(target.is1, is1)
    SHOULD_EQUAL(target.is2, is2)
    SHOULD_EQUAL(target.value, value)
    SHOULD_EQUAL(target.value2, value2)
  };
  
  IT("値がきちんと設定されているか")
    setup();
    equals();
  END_IT
  
  IT("versionに代入しても大丈夫か")
    setup();
    target.version = version = 200;
    equals();
  END_IT
  
  IT("is1に代入しても大丈夫か")
    setup();
    target.is1 = is1 = 0;
    equals();
  END_IT
  
  IT("is2に代入しても大丈夫か")
    setup();
    target.is2 = is2 = 1;
    equals();
  END_IT
  
  IT("valueに代入しても大丈夫か")
    setup();
    value = 111227;
    target.value = value;
    value &= 0xFFF;
    equals();
  END_IT
END_DESCRIBE

int main(void) {
  return CSpec_Run(DESCRIPTION(bitfield), CSpec_NewOutputVerbose());
}
