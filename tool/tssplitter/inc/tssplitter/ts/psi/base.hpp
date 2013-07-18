#pragma once
#include <tssplitter/ts/psi/basic_traits.hpp>
#include <tssplitter/ts/psi/pat.hpp>
#include <tssplitter/ts/psi/pmt.hpp>

namespace tssplitter { namespace ts { namespace psi {
  struct base: public basic_traits<base> {
    using static_container_type = last_basic_bitfield::inclusion_container_type;
    
    union {
      PSI_BASIC_FIELD;
      
      psi::pat pat;
      psi::pmt pmt;
    };
  };
}}}
