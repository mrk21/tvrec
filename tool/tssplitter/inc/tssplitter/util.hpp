#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <boost/format.hpp>

namespace tssplitter {
  using byte_t        =       uint8_t;
  using byteptr       =       uint8_t *;
  using const_byteptr = const uint8_t *;
  using byteref       =       uint8_t &;
  using const_byteref = const uint8_t &;
  
  namespace util {
    constexpr uint32_t mask(std::size_t bit) {
      return bit < 32 ? (1 << bit)-1 : -1;
    }
    
    constexpr uint32_t ceil(double value) {
      return (value - static_cast<uint32_t>(value)) > 0.0001 ? value + 1 : value;
    }
    
    template<typename T> constexpr const_byteptr to_byteptr(const T * addr) { return reinterpret_cast<const_byteptr>(addr); }
    template<typename T> constexpr       byteptr to_byteptr(      T * addr) { return reinterpret_cast<      byteptr>(addr); }
    
    template<typename T>
    uint32_t multibyte_value(std::size_t byte, const T * addrss) {
      auto base = to_byteptr(addrss);
      uint32_t value = 0x00;
      byte &= mask(byte*8);
      
      for (std::size_t i=0; i < byte; ++i) {
        value |= base[i] << ((byte-i-1)*8);
      }
      return value;
    }
    
    template<typename ByteContainer>
    std::ostream & output_bytes(const ByteContainer & container, std::ostream & os) {
      for (uint32_t v: container) os << boost::format("%02X ") % v;
      return os;
    }
    
    template<typename Derived>
    struct crtp_helper {
    protected:
      auto derived(void)       { return static_cast<      Derived *>(this); }
      auto derived(void) const { return static_cast<const Derived *>(this); }
    };
    
    struct union_helper {
    protected:
      template<typename T> auto member_type(void)       { return reinterpret_cast<      T *>(this); }
      template<typename T> auto member_type(void) const { return reinterpret_cast<const T *>(this); }
      
      auto to_byteptr(void)       { return util::to_byteptr(this); }
      auto to_byteptr(void) const { return util::to_byteptr(this); }
    };
  }
}
