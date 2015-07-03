/*/
#include "undo_stack.h"

#include <QtCore>

void CUndoStack::redo(void)
{
  Q_ASSERT(false);
//  qDebug() << redoText();
  QUndoStack::redo();
}

void CUndoStack::undo(void)
{
  Q_ASSERT(false);
//  qDebug() << undoText();
  QUndoStack::undo();
}
/*/
