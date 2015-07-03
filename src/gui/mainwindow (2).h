#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class CMainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    CMainWindow(QWidget *parent = 0);
    ~CMainWindow(void);

  private slots:
    void changedTabs(int tab_number);

  private:
    QAction* m_undo;
    QAction* m_redo;
};

#endif
