#include "block_graphics_item.h"

CBlockGraphicsItem::CBlockGraphicsItem(QGraphicsItem* parent)
  : QGraphicsImageItem(parent),
    m_id_mem(nullptr)
{
}

QImage CBlockGraphicsItem::image(void) const
{
  return GData::GetImage(imageID());
}

uint16_t CBlockGraphicsItem::imageID(void) const
{
  if(m_id_mem != nullptr)
    return m_type + *m_id_mem;
  return QGraphicsImageItem::imageID();
}

void CBlockGraphicsItem::setImageID(uint16_t image_id)
{
  if(m_id_mem != nullptr)
    *m_id_mem = image_id & 0xFF;
  else
    QGraphicsImageItem::setImageID(image_id);
}

void CBlockGraphicsItem::linkImageID(uint8_t* id_mem)
{
  m_id_mem = id_mem;
  if(id_mem >= GData::GetFileData()->get_tile_ptr())
    m_type = eTile;
  else
    m_type = eSprite;
  update();
}

void CBlockGraphicsItem::savePosition(int x, int y)
{
  setData(ePosition, QPoint(x, y));
}

QPoint CBlockGraphicsItem::loadPosition(void)
{
  return data(ePosition).toPoint();
}
