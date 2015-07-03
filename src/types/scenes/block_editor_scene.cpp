#include "block_editor_scene.h"
#include <types/undo_commands/undo_image_item_alteration.h>


class CInvalidateBlock : public QUndoCommand
{
  public:
    CInvalidateBlock(QGraphicsImageItem* item, QUndoCommand* parent = nullptr)
      : QUndoCommand(QString("invalidate block: %1").arg(item->imageID())), m_item(item) { }

    void undo(void) {
      qDebug() << "undo" << text();
      m_item->invalidate(); }

    void redo(void) {
      qDebug() << "redo" << text();
      m_item->invalidate(); }

  protected:
    QGraphicsImageItem* m_item;
};


CBlockEditorScene::CBlockEditorScene(QObject* parent)
  : CBlockGraphicsScene(parent)
{
}

CBlockEditorScene::CBlockEditorScene(qreal x, qreal y, qreal width, qreal height, QObject* parent)
  : CBlockGraphicsScene(x, y, width, height, parent)
{
}

CBlockEditorScene::~CBlockEditorScene(void)
{
}


void CBlockEditorScene::setMacroBlock(uint id, uint x, uint y)
{
  QPair<const uint, const uint> pos (x, y);
  QGraphicsImageItem* block = m_blocks.value(pos, nullptr);

//  qDebug() << "void CBlockEditorScene::setMacroBlock("<< id << "," << x << "," << y <<")";
  if(block == nullptr)
  {
//    qDebug() << "  if(block == nullptr)";
    block = new QGraphicsImageItem(id, nullptr);
    addItem(block);

    if(m_spacing_x.isEmpty())
    {
      m_spacing_x.resize(block->image().width ());
      m_spacing_y.resize(block->image().height());
      rebuildSpacing();
    }

    block->setPos(m_spacing_x[x], m_spacing_y[y]);
    m_blocks.insert(pos, block);

    connect(this, SIGNAL(autoOptimize(bool)), block, SLOT(setAutoOptimize(bool)));
    connect(block, SIGNAL(statusMessage(QString, int)), this, SIGNAL(statusMessage(QString,int)));
  }
  else
  {
//    qDebug() << " ELSE if(block != nullptr)";
    block->setImageID(id);
  }

//  qDebug() << "block->imageID() =" << block->imageID();

//  invalidate();
  update();
//  qDebug() << "updated\n";
//  this->update(this->sceneRect());
  emit macroBlockSet(id, x, y);
}

void CBlockEditorScene::setBlock(uint id, uint x, uint y)
{
//  qDebug() << "void CBlockEditorScene::setBlock(uint id, uint x, uint y)";
  QGraphicsImageItem* block = m_blocks.value(QPair<const uint, const uint>(x / CBlockGraphicsScene::blockWidth (),
                                                                           y / CBlockGraphicsScene::blockHeight()));
  //GData::startUndoMacro("block->setPixel(id, x, y);");
  GData::addUndoAction(new CInvalidateBlock(block));
  block->setPixel(id, x, y);
  GData::addUndoAction(new CInvalidateBlock(block));
  //GData::endUndoMacro();

  invalidate();
  update();
  emit blockSet(id, x, y);
}

QPoint CBlockEditorScene::getPoint(uint x, uint y) // override hack so that the view will work properly.  just returns the input values as a QPoint.
{
  return QPoint(x, y);
}
