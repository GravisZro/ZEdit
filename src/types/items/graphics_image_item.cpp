#include "graphics_image_item.h"

#include <common/global.h>
#include <types/scenes/block_graphics_scene.h>
#include <types/undo_commands/undo_data_alteration.h>
#include <types/undo_commands/undo_image_item_alteration.h>

#include <common/color_converters.h>

#include <queue>


class CUndoGraphicsItemData : public QUndoCommand
{
  public:
    CUndoGraphicsItemData(QGraphicsImageItem* item, const int key, QVariant new_value, QUndoCommand* parent = nullptr)
      : QUndoCommand(QString("set Key %1 for item at %2 from %3 to %4")
                     .arg(key)
                     .arg(item->imageID())
                     //.arg(QString::number(reinterpret_cast<uint64_t>(item), 16), 16, QChar('0'))
                     .arg(item->data(key).toInt())
                     .arg(new_value.toInt()), parent),
        m_item(item),
        m_key(key),
        m_new_value(new_value),
        m_old_value(item->data(key))
    {
    }

    void undo(void)
    {
      //qDebug() << "undo" << text();
      m_item->setData(m_key, m_old_value);
      m_item->invalidate();
    }

    void redo(void)
    {
      //qDebug() << "redo" << text();
      m_item->setData(m_key, m_new_value);
      m_item->invalidate();
    }

  protected:
    QGraphicsImageItem* m_item;
    const int m_key;
    const QVariant m_new_value;
    const QVariant m_old_value;
};


class CUndoSetGraphicsImageID : public QUndoCommand
{
  public:
    CUndoSetGraphicsImageID(QGraphicsImageItem* item, int new_id, QUndoCommand* parent = nullptr)
      : QUndoCommand(QString("set Image ID for for item at 0x%1 from %2 to %3")
                     .arg(QString::number(reinterpret_cast<uint64_t>(item), 16), 8, QChar('0'))
                     .arg(item->imageID())
                     .arg(new_id), parent),
        m_item(item),
        m_new_id(new_id),
        m_old_id(item->imageID())
    {
    }

    void undo(void)
    {
      //qDebug() << "undo" << text();
      //QObject::disconnect(GData::GetImage(m_new_id), SIGNAL(updated()), m_item, SLOT(invalidate()));

      m_item->setData(eImageIndex, m_old_id);
      m_item->invalidate();
      GData::GetImage(m_new_id)->update();


      //GData::GetImage(m_old_id)->blockSignals(true);
      //GData::GetImage(m_old_id)->setImageID(m_old_id);
      //GData::GetImage(m_old_id)->blockSignals(false);
      //m_item->setImageID(m_old_id);
      //QObject::connect(GData::GetImage(m_old_id), SIGNAL(updated()), m_item, SLOT(invalidate()), Qt::DirectConnection);
    }

    void redo(void)
    {
      //qDebug() << "redo" << text();
      //QObject::disconnect(GData::GetImage(m_old_id), SIGNAL(updated()), m_item, SLOT(invalidate()));

      m_item->setData(eImageIndex, m_new_id);
      m_item->invalidate();
      GData::GetImage(m_new_id)->update();

      //GData::GetImage(m_new_id)->blockSignals(true);
      //GData::GetImage(m_new_id)->setImageID(m_new_id);
      //GData::GetImage(m_new_id)->blockSignals(false);
      //m_item->setImageID(m_new_id);
      //QObject::connect(GData::GetImage(m_new_id), SIGNAL(updated()), m_item, SLOT(invalidate()), Qt::DirectConnection);
    }

  protected:
    QGraphicsImageItem* m_item;
    const uint m_new_id;
    const uint m_old_id;
};






QGraphicsImageItem::QGraphicsImageItem(uint image_id, QGraphicsItem* parent)
  : QGraphicsImageItem(parent)
{
  setData(eImageIndex, image_id);
}

QGraphicsImageItem::QGraphicsImageItem(QGraphicsItem* parent)
  : QGraphicsObject(parent),
    m_offset(0.0, 0.0),
    m_auto_optimize(true),
    m_state(-1)
{
}

QGraphicsImageItem::~QGraphicsImageItem(void)
{
}

