#include "template_array.h"
#if 0
#include <string.h>
#include <cassert>

template <typename value_type, typename size_type> std::unordered_map<uint64_t, std::vector<value_type> > CTemplateArray<value_type, size_type>::s_mempool;
template <typename value_type, typename size_type> std::unordered_map<uint64_t, uint64_t> CTemplateArray<value_type, size_type>::s_mempool_references;
template <typename value_type, typename size_type> uint64_t CTemplateArray<value_type, size_type>::s_key_counter = 0;

template <typename value_type, typename size_type>
void CTemplateArray<value_type, size_type>::add_reference(const uint64_t& key)
{
  if(s_mempool_references.find(key) == s_mempool_references.end())
    s_mempool_references[key] = 1;
  else
    s_mempool_references[key]++;
}

template <typename value_type, typename size_type>
void CTemplateArray<value_type, size_type>::sub_reference(const uint64_t& key)
{
  assert(s_mempool_references.find(key) != s_mempool_references.end());

  s_mempool_references[key]--;
  if(s_mempool_references[key] == 0)
    s_mempool.erase(key);
}

template <typename value_type, typename size_type>
CTemplateArray<value_type, size_type>::CTemplateArray(void)
  : m_key(s_key_counter++)
{
  add_reference(m_key);
}

template <typename value_type, typename size_type>
CTemplateArray<value_type, size_type>::CTemplateArray(const CTemplateArray& x)
  : m_key(x.m_key)
{
  add_reference(m_key);
}

template <typename value_type, typename size_type>
CTemplateArray<value_type, size_type>::~CTemplateArray(void)
{
  sub_reference(m_key);
}

template <typename value_type, typename size_type>
CTemplateArray<value_type, size_type>::CTemplateArray(const size_type size, const value_type value)
  : m_key(s_key_counter++)
{
  add_reference(m_key);
  mem().resize(size, value);
}

template <typename value_type, typename size_type>
CTemplateArray<value_type, size_type>::CTemplateArray(value_type* data, const size_type size)
  : m_key(s_key_counter++)
{
  add_reference(m_key);
  mem().resize(size);
  memcpy(this->data(), data, sizeof(value_type) * size);
}

template <typename value_type, typename size_type>
CTemplateArray<value_type, size_type>& CTemplateArray<value_type, size_type>::section(size_type offset, size_type size)
{

}


template <typename value_type, typename size_type>
CTemplateArray<value_type, size_type>& CTemplateArray<value_type, size_type>::operator= (const CTemplateArray<value_type, size_type>& x)
{
  sub_reference(m_key);
  m_key = x.m_key;
  add_reference(m_key);
  return *this;
}

template <typename value_type, typename size_type>
void CTemplateArray<value_type, size_type>::deepcopy(const CTemplateArray<value_type, size_type>& x)
{
  s_mempool[m_key] = s_mempool[x.m_key];
}
#endif

template <typename value_type, typename size_type>
CTemplateMatrix<value_type, size_type>::CTemplateMatrix(value_type* data, const size_type width, const size_type height, const size_type stride)
  : CTemplateArray<value_type, size_type>(data, (width + stride) * height),
    m_width(width),
    m_height(height),
    m_stride(stride)
{
}

template <typename value_type, typename size_type>
CTemplateMatrix<value_type, size_type>::CTemplateMatrix(const CTemplateArray<value_type, size_type>& x, const size_type width, const size_type height, const size_type stride)
  : CTemplateArray<value_type, size_type>(x),
    m_width(width),
    m_height(height),
    m_stride(stride)
{
}

template <typename value_type, typename size_type>
CTemplateArray<value_type, size_type> CTemplateMatrix<value_type, size_type>::row(size_type n)
{
  // this function is really broken :(((((
  //assert((n * CTemplateArray<value_type, size_type>::stride()) + CTemplateArray<value_type, size_type>::width() <= CTemplateArray<value_type, size_type>::size());
  return CTemplateArray<value_type, size_type>(CTemplateArray<value_type, size_type>::data() + (n * (m_width + m_stride)), m_width);
}
