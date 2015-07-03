#include "map_editor_tab.h"

#include <common/global.h>

#include <types/scenes/block_graphics_scene.h>
#include <types/views/block_graphics_view.h>
#include <types/widgets/specialized/tools.h>
#include <types/widgets/custom_action.h>
#include <types/widgets/specialized/editing_toolbar.h>

CMapEditorTab::CMapEditorTab(QWidget* parent, Qt::WindowFlags f)
  : CTabBase(parent, f)
{
  CEditingToolbar* editing_toolbar = new CEditingToolbar(eZoomInOut | eToolbar);
  CTools* tools = new CTools( eTiles | eTileDisplay, this);

  CBlockGraphicsScene* map_scene = new CBlockGraphicsScene(0, 0, 6400, 6400, this);
  map_scene->setSpacingAmount(1, 1);
  map_scene->setSpacingInterval(GData::GetFileData().screen_size.width,
                                GData::GetFileData().screen_size.height);
  map_scene->setMappingSource(GData::GetMapData());

  int sx = GData::GetFileData().map_size.width  * GData::GetFileData().screen_size.width ;
  int sy = GData::GetFileData().map_size.height * GData::GetFileData().screen_size.height;

  CBlockGraphicsView* map = new CBlockGraphicsView(this);
  map->setScene(map_scene);
  map->setZoomEnabled(true);
  map->setSceneRect(0, 0,
                    (sx * 8) + ((sx / map_scene->spacingInterval().x()) * map_scene->spacingAmount().x()) - map_scene->spacingAmount().x(),
                    (sy * 8) + ((sy / map_scene->spacingInterval().y()) * map_scene->spacingAmount().y()) - map_scene->spacingAmount().y());
  map->centerOn(0, 0);

///*
  QScrollBar* scroller = map->horizontalScrollBar();
  scroller->setRange(0, 2);
  scroller->setSingleStep(1);
  //scroller->setSingleStep((scroller->maximum() - scroller->minimum()) / map->width());

  scroller = map->verticalScrollBar();
  scroller->setRange(0, 2);
  scroller->setSingleStep(1);
//  scroller->setSingleStep((scroller->maximum() - scroller->minimum()) / map->height());
//*/

  grid()->setColumnStretch(0, 2);
  addWidget(map            , 0, 0, 2, 1);
  addWidget(editing_toolbar, 0, 1, 1, 1);
  addWidget(tools          , 1, 1, 1, 1);

  tools->setMode(ToolMode::ePick);
  map->setMode(ToolMode::eDraw);

  connect(map  , SIGNAL(pressToolSelected(uint, ToolMode)), tools, SLOT(updateTileDisplay(uint,ToolMode)), Qt::DirectConnection);
  connect(tools, SIGNAL(pressToolSelected(uint, ToolMode)), map, SLOT(setBlockID(uint)), Qt::DirectConnection);
  connect(tools, SIGNAL(modeChanged(ToolMode)), map, SLOT(setMode(ToolMode)), Qt::DirectConnection);
}

CMapEditorTab::~CMapEditorTab(void)
{
}
