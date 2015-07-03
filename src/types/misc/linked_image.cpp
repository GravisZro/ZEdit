#include "linked_image.h"

#include <common/global.h>
#include <types/misc/pce_types.h>
#include <common/color_converters.h>
#include <types/undo_commands/undo_data_alteration.h>

class CUndoLinkedImageAlteration : public QUndoCommand
{
  public:
    explicit CUndoLinkedImageAlteration(QLinkedImage* linked_image, const QImage& old_image, const QImage& new_image, QUndoCommand* parent = nullptr)
      : CUndoLinkedImageAlteration(linked_image, old_image, UCHAR_MAX, false, new_image, UCHAR_MAX, false, parent) { }

    explicit CUndoLinkedImageAlteration(QLinkedImage* linked_image, const QImage& old_image, const bool old_is_optimized, const QImage& new_image, const bool new_is_optimized, QUndoCommand* parent = nullptr)
      : CUndoLinkedImageAlteration(linked_image, old_image, UCHAR_MAX, old_is_optimized, new_image, UCHAR_MAX, new_is_optimized, parent) { }

    explicit CUndoLinkedImageAlteration(QLinkedImage* linked_image, const QImage& old_image, QUndoCommand* parent = nullptr)
      : CUndoLinkedImageAlteration(linked_image, old_image, false, parent) { }

    explicit CUndoLinkedImageAlteration(QLinkedImage* linked_image, const QImage& old_image, const bool old_is_optimized, QUndoCommand* parent = nullptr)
      : CUndoLinkedImageAlteration(linked_image, old_image, UCHAR_MAX, old_is_optimized, *linked_image, UCHAR_MAX, linked_image->indexesOptimized(), parent) { }

    explicit CUndoLinkedImageAlteration(QLinkedImage* linked_image, const QImage& old_image, const uint16_t old_palette_id, QUndoCommand* parent = nullptr)
      : CUndoLinkedImageAlteration(linked_image, old_image, old_palette_id, false, parent) { }

    explicit CUndoLinkedImageAlteration(QLinkedImage* linked_image, const QImage& old_image, const uint16_t old_palette_id, const bool old_is_optimized, QUndoCommand* parent = nullptr)
      : CUndoLinkedImageAlteration(linked_image, old_image, old_palette_id, old_is_optimized, *linked_image, linked_image->paletteID(), linked_image->indexesOptimized(), parent) { }


    explicit CUndoLinkedImageAlteration(QLinkedImage* linked_image, const QImage& old_image, const uint16_t old_palette_id, const QImage& new_image, const int new_palette_id, QUndoCommand* parent = nullptr)
      : CUndoLinkedImageAlteration(linked_image, old_image, old_palette_id, false, new_image, new_palette_id, false, parent) { }

    explicit CUndoLinkedImageAlteration(QLinkedImage* linked_image, const QImage& old_image, const uint16_t old_palette_id, const bool old_is_optimized,
                                        const QImage& new_image, const uint16_t new_palette_id, const bool new_is_optimized,
                                        QUndoCommand* parent = nullptr)

      : QUndoCommand(QString("Set image data for QLinkedImage #%1").arg(linked_image->imageID()), parent),
        m_linked_image(linked_image),
        m_old_image(old_image),
        m_new_image(new_image),
        m_old_palette_id(old_palette_id),
        m_new_palette_id(new_palette_id),
        m_old_is_optimized(old_is_optimized),
        m_new_is_optimized(new_is_optimized)
    {
    }

    void undo(void)
    {
      if((m_old_palette_id & m_new_palette_id) != USHRT_MAX)
      {
        CPaletteData(GData::GetFileData().get_palette(m_old_palette_id)) = m_old_image.QImage::colorTable();
        GData::GetFileData().set_image_pal_idx(m_linked_image->imageID(), m_old_palette_id);
      }

      m_linked_image->setImageData(m_old_image);
      m_linked_image->syncIndexCounter();
      m_linked_image->syncColorsUsed();
      m_linked_image->setIndexesOptimized(m_old_is_optimized);
      m_linked_image->update();
    }

