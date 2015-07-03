#ifndef MAP_TOOLS_H
#define MAP_TOOLS_H

#include <QtGui>
#include <types/views/block_graphics_view.h>
#include <types/scenes/block_graphics_scene.h>

class CTools : public QWidget
{
    Q_OBJECT
  public:

    CTools(int sections, QWidget* parent = nullptr);
    ~CTools(void);

    ToolMode mode(void) const { return m_mode; }

  public slots:
    void setMode(ToolMode mode);
    void updateTileDisplay(uint id, ToolMode mode);
    void updateBlock(uint id, uint x, uint y);

  signals:
    void blockSet(uint x, uint y, uint id);
    void pressToolSelected(uint id, ToolMode mode);
    void modeChanged(ToolMode mode);

  private:
    ToolMode m_mode;
    CBlockGraphicsView*  m_tiles_view;
    CBlockGraphicsView*  m_sprites_view;
    CBlockGraphicsScene* m_block_display_scene;
};

#endif
