#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <QString>

#include "structs.h"

template<typename T>
class CMatrix : public std::vector<std::vector<T>>
{
public:
  CMatrix(unsigned int columns = 1, unsigned int rows = 1, T fill = T())
    : std::vector<std::vector<T>>(rows, std::vector<T>(columns))
  {
    for(auto pos = std::vector<std::vector<T>>::begin(); pos != std::vector<std::vector<T>>::end(); ++pos)
      for(auto subpos = pos->begin(); subpos != pos->end(); ++subpos)
        *subpos = fill;
  }

  operator QString(void)
  {
    coords_t coords = { 0, 0 };
    QString output("CMatrix:\n");
    for(auto pos = std::vector<std::vector<T>>::begin(); pos != std::vector<std::vector<T>>::end(); ++pos, ++coords.y)
      for(auto subpos = pos->begin(); subpos != pos->end(); ++subpos, ++coords.x)
        output.append(QString("  (%1, %2) value: %3\n").arg(coords.x).arg(coords.y).arg(*subpos));

    return output;
  }

  void resize(unsigned int columns, unsigned int rows)
  {
    std::vector<std::vector<T>>::resize(rows);
    for(auto pos = std::vector<std::vector<T>>::begin(); pos != std::vector<std::vector<T>>::end(); ++pos)
      pos->resize(columns);
  }

  inline unsigned int rows   (void) const { return height(); }
  inline unsigned int columns(void) const { return width (); }

  inline unsigned int height (void) const { return std::vector<std::vector<T>>::size(); }
  inline unsigned int width  (void) const { return std::vector<std::vector<T>>::front().size(); }
};

typedef CMatrix<uint8_t > CByteMatrix;
typedef CMatrix<uint16_t> CWordMatrix;


class CSequentialMatrix : public CMatrix<uint16_t>
{
public:
  CSequentialMatrix (unsigned int columns = 1, unsigned int rows = 1)
    : CMatrix<uint16_t> (columns, rows)
  {
    uint16_t count = 0;
    for(auto row = begin(); row != end(); ++row)
      for(auto column = row->begin(); column < row->end(); ++column, ++count)
        *column = count;
  }
};

#endif // MATRIX_H
