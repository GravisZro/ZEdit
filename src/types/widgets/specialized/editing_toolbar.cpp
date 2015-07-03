#include "editing_toolbar.h"

#include <common/global.h>
#include <types/widgets/custom_action.h>
#include <types/widgets/specialized/editing_toolbar.h>

#define ADD_BUTTON(icon, name, value) \
  (new CCustomAction(tools->addAction(QIcon(icon), name), value, this, SLOT(setMode(ToolMode)), this))

#define ADD_BUTTON2(icon, name, value) \
  (new CCustomAction(zoom_in_out->addAction(QIcon(icon), name), value, this, SLOT(setMode(ToolMode)), this))

CEditingToolbar::CEditingToolbar(int sections, QWidget* parent)
  : QWidget(parent)
{
  QVBoxLayout* vbox = new QVBoxLayout();
  QToolBar* zoom_in_out;
  QToolBar* tools;


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

  setLayout(vbox);
}

CEditingToolbar::~CEditingToolbar(void)
{
}

void CEditingToolbar::setMode(ToolMode mode)
{
  m_mode = mode;
  emit modeChanged(m_mode);
}

ToolMode CEditingToolbar::mode(void) const
{
  return m_mode;
}
