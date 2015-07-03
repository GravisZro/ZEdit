#include "global.h"

#include <QSet>
#include <QByteArray>
#include <QImage>

#include <types/misc/zfile_io.h>
#include <types/misc/pce_types.h>
#include <types/undo_commands/undo_data_alteration.h>


static std::unordered_map<int, QUndoStack*> g_undos;
static QUndoStack* g_current_undo_stack = GData::getUndoStack(0);

QUndoStack* GData::getUndoStack(int num)
{
  if(num != -1)
  {
    QUndoStack*& stack_ptr = g_undos[num];
    if(stack_ptr == nullptr)
      stack_ptr = new QUndoStack();
    g_current_undo_stack = stack_ptr;
  }

  return g_current_undo_stack;
}

void GData::setUndoStack(int num)
{
  QUndoStack*& stack_ptr = g_undos[num];
  if(stack_ptr == nullptr)
    stack_ptr = new QUndoStack();
  g_current_undo_stack = stack_ptr;
}

void GData::addUndoAction(QUndoCommand* command)
{
  getUndoStack()->push(command);
}

void GData::startUndoMacro(const char* macro_name)
{
  GData::startUndoMacro(QString(macro_name));
}

void GData::startUndoMacro(const QString& macro_name)
{
  getUndoStack()->beginMacro(macro_name.toLocal8Bit());
}

void GData::endUndoMacro(void)
{
  getUndoStack()->endMacro();
}

static std::unordered_map<int, QLinkedImage*> g_images;

std::unordered_map<int, QLinkedImage*>& GData::GetAllImages(void)
{
  return g_images;
}

ZFile& GData::GetFileData(void)
{
  static ZFile* z_file = nullptr;

  if(z_file == nullptr)
  {
    z_file = new ZFile();

    QFile file(":/00000.zel");
    z_file->read(&file);
  }
  return *z_file;
}

CWordMatrix& GData::GetMapData(void)
{
  return GData::GetFileData().maps.tiles;
}

QLinkedImage* GData::GetImage(int id)
{
  QLinkedImage* img = g_images[id];
  if(img == nullptr)
    ReinitializeAllImages();
  return g_images[id];
}

static QSet<QGraphicsView*> g_views;

void GData::invalidateGraphicsViews(void)
{
  for(auto pos = g_views.begin(); pos != g_views.end(); pos++)
    if((*pos)->scene() != nullptr)
      (*pos)->invalidateScene();
}

void GData::registerGraphicsView(QGraphicsView* view)
{
  g_views.insert(view);
}

void GData::unregisterGraphicsView(QGraphicsView* view)
{
  g_views.remove(view);
}


void GData::ReinitializeAllImages(void)
{
  qDebug("void GData::ReinitializeAllImages(void)");
  for(uint16_t id = 0; id < GData::GetFileData().sprites.patterns.size(); ++id)
  {
    QLinkedImage* img = g_images[id | ZValues::sprite_mask];
    if(img == nullptr)
      g_images[id] = img = new QLinkedImage(16, 16, QImage::Format_Indexed8);
    img->setImageID(id);
  }

  for(uint16_t id = 0; id < GData::GetFileData().tiles.patterns.size(); ++id)
  {
    QLinkedImage* img = g_images[id];
    if(img == nullptr)
      g_images[id] = img = new QLinkedImage(8, 8, QImage::Format_Indexed8);
    img->setImageID(id);
  }

  invalidateGraphicsViews();
}

QVector<QRgb> GData::GetPaletteNum(int pal_num)
{
  return CPaletteData(GData::GetFileData().get_palette(pal_num)).operator QVector<QRgb>();
}
