#ifndef GRAPHICS_IMAGE_ITEM_H
#define GRAPHICS_IMAGE_ITEM_H

#include <common/global.h>

#include <QGraphicsObject>
#include <QPainter>
#include <QImage>

class QGraphicsImageItem : public QGraphicsObject
{
    Q_OBJECT
  public:
    QGraphicsImageItem(uint image_id, QGraphicsItem* parent = nullptr );
    QGraphicsImageItem(QGraphicsItem* parent = nullptr );
    ~QGraphicsImageItem (void);

    QVariant data(int key) const;
    void setData (int key, const QVariant& value);

    void savePosition(int x, int y);
    QPoint loadPosition(void);

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr);

    virtual QPainterPath opaqueArea(void) const;
    virtual QRectF boundingRect(void) const;

    QPointF offset(void) const;
    const QImage& image(void) const;
    QRgb pixel(int x, int y);
    bool autoOptimize(void) const { return m_auto_optimize; }

    virtual uint16_t imageID(void) const;
  public slots:
    void setAutoOptimize(bool enable = true);
    void setPixel  (QRgb color, int x, int y); // index or QRgb value
    void setOffset (QPointF offset);
    virtual void setImageID(uint16_t image_id);
    void invalidate(void);

  signals:
    void statusMessage(const QString& message, int timeout);

  protected:
    QPointF m_offset;
    bool m_auto_optimize;
    QPixmap m_cached;

    int m_state;
};

#endif // GRAPHICS_IMAGE_ITEM_H
