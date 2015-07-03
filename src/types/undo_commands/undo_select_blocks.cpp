#include "undo_select_blocks.h"


#include <types/items/graphics_outlined_polygon.h>

CUndoSelectBlocks::CUndoSelectBlocks(CGraphicsOutlinedPolygon* selection, QUndoCommand* parent)
  : QUndoCommand("Block Selection", parent),
    m_old_state(selection->previousState()),
    m_new_state(selection->currentState()),
    m_selection(selection)
{
}

void CUndoSelectBlocks::undo(void)
{
  Q_ASSERT(false);
//  qDebug() << "undo" << text();
  m_selection->setState(m_old_state);
}

void CUndoSelectBlocks::redo(void)
{
  Q_ASSERT(false);
//  qDebug() << "redo" << text();
  m_selection->setState(m_new_state);
}
