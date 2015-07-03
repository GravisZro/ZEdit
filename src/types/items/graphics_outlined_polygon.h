#ifndef GRAPHICS_OUTLINED_POLYGON_H
#define GRAPHICS_OUTLINED_POLYGON_H

#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QPolygonF>

class CPolygonItemState
{
  public:
    CPolygonItemState(void) {}
    CPolygonItemState(QPolygonF polygon, bool visibile) : poly(polygon), vis(visibile) { }

    void clear(void) { poly.clear(); vis.clear(); }

    void setPolygon(const QPolygonF& polygon) { if(poly.empty()) { poly = polygon; } }
    void setVisible(bool visible) { if(vis.isValid()) { vis = visible; } }

    const QPolygonF& polygon(void) const { return poly; }
    bool visibility(void) const { return vis.toBool(); }
  private:
    QPolygonF poly;
    QVariant  vis;
};

class CGraphicsOutlinedPolygon : public QGraphicsPolygonItem
{
  public:
    CGraphicsOutlinedPolygon(QGraphicsItem* parent = nullptr);

    void setColor(QRgb color);
    void setColor(Qt::GlobalColor color) { setColor(QColor(color).rgb()); }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr);

    void setPolygon(const QPolygonF& polygon);
    void setVisible(bool visible);


    void setState(const CPolygonItemState& state);
    const CPolygonItemState  currentState (void) const;
    const CPolygonItemState& previousState(void) const;
    void clearPreviousState(void);

  protected:
    CPolygonItemState m_previous_state;
    QColor m_color;
    QColor m_translucent_color;
};

#endif // GRAPHICS_OUTLINED_POLYGON_H
