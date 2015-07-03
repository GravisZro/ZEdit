#ifndef COLOR_SELECT_H
#define COLOR_SELECT_H

#include <QWidget>

class CColorSelect : public QWidget
{
    Q_OBJECT
  public:
    CColorSelect(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
    ~CColorSelect(void);

    QSize sizeHint(void) const;
    QSize size    (void) const { return sizeHint(); }

    qreal scale(void) const;
    void setScale(qreal scale);

  signals:
    void currentColor(QRgb color);

  public slots:
    void paint(void);
    void setColor(QRgb color);
    void sendCurrentColor(void);

  protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event); // common mouse event
    void mouseMoveEvent (QMouseEvent* event) { mousePressEvent(event); }

  private:
    qreal m_scale;
    uint m_r, m_g, m_b;
    QImage m_image;
    QPoint m_previous_position;
    QPoint m_pixel_xy;
};

#endif
