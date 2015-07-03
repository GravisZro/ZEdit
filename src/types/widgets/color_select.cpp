#include "color_select.h"

#include <common/color_converters.h>

#include <QtCore>
#include <QPainter>
#include <QMouseEvent>

//static QImage g_image("hsl.png");
//static QSize image_size(g_image.size());

static QSize image_size(42, 16);
//static QSize image_size(256, 256);

CColorSelect::CColorSelect(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_scale(12.0),
    m_r(0),
    m_g(0),
    m_b(0)
{
  //setScale(6.0);
}

CColorSelect::~CColorSelect(void)
{
}

qreal CColorSelect::scale(void) const
{
  return m_scale;
}

void CColorSelect::setScale(qreal scale)
{
  m_scale = scale;
}

QSize CColorSelect::sizeHint(void) const
{
  return image_size * scale(); // scaled image size
}

void CColorSelect::paintEvent(QPaintEvent* event)
{
  paint();
  QPainter painter(this);
  qreal s = scale();

  if(m_image.pixel(m_pixel_xy) != const_qRgb(m_r, m_g, m_b))
  {
    m_pixel_xy.setX(((m_g << 3) + m_b) % 32);
    m_pixel_xy.setY(m_r + (8 * (((m_g << 3) + m_b) / 32)));
  }

  painter.drawImage(0, 0, m_image.scaled(size()));
  painter.setPen(m_g > 3 ? Qt::black : Qt::white);
  if(m_image.pixel(m_pixel_xy) & ALPHA_MASK)
    painter.drawRect(QRectF(m_pixel_xy.x() * s, m_pixel_xy.y() * s, s, s));
}

void CColorSelect::paint(void) // translates the data to the QImage
{
  if(m_image.isNull())
  {
    //(1016, 616);
    /*
    QPixmap pixmap(image_size);
    QPainter painter(&pixmap);
    painter.drawImage(0, 0, );
    painter.drawImage(513, 0, QImage("hex2.png"));
    m_image = pixmap.toImage();
    */

    QSet<QRgb> colors;

    for(int r = 0; r < 8; ++r)
      for(int g = 0; g < 8; ++g)
        for(int b = 0; b < 8; ++b)
          colors.insert(convertFrom9BitTo24(qRgb(r, g, b)));

#if 0
    m_image = g_image;

    for(int y = 0; y < m_image.height(); ++y)
    {
      for(int x = 0; x < m_image.width(); ++x)
      {
        m_image.setPixel(x, y, round24(m_image.pixel(x, y)));
        colors.remove(m_image.pixel(x, y));
      }
    }

//    qDebug() << "colors missing:" << colors.size();

#elif 1
    m_image = QImage(image_size, QImage::Format_ARGB32);
    m_image.fill(Qt::transparent);
    //m_image.fill(Qt::black);

    for(int r = 0; r < 8; ++r)
      for(int g = 0; g < 8; ++g)
        for(int b = 0; b < 8; ++b)
          m_image.setPixel(((g << 3) + b) % 32, r + (8 * (((g << 3) + b) / 32)), convertFrom9BitTo24(qRgb(r, g, b)));

    for(int i = 0; i < 8; ++i)
    {
      int levl = convertFrom9BitTo24(i);

      // light
      m_image.setPixel(34, 7 - i, qRgb(0xFF, levl, levl)); // red
      m_image.setPixel(35, 7 - i, qRgb(0xFF, 0xFF, levl)); // yellow
      m_image.setPixel(36, 7 - i, qRgb(levl, 0xFF, levl)); // green
      m_image.setPixel(37, 7 - i, qRgb(levl, 0xFF, 0xFF)); // cyan
      m_image.setPixel(38, 7 - i, qRgb(levl, levl, 0xFF)); // blue
      m_image.setPixel(39, 7 - i, qRgb(0xFF, levl, 0xFF)); // magenta

      // dark
      m_image.setPixel(34, 15 - i, qRgb(levl, 0x00, 0x00)); // red
      m_image.setPixel(35, 15 - i, qRgb(levl, levl, 0x00)); // yellow
      m_image.setPixel(36, 15 - i, qRgb(0x00, levl, 0x00)); // green
      m_image.setPixel(37, 15 - i, qRgb(0x00, levl, levl)); // cyan
      m_image.setPixel(38, 15 - i, qRgb(0x00, 0x00, levl)); // blue
      m_image.setPixel(39, 15 - i, qRgb(levl, 0x00, levl)); // magenta

      m_image.setPixel(40, 15 - (i * 2), qRgb(levl, levl, levl)); // grayscale
      m_image.setPixel(40, 14 - (i * 2), qRgb(levl, levl, levl)); // grayscale
    }
#elif 1
    m_image = QImage(image_size, QImage::Format_ARGB32);
    m_image.fill(Qt::transparent);
    m_image.fill(Qt::black);

    QSet<QRgb> colors_found;
    QRgb tmp;

    const uint64_t size = 256;
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
        tmp = round24(color.rgb());
        colors_found.insert(tmp);
        colors.remove(convertFrom24To9Bit(color.rgb()));
        m_image.setPixel(x, y, tmp);
      }
    }

//    QList<QRgb> color_list = colors_found.toList();
//    qDebug() << "colors found:" << colors_found.size();

    /*
    printf("\n");
    while(!color_list.empty())
    {
      printf("0x%x, ", color_list.takeFirst());
    }
    printf("\nend\n");
    fflush(stdout);
*/
    //qDebug() << "colors missing:" << colors.size();
    /*
    color_list = colors.toList();
    printf("\n");
    while(!color_list.empty())
    {
      //Q_ASSERT((color_list.at(0) & 0xFFE0E0E0) == color_list.at(0));
      printf("0x%x, ", color_list.takeFirst());
    }
    printf("\nend\n");
    fflush(stdout);
*/
#elif 1

#endif
  }
}

void CColorSelect::setColor(QRgb color)
{
  setUpdatesEnabled(false);

  m_r = qRed(color);
  m_g = qGreen(color);
  m_b = qBlue(color);

  if((m_r | m_g | m_b) > 7 &&
     (m_r != channelFrom24To9Bit(m_r) ||
      m_g != channelFrom24To9Bit(m_g) ||
      m_b != channelFrom24To9Bit(m_b)))
  {
    m_r = channelFrom24To9Bit(m_r);
    m_g = channelFrom24To9Bit(m_g);
    m_b = channelFrom24To9Bit(m_b);
  }

  repaint();

  setUpdatesEnabled(true);
}

void CColorSelect::mousePressEvent(QMouseEvent* event)
{
  if(m_previous_position == event->pos() / scale() ||
     event->x() < 0 || event->y() < 0 ||
     event->x() >= width() ||
     event->y() >= height())
    { return; } // ignore useless calls

  m_previous_position = event->pos() / scale();
  m_pixel_xy.setX(event->x() / scale());
  m_pixel_xy.setY(event->y() / scale());

  const QRgb color = m_image.pixel(m_pixel_xy);
  if(m_image.pixel(m_pixel_xy) & ALPHA_MASK)
  {
    setColor(color);
    emit currentColor(color);
  }
}

void CColorSelect::sendCurrentColor(void)
{
  if((m_r | m_g | m_b) > 7)
    emit currentColor(const_qRgb(m_r, m_g, m_b));
  else
    emit currentColor(convertFrom9BitTo24(const_qRgb(m_r, m_g, m_b)));
}
