#ifndef MAP_EDITOR_TAB_H
#define MAP_EDITOR_TAB_H

#include "tab_base.h"

class QGraphicsImageItem;

class CMapEditorTab : public CTabBase
{
    Q_OBJECT
  public:
    CMapEditorTab(QWidget* parent, Qt::WindowFlags f = 0);
    ~CMapEditorTab(void);
};

#endif
