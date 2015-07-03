#include "block_editor_tab.h"

#include <common/global.h>
#include <types/widgets/color_select.h>
#include <types/widgets/color_display_widget.h>
#include <types/scenes/block_editor_scene.h>

#include <types/widgets/specialized/editing_toolbar.h>
#include <types/widgets/specialized/tools.h>
#include <types/widgets/specialized/palette_buttons.h>
#include <types/widgets/color_display_widget.h>
#include <types/widgets/multibutton.h>

CBlockEditorTab::CBlockEditorTab(QWidget* parent, Qt::WindowFlags f)
  : CTabBase(parent, f)
{
  m_tabs = new QTabWidget();

  CTabBase* image_editing_tab     = new CTabBase();
  CTabBase* tile_properties_tab   = new CTabBase();
  CTabBase* sprite_properties_tab = new CTabBase();

  m_tabs->addTab(image_editing_tab, "Image Editing");
  m_tabs->addTab(tile_properties_tab, "Tile Properties");
  m_tabs->addTab(sprite_properties_tab, "Sprite Properties");


  QVBoxLayout* tmp_vbox;
  //QVBoxLayout* tmp_hbox;
  QGridLayout* tmp_grid;

  CTools* tools = new CTools( eTiles | eSprites, this);

  grid()->setColumnStretch(0, 2);
  grid()->setColumnStretch(3, 2);
  grid()->setRowStretch   (3, 2);

  addWidget(m_tabs , 0, 1, 2, 1);
  addWidget(tools, 0, 2, 1, 1);

// =============== IMAGE EDITING TAB ===============

  CEditingToolbar* editbar = new CEditingToolbar(eToolbar);
  editbar->setMode(ePick);

  QCheckBox* auto_optimize = new QCheckBox("Automatically optimize palettes");
  auto_optimize->setChecked(true);
  auto_optimize->setDisabled(true); // manual editing is for chumps :P

  QGroupBox* global_palette_gbox = new QGroupBox("Global Palette");
  CCompositePalette* composite_palette = new CCompositePalette(8, QSize(16, 16));
  composite_palette->update();
  global_palette_gbox->setLayout(tmp_vbox = new QVBoxLayout());
  tmp_vbox->addWidget(composite_palette);
  tmp_vbox->addStretch();

  m_current_color = new CColorWidget(this);

  m_color_select = new CColorSelect(this); // color selector for drawing or modifying the palette
  //m_color_select->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  m_block_view = new CBlockGraphicsView (this);
  qDebug() << "m_block_view" << m_block_view;

  CBlockEditorScene* block_scene = new CBlockEditorScene(0, 0, 16, 16, this);

  m_block_view->setScene(block_scene);
  m_block_view->setSceneRect(0, 0, 16, 16, true, true);
  m_block_view->setMode(eDraw);
  m_block_view->scale(16, 16);

  m_palette_buttons = new CPaletteButtons();

  tmp_grid = image_editing_tab->grid();
  tmp_grid->addWidget(editbar            , 0, 0, 1, 2, Qt::AlignRight);
  tmp_grid->addWidget(auto_optimize      , 1, 1, 1, 1);
  tmp_grid->addWidget(global_palette_gbox, 2, 0, 1, 1);
  tmp_grid->addWidget(m_block_view       , 2, 1, 2, 1);
  tmp_grid->addWidget(m_current_color    , 4, 0, 1, 1);
  tmp_grid->addWidget(m_palette_buttons  , 4, 1, 1, 1);
  tmp_grid->addWidget(m_color_select     , 5, 0, 1, 2);

  connect(auto_optimize, SIGNAL(toggled(bool)), block_scene, SIGNAL(autoOptimize(bool)));                                          // enable/disable palette auto optimizing
  connect(block_scene  , SIGNAL(blockSet      (uint,uint,uint)), m_palette_buttons, SLOT(update())        , Qt::DirectConnection); // palette could have completely changed
  connect(block_scene  , SIGNAL(macroBlockSet (uint,uint,uint)), m_palette_buttons, SLOT(setBlockID(uint)), Qt::DirectConnection); // update image palette buttons
  connect(block_scene  , SIGNAL(blockSet      (uint,uint,uint)), composite_palette, SLOT(update())        , Qt::DirectConnection); // update global palette buttons

  // =============== TILE PROPERTIES TAB ===============

  tmp_grid = new QGridLayout();
  tile_properties_tab->addLayout(tmp_grid, 0, 0, 1, 1);

  m_tile_image_buttons = new CMultiButton();
  tmp_grid->addWidget(m_tile_image_buttons);
  m_tile_image_buttons->scale(8.0, 8.0);





  // =============== SPRITE PROPERTIES TAB ===============



  // =============== CONNECTIONS ==============

  //connect(block_scene, SIGNAL(blockInvalidated(uint,uint,uint)), tools, SLOT(updateBlock(uint,uint,uint)));

  connect(block_scene  , SIGNAL(macroBlockSet (uint,uint,uint)), m_block_view, SLOT(update()), Qt::DirectConnection); // update image palette buttons

  connect(tools, SIGNAL(pressToolSelected(uint, ToolMode)), block_scene, SLOT(setMacroBlock(uint)), Qt::DirectConnection);
  connect(tools, SIGNAL(pressToolSelected(uint, ToolMode)), this, SLOT(setBlock(uint)), Qt::DirectConnection); // update tab visibility
  connect(tools, SIGNAL(pressToolSelected(uint, ToolMode)), m_tile_image_buttons, SLOT(setBlockID(uint)), Qt::DirectConnection);

  connect(block_scene, SIGNAL(statusMessage(QString, int)), this, SIGNAL(statusMessage(QString, int)));

  connectColorUpdates({ m_color_select, m_palette_buttons, composite_palette, m_current_color, m_palette_buttons, m_block_view, m_tile_image_buttons }); // connect color update signals/slots together

  block_scene->setMacroBlock(0, 0, 0);
}

