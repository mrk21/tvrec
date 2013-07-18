#pragma once
#include <tssplitter/flexible_container.hpp>
#include <tssplitter/ts/psi/basic_traits.hpp>

namespace tssplitter { namespace ts { namespace psi {
  struct pat: public basic_traits<pat> {
    using transport_stream_id_type       =            last_basic_bitfield::next_bitfield<16>;
    using reserved_2_type                =       transport_stream_id_type::next_bitfield< 2>;
    using version_number_type            =                reserved_2_type::next_bitfield< 5>;
    using current_or_next_indicator_type =            version_number_type::next_bitfield< 1>;
    using section_number_type            = current_or_next_indicator_type::next_bitfield< 8>;
    using last_session_id_type           =            section_number_type::next_bitfield< 8>;
    using static_container_type = last_session_id_type::inclusion_container_type;
    
    struct section_type {
      using program_num_type =                        bitfield<16>;
      using reserved_type    = program_num_type::next_bitfield< 3>;
      using program_pid_type =    reserved_type::next_bitfield<13>;
      using container_type = program_pid_type::inclusion_container_type;
      
      union {
        container_type    data;
        program_num_type  program_num;
        reserved_type     reserved;
        program_pid_type  program_pid;
      };
      
      std::size_t size(void) const;
      bool is_pmt(void) const;
    };
    
    struct section_container_type: public flexible_container<section_container_type, section_type> {
      byteptr base_address(void);
      std::size_t length(void) const;
    };
    
    union {
      PSI_BASIC_FIELD;
      
      transport_stream_id_type        transport_stream_id;
      reserved_2_type                 reserved_2;
      version_number_type             version_number;
      current_or_next_indicator_type  current_or_next_indicator;
      section_number_type             section_number;
      last_session_id_type            last_session_id;
      
      section_container_type  sections;
    };
  };
}}}
