#include "tab_base.h"

#include <QAction>
#include <common/global.h>

CTabBase::CTabBase(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
  setLayout(new QGridLayout());
}

CTabBase::~CTabBase(void)
{
}

QGridLayout* CTabBase::grid(void)
{
  return static_cast<QGridLayout*>(layout());
}

void CTabBase::addItem (QLayoutItem* item, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment)
{
  grid()->addItem(item, row, column, rowSpan, columnSpan, alignment);
}

void CTabBase::addLayout( QLayout* layout, int row, int column, Qt::Alignment alignment )
{
  grid()->addLayout(layout, row, column, alignment);
}

void CTabBase::addLayout( QLayout* layout, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment )
{
  grid()->addLayout(layout, row, column, rowSpan, columnSpan, alignment);
}

void CTabBase::addWidget( QWidget* widget, int row, int column, Qt::Alignment alignment )
{
  grid()->addWidget(widget, row, column, alignment);
}

void CTabBase::addWidget( QWidget* widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment )
{
  grid()->addWidget(widget, fromRow, fromColumn, rowSpan, columnSpan, alignment);
}
