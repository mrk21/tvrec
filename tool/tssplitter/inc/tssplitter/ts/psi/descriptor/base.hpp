#pragma once
#include <tssplitter/ts/psi/descriptor/basic_traits.hpp>

namespace tssplitter { namespace ts { namespace psi { namespace descriptor {
  struct base: public basic_traits<base> {
    using static_container_type = last_basic_bitfield::inclusion_container_type;
    
    union {
      DESCRIPTOR_BASIC_FIELD;
    };
  };
}}}}
