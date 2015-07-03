#include "multibutton.h"

#include <types/scenes/block_graphics_scene.h>

CMultiButton::CMultiButton(QWidget* parent)
  : CBlockGraphicsView(parent),
    m_id(0),
    m_mask(0)
{
  m_scene = new CBlockGraphicsScene(0, 0, 16, 16, parent);

  setScene(m_scene);
  setSceneRect(0, 0, 16, 16, true, true);
  m_scene->setMappingSource(CByteMatrix(1, 1, 0));

}

void CMultiButton::setBlockID(uint id)
{
  m_id = id;
  m_scene->setBlock(m_id, (uint)0, (uint)0);
  //m_mask = GData::GetFileData().get_tile_mask(m_id);

  scene()->update();
}


void CMultiButton::drawForeground(QPainter* painter, const QRectF& rect)
{
  QColor off(0x00, 0x00, 0x00, 150);
  QColor on (0xFF, 0xFF, 0xFF, 150);

  painter->fillRect(0, 0, 8, 8, m_mask & 0x01 ? on : off);
  painter->fillRect(8, 0, 8, 8, m_mask & 0x02 ? on : off);
  painter->fillRect(0, 8, 8, 8, m_mask & 0x04 ? on : off);
  painter->fillRect(8, 8, 8, 8, m_mask & 0x08 ? on : off);

}

void CMultiButton::mouseReleaseEvent(QMouseEvent* event)
{
/*
  int id  = ((width() / 2) > event->x() ? 0 : 1) + ((height() / 2) > event->y() ? 0 : 2);

  m_mask ^= 1 << id;
  GData::GetFileData().tiles.masks[m_id] = m_mask;
      //.set_tile_mask(m_id, m_mask);

  emit buttonToggled(id, m_mask & (1 << id));
  scene()->update();
  qDebug() << "id:" << m_id << "  quadrant:" << id;
*/
}