    void redo(void)
    {
      if((m_old_palette_id & m_new_palette_id) != USHRT_MAX)
      {
        CPaletteData(GData::GetFileData().get_palette(m_new_palette_id)) = m_new_image.QImage::colorTable();
        GData::GetFileData().set_image_pal_idx(m_linked_image->imageID(), m_new_palette_id);
      }

      m_linked_image->setImageData(m_new_image);
      m_linked_image->syncIndexCounter();
      m_linked_image->syncColorsUsed();
      m_linked_image->setIndexesOptimized(m_new_is_optimized);
      m_linked_image->update();
    }

  protected:
    QLinkedImage* m_linked_image;
    const QImage m_old_image;
    const QImage m_new_image;
    const uint16_t m_old_palette_id;
    const uint16_t m_new_palette_id;
    const bool m_old_is_optimized;
    const bool m_new_is_optimized;
};


QLinkedImage::QLinkedImage(int width, int height, Format format, QObject* parent)
  : QObject(parent),
    QImage(width, height, format),
    m_state(0),
    m_image_id(0),
    m_index_count(ZValues::colors_in_palette, 0),
    m_auto_optimize_enabled(true),
    m_indexes_optimized(false)
{
  setColorCount(ZValues::colors_in_palette);
}

QLinkedImage::operator image_state_t(void)
{
  // make sure evertyhing is up to date
  syncPalette();
  syncImageData();
  syncColorsUsed();

  image_state_t x;
  x.image = *this;
  x.image_id = imageID();
  x.palette_id = paletteID();

  return x;
}

const image_state_t& QLinkedImage::operator =(const image_state_t& x)
{
  m_image_id = x.image_id;
  GData::GetFileData().set_image_pal_idx(imageID(), x.palette_id);
/*
  if(imageType() == eTile)
    CTileData(GData::GetFileData().get_tile(relativeImageID())).encode(QByteArray(reinterpret_cast<const char*>(x.image.bits(), x.image.byteCount())));
  else
    CSpriteData(GData::GetFileData().get_sprite(relativeImageID())).encode(QByteArray(reinterpret_cast<const char*>(x.image.bits(), x.image.byteCount())));
*/
  CPaletteData(GData::GetFileData().get_palette(paletteID())) = x.image.QImage::colorTable();

  return x;
}

inline EImageType QLinkedImage::imageType(void) const
{
  if(imageID() < ZValues::sprite_mask)
    return eTile;
  return eSprite;
}

const QImage&	QLinkedImage::operator= (const QImage& image)
{
  //GData::addUndoAction(new CUndoLinkedImageAlteration(this, image, paletteID()));
  return image;
}

const QVector<QRgb>& QLinkedImage::operator =(const QVector<QRgb>& colors)
{
  Q_ASSERT(false);
  //setColorTable(colors);
  return colors;
}

//==========================================================

void QLinkedImage::setImageData(const QImage& image)
{
//  qDebug() << "setImageData";
/*
    if(imageType() == eTile)
      CTileData(GData::GetFileData().get_tile(relativeImageID())) = image;
    else
      CSpriteData(GData::GetFileData().get_sprite(relativeImageID())) = image;

    memcpy(bits(), image.bits(), image.byteCount());
*/
}

void QLinkedImage::syncImageData(void)
{
/*
  QByteArray image_data;
  if(imageType() == eTile)
    image_data = CTileData(GData::GetFileData().get_tile(relativeImageID())).decode();
  else
    image_data = CSpriteData(GData::GetFileData().get_sprite(relativeImageID())).decode();

  memcpy(bits(), image_data.data(), image_data.size());
*/

  syncIndexCounter();
}

//==========================================================

void QLinkedImage::setPaletteID(uint16_t id)
{
  GData::GetFileData().set_image_pal_idx(imageID(), id); // change the palette number
  QImage::setColorTable(CPaletteData(GData::GetFileData().get_palette(id))); //
  syncColorsUsed();
}

uint16_t QLinkedImage::paletteID(void) const
{
  return GData::GetFileData().get_image_pal_idx(imageID()); // get palette number used
}

void QLinkedImage::syncPalette(void)
{
  QImage::setColorTable(CPaletteData(GData::GetFileData().get_palette(paletteID()))); // reload palette
  syncColorsUsed(); // resync which colors are used
}

//==========================================================

