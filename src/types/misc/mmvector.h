#ifndef MMVECTOR_H
#define MMVECTOR_H

#include <stdint.h>
#include <sys/types.h>

namespace std
{
  template <typename value_type>
  class mmvector
  {
    public:
// constructors
      mmvector(void);

      mmvector(mmvector<value_type>& x);
      mmvector(const mmvector<value_type>& x);
      template<typename V> mmvector(mmvector<V>& x);
      template<typename V> mmvector(V* memory, size_t size);
// destructor
      ~mmvector(void);

      template<typename V> void copy(const mmvector<V>& x); // does a deep copy of data

      template<typename V>       mmvector<V>& convert(void)       { return mmvector<V>(*this); }
      template<typename V> const mmvector<V>& convert(void) const { return mmvector<V>(*this); }

      mmvector<value_type>& operator= (const mmvector<value_type>& x);
      template<typename V> mmvector<V>& operator= (const mmvector<V>& x);

      template<typename V> operator V*(void);

      template<typename V> value_type* operator +(const V& x);
      template<typename V> V* operator +(V* x);

      template<typename V> V* data(void) const;
                  value_type* data(void) const;

      bool empty(void) const;
      void clear(void);
      size_t size (void) const;
      size_t bytes(void) const;

            value_type& at(size_t n)       { return operator [](n); }
      const value_type& at(size_t n) const { return operator [](n); }

            value_type& operator[] (size_t n);
      const value_type& operator[] (size_t n) const;


      bool allocated(void) const { return m_allocated; }
      mmvector<value_type>& allocate(size_t n);
      void deallocate(void);

    private:
      value_type* m_memory;
      size_t      m_bytes;
      bool        m_allocated;
  };
}
#include "mmvector.cpp"
#endif // MMVECTOR_H
