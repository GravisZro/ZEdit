#ifndef EDITING_TOOLBAR_H
#define EDITING_TOOLBAR_H

#include <QtGui>
#include <types/views/block_graphics_view.h>

class CEditingToolbar : public QWidget
{
    Q_OBJECT
  public:
    CEditingToolbar(int sections, QWidget* parent = nullptr);
    ~CEditingToolbar(void);

    ToolMode mode(void) const;

  public slots:
    void setMode(ToolMode mode);
  signals:
    void pressToolSelected(uint id, ToolMode mode);
    void modeChanged(ToolMode mode);

  private:
    ToolMode m_mode;
};


#endif // EDITING_TOOLBAR_H