QSet<QRgb> QLinkedImage::colorsUsed(void) // compiles a list of colors used in this image.
{
  QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(bits()), byteCount());
  QVector<QRgb> color_table = QImage::colorTable();
  QSet<QRgb> colors_used;

  for(int8_t color_index = 1; color_index < ZValues::colors_in_palette; ++color_index) // exclude transparent color
  {
    if(m_color_count[QImage::color(color_index)])
      colors_used.insert(QImage::color(color_index));
    else if(!indexesOptimized() && autoOptimizeEnabled()) // found a duplicate color and automagic optimization enabled? consolidate this index (only for this image).  this is also done in automagic section of
      replaceIndex(color_index, static_cast<int8_t>(color_table.indexOf(QImage::color(color_index))));
  }

  // clean up unused colors
  auto list = m_color_count.keys();
  for(auto pos = list.begin(); pos != list.end(); ++pos)
  {
    if(!color_table.contains(*pos))
      m_color_count.remove(*pos);
  }
  return colors_used;
}

void QLinkedImage::syncColorsUsed(void)
{
//  qDebug() << "sync colors";
  m_color_count.clear();
  auto end = constBits() + byteCount();
  for(auto pos = constBits(); pos != end; ++pos)
    ++m_color_count[QImage::color(*pos)];
  m_color_count[QImage::color(0)] = 0; // exclude transparent color
}

//==========================================================

QSet<int> QLinkedImage::indexesUsed(void) // compiles a list of color indexes used in this image.
{
  QSet<int> indexes;
  for(int8_t color_index = 1; color_index < ZValues::colors_in_palette; ++color_index) // exclude transparent color
    if(m_index_count[color_index])
      indexes.insert(color_index);
  return indexes;
}

void QLinkedImage::syncIndexCounter(void)
{
  m_index_count = QVector<int>(ZValues::colors_in_palette, 0);
  auto end = constBits() + byteCount();
  for(auto pos = constBits(); pos != end; ++pos)
    ++m_index_count[*pos];
  m_index_count[0] = 0; // exclude transparent color
}


//==========================================================


void QLinkedImage::setImage(const QImage& image)
{
  QImage::setColorTable(image.QImage::colorTable());
  setImageData(image);
  update();
}

void QLinkedImage::setImageID(uint16_t id) // update everything from file data
{
  m_image_id = id;
  syncImageData();
  syncPalette();
  update();
}


void QLinkedImage::setImageColor(uint old_index, QRgb new_color24)
{
  Q_ASSERT(!old_index || old_index == ALPHA_VALUE); // cannot change transparent

  if(old_index & ALPHA_MASK) // not an index
    old_index = QImage::colorTable().indexOf(old_index);

  uint16_t new_index = QImage::colorTable().indexOf(new_color24);
  if(new_index != 0xFF) // only if color was found (if not found, the color was not used in the image)
  {
    auto* pos = bits();
    auto* end = pos + byteCount();
    for(; pos != end; ++pos)
      if(*pos == old_index)
        *pos = new_index;

    update();
  }
}


bool QLinkedImage::replaceIndex(uint16_t target, uint16_t replacement)
{
  QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(bits()), byteCount());
  bool found = data.contains(target);
  if(found)
  {
    m_index_count[replacement] += m_index_count[target];
    m_index_count[target] = 0;
    data.replace(target, replacement);
  }

  return found;
}

//==========================================================

void QLinkedImage::setLocalColor(uint old_index, uint new_index) // incomplete
{
  const QRgb old_color24 = round24(old_index);
  const QRgb new_color24 = round24(new_index);
  // save undo info here

  if(old_index & ALPHA_MASK) // not an index
    old_index = QImage::colorTable().indexOf(old_color24);
  if(new_index & ALPHA_MASK) // not an index
    new_index = QImage::colorTable().indexOf(new_color24);
  if(new_index == UINT_MAX)  // color is not in color table
  {
    std::unordered_map<int, QLinkedImage*>& images = GData::GetAllImages();
    QSet<QRgb> all_colors; // stores all the colors used by images that use a specific palette

    for(uint16_t image_num = imageType();
                 image_num < ZValues::max_images + imageType();
               ++image_num) // check all images
      if(GData::GetFileData().get_image_pal_idx(image_num) == paletteID()) // find the image uses the same palette number as this image
        all_colors += images[image_num]->colorsUsed(); // create a master palette

    new_index = all_colors.size(); // next availible color in the palette
  }

  update();
}

