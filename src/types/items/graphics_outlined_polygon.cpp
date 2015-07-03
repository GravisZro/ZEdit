#include "graphics_outlined_polygon.h"

#include <QPainter>
#include <QDebug>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>


CGraphicsOutlinedPolygon::CGraphicsOutlinedPolygon(QGraphicsItem* parent)
  : QGraphicsPolygonItem(parent)
{
  setColor(Qt::blue);
  setZValue(1);
}

void CGraphicsOutlinedPolygon::setColor(QRgb color)
{
  m_color.setRgb(color);
  m_translucent_color = m_color;
  m_translucent_color.setAlpha(0x44);
  update(boundingRect());
}

void CGraphicsOutlinedPolygon::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  QPen p;
  p.setWidth(1);
  p.setColor(m_color);
  setBrush(m_translucent_color);

  QGraphicsPolygonItem::paint(painter, option, widget);

  painter->setPen(p);
  painter->drawPath(shape());
}

void CGraphicsOutlinedPolygon::setPolygon(const QPolygonF& polygon)
{
  m_previous_state.setPolygon(this->polygon());
  QGraphicsPolygonItem::setPolygon(polygon);
}

void CGraphicsOutlinedPolygon::setVisible(bool visible)
{
  m_previous_state.setVisible(visible);
  QGraphicsPolygonItem::setVisible(visible);
}


void CGraphicsOutlinedPolygon::setState(const CPolygonItemState& state)
{
  setPolygon(state.polygon());
  setVisible(state.visibility());
}

const CPolygonItemState  CGraphicsOutlinedPolygon::currentState (void) const
{
  return CPolygonItemState(polygon(), isVisible());
}

const CPolygonItemState& CGraphicsOutlinedPolygon::previousState(void) const
{
  return m_previous_state;
}

void CGraphicsOutlinedPolygon::clearPreviousState(void)
{
  m_previous_state.clear();
}
