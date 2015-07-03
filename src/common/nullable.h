#ifndef NULLABLE_H
#define NULLABLE_H

#include <cassert>

namespace std
{
template<typename T>
class nullable
{
  public:
    nullable(void) : m_data(nullptr) { }
    nullable(const T& x) { operator =( x ); }
    nullable(const nullable<T>& x) : m_data(nullptr) { operator =( x ); }

   ~nullable(void) { deallocate(); }

    const T& operator =(const T& x)
    {
      allocate();
      *m_data = x;
      return x;
    }

    const nullable<T>& operator =(const nullable<T>& x)
    {
      if(x.m_data != nullptr)
      {
        allocate();
        *m_data = *x.m_data;
      }
      else
        deallocate();
      return x;
    }

    template<typename V>
    T& operator [](const V& x) { assert(!isNull()); return m_data[x]; }

    T* operator ->(void) { assert(!isNull()); return m_data; }

    operator nullable<T>(void) { return *this; }
    operator T(void) { assert(!isNull()); return *m_data; }

    bool isNull(void) const { return m_data == nullptr; }

    template<bool>
    operator bool (void) const { return isNull(); }


    inline void reset  (void) { deallocate(); }
    inline void clear  (void) { deallocate(); }
    inline void nullify(void) { deallocate(); }

    inline void deallocate(void)
    {
      if(m_data != nullptr)
        delete m_data;
      m_data = nullptr;
    }

    inline void allocate(void)
    {
      if(m_data == nullptr)
        m_data = new T();
    }

  private:
    T* m_data;
};
}

#endif // NULLABLE_H
