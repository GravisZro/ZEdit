#ifndef MULTIBUTTON_H
#define MULTIBUTTON_H

#include <common/global.h>
#include <types/views/block_graphics_view.h>


class CBlockGraphicsScene;

class CMultiButton : public CBlockGraphicsView
{
  Q_OBJECT
public:
  CMultiButton(QWidget* parent = nullptr);

public slots:
  void setBlockID(uint id);

signals:
  void buttonToggled(int id, bool checked);

protected:
  void drawForeground(QPainter* painter, const QRectF& rect);
  void mouseReleaseEvent(QMouseEvent* event);

private:
  CBlockGraphicsScene* m_scene;
  uint    m_id;
  uint8_t m_mask;
};

#endif // MULTIBUTTON_H
