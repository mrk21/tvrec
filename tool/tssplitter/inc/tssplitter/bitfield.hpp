#pragma once
#include <tssplitter/byte_container.hpp>

namespace tssplitter {
  template<std::size_t Size, std::size_t Offset = 0>
  struct bitfield {
    constexpr static auto SIZE = Size;
    constexpr static auto OFFSET = Offset;
    constexpr static auto NEXT_OFFSET = OFFSET + SIZE;
    
    constexpr static auto START_POSITION = OFFSET / 8;
    constexpr static auto START_PADDING  = OFFSET % 8;
    constexpr static auto END_POSITION = util::ceil(NEXT_OFFSET / 8.0);
    constexpr static auto END_PADDING = END_POSITION*8 - NEXT_OFFSET;
    
    constexpr static auto CONTAINER_SIZE = END_POSITION - START_POSITION;
    constexpr static auto INCLUSION_CONTAINER_SIZE = END_POSITION;
    
    constexpr static auto MAX_VALUE = util::mask(SIZE);
    constexpr static auto CONTAINER_MASK = MAX_VALUE << END_PADDING;
    
    
    using container_type = byte_container<CONTAINER_SIZE>;
    using inclusion_container_type = byte_container<INCLUSION_CONTAINER_SIZE>;
    
    template<std::size_t NextSize>
    using next_bitfield = bitfield<NextSize, NEXT_OFFSET>;
    
    
          container_type & container(void)       { return container_type::from(this + START_POSITION); }
    const container_type & container(void) const { return container_type::from(this + START_POSITION); }
    
    uint32_t get(void) const {
      return (this->container().value() & CONTAINER_MASK) >> END_PADDING;
    }
    
    bitfield & set(uint32_t value) {
      value = (value << END_PADDING) & CONTAINER_MASK;
      auto clear_mask = ~CONTAINER_MASK;
      
      auto & container = this->container();
      auto v_it = container_type::from(value).rbegin();
      auto m_it = container_type::from(clear_mask).rbegin();
      
      for (auto it = container.begin();  it != container.end();  ++it) {
        *it &= *m_it++;
        *it |= *v_it++;
      }
      return *this;
    }
    
    operator uint32_t(void) const {
      return this->get();
    }
    
    bitfield & operator=(uint32_t value) {
      return this->set(value);
    }
    
    friend std::ostream & operator<<(std::ostream & os, const bitfield & v){
      return os << v.get();
    }
  };
}
