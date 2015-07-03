/*/
#ifndef UNDO_STACK_H
#define UNDO_STACK_H

#include <QUndoStack>

class CUndoStack : public QUndoStack
{
  public:
    CUndoStack(void) : QUndoStack() { }

  public slots:
    void redo(void);
    void undo(void);
};

#endif // UNDO_STACK_H
/*/
