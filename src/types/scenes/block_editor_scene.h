#ifndef BLOCK_EDITOR_SCENE_H
#define BLOCK_EDITOR_SCENE_H

#include "block_graphics_scene.h"

class CBlockEditorScene : public CBlockGraphicsScene
{
    Q_OBJECT
  public:
    explicit CBlockEditorScene(QObject* parent = nullptr);
    explicit CBlockEditorScene(qreal x, qreal y, qreal width, qreal height, QObject* parent = nullptr);
    ~CBlockEditorScene(void);

    virtual QPoint getPoint(uint x, uint y);

    virtual qreal blockHeight(void) const { return 1.0; }
    virtual qreal blockWidth (void) const { return 1.0; }

  signals:
    void statusMessage(const QString& message, int timeout);
    void macroBlockSet(uint id, uint x, uint y);
    void autoOptimize(bool enable);

  public slots:
    void setBlock     (uint id, uint x, uint y);
    void setMacroBlock(uint id, uint x, uint y);
    void setMacroBlock(uint id) { setMacroBlock(id, 0, 0); }
};

#endif // BLOCK_EDITOR_SCENE_H
