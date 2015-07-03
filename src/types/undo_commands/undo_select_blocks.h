#ifndef UNDO_SELECT_BLOCKS_H
#define UNDO_SELECT_BLOCKS_H

#include <common/global.h>
#include <types/items/graphics_outlined_polygon.h>

class CGraphicsOutlinedPolygon;


class CUndoSelectBlocks : public QUndoCommand
{
  public:
    CUndoSelectBlocks(CGraphicsOutlinedPolygon* selection, QUndoCommand* parent = nullptr);

    void undo(void);
    void redo(void);

  protected:
    CPolygonItemState m_old_state;
    CPolygonItemState m_new_state;
    CGraphicsOutlinedPolygon* m_selection;
};

#endif // UNDO_SELECT_BLOCKS_H
