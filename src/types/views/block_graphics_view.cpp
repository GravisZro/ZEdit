#include "block_graphics_view.h"

#include <types/items/graphics_outlined_polygon.h>
#include <types/scenes/block_graphics_scene.h>
#include <common/global.h>
#include <types/misc/pce_types.h>
#include <types/undo_commands/undo_select_blocks.h>
#include <types/undo_commands/undo_data_alteration.h>


CBlockGraphicsView::CBlockGraphicsView(QWidget* parent)
  : QGraphicsView(parent),
    m_mode(eNone),
    m_zoom_enabled(false),
    m_block_id(0)
{
  setCacheMode(QGraphicsView::CacheBackground);

  m_selection = { new CGraphicsOutlinedPolygon(), 0, 0, 0, 0 };
  m_selection.item->setColor(Qt::blue);
  m_selection.item->setVisible(false);

  GData::registerGraphicsView(this);
}

CBlockGraphicsView::~CBlockGraphicsView(void)
{
  GData::unregisterGraphicsView(this);
}

CBlockGraphicsScene* CBlockGraphicsView::scene(void)
{
  return static_cast<CBlockGraphicsScene*>(QGraphicsView::scene());
}


void CBlockGraphicsView::setScene(QGraphicsScene* scene)
{
  QGraphicsView::setScene(scene);
  scene->addItem(m_selection.item);
  setBackgroundBrush(QBrush(QPixmap(":/other/transparent.png")));
  //setBackgroundBrush(Qt::black);
}

void CBlockGraphicsView::setSceneRect (qreal x, qreal y, qreal w, qreal h, bool min, bool max)
{
  if(min)
  {
    viewport()->setMinimumSize(w, h);
    setMinimumSize(w + 4, h + 4);
  }
  if(max)
  {
    viewport()->setMaximumSize(w, h);
    setMaximumSize(w + 4, h + 4);
  }
  if(min && max)
  {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy  (Qt::ScrollBarAlwaysOff);
  }

  QGraphicsView::setSceneRect(x, y, w, h);
}

void CBlockGraphicsView::scale(qreal sx, qreal sy)
{
  if(viewport()->minimumWidth())
  {
    viewport()->setMinimumSize(sx * viewport()->minimumWidth (),
                               sy * viewport()->minimumHeight());
    setMinimumSize(viewport()->minimumWidth () + 4,
                   viewport()->minimumHeight() + 4);
  }

  if(viewport()->maximumWidth() != QWIDGETSIZE_MAX)
  {
    viewport()->setMaximumSize(sx * viewport()->maximumWidth (),
                               sy * viewport()->maximumHeight());
    setMaximumSize(viewport()->maximumWidth () + 4,
                   viewport()->maximumHeight() + 4);
  }

  QGraphicsView::scale(sx, sy);

  //setBackgroundBrush(Qt::black);
  setBackgroundBrush(QBrush(QPixmap(":/other/micro_transparent.png")));
}

