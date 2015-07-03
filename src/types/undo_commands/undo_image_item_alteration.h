/*/
#ifndef UNDO_IMAGE_ITEM_ALTERATION_H
#define UNDO_IMAGE_ITEM_ALTERATION_H

#include <common/global.h>
#include <types/misc/pce_types.h>
#include <types/misc/linked_image.h>

class QGraphicsImageItem;

class CUndoImageItemAlteration : public QUndoCommand
{
  public:
    CUndoImageItemAlteration(QGraphicsImageItem* item, const image_state_t& old_state, QUndoCommand* parent = nullptr);

    void undo(void);
    void redo(void);

  protected:
    QGraphicsImageItem* m_item;
    const image_state_t m_old_state;
    const image_state_t m_new_state;
};

#endif // UNDO_IMAGE_ITEM_ALTERATION_H
/*/
