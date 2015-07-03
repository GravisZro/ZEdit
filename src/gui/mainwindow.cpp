#include "mainwindow.h"

#include <common/global.h>

#include <gui/tabs/map_editor_tab.h>
#include <gui/tabs/block_editor_tab.h>
//#include <gui/tabs/palette_editor_tab.h>

CMainWindow::CMainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  setMinimumSize(900, 825);
  QTabWidget* tabs = new QTabWidget();

  CBlockEditorTab* block_editor;

  tabs->addTab(new CMapEditorTab(this), "Map Editor");
  tabs->addTab(block_editor = new CBlockEditorTab(this), "Sprite/Tile Editor");
  //tabs->addTab(new CPaletteEditorTab(this), "Manual Palette Editor");
  //tabs->setCurrentIndex(1);


  setStatusBar(new QStatusBar());
  setCentralWidget(tabs);

  //setCentralWidget(block_editor = new CBlockEditorTab(this));

  connect(block_editor, SIGNAL(statusMessage(QString, int)), statusBar(), SLOT(showMessage(QString, int)));

  m_undo = new QAction(this);
  m_redo = new QAction(this);

  addAction(m_undo);
  addAction(m_redo);

  m_undo->setShortcut(QKeySequence::Undo);
  m_redo->setShortcut(QKeySequence::Redo);

  connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(changedTabs(int)));

  for(int i = 0; i < tabs->count(); ++i)
    GData::getUndoStack(i)->clear();

  changedTabs(tabs->currentIndex()); // init tab


  QMenuBar* menubar = new QMenuBar();
  QMenu* file_menu = menubar->addMenu("&File");
  QAction* save_action = file_menu->addAction("&Save");
  save_action->setEnabled(false);
  setMenuBar(menubar);

  connect(save_action, SIGNAL(triggered()), this, SLOT(saveFile()));
}

CMainWindow::~CMainWindow(void)
{
}

void CMainWindow::changedTabs(int tab_number)
{
  disconnect(m_undo, SIGNAL(triggered()), 0, 0);
  disconnect(m_redo, SIGNAL(triggered()), 0, 0);
  connect(m_undo, SIGNAL(triggered()), GData::getUndoStack(tab_number), SLOT(undo()));
  connect(m_redo, SIGNAL(triggered()), GData::getUndoStack(tab_number), SLOT(redo()));
}

void CMainWindow::saveFile(void)
{
  QFile file("00000.zel");
  GData::GetFileData().write(&file);
  file.close();
}
