#ifndef BLOCK_GRAPHICS_VIEW_H
#define BLOCK_GRAPHICS_VIEW_H

#include <QGraphicsView>

enum ModeType
{
  eBlock = 0,
  ePixel
};

enum ToolMode
{
  eNone = 0,
  eSingleSelect,
  eRectSelect,
  eFuzzySelect,
  eSelectNone,
  eClearSelection,
  eMoveSelection,
  eDeleteSelection,
  eCutSelection,
  ePasteSelection,
  eDraw,
  ePick,
  eFill,
  eZoomIn,
  eZoomOut
};

enum EToolSections
{
  eZoomInOut   = 0x0001,
  eToolbar     = 0x0002,
  eTiles       = 0x0004,
  eTileDisplay = 0x0008,
  eSprites     = 0x0010
};

class CGraphicsOutlinedPolygon;
class QGraphicsScene;
class QUndoCommand;
class CBlockGraphicsScene;

class CBlockGraphicsView : public QGraphicsView
{
    Q_OBJECT
  public:
    CBlockGraphicsView(QWidget* parent = nullptr);
    ~CBlockGraphicsView(void);

    void setZoomEnabled(bool enable = true) { m_zoom_enabled = enable; }
    bool zoomEnabled(void) const { return m_zoom_enabled; }

    CBlockGraphicsScene* scene(void);


    void setScene(QGraphicsScene* scene);
    void setSceneRect (qreal x, qreal y, qreal w, qreal h, bool min = false, bool max = false);
    void scale(qreal sx, qreal sy);

    ToolMode mode(void) { return m_mode; }

  signals:
    void modeChanged(ToolMode mode);
    void pressToolSelected(uint id, ToolMode mode);
    void moveToolSelected (uint id, ToolMode mode);

  public slots:
    void setMode(ToolMode _mode);
    void setBlockID(uint id) { m_block_id = id; }
    void setColor(QRgb color) { setBlockID(color); }

  protected:
    void mousePressEvent  (QMouseEvent* event);
    void mouseMoveEvent   (QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent       (QWheelEvent* event);

    void dragEnterEvent(QDragEnterEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dragMoveEvent (QDragMoveEvent*  event);
    void dropEvent     (QDropEvent*      event);

    void startDrag(Qt::DropActions supportedActions);

    void paintEvent(QPaintEvent* event);

  private:
    ToolMode m_mode;
    bool     m_zoom_enabled;
    uint     m_block_id;
    QPoint   m_previous_position;

    struct selection_s
    {
        CGraphicsOutlinedPolygon* item;
        int start_x;
        int start_y;
        int end_x;
        int end_y;
    } m_selection;
};

#endif
