#ifndef __PCE_TYPES__
#define __PCE_TYPES__

#include <QtCore>
#include <QColor>
#include <QImage>
#include <vector>
#include <stdint.h>

//===================================================

class CPaletteData : protected std::vector<uint8_t>
{
  public:
    CPaletteData(void);

    operator QVector<QRgb>(void) const; // QImage "color table" format
    const QVector<QRgb>& operator =(const QVector<QRgb>& colors);

    QRgb get(const uint16_t& color_num) const;
    void set(const uint16_t& color_num, const QRgb& value);

    QVector<QRgb> withoutDuplicates(void);
};

//===================================================

template<uint8_t width, uint8_t height>
class CImageData : protected std::vector<uint8_t>
{
  public:
    CImageData(void);

    const QImage& operator =(const QImage& img);
    operator QImage(void);

    inline QSize size(void) const { return QSize(width, height); }

    virtual uint8_t get(const uint8_t& x, const uint8_t& y) const = 0;
    virtual void	  set(const uint8_t& x, const uint8_t& y, const uint8_t& value) = 0;
};

class CSpriteData : public CImageData<16, 16>
{
  public:
    uint8_t get(const uint8_t& x, const uint8_t& y) const;
    void	  set(const uint8_t& x, const uint8_t& y, const uint8_t& value);
};

class CTileData : public CImageData<8, 8>
{
  public:
    uint8_t get(const uint8_t& x, const uint8_t& y) const;
    void	  set(const uint8_t& x, const uint8_t& y, const uint8_t& value);
};

#endif
