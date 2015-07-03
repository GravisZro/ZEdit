#include "tools.h"

#include <common/global.h>
#include <types/scenes/block_graphics_scene.h>
#include <types/widgets/custom_action.h>
#include <gui/tabs/map_editor_tab.h>

#define ADD_BUTTON(icon, name, value) \
  (new CCustomAction(tools->addAction(QIcon(icon), name), value, this, SLOT(setMode(ToolMode)), this))

#define ADD_BUTTON2(icon, name, value) \
  (new CCustomAction(zoom_in_out->addAction(QIcon(icon), name), value, this, SLOT(setMode(ToolMode)), this))

CTools::CTools(int sections, QWidget* parent)
  : QWidget(parent)
{
  QVBoxLayout* vbox = new QVBoxLayout();
  CBlockGraphicsScene* sprites_scene = nullptr;
  CBlockGraphicsView* tile_display_view = nullptr;
  CBlockGraphicsScene* tiles_scene = nullptr;
  QToolBar* zoom_in_out;
  QToolBar* tools;
  m_block_display_scene = nullptr;

  setLayout(vbox);

  //================================

  if(sections & eZoomInOut)
  {
    zoom_in_out = new QToolBar(this);
    zoom_in_out->setIconSize(QSize(16, 16));

    ADD_BUTTON2(":/buttons/zoom_in.png" , "Zoom In" , eZoomIn  );
    ADD_BUTTON2(":/buttons/zoom_out.png", "Zoom Out", eZoomOut );

    vbox->addWidget(zoom_in_out);
  }

  if(sections & eToolbar)
  {
    tools = new QToolBar(this);
    tools ->setIconSize(QSize(24, 24));

    ADD_BUTTON(":/buttons/rect_select.png"    , "Rectangular Select", eRectSelect     );
    ADD_BUTTON(":/buttons/fuzzy_select.png"   , "Fuzzy Selection"   , eFuzzySelect    )->setDisabled(true);
    ADD_BUTTON(QCommonStyle().standardIcon(
               QStyle::SP_DialogCancelButton) , "Delete Selected"   , eDeleteSelection);
    ADD_BUTTON(":/buttons/move.png"           , "Move Selection"    , eMoveSelection  )->setDisabled(true);
    ADD_BUTTON(":/buttons/pencil.png"         , "Draw"              , eDraw           );
    ADD_BUTTON(":/buttons/color_picker.png"   , "Tile Picker"       , ePick           );
    ADD_BUTTON(":/buttons/bucket_fill.png"    , "Fill"              , eFill           )->setDisabled(true);

    vbox->addWidget(tools);
  }

  //================================

  dimensions_t tiles_dimensions   = { (8 + 1) * 32, (8 + 1) * 32 };
  dimensions_t sprites_dimensions = { (16 + 1) * 16, (16 + 1) * 16 };

  if(sections & eTiles)
  {
    m_tiles_view = new CBlockGraphicsView (this);
    tiles_scene  = new CBlockGraphicsScene(0, 0, tiles_dimensions.columns, tiles_dimensions.rows, this);

    tiles_scene->setSpacingAmount(1, 1);
    tiles_scene->setSpacingInterval(1, 1);

    m_tiles_view->setScene(tiles_scene);
    m_tiles_view->setSceneRect(0, 0, tiles_dimensions.columns, tiles_dimensions.rows, true, true);
    tiles_scene->setMappingSource(CSequentialMatrix(32, 32));
    m_tiles_view->setMode(ePick);
    m_tiles_view->setZoomEnabled(true);

    vbox->addSpacing(10);

    QGroupBox* tile_pal_gbox = new QGroupBox("Tile Palette");
    tile_pal_gbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tile_pal_gbox->setLayout(new QVBoxLayout());
    tile_pal_gbox->layout()->addWidget(m_tiles_view);
    vbox->addWidget(tile_pal_gbox);

    connect(m_tiles_view, SIGNAL(pressToolSelected(uint, ToolMode)), this, SIGNAL(pressToolSelected(uint, ToolMode)), Qt::DirectConnection);
    qDebug() << "CTools - eTiles:" << m_tiles_view;
  }

  //================================

  if(sections & eSprites)
  {
    m_sprites_view = new CBlockGraphicsView (this);
    sprites_scene  = new CBlockGraphicsScene(0, 0, tiles_dimensions.columns, tiles_dimensions.rows, this);

    sprites_scene->setSpacingAmount(1, 1);
    sprites_scene->setSpacingInterval(1, 1);

    m_sprites_view->setScene(sprites_scene);
    m_sprites_view->setSceneRect(0, 0, sprites_dimensions.columns, sprites_dimensions.rows, true, true);
    sprites_scene->setMappingSource(CSequentialMatrix(16, 16));
    m_sprites_view->setMode(ePick);

    vbox->addSpacing(10);

    QGroupBox* sprite_pal_gbox = new QGroupBox("Sprite Palette");
    sprite_pal_gbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sprite_pal_gbox->setLayout(new QVBoxLayout());
    sprite_pal_gbox->layout()->addWidget(m_sprites_view);
    vbox->addWidget(sprite_pal_gbox);

    connect(m_sprites_view, SIGNAL(pressToolSelected(uint, ToolMode)), this, SIGNAL(pressToolSelected(uint, ToolMode)), Qt::DirectConnection);
    qDebug() << "CTools - eSprites:" << m_sprites_view;
  }

  //================================

  if(sections & eTileDisplay)
  {
    tile_display_view     = new CBlockGraphicsView (this);
    m_block_display_scene = new CBlockGraphicsScene(0, 0, 16, 16, this);

    tile_display_view->setScene(m_block_display_scene);
    tile_display_view->setSceneRect(0, 0, 16, 16, true, true);
    m_block_display_scene->setMappingSource(CByteMatrix(1, 1, 0));
    tile_display_view->scale(8, 8);

    vbox->addSpacing(10);

    QGroupBox* tile_display_gbox = new QGroupBox("Current Tile");
    tile_display_gbox->setLayout(new QHBoxLayout());
    tile_display_gbox->layout()->addWidget(tile_display_view);
    vbox->addWidget(tile_display_gbox);
  }

  vbox->addStretch();

  //================================

  connect(this, SIGNAL(pressToolSelected(uint, ToolMode)), this, SLOT(updateTileDisplay(uint, ToolMode)), Qt::DirectConnection);

  if(tiles_scene != nullptr)
    connect(tiles_scene  , SIGNAL(blockSet(const uint,const uint,const uint)), this, SIGNAL(blockSet(const uint,const uint,const uint)), Qt::DirectConnection);
  if(sprites_scene != nullptr)
    connect(sprites_scene, SIGNAL(blockSet(const uint,const uint,const uint)), this, SIGNAL(blockSet(const uint,const uint,const uint)), Qt::DirectConnection);
}

CTools::~CTools(void)
{
}

void CTools::setMode(ToolMode mode)
{
  m_mode = mode;
  emit modeChanged(m_mode);
}

void CTools::updateTileDisplay(uint id, ToolMode mode)
{
  if(mode == ePick && m_block_display_scene != nullptr)
    m_block_display_scene->setBlock(id, (uint)0, (uint)0);
}

void CTools::updateBlock(uint id, uint x, uint y)
{
  m_sprites_view->invalidateScene();
  m_tiles_view  ->invalidateScene();
}
