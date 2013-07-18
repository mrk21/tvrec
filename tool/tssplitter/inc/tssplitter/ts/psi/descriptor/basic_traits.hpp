#pragma once
#include <tssplitter/bitfield.hpp>

namespace tssplitter { namespace ts { namespace psi { namespace descriptor {
  using descriptor_tag_type    =                           bitfield<8>;
  using descriptor_length_type = descriptor_tag_type::next_bitfield<8>;
  using last_basic_bitfield = descriptor_length_type;
  
  #define DESCRIPTOR_BASIC_FIELD \
    container_type         data;              \
    descriptor_tag_type    descriptor_tag;    \
    descriptor_length_type descriptor_length; \
  
  template<typename Derived>
  struct basic_traits: public util::crtp_helper<Derived> {
    struct container_type: public variable_byte_container<container_type, typename Derived::static_container_type>, public util::union_helper {
      std::size_t runtime_size(void) const {
        return this->member_type<basic_traits>()->size();
      }
    };
    
    std::size_t size(void) const {
      return descriptor_length_type::END_POSITION + this->derived()->descriptor_length;
    }
  };
}}}}
