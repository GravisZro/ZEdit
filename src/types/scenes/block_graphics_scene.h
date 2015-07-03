#ifndef BLOCK_GRAPHICS_SCENE_H
#define BLOCK_GRAPHICS_SCENE_H

#include <QGraphicsScene>
#include <QList>

#include <common/global.h>
#include <types/misc/pce_types.h>
#include <types/items/block_graphics_item.h>


class CBlockGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
  public:
    explicit CBlockGraphicsScene(QObject* parent = nullptr);
    explicit CBlockGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject* parent = nullptr);
    ~CBlockGraphicsScene(void);

    virtual QPoint spacingAmount  (void) const { return QPoint(m_block_spacing_x         , m_block_spacing_y         ); }
    virtual QPoint spacingInterval(void) const { return QPoint(m_block_spacing_interval_x, m_block_spacing_interval_y); }

    virtual qreal blockHeight(void) const { return 16.0; }
    virtual qreal blockWidth (void) const { return 16.0; }

    template <typename value_type>
    void setMappingSource(CMatrix<value_type> data);

    virtual QPoint getPoint(uint x, uint y); // return the closest block to scene x/y position

    virtual uint block(qreal x, qreal y);
    virtual uint block(uint x, uint y);

  signals:
    void blockSet(uint id, uint x, uint y);

  public slots:
    virtual void setBlock(uint id, qreal x, qreal y);
    virtual void setBlock(uint id, uint  x, uint  y);

    virtual void setSpacingAmount  (uint x, uint y); // number of pixels to add for spacing
    virtual void setSpacingInterval(uint x, uint y); // number of images between adding spacing pixels

  protected:
    virtual QPoint getPointIndex(uint x, uint y); // return the index of the closest block to mouse x/y position
    void rebuildSpacing(void);

    QHash<QPair<const uint, const uint>, QGraphicsImageItem*> m_blocks;

    QVector<uint> m_spacing_x; // precalculated locations
    QVector<uint> m_spacing_y;

    uint m_block_spacing_x;
    uint m_block_spacing_y;
    uint m_block_spacing_interval_x;
    uint m_block_spacing_interval_y;
};


template <typename value_type>
void CBlockGraphicsScene::setMappingSource(CMatrix<value_type> data)
{
  const uint w = data.width ();
  const uint h = data.height();

  m_spacing_x.resize(w + 1);
  m_spacing_y.resize(h + 1);

  rebuildSpacing();

  for(uint y = 0; y < h; ++y)
    for(uint x = 0; x < w; ++x)
      setBlock(data[x][y], x, y);//.at((y * w) + x));
}

#endif // BLOCK_GRAPHICS_SCENE_H