void QLinkedImage::setPaletteColor(uint old_index, uint new_index) // this needs attention :((((
{
  QRgb old_color24, new_color24;
  QVector<QRgb> color_table = QImage::colorTable();
  CPaletteData pal(GData::GetFileData().get_palette(paletteID()));

  // save undo info here

  if(old_index & ALPHA_MASK) // get index to replace
    old_index = color_table.indexOf(old_color24 = round24(old_index), 1);
  new_color24 = (new_index & ALPHA_MASK) ? new_index : color_table[new_index];

  if(pal.get(old_index) != new_color24) // dont replace colors with themselves
  {
    pal.set(old_index, new_color24);
    //GData::addUndoAction(new CUndoPaletteAlteration(palette_offset(paletteID()), old_data));
    GData::ReinitializeAllImages(); // <-- you lazy programmer! >:(
  }

  //GData::addUndoAction(new CUndoPaletteAlteration(palette_offset(paletteID()), old_data));

  update();
}


static QSet<QRgb> colorsUsedInPalette(uint16_t pal_id)
{
  QSet<QRgb> all_colors; // stores all the colors used by images that use a specific palette
/*
  for(uint16_t image_num = pal_id  < ZValues::max_image_palettes ? eTile : eSprite;
               image_num < ZValues::max_images + (image_num & ZValues::max_images);
             ++image_num)
  {
    if(GData::GetFileData().get_image_pal_idx(image_num) == pal_id) // if image uses the same palette
      all_colors += GData::GetImage(image_num)->colorsUsed(); // create master palette
  }
*/
  return all_colors;
}

static QSet<int> indexesUsedInPalette(uint16_t pal_id)
{

  if(GData::GetFileData().properties.use_index_map)
  {
    if(pal_id & ZValues::sprite_mask)
    {
    }
    else
    {
    }
  }
  else
  {
    if(pal_id & ZValues::sprite_mask)
    {
    }
    else
    {
    }
  }

  QSet<int> indexes; // stores all the indexes used by images that use a specific palette
/*
  for(uint16_t image_num = pal_id  < ZValues::max_image_palettes ? eTile : eSprite;
               image_num < ZValues::max_images + (image_num & ZValues::max_images);
             ++image_num)
  {
    if(GData::GetFileData().get_image_pal_idx(image_num) == pal_id) // if image uses the same palette
      indexes += GData::GetImage(image_num)->indexesUsed(); // master list of indexes
  }
*/
  return indexes;
}


struct palidx_t
{
    QSet<QRgb> palette; // colors in palette
    QSet<uint> image_nums; // tiles/sprites that use this palette
};

