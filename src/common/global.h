#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QtWidgets>

#include <stdint.h>
#include <unordered_map>
#include <unordered_set>
#include <common/nullable.h>

//#include <types/misc/template_array.h>
#include <types/misc/matrix.h>
#include <types/misc/linked_image.h>
#include <types/misc/zfile_io.h>

enum EDataKeys //: int
{
  eImageIndex = 0,
  eOptimizeTiles,
  eOptimizeSprites,
  ePosition,
  eImageType
};

class QLinkedImage;

class GData
{
  public:
    //static QGraphicsScene
    //static int getFileNum(void);
    static ZFile& GetFileData(void);
    static CWordMatrix& GetMapData(void);

    static QLinkedImage* GetImage     (int id);
    static QVector<QRgb> GetPaletteNum(int pal_num);

    static std::unordered_map<int, QLinkedImage*>& GetAllImages(void);

    //static void AutoOptimizeColors(const EDataKeys type);
    static void ReinitializeAllImages(void); // use sparingly!!!!

    static void invalidateGraphicsViews(void);
    static void registerGraphicsView(QGraphicsView* view);
    static void unregisterGraphicsView(QGraphicsView* view);

    static QUndoStack* getUndoStack(int num = -1);
    static void        setUndoStack(int num);

    static void addUndoAction(QUndoCommand* command);
    static void startUndoMacro(const char* macro_name);
    static void startUndoMacro(const QString& macro_name);
    static void endUndoMacro(void);
};

#endif
