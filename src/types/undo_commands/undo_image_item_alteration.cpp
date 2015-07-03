/*/
#include "undo_image_item_alteration.h"

#include <types/misc/zfile_io.h>
#include <types/items/graphics_image_item.h>
#include <types/undo_commands/undo_data_alteration.h>

CUndoImageItemAlteration::CUndoImageItemAlteration(QGraphicsImageItem* item, const image_state_t& old_state, QUndoCommand* parent)
  : QUndoCommand("Set Image for Item", parent),
    m_item(item),
    m_old_state(old_state),
    m_new_state(*GData::GetImage(m_item->imageID()))
{
}

void CUndoImageItemAlteration::undo(void)
{
  Q_ASSERT(false);
  QLinkedImage* image = GData::GetImage(m_item->imageID());
  //= m_old_image;
  m_item->invalidate();
}

void CUndoImageItemAlteration::redo(void)
{
  Q_ASSERT(false);
  QLinkedImage* image = GData::GetImage(m_item->imageID());
  //= m_new_image;
  m_item->invalidate();
}
/*/
