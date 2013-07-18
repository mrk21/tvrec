#pragma once
#include <iterator>
#include <tssplitter/util.hpp>

namespace tssplitter {
  // You must implement methods listed below to the derived class!
  // *       byteptr base_address(void); -> Return the first element address.
  // * const_byteptr base_address(void) const; -> Return the first element address. [const ver., optional]
  // * std::size_t length(void) const; -> Return the container byte size.
  // 
  // You must implement methods listed below to Element type.
  // * std::size_t size(void) const; -> Return an element byte size.
  // 
  template<typename Derived, typename Element>
  class flexible_container: public util::crtp_helper<Derived>, public util::union_helper {
    template<typename BytePointer, typename Pointer, typename Reference>
    class iterator_type: public std::iterator<std::forward_iterator_tag, Element> {
    public:
      using byteptr = BytePointer;
      using pointer = Pointer;
      using reference = Reference;
      
    private:
      byteptr base_address;
      byteptr current_address;
      std::size_t length;
      
    public:
      iterator_type(byteptr base_address, std::size_t length=0) :
        base_address(base_address), length(length), current_address(length == 0 ? nullptr : base_address)
      {}
      
      reference operator*(void) {
        return *this->current();
      }
      
      iterator_type & operator++(void) {
        if (this->current_address == nullptr) return *this;
        this->current_address += this->current()->size();
        
        auto diff = this->current_address - this->base_address;
        if (diff >= this->length) this->current_address = nullptr;
        return *this;
      }
      
      bool operator!=(const iterator_type & v) {
        return this->current_address != v.current_address;
      }
      
    private:
      pointer current(void) {
        return reinterpret_cast<pointer>(this->current_address);
      }
    };
    
  public:
    using value_type      =       Element;
    using pointer         =       Element *;
    using const_pointer   = const Element *;
    using reference       =       Element &;
    using const_reference = const Element &;
    
    using       iterator = iterator_type<      byteptr,       pointer,       reference>;
    using const_iterator = iterator_type<const_byteptr, const_pointer, const_reference>;
    
          iterator begin(void)       { return iterator(this->derived()->base_address(), this->derived()->length()); }
    const_iterator begin(void) const { return iterator(this->derived()->base_address(), this->derived()->length()); }
    
          iterator end(void)       { return iterator(this->derived()->base_address()); }
    const_iterator end(void) const { return iterator(this->derived()->base_address()); }
  };
}
