#ifndef IMAGE_EDITOR_TAB_H
#define IMAGE_EDITOR_TAB_H

#include "tab_base.h"

class CColorSelect;
class CColorWidget;
class CBlockGraphicsView;
class CPaletteButtons;
class CMultiButton;

class CBlockEditorTab : public CTabBase
{
    Q_OBJECT
  public:
    CBlockEditorTab(QWidget* parent, Qt::WindowFlags f = 0);
    ~CBlockEditorTab(void);

  signals:
    void statusMessage(const QString& message, int timeout);

  private slots:
    void setBlock(uint id);

  private:
    void connectColorUpdates(QList<QObject*> objects);

  private:
    CMultiButton* m_tile_image_buttons;

    QTabWidget* m_tabs;
    CBlockGraphicsView* m_block_view;
    CPaletteButtons* m_palette_buttons;
    CColorSelect* m_color_select; // color selector for drawing or modifying the palette
    CColorWidget* m_current_color;
};

#endif
