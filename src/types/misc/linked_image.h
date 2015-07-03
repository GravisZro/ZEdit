#ifndef LINKED_IMAGE_H
#define LINKED_IMAGE_H

#include <QtGui>
#include <stdint.h>

#include <common/nullable.h>
#include <types/misc/pce_types.h>
#include <types/misc/zfile_io.h>
//#include <types/undo_commands/undo_linked_image_alteration.h>

enum EImageType : uint16_t
{
  eTile   = 0x0000,
  eSprite = ZValues::sprite_mask,
};

struct image_state_t
{
  QImage image;
  int image_id;
  int palette_id;
};

class CUndoLinkedImageAlteration;

class QLinkedImage : public QObject,
                     public QImage
{
    Q_OBJECT
  public:
    explicit QLinkedImage(int width, int height, Format format, QObject* parent = nullptr);

    operator image_state_t(void);
    const image_state_t& operator =(const image_state_t& state);

    const QImage& operator= (const QImage& image);
    const QVector<QRgb>& operator =(const QVector<QRgb>& colors);

    QSet<QRgb> colorsUsed (void);
    QSet<int>  indexesUsed(void);

    bool replaceIndex(uint16_t target, uint16_t replacement);

    inline uint16_t relativeImageID(void) const { return imageID() & ZValues::image_mask; }
    inline uint16_t imageID(void) const { return m_image_id; }
    uint16_t paletteID(void) const;

    bool autoOptimizeEnabled(void);

    void update(void);

    const int& state(void) const;
    EImageType imageType(void) const;

  public slots:
    void setAutoOptimizeEnabled(bool enable = true);

    //void setColor(int index, QRgb colorValue) { Q_ASSERT(false); }
    void setPaletteColor(uint old_index_or_rgb, uint new_index_or_rgb); // changes all images that use this palette
    void setLocalColor  (uint old_index_or_rgb, uint new_index_or_rgb); // changes the local palette
    void setImageColor  (uint old_index_or_rgb, QRgb new_color);        // replaces a color in the current image without affecting other images

    void setImageID  (uint16_t id);
    void setPaletteID(uint16_t id);
    void setPixel(int x, int y, uint index_or_rgb);

  signals:
    void updated(void);
    void statusMessage(QString message, int timeout);
    void updatedPalette(QVector<QRgb> colors);

  private:
    void syncIndexCounter(void);

    void optimizeIndexes(uint8_t pal_id);
    void setIndexesOptimized(bool is_optimized = true) { m_indexes_optimized = is_optimized; }
    bool indexesOptimized(void) const { return m_indexes_optimized; }

    void setImageData(const QImage& image);
    void setImage(const QImage& image);

    void syncPalette   (void); // copy palette from file
    void syncImageData (void); // copy image data from file
    void syncColorsUsed(void); // repopulates the color count with current data

    void basicPixelUpdate(int x, int y, uint index_or_rgb);

    static bool allowDuplicateColors(void) { return false; }

#if 1

    void indexOptimizer(QVector<QRgb>& new_color_table, QVector<uint8_t>& translator);
    QVector<uint8_t> colorTableTranslator(const QVector<QRgb>& color_table);
    static QVector<uint8_t> createIndexTranslator(const QVector<QRgb>& before, const QVector<QRgb>& after);
    void translateIndexes(const QVector<uint8_t>& translator);
#endif
  private:
    int m_state;
    uint16_t m_image_id;
    QHash<QRgb, int> m_color_count;
    QVector<int> m_index_count;
    bool m_auto_optimize_enabled;
    bool m_indexes_optimized;

    friend class CUndoLinkedImageAlteration;
};

#endif // LINKED_IMAGE_H
