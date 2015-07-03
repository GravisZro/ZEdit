#ifndef COLOR_CONVERTERS_H
#define COLOR_CONVERTERS_H

const QRgb ALPHA_MASK  = 0xFF000000;
const QRgb ALPHA_VALUE = 0x01000000;

constexpr int const_qRed  (QRgb rgb) { return ((rgb >> 16) & 0xFF); } // get red part of RGB
constexpr int const_qGreen(QRgb rgb) { return ((rgb >>  8) & 0xFF); } // get green part of RGB
constexpr int const_qBlue (QRgb rgb) { return (rgb & 0xFF); }         // get blue part of RGB

constexpr QRgb const_qGray(QRgb rgb)
{
  return (const_qRed  (rgb) * 11 +
          const_qGreen(rgb) * 16 +
          const_qBlue (rgb) *  5) / 32;
}

constexpr bool colorSort(const QRgb& a, const QRgb& b)
{
  return a > b;
         //((const_qGray(a) << 8) | const_qRed(a)) >
         //((const_qGray(b) << 8) | const_qRed(b));
}

constexpr QRgb const_qRgb(uint r, uint g, uint b)// set RGB value
{
  return (0xFFu << 24) |
         ((r & 0xFF) << 16) |
         ((g & 0xFF) <<  8) |
          (b & 0xFF);
}

constexpr QRgb channelRound24(uint channel)
{

  return channel >= 0x0112 ? 0x0124 :
         channel >= 0x00ED ? 0x00FF :
         channel >= 0x00C9 ? 0x00DB :
         channel >= 0x00A4 ? 0x00B6 :
         channel >= 0x0080 ? 0x0092 :
         channel >= 0x005B ? 0x006D :
         channel >= 0x0037 ? 0x0049 :
         channel >= 0x0012 ? 0x0024 : 0;
}

constexpr QRgb channelFrom24To9Bit(uint channel)
{
  return channel >= 0x0112 ? 8 :
         channel >= 0x00ED ? 7 :
         channel >= 0x00C9 ? 6 :
         channel >= 0x00A4 ? 5 :
         channel >= 0x0080 ? 4 :
         channel >= 0x005B ? 3 :
         channel >= 0x0036 ? 2 :
         channel >= 0x0012 ? 1 : 0;
}

constexpr QRgb channelFrom9BitTo24(uint channel)
{
  return channel >= 8 ? 0x0124 :
         channel >= 7 ? 0x00FF :
         channel >= 6 ? 0x00DB :
         channel >= 5 ? 0x00B6 :
         channel >= 4 ? 0x0092 :
         channel >= 3 ? 0x006D :
         channel >= 2 ? 0x0049 :
         channel >= 1 ? 0x0024 : 0;
}

//constexpr int channelFrom9BitTo24(int channel) { return (int)((qreal)channel * 36.42857142857); }

constexpr QRgb convertFromNativeFormatTo9Bit(uint16_t value)
{
  // Convert from Native PCE color format to 9 Bit
  // From Native:         GGRRRBBB0000000G
  // To   9 Bit : 00000RRR00000GGG00000BBB
  return ALPHA_MASK |
          ((value & 0x3800) <<  5) | // R
          ((value & 0xC000) >>  6) | // G
          ((value & 0x0001) << 10) | // G
          ((value & 0x0700) >>  8);  // B
}

constexpr uint16_t convertFrom9BitToNativeFormat(QRgb color)
{
  // Convert from Native PCE color format to 9 Bit
  // From 9 Bit : 00000RRR00000GGG00000BBB
  // To   Native:         GGRRRBBB0000000G
  return ((color >>  5) & 0x3800) | // R
         ((color <<  6) & 0xC000) | // G
         ((color >> 10) & 0x0001) | // G
         ((color <<  8) & 0x0700);  // B
}

constexpr QRgb convertFrom9BitTo24(QRgb color)
{
  // Convert from 9 Bit to 24 Bit
  // From 9 Bit : 00000RRR00000GGG00000BBB
  // To   24 Bit: RRRRRRRRGGGGGGGGBBBBBBBB
  return const_qRgb(channelFrom9BitTo24(const_qRed  (color)),
                    channelFrom9BitTo24(const_qGreen(color)),
                    channelFrom9BitTo24(const_qBlue (color)));
}

constexpr QRgb convertFrom24To9Bit(QRgb color)
{
  // Convert from 24 Bit to 9 Bit
  // To   24 Bit: RRRRRRRRGGGGGGGGBBBBBBBB
  // From 9 Bit : 00000RRR00000GGG00000BBB
  return const_qRgb(channelFrom24To9Bit(const_qRed  (color)),
                    channelFrom24To9Bit(const_qGreen(color)),
                    channelFrom24To9Bit(const_qBlue (color)));
}


constexpr QRgb round24(QRgb color)
{
  // Convert from 24 Bit to 9 Bit
  // To   24 Bit: RRRRRRRRGGGGGGGGBBBBBBBB
  // From 9 Bit : 00000RRR00000GGG00000BBB
  return const_qRgb(channelRound24(const_qRed  (color)),
                    channelRound24(const_qGreen(color)),
                    channelRound24(const_qBlue (color)));
}

constexpr uint16_t convertFrom24ToNativeFormat(QRgb color)
{
  return convertFrom9BitToNativeFormat(convertFrom24To9Bit(color));
}

constexpr QRgb convertFromNativeFormatTo24(uint16_t value)
{
  return convertFrom9BitTo24(convertFromNativeFormatTo9Bit(value));
}

#endif // COLOR_CONVERTERS_H
