#pragma once
#include <iostream>
#include <array>
#include <algorithm>
#include <boost/format.hpp>
#include <tssplitter/util.hpp>

namespace tssplitter {
  template<typename Derived, bool CanConvertInteger>
  struct byte_container_int32 {};
  
  template<typename Derived>
  struct byte_container_int32<Derived, true>: public util::crtp_helper<Derived> {
    uint32_t value(void) const {
      return util::multibyte_value(this->derived()->size(), this->derived()->data());
    }
    
    operator uint32_t(void) const {
      return this->derived()->value();
    }
  };
  
  
  template<std::size_t N>
  struct byte_container: public std::array<uint8_t, N>, public byte_container_int32<byte_container<N>, N <= sizeof(uint32_t)> {
    using base_type = std::array<uint8_t, N>;
    using               iterator = typename base_type::              iterator;
    using         const_iterator = typename base_type::        const_iterator;
    using       reverse_iterator = typename base_type::      reverse_iterator;
    using const_reverse_iterator = typename base_type::const_reverse_iterator;
    
    constexpr static auto SIZE = N;
    
    template<typename T> constexpr static const byte_container & from(const T * address  ) { return *reinterpret_cast<const byte_container *>(address); }
    template<typename T> constexpr static       byte_container & from(      T * address  ) { return *reinterpret_cast<      byte_container *>(address); }
    template<typename T> constexpr static const byte_container & from(const T & reference) { return from(&reference); }
    template<typename T> constexpr static       byte_container & from(      T & reference) { return from(&reference); }
    
    friend std::ostream & operator<<(std::ostream & os, const byte_container & container) {
      return util::output_bytes(container, os);
    }
  };
  
  
  // You must implement methods listed below to the derived class!
  // * std::size_t runtime_size(void) const;
  template<typename Derived, typename StaticByteContainer>
  struct variable_byte_container: public StaticByteContainer, public util::crtp_helper<Derived> {
    using base_type = StaticByteContainer;
    using               iterator = typename base_type::              iterator;
    using         const_iterator = typename base_type::        const_iterator;
    using       reverse_iterator = typename base_type::      reverse_iterator;
    using const_reverse_iterator = typename base_type::const_reverse_iterator;
    
    std::size_t size(void) const {
      return this->derived()->runtime_size();
    }
    
    std::size_t max_size (void) const {
      return this->size();
    }
    
    std::size_t static_size(void) const {
      return base_type::SIZE;
    }
    
          iterator end(void)       { return this->begin() + this->size(); }
    const_iterator end(void) const { return this->begin() + this->size(); }
    
          reverse_iterator rbegin(void)       { return this->rend() + this->size(); }
    const_reverse_iterator rbegin(void) const { return this->rend() + this->size(); }
    
    friend std::ostream & operator<<(std::ostream & os, const Derived & container) {
      return util::output_bytes(container, os);
    }
  };
}
