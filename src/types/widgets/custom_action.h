#ifndef CUSTOM_ACTION_H
#define CUSTOM_ACTION_H

#include <QAction>
#include <QVariant>

#include <types/views/block_graphics_view.h>

class CCustomAction : public QObject
{
    Q_OBJECT
  public:
    CCustomAction(QAction* action, const ToolMode value, const QObject* receiver, const char* member, QObject* parent)
      : QObject(parent),
        m_action(action),
        m_mode(value)
    {
      connect(m_action, SIGNAL(triggered()), this, SLOT(actionSlotToolMode()));
      connect(this, SIGNAL(action(ToolMode)), receiver, member);
    }

    CCustomAction(QAction* action, const QVariant& value, const QObject* receiver, const char* member, QObject* parent)
      : QObject(parent),
        m_action(action),
        m_value(value)
    {
      connect(m_action, SIGNAL(triggered()), this, SLOT(actionSlot()));
      connect(this, SIGNAL(action(const QVariant&)), receiver, member);
    }

    CCustomAction(QAction* action, const QVariant& type, const QVariant& value, const QObject* receiver, const char* member, QObject* parent)
      : QObject(parent),
        m_action(action),
        m_value(value),
        m_type(type)
    {
      connect(m_action, SIGNAL(triggered()), this, SLOT(actionSlotWithType()));
      connect(this, SIGNAL(action(const QVariant&,const QVariant&)), receiver, member);
    }

    void setDisabled(bool state) { m_action->setDisabled(state); }
    
  signals:
    void action(const ToolMode value);
    void action(const QVariant& value);
    void action(const QVariant& type, const QVariant& value);
    
  private slots:
    void actionSlotToolMode (void) { emit action(m_mode         ); }
    void actionSlot         (void) { emit action(m_value        ); }
    void actionSlotWithType (void) { emit action(m_type, m_value); }

  private:
    QAction* m_action;
    QVariant m_value;
    QVariant m_type;
    ToolMode m_mode;
};

#endif // CUSTOM_ACTION_H
