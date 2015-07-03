#ifndef LABMDA_SLOT_H
#define LABMDA_SLOT_H

#include <QtCore>
/*

class connect_functor_helper
    : public QObject
{
    Q_OBJECT
public:
    template<typename TSlotFunc>
    connect_functor_helper(QObject *parent, TSlotFunc f)
      : QObject(parent)
    {
      //connect(parent, )
    }

public slots:
    void signaled()//Args... args)
    {
      //  m_function();
    }
  private:
    //m_function
};

Q_DECLARE_METATYPE(MyStruct)


template <class T, typename... Args>
bool connect(QObject *sender, const char *signal, const T &reciever, Qt::ConnectionType type = Qt::AutoConnection) {
    return QObject::connect(sender, signal, new connect_functor_helper(sender, reciever), SLOT(signaled(Args...)), type);
}
*/
#endif // LABMDA_SLOT_H