QVariant QGraphicsImageItem::data(int key) const
{
  if(key == eImageIndex)
    return QVariant::fromValue(QGraphicsItem::data(eImageType ).toUInt() +
                               QGraphicsItem::data(eImageIndex).toUInt());
  return QGraphicsItem::data(key);
}

void QGraphicsImageItem::setData (int key, const QVariant& value)
{
  if(key == eImageIndex)
  {
    QGraphicsItem::setData(eImageType, eTile);
    QGraphicsItem::setData(eImageIndex, value.toInt());
  }
  else
    QGraphicsItem::setData(key, value);
}


void QGraphicsImageItem::savePosition(int x, int y)
{
  setData(ePosition, QPoint(x, y));
}

QPoint QGraphicsImageItem::loadPosition(void)
{
  return data(ePosition).toPoint();
}

void QGraphicsImageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  Q_UNUSED(widget);
  Q_UNUSED(option);

  QLinkedImage* image = GData::GetImage(imageID());
  if(m_state != image->state())
  {
    //qDebug() << "QGraphicsImageItem::imageID() =" << QGraphicsImageItem::imageID();
    m_state = image->state();
    auto colors = image->colorTable();
    QRgb backup = image->color(0);

    image->setColor(0, 0x88000000);
    image->setColor(1, 0xFF888888);
    image->setColor(2, 0xFFAAAAAA);
    image->setColor(3, 0xFFFFFFFF);

    image->setColor(4, 0xFF000044);
    image->setColor(5, 0xFF000088);
    image->setColor(6, 0xFF0000AA);
    image->setColor(7, 0xFF0000FF);

    image->setColor(8, 0xFF004400);
    image->setColor(9, 0xFF008800);
    image->setColor(10, 0xFF00AA00);
    image->setColor(11, 0xFF00FF00);

    image->setColor(12, 0xFF440000);
    image->setColor(13, 0xFF880000);
    image->setColor(14, 0xFFAA0000);
    image->setColor(15, 0xFFFF0000);

    m_cached = QPixmap::fromImage(*image);
    m_cached.setMask(QBitmap::fromImage(image->createMaskFromColor(ALPHA_VALUE)));
    image->setColorTable(colors);
  }

  painter->drawPixmap(offset(), m_cached);
}

QPainterPath QGraphicsImageItem::opaqueArea(void) const
{
  return shape();
}

const QImage& QGraphicsImageItem::image(void) const
{
  return *GData::GetImage(imageID());
}

uint16_t QGraphicsImageItem::imageID(void) const
{
  return data(eImageIndex).value<uint16_t>();
}

void QGraphicsImageItem::setImageID(uint16_t image_id)
{
  prepareGeometryChange();
  //GData::startUndoMacro(QString("setImageID(%1)").arg(image_id));
  GData::addUndoAction(new CUndoSetGraphicsImageID(this, image_id));
//GData::addUndoAction(new CUndoGraphicsItemData(this, eImageIndex, image_id));
  //GData::endUndoMacro();
}

void QGraphicsImageItem::setOffset(QPointF offset)
{
  if (m_offset != offset)
  {
    prepareGeometryChange();
    m_offset = offset;
  }
  invalidate();
}

QPointF QGraphicsImageItem::offset (void) const
{
  return m_offset;
}

QRectF QGraphicsImageItem::boundingRect(void) const
{
  QLinkedImage* image = GData::GetImage(imageID());

  if (flags() & ItemIsSelectable)
    return QRectF(offset(), image->size()).adjusted(-0.5, -0.5, 0.5, 0.5);
  else
    return QRectF(offset(), image->size());
}

QRgb QGraphicsImageItem::pixel(int x, int y)
{
  QLinkedImage* image = GData::GetImage(imageID());
  return image->pixel(x, y);
}

void QGraphicsImageItem::setPixel(QRgb color, int x, int y)
{
  QLinkedImage* image = GData::GetImage(imageID());
  image->setPixel(x, y, color);
}

void QGraphicsImageItem::setAutoOptimize(bool enable)
{
  m_auto_optimize = enable;
  GData::GetImage(imageID())->setAutoOptimizeEnabled(enable);
}

void QGraphicsImageItem::invalidate(void)
{
  m_state = -1;
  //QGraphicsItem::update(boundingRect());
  GData::invalidateGraphicsViews(); // very sloppy >:(
}
