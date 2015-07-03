#include "block_button_item.h"

#include <common/global.h>
#include <types/scenes/block_graphics_scene.h>
#include <types/undo_commands/undo_data_alteration.h>
#include <types/undo_commands/undo_image_item_alteration.h>

CBlockButtonItem::CBlockButtonItem(QGraphicsItem* parent)
  : QGraphicsImageItem(parent)
{
}

void CBlockButtonItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  Q_UNUSED(widget);
  Q_UNUSED(option);

  QLinkedImage* image = GData::GetImage(imageID());
  if(m_state != image->state() || m_cached.isNull())
  {
    m_state = image->state();
    m_cached = QPixmap::fromImage(*image);
  }

  painter->drawPixmap(offset(), m_cached);
}
