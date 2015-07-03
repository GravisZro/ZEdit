#if 0
#ifndef __UTILITIES__
#define __UTILITIES__

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstring>

#include <types/misc/mmvector.h>

#include <QtCore>
#if 1

template <typename value_type, typename size_type = uint64_t>
class CTemplateArray : public std::mmvector<value_type>
{
  public:
    CTemplateArray(void) : std::mmvector<value_type>() { }

    template<typename V>
    CTemplateArray(const CTemplateArray<V>& x, bool copy = false) : CTemplateArray(reinterpret_cast<V*>(x.data()), x.size()) { if(copy) { this->copy(x); } }

    template<typename V>
    CTemplateArray(const CTemplateArray<V>& x, const size_type size, const size_type offset = 0) : CTemplateArray(reinterpret_cast<V*>(x.data()) + offset, size) { }

    CTemplateArray(const size_type size, const value_type value = 0)
      : std::mmvector<value_type>()
    {
      std::mmvector<value_type>::allocate(size);
      memset(std::mmvector<value_type>::data(), value, std::mmvector<value_type>::bytes());
    }

    template<typename V>
    CTemplateArray(V* data, const size_type size)
      : std::mmvector<value_type>(data, size) { }

    //virtual ~CTemplateArray(void) { }

          value_type& col(size_type n)       { return std::mmvector<value_type>::operator [](n); }
    const value_type& col(size_type n) const { return std::mmvector<value_type>::operator [](n); }

    operator QString(void) const { return QString::fromLocal8Bit(QByteArray::fromRawData(reinterpret_cast<const char*>(this->data()), this->bytes() * 2).toHex().constData()); }
    operator QByteArray(void) const { return QByteArray::fromRawData(reinterpret_cast<const char*>(this->data()), this->bytes()); }
};


#elif 0
template <typename value_type, typename size_type = uint64_t>
class ExtAllocator : public std::allocator<value_type>
{
  public:

    ExtAllocator(const value_type* memory, const size_type& size)
      : std::allocator<value_type>(),
        m_memory(memory),
        m_size(size)
    {
    }

    ExtAllocator(void) : ExtAllocator(0, 0) { }


    template <class U>
    ExtAllocator (const ExtAllocator<U>& alloc)
      : std::allocator<U>(alloc),
        m_memory(alloc.m_memory),
        m_size(alloc.m_size)
    {
    }

    size_type max_size(void) const
    {
      if(m_size)
        return m_size;
      return std::allocator<value_type>::max_size();
    }

    value_type* allocate (size_type n, std::allocator<void>::const_pointer hint = 0)
    {
      if(m_size)
        return m_memory;
      return std::allocator<value_type>::allocate(n, hint);
    }

    void deallocate (value_type* p, size_type n)
    {
      if(!m_size)
        std::allocator<value_type>::deallocate(p, n);
    }


  private:
    const value_type* m_memory;
    size_type m_size;
};

template <typename value_type, typename size_type = uint64_t>
class CTemplateArray : public std::vector<value_type>
{
  public:
    CTemplateArray(void) : std::vector<value_type>(ExtAllocator<value_type>()) { }

    CTemplateArray(const size_type size, const value_type value = 0)
      : std::vector<value_type>(size, value, ExtAllocator<value_type>()) { }

    CTemplateArray(const CTemplateArray& x)
      : std::vector<value_type>(x, ExtAllocator<value_type>(x.data(), x.size())) { }

    CTemplateArray(const CTemplateArray& x, const size_type size, const size_type offset = 0)
      : std::vector<value_type>(x, ExtAllocator<value_type>(x.data() + offset, size)) { }

    CTemplateArray(const value_type* x, const size_type size, const size_type offset = 0)
      : std::vector<value_type>(size, ExtAllocator<value_type>(x + offset, size)) { }//std::vector<value_type>::resize(size);}

    virtual ~CTemplateArray(void) { }


          value_type& col (size_type n)       { return std::vector<value_type>::at(n); }
    const value_type& col (size_type n) const { return std::vector<value_type>::at(n); }

};
#elif 1
template <typename value_type, typename size_type = uint64_t>
class CTemplateArray
{
  public:

    CTemplateArray(void);
    CTemplateArray(const CTemplateArray& x);
    //CTemplateArray(const CTemplateArray& x, size_type offset, size_type size);
    CTemplateArray(const size_type size, const value_type value = 0);
    CTemplateArray(value_type* data, const size_type size);
    ~CTemplateArray(void);

    void deepcopy(const CTemplateArray& x);

    CTemplateArray section(size_type offset, size_type size);

    size_type size(void) const noexcept { return mem().size(); }

    void resize (size_type n, value_type value = 0) { mem().resize(n, value); }

    bool empty(void) const noexcept { return mem().empty(); }

    void reserve (size_type n) { mem().reserve(n); }

    template<typename T> operator std::vector<T> (void)
    {
      std::vector<T> v(sizeof(value_type) * size() / sizeof(T));
      memcpy(v.data(), data, sizeof(value_type) * size());
      return v;
    }

    template<typename T> T& operator[] (size_type n)       { return *static_cast<T*>(&mem().operator[] (n)); }
    template<typename T> T& operator[] (size_type n) const { return *static_cast<T*>(&mem().operator[] (n)); }