void QLinkedImage::setPixel(int x, int y, uint index)
{
  // current info
  QRgb color24;
  EImageType type = imageType();
  int start = type;
  int end = type + ZValues::max_images;
  uint16_t pal_id = paletteID();

  // backup data
  uint old_index = QImage::pixelIndex(x, y);
  QRgb old_color24 = QImage::color(old_index);
  QImage old_image(*this);// = this->copy();//(this->size(), this->format());
  bool old_optimized = indexesOptimized();
  CByteArray old_palette_assignments(GData::GetFileData().get_image_pal_idx(start, ZValues::max_images), true); // backup palette indexes

  GData::startUndoMacro(QString("QLinkedImage::setPixel(%1, %2, %3)").arg(x).arg(y).arg(index));

  if(index & ALPHA_MASK) // not an index
  {
    color24 = round24(index);
    index = QImage::colorTable().indexOf(color24, 1);
  }
  else
    color24 = color(index);

  index = QImage::colorTable().indexOf(color24, 1); // find color in image palette

  try
  {
    if(index == UINT_MAX) // color isnt in image palette :/
    {
      if(colorsUsed().size() < ZValues::colors_in_palette)
      {
        std::unordered_map<int, QLinkedImage*>& images = GData::GetAllImages();
        QSet<int> palette_indexes = indexesUsedInPalette(pal_id);
        QList<int> unused_indexes = (QSet<int>::fromList(QList<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 })) - palette_indexes).toList();

        palette_indexes.insert(0); // always has transparent color

        // =========================== AUTOMAGIC PALETTE OPTIMIZATION ===========================
        if(false && autoOptimizeEnabled())
        {
          QSet<QRgb> all_colors = colorsUsedInPalette(pal_id); // stores all the colors used by images that use a specific palette
          int all_colors_count = all_colors.size();

          Q_ASSERT(all_colors_count <= ZValues::colors_in_palette);

          if(palette_indexes.size() < ZValues::colors_in_palette &&
             !allowDuplicateColors()) // remove duplicate colors from this palette
          {
            optimizeIndexes(pal_id);

            palette_indexes = indexesUsedInPalette(pal_id);
            unused_indexes = (QSet<int>::fromList(QList<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 })) - palette_indexes).toList();
            qSort(unused_indexes.begin(), unused_indexes.end(), qLess<int>());
            index = unused_indexes.takeFirst();
          }
          else if(palette_indexes.size() == ZValues::colors_in_palette)
          {
            // begin the magic!
            // <HOLD ONTO YOUR BUTTS>

            QHash<int, QSet<QRgb> > image_palettes; // current sets of colors used for each image
            QHash<int, QMultiHash<QRgb, int> > image_color_indexes;

            // get which colors used for each image
            for(int image_num = start; image_num < end; ++image_num) // get palette for each image (does include the transparent colors)
            {
              QLinkedImage* image = images[image_num];
              for(int color_num = 0; color_num < ZValues::colors_in_palette; ++color_num)
                image_color_indexes[image_num].insert(image->color(color_num), color_num);

               // note: if the image is empty, it's palette and palette index number will not be modified
              image_palettes[image_num] = images[image_num]->colorsUsed();
            }

            image_palettes[imageID()].insert(color24); // add the new color to the current image
            if(m_index_count[old_index] == 1)
              image_palettes[imageID()].remove(old_color24);

            // build new palettes from scratch
            CByteArray palette_assignments(ZValues::max_images);
            QVector<palidx_t> palidx(ZValues::max_image_palettes); // stores new palettes and which images use which palette

            if(!allowDuplicateColors())
            {
              for(int pal_num = 0; pal_num < ZValues::max_image_palettes; ++pal_num) // iterate through all 16 palettes
              {
                QSet<QRgb>& palette          = palidx[pal_num].palette;
                QSet<uint>& images_using_pal = palidx[pal_num].image_nums;

                // find the images with the least number new colors
                int smallest_value;
                int best_image_index;

                do
                {
                  best_image_index = smallest_value = INT_MAX;

                  for(int image_num = start; image_num < end; ++image_num) // for each image
                  {
                    if(!image_palettes[image_num].empty()) // if not already used
                    {
                      int num_new_colors = (image_palettes[image_num] - palette).size(); // get the number of new colors that would have to be added to the palette for this image

                      if(palette.size() + num_new_colors < ZValues::colors_in_palette && // if the new colors would fit in the palette AND
                         num_new_colors < smallest_value)                                // it requires less colors than the previous image(s)
                      {
                        smallest_value = num_new_colors; // save the number of colors that would have to be added to the palette
                        best_image_index = image_num;
                      }
                    }
                  }

                  if(best_image_index != INT_MAX)
                  {
                    palette_assignments[best_image_index] = pal_num;
                    images_using_pal.insert(best_image_index);
                    palette += image_palettes[best_image_index];
                    image_palettes[best_image_index].clear();
                  }
                } while (best_image_index != INT_MAX);

//                if(!palette.empty()) { printf("new palette %i:", pal_num); for(auto pos = palette.begin(); pos != palette.end(); ++pos) { printf(" 0x%06x", (*pos) & RGB_MASK); } printf("\n"); fflush(stdout); }
              }
            }
            else // allowDuplicateColors() == true
            {
              Q_ASSERT(false); // no code for this option yet!
            }
            // now we have the new palettes and tile/sprite indexes :D

            GData::GetFileData().get_image_pal_idx(start, ZValues::max_images) = palette_assignments; // write new palette assignments to file

            // <update every image and palette>
            for(int palette_num = 0; palette_num < palidx.size(); ++palette_num)
            {
              QSet<QRgb>& palette = palidx[palette_num].palette; // referenced memory
              QVector<uint> images_using_pal = QVector<uint>::fromList(palidx[palette_num].image_nums.values()); // copy

              QVector<QRgb> color_table = QVector<QRgb>::fromList(palette.values());
              QVector<QRgb> old_color_table;
              QVector<QRgb> new_color_table(ZValues::colors_in_palette, 0);

              // <finish making new palette>
              // <reorder color_table to match the old palette as close as possible>
              color_table.resize(ZValues::colors_in_palette); // expand to fit an entire palette
              old_color_table = CPaletteData(GData::GetFileData().get_palette((type >> 4) | palette_num)).operator QVector<QRgb>();

              QList<uint> unused_indexes;
              for(int color_index = 1; color_index < ZValues::colors_in_palette; ++color_index) // exclude transparent color
              {
                const QRgb& color = old_color_table[color_index];
                if(palette.contains(color))
                {
                  new_color_table[color_index] = color;
                  palette.remove(color);
                }
                else
                  unused_indexes.push_back(color_index);
              }

              QList<QRgb> remaining_colors = QList<QRgb>::fromSet(palette);
              while(!remaining_colors.empty())
                 new_color_table[unused_indexes.takeFirst()] = remaining_colors.takeFirst();

              QRgb tmpcolor = ALPHA_VALUE;
/*
              while(color_table.contains(tmpcolor)) // find the first unique color
              {
                if((tmpcolor & 0x000001FF) == 0x00000100)
                  tmpcolor -= 0x00000100;
                else if((tmpcolor & 0x0001FF00) == 0x00010000)
                  tmpcolor -= 0x00010000;
                else if((tmpcolor & 0x01FF0000) == 0x01000000)
                  tmpcolor -= 0x01000000;
              }
*/
              new_color_table[0] = tmpcolor; // add transparent to front of palette

              while(!unused_indexes.empty())
                new_color_table[unused_indexes.takeFirst()] = 0; // fill the rest of the palette with black

              color_table = new_color_table;
              // </reorder color_table to match the old palette as close as possible>


              if(old_color_table == color_table) // this palette didnt change at all!
                continue;

              // <translate images to new palette>
              for(int index_num = 0; index_num < images_using_pal.size(); ++index_num)
              {
                const int& image_num = images_using_pal[index_num];
                QLinkedImage* image = images[image_num];
                QImage old_image = *image;
                bool old_optimized = image->indexesOptimized();
                int byte_count = image->byteCount();
                uint16_t* data = image->bits();

                GData::GetFileData().set_image_pal_idx(image->imageID(), palette_num);

                // <create index translator>
                uint16_t translator[ZValues::colors_in_palette];
                Q_ASSERT(image->colorCount() == ZValues::colors_in_palette);
                for(int color_index = 0; color_index < ZValues::colors_in_palette; ++color_index)
                  if((translator[color_index] = color_table.indexOf(image->color(color_index), 1)) == 0xFF)
                    translator[color_index] = 0;
                // </create index translator>

                // <update image data to use new color table>
                for(int k = 0; k < byte_count; ++k)
                  data[k] = translator[data[k]];

                image->setColorTable(color_table);
                // </update image data to use new color table>

                // the undo action will set/restore all the palette data.  no need to save it manually
                GData::addUndoAction(new CUndoLinkedImageAlteration(image, old_image, old_palette_assignments[image_num & 0xFF], old_optimized)); // set the new image data and palette in QImage and file
              }
              // </translate images to new palette>
            }
            // </update every image and palette>

            // </HOLD ONTO YOUR BUTTS>

            index = QImage::colorTable().indexOf(color24, 1);
          }
          else
          {
            qDebug() << "palette_indexes.size() > all_colors_count  : " << palette_indexes.size() << ">" << all_colors_count;
            qDebug() << palette_indexes;

            Q_ASSERT(false);
          }
        }
        // =========================== MANUAL PALETTE COLOR ADDITION ===========================
        else
        {
          if(!unused_indexes.empty())
          {
            qSort(unused_indexes.begin(), unused_indexes.end(), qLess<int>());
            index = unused_indexes.takeFirst();
          }
          else // failure! D:
          {
            throw(QString("%1 Palette #%2 is full.  Enable automatic palette optimization to solve this problem.")
                .arg(type == eTile ? "Tile" : "Sprite")
                .arg(relativeImageID()));
          }
        }
      }
      else
      {
        throw(QString("It is not possible to add more colors to %1 #%2.")
            .arg(type == eTile ? "Tile" : "Sprite")
            .arg(relativeImageID()));
      }
    }

