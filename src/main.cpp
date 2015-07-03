#include <QApplication>
#include <gui/mainwindow.h>
#include <QMetaType>
#include <stdint.h>
#include <common/global.h>

#include <common/color_converters.h>

Q_DECLARE_METATYPE( int8_t);
Q_DECLARE_METATYPE(uint8_t);
Q_DECLARE_METATYPE( int16_t);
Q_DECLARE_METATYPE(uint16_t);
Q_DECLARE_METATYPE( int32_t);
Q_DECLARE_METATYPE(uint32_t);
Q_DECLARE_METATYPE( int64_t);
Q_DECLARE_METATYPE(uint64_t);

int main(int argc, char *argv[])
{
/*
  QSet<QRgb> colors;
  QSet<QRgb> colors_found;
  QRgb tmp;

  for(int r = 0; r < 8; ++r)
    for(int g = 0; g < 8; ++g)
      for(int b = 0; b < 8; ++b)
        colors.insert(qRgb(r, g, b));

  const uint64_t size = 100000;
  const qreal ratio = 1.0 / size;
  const qreal saturation = 1.0;
  qreal lightness;
  qreal hue;

  for (uint64_t y = 0; y < size; ++y)
  {
    lightness = 1.0 - y * ratio;
    for (uint64_t x = 0; x < size && !colors.empty(); ++x)
    {
      hue = x * ratio;
      QColor color = QColor::fromHslF(hue, saturation, lightness).rgb();
      colors_found.insert(round24(color.rgb()));
      colors.remove(convertFrom24To9Bit(color.rgb()));
    }
  }

  qDebug() << "colors found:" << colors_found.size();
  qDebug() << "colors missing:" << colors.size();
  return 0;
*/
  qRegisterMetaType< int8_t >();
  qRegisterMetaType<uint8_t >();
  qRegisterMetaType< int16_t>();
  qRegisterMetaType<uint16_t>();
  qRegisterMetaType< int32_t>();
  qRegisterMetaType<uint32_t>();
  qRegisterMetaType< int64_t>();
  qRegisterMetaType<uint64_t>();

  qDebug("\nmain()");

  Q_INIT_RESOURCE(map_tools);
  QApplication a(argc, argv);
  CMainWindow w;
  w.show();
  return a.exec();
}