    typename std::vector<value_type>::reference col (size_type n) { return mem().operator[] (n); }

    typename std::vector<value_type>::reference       operator[] (size_type n)       { return mem().operator[] (n); }
    typename std::vector<value_type>::const_reference operator[] (size_type n) const { return mem().operator[] (n); }

    template<typename T> T& at (size_type n)       { return *static_cast<T*>(&mem().at(n)); }
    template<typename T> T& at (size_type n) const { return *static_cast<T*>(&mem().at(n)); }

    value_type col(size_type n) const { return mem().at(n); }
    value_type at (size_type n) const { return mem().at(n); }

    value_type front(void) const { return mem().front(); }
    value_type back (void) const { return mem().back (); }

    value_type* data(void)       noexcept { return mem().data(); }
    const value_type* data(void) const noexcept { return mem().data(); }

    void assign (size_type n, const value_type& val) { mem().assign(n, val); }

    void push_back (const value_type& val) { mem().push_back(val); }
    void pop_back(void) { mem().pop_back(); }

    void clear(void) noexcept { mem().clear(); }

    typename std::vector<value_type>::iterator               begin  (void)       noexcept { return mem().begin(); }
    typename std::vector<value_type>::const_iterator         begin  (void) const noexcept { return mem().begin(); }

    typename std::vector<value_type>::iterator               end    (void)       noexcept { return mem().end(); }
    typename std::vector<value_type>::const_iterator         end    (void) const noexcept { return mem().end(); }

    typename std::vector<value_type>::reverse_iterator       rbegin (void)       noexcept { return mem().rbegin(); }
    typename std::vector<value_type>::const_reverse_iterator rbegin (void) const noexcept { return mem().rbegin(); }

    typename std::vector<value_type>::reverse_iterator       rend   (void)       noexcept { return mem().rend(); }
    typename std::vector<value_type>::const_reverse_iterator rend   (void) const noexcept { return mem().rend(); }

    CTemplateArray& operator= (const CTemplateArray& x);

    inline const uint64_t& key(void) const { return m_key; }

  protected:
    std::vector<value_type>& mem(void)       { return s_mempool[m_key]; }
    std::vector<value_type>& mem(void) const { return s_mempool[m_key]; }

    uint64_t m_key;
    static void add_reference(const uint64_t& key);
    static void sub_reference(const uint64_t& key);
    static std::unordered_map<uint64_t, std::vector<value_type> > s_mempool;
    static std::unordered_map<uint64_t, uint64_t> s_mempool_references;
    static uint64_t s_key_counter;
};
#endif

/*
  stride = how many bytes are between rows
  height = number of rows in the matrix
  width  = number of columns (used data)

  excess bytes = stride - width
  total bytes  = height * stride
  usable bytes = height * width

  a byte matrix is aligned like this:
  (height: 8, width: 10, stride: 10)

         columns
         0123456789
  row 0 +----------
  row 1 +----------
  row 2 +----------
  row 3 +----------
  row 4 +----------
  row 5 +----------
  row 6 +----------
  row 7 +----------

  a byte matrix with a stride is aligned like this:
  (height: 8, width: 10, stride: 16)

         columns                  _width_                 __ stride ___
         0123456789              /  (10) \               /    (16)     \
  row 0 +----------_____        +----------_____        +----------_____
  row 1 +----------_____    h / +----------_____    h / +----------_____
  row 2 +----------_____    e | +----------_____    e | +----------_____
  row 3 +----------_____    i | +----------_____    i | +----------_____
  row 4 +----------_____    g | +----------_____    g | +----------_____
  row 5 +----------_____    h | +----------_____    h | +----------_____
  row 6 +----------_____    t \ +----------_____    t \ +----------_____
  row 7 +----------_____        +----------_____        +----------_____
           ^         ^           |        |              |        /     \
           |         |           0        9              0       10     15
          used    excess
          dytes    bytes
*/

template <typename value_type, typename size_type = uint64_t>
class CTemplateMatrix : public CTemplateArray<value_type, size_type>
{
  public:

    CTemplateMatrix(void) {}
    CTemplateMatrix(const size_type width, const size_type height, const value_type value = 0) { resize(width * height); }

    CTemplateMatrix(value_type* data, const size_type width, const size_type height, const size_type stride = 0);
    CTemplateMatrix(const CTemplateArray<value_type, size_type>& x, const size_type width, const size_type height, const size_type stride = 0);

    ~CTemplateMatrix(void){}

    CTemplateArray<value_type, size_type> row (size_type n);

    inline size_type width (void) const { return m_width ; }
    inline size_type height(void) const { return m_height; }
    inline size_type stride(void) const { return m_stride; }

  private:
    size_type m_width;
    size_type m_height;
    size_type m_stride;
};


typedef CTemplateArray <uint8_t , size_t> CByteArray;
typedef CTemplateArray <uint16_t, size_t> CWordArray;
typedef CTemplateMatrix<uint8_t , size_t> CByteMatrix;
typedef CTemplateMatrix<uint16_t, size_t> CWordMatrix;

#include "template_array.cpp"
#endif
#endif
