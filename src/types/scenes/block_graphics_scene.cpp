#include "block_graphics_scene.h"

CBlockGraphicsScene::CBlockGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject* parent)
  : QGraphicsScene(x, y, width, height, parent),
    m_block_spacing_x(0),
    m_block_spacing_y(0),
    m_block_spacing_interval_x(0),
    m_block_spacing_interval_y(0)
{
  //setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  setBackgroundBrush(Qt::transparent);
}

CBlockGraphicsScene::CBlockGraphicsScene(QObject* parent)
  : QGraphicsScene(parent)
{
  //setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

CBlockGraphicsScene::~CBlockGraphicsScene(void)
{
}

void CBlockGraphicsScene::rebuildSpacing(void)
{
  m_spacing_y[0] = m_spacing_x[0] = 0;
  for(int x = 1; x < m_spacing_x.size(); ++x)
  {
    m_spacing_x[x] = m_spacing_x[x - 1] + blockWidth();
    if(m_block_spacing_interval_x && !(x % m_block_spacing_interval_x))
      m_spacing_x[x] += m_block_spacing_x;
  }

  for(int y = 1; y < m_spacing_y.size(); ++y)
  {
    m_spacing_y[y] = m_spacing_y[y - 1] + blockHeight();
    if(m_block_spacing_interval_y && !(y % m_block_spacing_interval_y))
      m_spacing_y[y] += m_block_spacing_y;
  }
}

//==================================================================

void CBlockGraphicsScene::setSpacingAmount (uint x, uint y)
{
  m_block_spacing_x = x;
  m_block_spacing_y = y;

  if(m_spacing_x.size() &&
     m_spacing_y.size())
    rebuildSpacing();
}

//==================================================================

void CBlockGraphicsScene::setSpacingInterval(uint x, uint y)
{
  m_block_spacing_interval_x = x;
  m_block_spacing_interval_y = y;

  if(m_spacing_x.size() &&
     m_spacing_y.size())
    rebuildSpacing();
}

QPoint CBlockGraphicsScene::getPointIndex(uint x, uint y)
{
  Q_ASSERT(m_spacing_x[1]);

  int i = 1;
  while(i < (m_spacing_x.size() - 1) && m_spacing_x[i] < x) { ++i; }
  --i;
  x = i;

  i = 1;
  while(i < (m_spacing_y.size() - 1) && m_spacing_y[i] < y) { ++i; }
  --i;
  y = i;

  return QPoint(x, y);
}

QPoint CBlockGraphicsScene::getPoint(uint x, uint y)
{
  Q_ASSERT(m_spacing_x[1]);

  int i = 1;
  while(i < m_spacing_x.size() && m_spacing_x[i] < x) { ++i; }
  --i;
  x = m_spacing_x[i];

  i = 1;
  while(i < m_spacing_y.size() && m_spacing_y[i] < y) { ++i; }
  --i;
  y = m_spacing_y[i];

  return QPoint(x, y);
}

void CBlockGraphicsScene::setBlock(uint id, qreal x, qreal y)
{
  Q_ASSERT(m_spacing_x[1]);

  uint ux = x;
  uint uy = y;

  int i = 1;
  while(i < m_spacing_x.size() && m_spacing_x[i] <= ux) { ++i; }
  --i;
  ux = i;

  i = 1;
  while(i < m_spacing_y.size() && m_spacing_y[i] <= uy) { ++i; }
  --i;
  uy = i;

  setBlock(id, ux, uy);
}

void CBlockGraphicsScene::setBlock(uint id, uint x, uint y)
{
  QPair<const uint, const uint> pos (x, y);

  QGraphicsImageItem* block = m_blocks.value(pos, nullptr);

  if(block == nullptr)
  {
    block = new QGraphicsImageItem(nullptr);
    addItem(block);

    block->setImageID(id);
    block->savePosition(x, y);
    block->setPos(m_spacing_x[x], m_spacing_y[y]);
    m_blocks.insert(pos, block);
  }
  else
  {
    block->setImageID(id);
  }
  emit blockSet(id, x, y);
}

uint CBlockGraphicsScene::block(qreal x, qreal y)
{
  Q_ASSERT(m_spacing_x[1]);

  uint ux = x;
  uint uy = y;

  int i = 1;
  while(i < m_spacing_x.size() && m_spacing_x[i] <= ux) { ++i; }
  --i;
  ux = i;

  i = 1;
  while(i < m_spacing_y.size() && m_spacing_y[i] <= uy) { ++i; }
  --i;
  uy = i;

  return m_blocks[QPair<const uint, const uint>(ux, uy)]->imageID();
}


uint CBlockGraphicsScene::block(uint x, uint y) // virtual
{
  return m_blocks[QPair<const uint, const uint>(x, y)]->imageID();
}
