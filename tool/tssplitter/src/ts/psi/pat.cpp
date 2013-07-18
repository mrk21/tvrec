#include <tssplitter/ts/psi/pat.hpp>

namespace tssplitter { namespace ts { namespace psi {
/// section_type
  using section_type = pat::section_type;
  
  std::size_t section_type::size(void) const {
    return this->data.size();
  }
  
  bool section_type::is_pmt(void) const {
    return this->program_num != 0x00;
  }
  
  
  
/// section_container
  using section_container_type = pat::section_container_type;
  
  byteptr section_container_type::base_address(void) {
    return this->to_byteptr() + last_session_id_type::END_POSITION;
  }
  
  std::size_t section_container_type::length(void) const {
    return this->member_type<pat>()->section_length - (last_session_id_type::END_POSITION - section_length_type::END_POSITION + sizeof(crc_type));
  }
}}}
