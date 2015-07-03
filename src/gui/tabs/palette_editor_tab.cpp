#include "palette_editor_tab.h"

CPaletteEditorTab::CPaletteEditorTab(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
  QGridLayout* g = new QGridLayout();

  g->addWidget(new QLabel("I CAN HAZ HALP WIF DIS?"));
  setLayout(g);
}

CPaletteEditorTab::~CPaletteEditorTab(void)
{
}
