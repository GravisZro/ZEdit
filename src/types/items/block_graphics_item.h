#ifndef BLOCK_GRAPHICS_ITEM_H
#define BLOCK_GRAPHICS_ITEM_H

#include <common/global.h>
#include "graphics_image_item.h"

class CBlockGraphicsItem : public QGraphicsImageItem
{
    Q_OBJECT
  public:
    CBlockGraphicsItem(QGraphicsItem* parent);

    QImage image(void) const;
    uint16_t imageID(void) const;
    void savePosition(int x, int y);
    QPoint loadPosition(void);

  public slots:
    void setImageID(int image_id);
  public:
    void linkImageID(uint8_t* id_mem);

protected:
    EImageType m_type;
    uint8_t* m_id_mem;
};

#endif // BLOCK_GRAPHICS_ITEM_H