void CBlockGraphicsView::setMode(ToolMode _mode)
{
  //QPoint point;
  CMatrix<uint16_t> map;
  QList<QGraphicsItem*> selected;
  uint8_t value;

  switch(_mode)
  {
    case eZoomIn:
      scale(2.0, 2.0);
      return;

    case eZoomOut:
      scale(0.5, 0.5);
      return;

    case eRectSelect:
      m_selection.item->setVisible(true);
      break;

    case eFuzzySelect:

      break;

    case eDeleteSelection:

//      GData::startUndoMacro("Delete Selection");
      //map.copy(GData::GetFileData().get_map());
      selected = m_selection.item->collidingItems();
      for(auto pos = selected.begin(); pos != selected.end(); pos++)
      {
        static_cast<QGraphicsImageItem*>(*pos)->setImageID(0xFF);
/*
        point = qobject_cast<CBlockGraphicsScene*>(scene())->getPoint((*pos)->x(), (*pos)->y());
        value = GData::GetFileData().get_map_tile(point.x(), point.y());
        GData::GetFileData().set_map_tile(point.x(), point.y(), 0xFF);
*/
        //    ->get_bytes(point.x() + (ZValues::max_map_width * point.y()), 1)
        //addUndoAction(new CUndoTileMapAlteration(point.x() + (ZValues::max_map_width * point.y()),
        //                                                       GData::GetFileData().get_bytes(point.x() + (ZValues::max_map_width * point.y()), 1));
      }
      //coords_t coords = { 0, 0 };

      //GData::addUndoAction(new CUndoTileMapAlteration(coords, map));

      GData::endUndoMacro();
      break;

    case eMoveSelection:
      if(!m_selection.item->isVisible())
      {
        m_selection.item->setPolygon(QPolygonF(QRectF(0,0,0,0)));
        m_selection.item->setVisible(true);
      }
      break;

    case eDraw:
    case eFill:
    case ePick:
      m_selection.item->setVisible(false);

      break;

    default:
      break;
  }

//  invalidateScene(mapToScene(rect()).boundingRect());
//  invalidateScene();

  m_mode = _mode;
  emit modeChanged(m_mode);
}

void CBlockGraphicsView::mousePressEvent(QMouseEvent* event)
{
  QPolygon poly;
  QGraphicsItem* item;
  CBlockGraphicsScene* sc = qobject_cast<CBlockGraphicsScene*>(scene());
  QPointF pnt = mapToScene(event->x(), event->y());

  if(pnt.x() < 0 ||
     pnt.y() < 0 ||
     pnt.x() >= sc->width() ||
     pnt.y() >= sc->height())
  {
    //Q_ASSERT(false);
    return; // happens sometimes... because of borders(?)
  }

  pnt = sc->getPoint(pnt.x(), pnt.y());
  //qDebug() << "click:" << event->pos();

  switch(m_mode)
  {
    case ePick:
      if(!items(event->pos()).empty())
      {
          qDebug() << "pick:" << event->pos();
        emit pressToolSelected(static_cast<QGraphicsImageItem*>(itemAt(event->pos()))->imageID(), m_mode);
      }
      else
      {
          qDebug() << "fail:" << event->pos();
      }
      break;

    case eDraw:
      sc->setBlock(m_block_id, pnt.x(), pnt.y());
      break;

    case eRectSelect:
      invalidateScene(mapToScene(rect()).boundingRect());

      m_selection.start_x = pnt.x();
      m_selection.start_y = pnt.y();
      m_selection.end_x   = pnt.x() + 16;
      m_selection.end_y   = pnt.y() + 16;

      poly.setPoints(5,
                     m_selection.start_x, m_selection.start_y,
                     m_selection.start_x, m_selection.end_y,
                     m_selection.end_x, m_selection.end_y,
                     m_selection.end_x, m_selection.start_y,
                     m_selection.start_x, m_selection.start_y);

      m_selection.item->clearPreviousState();
      m_selection.item->setPolygon(poly);
      m_selection.item->setVisible(true);
      break;

    case eNone:
      //    qDebug("CBlockGraphicsView: no mode has been selected");
      break;
    default:
      qDebug("CBlockGraphicsView: unknown mode selected");
      break;
  }

  QGraphicsView::mousePressEvent(event);
}

void CBlockGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
  if(event->buttons())
  {
    QPolygon poly;
    QGraphicsItem* item;
    CBlockGraphicsScene* sc = qobject_cast<CBlockGraphicsScene*>(scene());
    QPointF pnt = mapToScene(event->x(), event->y());

    if(m_previous_position == sc->getPoint(pnt.x(), pnt.y()))
      return;
    m_previous_position = sc->getPoint(pnt.x(), pnt.y());

    if(pnt.x() < 0 || pnt.y() < 0 ||
       pnt.x() >= sc->width() ||
       pnt.y() >= sc->height())
      return;

    pnt = sc->getPoint(pnt.x(), pnt.y());

    switch(m_mode)
    {
      case ePick:
        if(!items(event->pos()).empty())
        {
//        0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255, 272
          qDebug() << "mouse" << event->pos();
          emit pressToolSelected(static_cast<QGraphicsImageItem*>(itemAt(event->pos()))->imageID(), m_mode);
        }
        else
        {
          m_previous_position = QPoint();
          qDebug() << "NOOOOoooo!";
        }
        break;
      case eDraw:
        sc->setBlock(m_block_id, pnt.x(), pnt.y());
        break;

      case eRectSelect:
        m_selection.end_x = pnt.x();
        m_selection.end_y = pnt.y();

        if(pnt.x() >= m_selection.start_x)
          m_selection.end_x += sc->blockWidth();
        if(pnt.y() >= m_selection.start_y)
          m_selection.end_y += sc->blockHeight();

        poly.setPoints(5,
                       m_selection.start_x, m_selection.start_y,
                       m_selection.start_x, m_selection.end_y,
                       m_selection.end_x, m_selection.end_y,
                       m_selection.end_x, m_selection.start_y,
                       m_selection.start_x, m_selection.start_y);
        m_selection.item->setPolygon(poly);
        break;

      case eNone:
            qDebug("CBlockGraphicsView: no mode has been selected");
        break;
      default:
        qDebug("CBlockGraphicsView: unknown mode selected");
        break;
    }
  }

  QGraphicsView::mouseMoveEvent(event);
}

void CBlockGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
  CBlockGraphicsScene* sc = qobject_cast<CBlockGraphicsScene*>(scene());

  switch(m_mode)
  {
    case ePick:
      break;
    case eDraw:
      break;

    case eRectSelect:
#ifndef QT_NO_UNDOCOMMAND
      GData::addUndoAction(new CUndoSelectBlocks(m_selection.item));
#endif
      break;

    case eNone:
      qDebug("CBlockGraphicsView: no mode has been selected");
      break;
    default:
      qDebug("CBlockGraphicsView: unknown mode selected");
      break;
  }

  QGraphicsView::mouseReleaseEvent(event);
}

void CBlockGraphicsView::wheelEvent(QWheelEvent* event)
{
  if(event->modifiers() == Qt::ControlModifier && zoomEnabled())
  {
    if(event->delta() > 0)
      scale(2.0, 2.0);
    else
      scale(0.5, 0.5);
    event->accept();
  }
  else
    QGraphicsView::wheelEvent(event);
}

void CBlockGraphicsView::dragEnterEvent(QDragEnterEvent* event)
{
  qDebug("void CBlockGraphicsView::dragEnterEvent(QDragEnterEvent* event)");

  if (event->mimeData()->hasFormat("application/x-zeditmapdata"))
  {
    if (event->source() == this)
    {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    }
    else
    {
      event->acceptProposedAction();
    }
  }
  else
  {
    event->ignore();
  }
}

void CBlockGraphicsView::dragLeaveEvent(QDragLeaveEvent* event)
{
  qDebug("void CBlockGraphicsView::dragLeaveEvent(QDragLeaveEvent* event)");
}

void CBlockGraphicsView::dragMoveEvent(QDragMoveEvent* event)
{
  qDebug("void CBlockGraphicsView::dragMoveEvent(QDragMoveEvent* event)");
}

void CBlockGraphicsView::dropEvent(QDropEvent* event)
{
  qDebug("void CBlockGraphicsView::dropEvent(QDropEvent* event)");
}

void CBlockGraphicsView::startDrag(Qt::DropActions supportedActions)
{
  qDebug("void CBlockGraphicsView::startDrag(Qt::DropActions supportedActions)");
}


void CBlockGraphicsView::paintEvent(QPaintEvent* event)
{
  qDebug() << "void CBlockGraphicsView::paintEvent(QPaintEvent* event)" << this;

  QGraphicsView::paintEvent(event);
}
