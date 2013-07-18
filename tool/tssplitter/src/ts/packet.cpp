#include <tssplitter/ts/packet.hpp>

namespace tssplitter { namespace ts {
using payload_type = packet::payload_type;
/// packet
  using header_type = packet::header_type;
  using adaption_field_type = header_type::adaption_field_type;
  using status_type = header_type::adaptation_field_status_type;
  
  payload_type * packet::payload(void) {
    auto * base = util::to_byteptr(this) + sizeof(header_type);
    
    switch (this->header.adaptation_field_exist) {
      case status_type::PAYLOAD_ONLY:
        return reinterpret_cast<payload_type *>(base);
      case status_type::ADEPTION_ONLY:
        return nullptr;
      case status_type::ADEPTION_WITH_PAYLOAD:
        return reinterpret_cast<payload_type *>(base + this->header.adaptation_field()->adaptation_field_length);
    }
  }
  
  std::size_t packet::payload_offset(void) const {
    auto payload = const_cast<packet *>(this)->payload();
    if (payload == nullptr) return 0;
    return util::to_byteptr(payload) - util::to_byteptr(this);
  }
  
  std::size_t packet::payload_size(void) const {
    return packet::SIZE - this->payload_offset();
  }
  
  std::ostream & operator<<(std::ostream & out, const ts::packet & packet) {
    return out << packet.data;
  }
  
  
  
/// header_type
  adaption_field_type * header_type::adaptation_field(void) {
    if (this->adaptation_field_exist == status_type::PAYLOAD_ONLY) return nullptr;
    return reinterpret_cast<adaption_field_type *>(util::to_byteptr(this) + sizeof(header_type));
  }
}}
