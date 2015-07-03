#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

struct dimensions_t
{
  union
  {
    uint16_t columns;
    uint16_t width;
  };

  union
  {
    uint16_t rows;
    uint16_t height;
  };
};

struct coords_t
{
  union
  {
    uint16_t x;
    uint16_t column;
  };

  union
  {
    uint16_t y;
    uint16_t row;
  };
};

#endif // STRUCTS_H