// =========================== COMMON IMAGE UPDATER ===========================
    if(index == UINT_MAX) // abandon all hope!
    {
      throw(QString("All %1 palettes are full!")
          .arg(type == eTile ? "Tile" : "Sprite"));
    }
    else
    {
      m_index_count[old_index]--;
      m_color_count[old_color24]--;

      m_index_count[index]++;
      m_color_count[color24]++;

      QImage::setColor(index, color24); // update palette
      QImage::setPixel(x, y, index); // update pixels
      GData::addUndoAction(new CUndoLinkedImageAlteration(this, old_image, old_palette_assignments[relativeImageID()], old_optimized)); // set the new image data/palette index/palette
    }
  }
  catch(QString message) // something has gone awry!
  {
    qDebug() << "error:" << message;
    emit statusMessage(message, 1000);
  }

  GData::endUndoMacro();
}

void QLinkedImage::optimizeIndexes(uint16_t pal_id)
{
  QVector<QRgb> new_color_table;
  QVector<uint16_t> translator;
  std::unordered_map<int, QLinkedImage*>& images = GData::GetAllImages();

  for(int image_num = 0; image_num < ZValues::max_tiles + ZValues::max_sprites; ++image_num)
  {
    if(GData::GetFileData().get_image_pal_idx(image_num) == pal_id)
    {
      QLinkedImage* image = images[image_num];
      image->syncPalette();
      if(translator.empty())
        image->indexOptimizer(new_color_table, translator);
      image->translateIndexes(translator);
      image->setColorTable(new_color_table);
      image->setIndexesOptimized();
    }
  }
}