CBlockEditorTab::~CBlockEditorTab(void)
{
}

void CBlockEditorTab::setBlock(uint id)
{
  if(id < 0x0100) // tiles
  {
    m_tabs->setTabEnabled(1, true);
    if(m_tabs->currentIndex() == 2)
      m_tabs->setCurrentIndex(1);
    m_tabs->setTabEnabled(2, false);
  }
  else // sprites
  {
    m_tabs->setTabEnabled(2, true);
    if(m_tabs->currentIndex() == 1)
      m_tabs->setCurrentIndex(2);
    m_tabs->setTabEnabled(1, false);
  }
}

void CBlockEditorTab::connectColorUpdates(QList<QObject*> objects)
{
  while(objects.size() > 1)
  {
    QObject* first = objects.takeFirst();
    for(auto pos = objects.begin(); pos != objects.end(); ++pos)
    {
      if(first->metaObject()->indexOfSignal("currentColor(QRgb)") != -1 &&
         (*pos)->metaObject()->indexOfSlot("setColor(QRgb)") != -1)
        connect(first, SIGNAL(currentColor(QRgb)), *pos , SLOT(setColor(QRgb)), Qt::DirectConnection);

      if((*pos)->metaObject()->indexOfSignal("currentColor(QRgb)") != -1 &&
         first->metaObject()->indexOfSlot("setColor(QRgb)") != -1)
        connect(*pos , SIGNAL(currentColor(QRgb)), first, SLOT(setColor(QRgb)), Qt::DirectConnection);

      if((*pos)->metaObject()->indexOfSignal("requestCurrentColor()") != -1 &&
         first->metaObject()->indexOfSlot("sendCurrentColor()") != -1)
        connect(*pos , SIGNAL(requestCurrentColor()), first, SLOT(sendCurrentColor()), Qt::DirectConnection);

      if(first->metaObject()->indexOfSignal("requestCurrentColor()") != -1 &&
         (*pos)->metaObject()->indexOfSlot("sendCurrentColor()") != -1)
        connect(first, SIGNAL(requestCurrentColor()), *pos, SLOT(sendCurrentColor()), Qt::DirectConnection);
    }
  }
}
