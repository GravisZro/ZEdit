#ifndef BLOCK_BUTTON_ITEM_H
#define BLOCK_BUTTON_ITEM_H

#include <types/items/graphics_image_item.h>

class CBlockButtonItem : public QGraphicsImageItem
{
    Q_OBJECT
  public:
    CBlockButtonItem(QGraphicsItem* parent = nullptr);

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr);

    
  signals:
    
  public slots:

};

#endif // BLOCK_BUTTON_ITEM_H
