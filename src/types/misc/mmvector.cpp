#include "mmvector.h"

#include <cassert>


#define TEST_DATA \
  assert(m_memory != nullptr); \
  assert(m_bytes != 0);

namespace std
{
// basic functions

  template<typename T>
  void mmvector<T>::clear(void)
  {
    assert(!m_allocated);
    m_memory = nullptr;
    m_bytes = 0;
  }

  template<typename T>
  bool mmvector<T>::empty(void) const
  {
    return size() == 0;
  }

  template<typename T>
  size_t mmvector<T>::size(void) const
  {
    return m_memory == nullptr ? 0 : m_bytes / sizeof(T);
  }

  template<typename T>
  size_t mmvector<T>::bytes(void) const
  {
    return m_bytes;
  }

  template<typename T>
  mmvector<T>& mmvector<T>::allocate(size_t n)
  {
    deallocate();

    m_bytes = n * sizeof(T);
    m_memory = new T[n];
    m_allocated = true;

    TEST_DATA;
    return *this;
  }

  template<typename T>
  void mmvector<T>::deallocate(void)
  {
    if(m_allocated == true)
    {
      TEST_DATA;
      m_bytes = 0;
      delete m_memory;
      m_memory = nullptr;
      m_allocated = false;
    }
  }


  template<typename T>
  T* mmvector<T>::data(void) const
  {
    TEST_DATA;
    return m_memory;
  }

  template<typename T>
  template<typename V>
  V* mmvector<T>::data(void) const
  {
    TEST_DATA;
    return reinterpret_cast<V*>(m_memory);
  }

// constructors
  template<typename T>
  mmvector<T>::mmvector(void)
    : m_memory(nullptr),
      m_bytes(0),
      m_allocated(false)
  {
  }

  template<typename T>
  mmvector<T>::mmvector(const mmvector<T>& x)
    : mmvector<T>(x.m_memory, x.m_bytes)
  {
  }

  template<typename T>
  mmvector<T>::mmvector(mmvector<T>& x)
    : mmvector<T>(x.m_memory, x.m_bytes)
  {
  }

  template<typename T>
  template<typename V>
  mmvector<T>::mmvector(mmvector<V>& x) // note: must convert the size
    : mmvector<T>(reinterpret_cast<T*>(x.data()), x.size() )
  {
  }

  template<typename T>
  template<typename V>
  mmvector<T>::mmvector(V* memory, size_t size)
    : m_memory(reinterpret_cast<T*>(memory)),
      m_bytes(size * sizeof(V)),
      m_allocated(false)
  {
    TEST_DATA;
  }

  // destructor

  template<typename T>
  mmvector<T>::~mmvector(void)
  {
    deallocate();
    m_memory = nullptr;
    m_bytes = 0;
  }

  // deep copy
  template<typename T>
  template<typename V>
  void mmvector<T>::copy(const mmvector<V>& x)
  {
    if(!allocated() || bytes() != x.bytes())
    {
      deallocate();
      allocate(x.bytes());
    }
    memcpy(data(), x.data(), x.bytes());
  }

// type instantiation operator

  template<typename T>
  template<typename V>
  mmvector<T>::operator V*(void)
  {
    TEST_DATA;
    return reinterpret_cast<V*>(m_memory);
  }

// assignment operator

  template<typename T>
  mmvector<T>& mmvector<T>::operator= (const mmvector<T>& x)
  {
    if(m_allocated)
    {
      assert(m_bytes == x.m_bytes);
      memcpy(m_memory, x.m_memory, m_bytes);
    }
    else
    {
      m_memory = x.m_memory;
      m_bytes  = x.m_bytes;
    }

    TEST_DATA;
    return *this;
  }

  template<typename T>
  template<typename V>
  mmvector<V>& mmvector<T>::operator= (const mmvector<V>& x)
  {
    if(m_allocated || x.m_allocated)
    {
      assert(m_bytes == x.m_bytes);
      memcpy(m_memory, x.m_memory, m_bytes);
    }
    else
    {
      m_memory = x.m_memory;
      m_bytes  = x.m_bytes;
    }

    TEST_DATA;
    return mmvector<V>(*this);
  }

// addition operator

  template<typename T>
  template<typename V>
  V* mmvector<T>::operator +(V* x)
  {
    TEST_DATA;
    assert(x < size());
    return reinterpret_cast<V*>(m_memory + x);
  }

  template<typename T>
  template<typename V>
  T* mmvector<T>::operator +(const V& x)
  {
    TEST_DATA;
    assert(x < size());
    return &m_memory[x];
  }


// array operator

  template<typename T>
  T& mmvector<T>::operator[] (size_t n)
  {
    TEST_DATA;
    assert(n < size());
    return m_memory[n];
  }

  template<typename T>
  const T& mmvector<T>::operator[] (size_t n) const
  {
    TEST_DATA;
    assert(n < size());
    return m_memory[n];
  }
}
