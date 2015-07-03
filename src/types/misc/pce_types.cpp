#include "pce_types.h"
#include "zfile_io.h"

#include <common/color_converters.h>


constexpr uint8_t getbit   (uint8_t bitnum, uint8_t data) { return (data >> bitnum) & 1; }
constexpr uint8_t clearbit (uint8_t bitnum, uint8_t data) { return data & (0x00 ^ (1 << bitnum)); }

constexpr uint8_t copybit (uint8_t dest_bit, uint8_t dest_data, uint8_t src_bit, uint8_t src_data)
  { return clearbit(dest_bit, dest_data) | (getbit(src_bit, src_data) << dest_bit); }

struct bits4_t
{
  uint8_t b0 : 1;
  uint8_t b1 : 1;
  uint8_t b2 : 1;
  uint8_t b3 : 1;
  uint8_t : 4;
  operator uint8_t (void) { return *reinterpret_cast<uint8_t*>(this); }
};

//========================================================================================


CPaletteData::CPaletteData(void)
  : std::vector<uint8_t>(ZValues::colors_in_palette)
{
}

CPaletteData::operator QVector<QRgb>(void) const // QImage "color table" format
{
  QVector<QRgb> colors;
  colors.reserve(std::vector<uint8_t>::size());
  for(auto pos = begin(); pos != end(); ++pos)
    colors.push_back(convertFrom9BitToNativeFormat(*pos));
  return colors;
}

const QVector<QRgb>& CPaletteData::operator =(const QVector<QRgb>& colors)
{
  clear();
  reserve(colors.size());
  for(auto pos = begin(); pos != end(); ++pos)
    push_back(convertFrom24ToNativeFormat(*pos));
  return colors;
}

QRgb CPaletteData::get(const uint16_t& color_num) const
{
  return convertFromNativeFormatTo24(at(color_num));
}

void CPaletteData::set(const uint16_t& color_num, const QRgb& value)
{
  at(color_num) = convertFrom24ToNativeFormat(value);
}


QVector<QRgb> CPaletteData::withoutDuplicates(void)
{
  QVector<QRgb> colors;
  QSet<uint16_t> raw_colors;

  for(auto pos = begin(); pos != end(); ++pos)
    raw_colors += *pos;
  colors.reserve(raw_colors.size());

//  foreach(uint16_t raw_color : raw_colors) // C++11 foreach
  for(auto pos2 = raw_colors.begin(); pos2 != raw_colors.end(); ++pos2)
    colors.push_back(convertFromNativeFormatTo24(*pos2));

  return colors;
}


//========================================================================================

template<uint8_t width, uint8_t height>
CImageData<width, height>::CImageData(void)
  : std::vector<uint8_t>(width * height / 2) // each pixel is 4 bits
{
}

template<uint8_t width, uint8_t height>
const QImage& CImageData<width, height>::operator =(const QImage& img)
{
  Q_ASSERT(width == img.width() &&
           height == img.height());

  const uint8_t* d = img.constBits();
  for(uint8_t y = 0; y < height; ++y)
    for(uint8_t x = 0; x < width; ++x)
      set(x, y, d[x + (y * width)]);

  return img;
}

template<uint8_t width, uint8_t height>
CImageData<width, height>::operator QImage(void)
{
  QImage img(width, height, QImage::Format_Indexed8);
  for(uint8_t y = 0; y < height; ++y)
    for(uint8_t x = 0; x < width; ++x)
      img.setPixel(x, y, get(x, y));
  img.setColorTable({
                      qRgb(0x00, 0x00, 0x00),
                      qRgb(0x80, 0x80, 0x80),
                      qRgb(0xFF, 0xFF, 0xFF),

                      qRgb(0x80, 0x00, 0x00),
                      qRgb(0xFF, 0x00, 0x00),
                      qRgb(0x00, 0x80, 0x00),
                      qRgb(0x00, 0xFF, 0x00),
                      qRgb(0x00, 0x00, 0x80),
                      qRgb(0x00, 0x00, 0xFF),

                      qRgb(0x80, 0x80, 0x00),
                      qRgb(0xFF, 0xFF, 0x00),
                      qRgb(0x00, 0x80, 0x80),
                      qRgb(0x00, 0xFF, 0xFF),
                      qRgb(0x80, 0x00, 0x80),
                      qRgb(0xFF, 0x00, 0xFF),

                      qRgb(0x40, 0x80, 0xFF),
                    });
  return img;
}

//========================================================================================

uint8_t CSpriteData::get(const uint8_t& x, const uint8_t& y) const
{
  const uint8_t* d = data() +
                     ((x & 8) ? 0 : 1) +  // left or right side
                     (y << 1);            // get row number y
  return bits4_t
  {
    getbit(x & 7, d[0x00]),
    getbit(x & 7, d[0x20]),
    getbit(x & 7, d[0x40]),
    getbit(x & 7, d[0x60])
  };
}

void CSpriteData::set(const uint8_t& x, const uint8_t& y, const uint8_t& value)
{
   uint8_t* d = data() +
                ((x & 8) ? 0 : 1) +  // left or right side
                (y << 1);            // get row number y

  d[0x00] = copybit(x, d[0x00], 1, value);
  d[0x20] = copybit(x, d[0x01], 0, value);
  d[0x40] = copybit(x, d[0x10], 3, value);
  d[0x60] = copybit(x, d[0x11], 2, value);
}

//========================================================================================

uint8_t CTileData::get(const uint8_t& x, const uint8_t& y) const
{
  const uint8_t* d = data() + y;
  return bits4_t
  {
    getbit(x, d[0x00]),
    getbit(x, d[0x01]),
    getbit(x, d[0x10]),
    getbit(x, d[0x11])
  };
}

void CTileData::set(const uint8_t& x, const uint8_t& y, const uint8_t& value)
{
  uint8_t* d = data() + y;
  d[0x00] = copybit(x, d[0x00], 1, value);
  d[0x01] = copybit(x, d[0x01], 0, value);
  d[0x10] = copybit(x, d[0x10], 3, value);
  d[0x11] = copybit(x, d[0x11], 2, value);
}
