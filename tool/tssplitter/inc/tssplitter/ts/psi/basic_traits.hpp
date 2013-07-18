#pragma once
#include <tssplitter/bitfield.hpp>
#include <tssplitter/byte_container.hpp>

namespace tssplitter { namespace ts { namespace psi {
  using pointer_field_type            =                                     bitfield< 8>;
  using table_id_type                 =            pointer_field_type::next_bitfield< 8>;
  using section_syntax_indicator_type =                 table_id_type::next_bitfield< 1>;
  using always_set_to_0_type          = section_syntax_indicator_type::next_bitfield< 1>;
  using reserved_1_type               =          always_set_to_0_type::next_bitfield< 2>;
  using section_length_type           =               reserved_1_type::next_bitfield<12>;
  using last_basic_bitfield = section_length_type;
  using crc_type = byte_container<sizeof(uint32_t)>;
  
  #define PSI_BASIC_FIELD \
    container_type                 data;                     \
    pointer_field_type             pointer_field;            \
    table_id_type                  table_id;                 \
    section_syntax_indicator_type  section_syntax_indicator; \
    always_set_to_0_type           always_set_to_0;          \
    reserved_1_type                reserved_1;               \
    section_length_type            section_length;           \
  
  template<typename Derived>
  struct basic_traits: public util::crtp_helper<Derived> {
    struct container_type: public variable_byte_container<container_type, typename Derived::static_container_type>, public util::union_helper {
      std::size_t runtime_size(void) const {
        return this->member_type<basic_traits>()->size();
      }
    };
    
    std::size_t size(void) const {
      return this->derived()->section_length + section_length_type::END_POSITION;
    }
    
    crc_type & crc(void) {
      return *reinterpret_cast<crc_type *>(util::to_byteptr(this) + this->size() - sizeof(crc_type));
    }
  };
}}}
