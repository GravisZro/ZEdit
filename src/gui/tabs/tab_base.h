#ifndef TAB_BASE_H
#define TAB_BASE_H

#include <QtWidgets>

class CTabBase : public QWidget
{
  Q_OBJECT
  public:
    CTabBase(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
   ~CTabBase(void);

    QGridLayout* grid(void);

    void addItem (QLayoutItem* item, int row, int column, int rowSpan = 1, int columnSpan = 1, Qt::Alignment alignment = 0 );
    void addLayout( QLayout* layout, int row, int column, Qt::Alignment alignment = 0 );
    void addLayout( QLayout* layout, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment = 0 );
    void addWidget( QWidget* widget, int row, int column, Qt::Alignment alignment = 0 );
    void addWidget( QWidget* widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = 0 );
};

#endif
