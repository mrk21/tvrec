#pragma once
#include <tssplitter/flexible_container.hpp>
#include <tssplitter/ts/psi/basic_traits.hpp>
#include <tssplitter/ts/psi/descriptor/base.hpp>

namespace tssplitter { namespace ts { namespace psi {
  struct pmt: public basic_traits<pmt> {
    using program_num_type               =            last_basic_bitfield::next_bitfield<16>;
    using reserved_2_type                =               program_num_type::next_bitfield< 2>;
    using version_number_type            =                reserved_2_type::next_bitfield< 5>;
    using current_or_next_indicator_type =            version_number_type::next_bitfield< 1>;
    using section_number_type            = current_or_next_indicator_type::next_bitfield< 8>;
    using last_section_number_type       =            section_number_type::next_bitfield< 8>;
    using reserved_3_type                =       last_section_number_type::next_bitfield< 3>;
    using pcr_pid_type                   =                reserved_3_type::next_bitfield<13>;
    using reserved_4_type                =                   pcr_pid_type::next_bitfield< 4>;
    using program_info_length_type       =                reserved_4_type::next_bitfield<12>;
    using static_container_type = program_info_length_type::inclusion_container_type;
    
    struct descriptor_container_type: public flexible_container<descriptor_container_type, descriptor::base> {
      byteptr base_address(void);
      std::size_t length(void) const;
    };
    
    struct section_type {
      using stream_type_type    =                           bitfield< 8>;
      using reserved_1_type     =    stream_type_type::next_bitfield< 3>;
      using elementary_pid_type =     reserved_1_type::next_bitfield<13>;
      using reserved_2_type     = elementary_pid_type::next_bitfield< 4>;
      using es_info_length_type =     reserved_2_type::next_bitfield<12>;
      using static_container_type = es_info_length_type::inclusion_container_type;
      
      struct container_type: public variable_byte_container<container_type, static_container_type>, public util::union_helper {
        std::size_t runtime_size(void) const {
          return this->member_type<section_type>()->size();
        }
      };
      
      struct descriptor_container_type: public flexible_container<descriptor_container_type, descriptor::base> {
        byteptr base_address(void);
        std::size_t length(void) const;
      };
      
      union {
        container_type  data;
        
        stream_type_type     stream_type;
        reserved_1_type      reserved_1;
        elementary_pid_type  elementary_pid;
        reserved_2_type      reserved_2;
        es_info_length_type  es_info_length;
        
        descriptor_container_type  descriptors;
      };
      
      std::size_t size(void) const;
      bool is_audio_stream(void) const;
      bool is_video_stream(void) const;
    };
    
    struct section_container_type: public flexible_container<section_container_type, section_type> {
      byteptr base_address(void);
      std::size_t length(void) const;
    };
    
    union {
      PSI_BASIC_FIELD;
      
      program_num_type                program_num;
      reserved_2_type                 reserved_2;
      version_number_type             version_number;
      current_or_next_indicator_type  current_or_next_indicator;
      section_number_type             section_number;
      last_section_number_type        last_section_number;
      reserved_3_type                 reserved_3;
      pcr_pid_type                    pcr_pid;
      reserved_4_type                 reserved_4;
      program_info_length_type        program_info_length;
      
      descriptor_container_type  descriptors;
      section_container_type  sections;
    };
  };
}}}
