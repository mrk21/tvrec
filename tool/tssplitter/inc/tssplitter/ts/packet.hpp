#pragma once
#include <tssplitter/bitfield.hpp>
#include <tssplitter/ts/basic_traits.hpp>
#include <tssplitter/ts/psi/base.hpp>

namespace tssplitter { namespace ts {
  struct packet {
    constexpr static auto SIZE = PACKET_SIZE;
    using container_type = byte_container<SIZE>;
    
    struct header_type {
      using sync_byte_type                    =                                         bitfield< 8>;
      using transport_error_indicator_type    =                    sync_byte_type::next_bitfield< 1>;
      using payload_unit_start_indicator_type =    transport_error_indicator_type::next_bitfield< 1>;
      using transport_priority_type           = payload_unit_start_indicator_type::next_bitfield< 1>;
      using pid_type                          =           transport_priority_type::next_bitfield<13>;
      using scrambling_control_type           =                          pid_type::next_bitfield< 2>;
      using adaptation_field_exist_type       =           scrambling_control_type::next_bitfield< 2>;
      using continuity_counter_type           =       adaptation_field_exist_type::next_bitfield< 4>;
      using container_type = continuity_counter_type::inclusion_container_type;
      
      enum adaptation_field_status_type {
        PAYLOAD_ONLY          = 0b01,
        ADEPTION_ONLY         = 0b10,
        ADEPTION_WITH_PAYLOAD = 0b11
      };
      
      struct adaption_field_type {
        using adaptation_field_length_type               =                                                 bitfield<8>;
        using discontinuity_indicator_type               =              adaptation_field_length_type::next_bitfield<1>;
        using random_access_indicator_type               =              discontinuity_indicator_type::next_bitfield<1>;
        using elementary_stream_priority_indicator_type  =              random_access_indicator_type::next_bitfield<1>;
        using pcr_flag_type                              = elementary_stream_priority_indicator_type::next_bitfield<1>;
        using opcr_flag_type                             =                             pcr_flag_type::next_bitfield<1>;
        using splicing_point_flag_type                   =                            opcr_flag_type::next_bitfield<1>;
        using transport_private_data_flag_type           =                  splicing_point_flag_type::next_bitfield<1>;
        using adaptation_field_extension_flag_type       =          transport_private_data_flag_type::next_bitfield<1>;
        using container_type = adaptation_field_extension_flag_type::inclusion_container_type;
        
        union {
          container_type data;
          
          adaptation_field_length_type               adaptation_field_length;
          discontinuity_indicator_type               discontinuity_indicator;
          random_access_indicator_type               random_access_indicator;
          elementary_stream_priority_indicator_type  elementary_stream_priority_indicator;
          pcr_flag_type                              pcr_flag;
          opcr_flag_type                             opcr_flag;
          splicing_point_flag_type                   splicing_point_flag;
          transport_private_data_flag_type           transport_private_data_flag;
          adaptation_field_extension_flag_type       adaptation_field_extension_flag;
        };
      };
      
      union {
        container_type  data;
        
        sync_byte_type                     sync_byte;
        transport_error_indicator_type     transport_error_indicator;
        payload_unit_start_indicator_type  payload_unit_start_indicator;
        transport_priority_type            transport_priority;
        pid_type                           pid;
        scrambling_control_type            scrambling_control;
        adaptation_field_exist_type        adaptation_field_exist;
        continuity_counter_type            continuity_counter;
      };
      
      adaption_field_type * adaptation_field(void);
    };
    
    union payload_type {
      psi::base psi;
    };
    
    union {
      container_type data;
      header_type header;
    };
    
    payload_type * payload(void);
    std::size_t payload_offset(void) const;
    std::size_t payload_size(void) const;
    
    template<typename Container>
    Container & copy_payload(Container & container) const {
      auto offset = this->payload_offset();
      if (offset == 0) return container;
      std::for_each(data.begin() + offset, data.end(), [&container](uint8_t byte){
        container.push_back(byte);
      });
      return container;
    }
    
    friend std::ostream & operator<<(std::ostream & out, const ts::packet & packet);
  };
}}