void QLinkedImage::indexOptimizer(QVector<QRgb>& new_color_table, QVector<uint16_t>& translator)
{
  QVector<QRgb> color_table = QImage::colorTable();
  new_color_table.resize(ZValues::colors_in_palette);
  translator.resize(ZValues::colors_in_palette);

  for(int color_index = 1; color_index < ZValues::colors_in_palette; ++color_index) // do not translate transparenc
  {
    translator[color_index] = color_table.indexOf(color_table[color_index], 1);
    if(translator[color_index] == 0xFF)
      translator[color_index] = 0;
    else
      new_color_table[color_index] = color_table[translator[color_index]];
  }
  // qDebug() << "translator:" << translator;
}

QVector<uint16_t> QLinkedImage::colorTableTranslator(const QVector<QRgb>& color_table)
{
  QVector<uint16_t> translator = createIndexTranslator(QImage::colorTable(), color_table);
  translateIndexes(translator);
  QImage::setColorTable(color_table);
  return translator;
}

// static
QVector<uint16_t> QLinkedImage::createIndexTranslator(const QVector<QRgb>& before, const QVector<QRgb>& after)
{
  Q_ASSERT((before.size() & after.size()) == ZValues::colors_in_palette);
  QVector<uint16_t> translator(ZValues::colors_in_palette);
  for(int color_index = 0; color_index < ZValues::colors_in_palette; ++color_index)
    if((translator[color_index] = after.indexOf(before[color_index])) == 0xFF)
      translator[color_index] = 0;

  return translator;
}

// update image data to use new color table
void QLinkedImage::translateIndexes(const QVector<uint16_t>& translator)
{
  uint16_t* data = bits();
  int byte_count = byteCount();
  for(int k = 0; k < byte_count; ++k)
    data[k] = translator[data[k]];
/*
  QVector<int> index_count(ZValues::colors_in_palette, 0);
  for(int i = 0; i < ZValues::colors_in_palette; ++i)
    index_count[translator[i]] = m_index_count[i];

  m_index_count = index_count;
*/
  m_index_count.fill(0);

  auto end = constBits() + byteCount();
  for(auto pos = constBits(); pos != end; ++pos)
    ++m_index_count[*pos];

}

void QLinkedImage::setAutoOptimizeEnabled(bool enable)
{
  m_auto_optimize_enabled = enable;
}

bool QLinkedImage::autoOptimizeEnabled(void)
{
  return m_auto_optimize_enabled;
}

const int& QLinkedImage::state(void) const
{
  return m_state;
}

void QLinkedImage::update(void)
{
  ++m_state;
  emit updated();
  emit updatedPalette(QImage::colorTable());
}
