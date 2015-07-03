#ifndef UNDO_DATA_ALTERATION_H
#define UNDO_DATA_ALTERATION_H

#include <common/global.h>

#include <QUndoCommand>

#include <utility>
#include <stdint.h>
#include <types/misc/mmvector.h>
#include <types/misc/template_array.h>
#include <types/misc/zfile_io.h>

/*
namespace messages
{
  inline const char* tile_deletion              (void) { return "Tile Deletion"             ; }
  inline const char* tile_pattern_alteration    (void) { return "Tile Pattern Alteration"   ; }
  inline const char* tile_palette_alteration    (void) { return "Tile Palette Alteration"   ; }
  inline const char* tile_index_alteration      (void) { return "Tile Index Alteration"     ; }
  inline const char* tile_mask_alteration       (void) { return "Tile Mask Alteration"      ; }

  inline const char* sprite_deletion            (void) { return "Sprite Deletion"           ; }
  inline const char* sprite_pattern_alteration  (void) { return "Sprite Pattern Alteration" ; }
  inline const char* sprite_palette_alteration  (void) { return "Sprite Palette Alteration" ; }
  inline const char* sprite_index_alteration    (void) { return "Sprite Index Alteration"   ; }
  inline const char* sprite_mask_alteration     (void) { return "Sprite Mask Alteration"    ; }


  inline const char* index_map_alteration       (void) { return "Index Map Alteration"      ; }
  inline const char* mask_map_alteration        (void) { return "Mask Map Alteration"       ; }
  inline const char* tile_map_alteration        (void) { return "Tile Map Alteration"       ; }
  inline const char* macro_map_alteration       (void) { return "Macro Map Alteration"      ; }
}
*/

namespace messages
{
  enum : uint16_t
  {
    tile_pattern_alteration = 0,
    tile_palette_alteration  ,
    tile_index_alteration    ,
    tile_mask_alteration     ,

    sprite_pattern_alteration,
    sprite_palette_alteration,
    sprite_index_alteration  ,
    sprite_mask_alteration   ,

    index_map_alteration     ,
    mask_map_alteration      ,
    tile_map_alteration      ,
    macro_map_alteration     ,
  };

  const char* strings[] =
  {
    "Tile Pattern Alteration"  ,
    "Tile Palette Alteration"  ,
    "Tile Index Alteration"    ,
    "Tile Mask Alteration"     ,

    "Sprite Pattern Alteration",
    "Sprite Palette Alteration",
    "Sprite Index Alteration"  ,
    "Sprite Mask Alteration"   ,

    "Index Map Alteration"     ,
    "Mask Map Alteration"      ,
    "Tile Map Alteration"      ,
    "Macro Map Alteration"     ,
  };
}



template<ZData data_select, const uint16_t msg_id, typename data_type, typename index_type>
class CUndoDataAlteration : public QUndoCommand
{
  public:
    explicit CUndoDataAlteration(const index_type index, const data_type& old_data,                            QUndoCommand* parent = nullptr);
    explicit CUndoDataAlteration(const index_type index, const data_type& old_data, const data_type& new_data, QUndoCommand* parent = nullptr);


/*
    explicit CUndoDataAlteration(ZData data_index, const data_type& old_data, QUndoCommand* parent = nullptr);
    explicit CUndoDataAlteration(ZData data_index, const data_type& old_data, const data_type& new_data, QUndoCommand* parent = nullptr);

    explicit CUndoDataAlteration(uint8_t old_data, QUndoCommand* parent = nullptr);
    explicit CUndoDataAlteration(uint8_t old_data, uint8_t new_data, QUndoCommand* parent = nullptr);
    explicit CUndoDataAlteration(ZData data_index, uint8_t old_data, QUndoCommand* parent = nullptr);
    explicit CUndoDataAlteration(ZData data_index, uint8_t old_data, uint8_t new_data, QUndoCommand* parent = nullptr);
*/
//    ~CUndoDataAlteration(void) { }

    virtual void undo(void);
    virtual void redo(void);

  protected:
    index_type m_index;
    data_type m_old_data;
    data_type m_new_data;
};

//typedef CUndoDataAlteration<ZData::HeaderData     , messages::data_alteration         > CUndoAllAlterations        ;
//typedef CUndoDataAlteration<ZData::Palettes       , messages::palette_alteration      > CUndoPaletteAlteration     ;

typedef CUndoDataAlteration<ZData::TilePatterns   , messages::tile_pattern_alteration   , CTileData   , uint16_t> CUndoTilePatternAlteration  ;
typedef CUndoDataAlteration<ZData::TilePalettes   , messages::tile_palette_alteration   , CPaletteData, uint16_t> CUndoTilePaletteAlteration  ;
typedef CUndoDataAlteration<ZData::TileIndexes    , messages::tile_index_alteration     , uint16_t    , uint16_t> CUndoTileIndexAlteration    ;
typedef CUndoDataAlteration<ZData::TileMasks      , messages::tile_mask_alteration      , block_mask_t, uint16_t> CUndoTileMaskAlteration     ;

typedef CUndoDataAlteration<ZData::SpritePatterns , messages::sprite_pattern_alteration , CSpriteData , uint16_t> CUndoSpritePatternAlteration;
typedef CUndoDataAlteration<ZData::SpritePalettes , messages::sprite_palette_alteration , CPaletteData, uint16_t> CUndoSpritePaletteAlteration;
typedef CUndoDataAlteration<ZData::SpriteIndexes  , messages::sprite_index_alteration   , uint16_t    , uint16_t> CUndoSpriteIndexAlteration  ;
typedef CUndoDataAlteration<ZData::SpriteMasks    , messages::sprite_mask_alteration    , block_mask_t, uint16_t> CUndoSpriteMaskAlteration   ;

typedef CUndoDataAlteration<ZData::IndexMap       , messages::index_map_alteration      , uint16_t    , coords_t> CUndoIndexMapAlteration     ;
typedef CUndoDataAlteration<ZData::MaskMap        , messages::mask_map_alteration       , block_mask_t, coords_t> CUndoMaskMapAlteration      ;
typedef CUndoDataAlteration<ZData::TileMap        , messages::tile_map_alteration       , uint16_t    , coords_t> CUndoTileMapAlteration      ;
typedef CUndoDataAlteration<ZData::MacroMap       , messages::macro_map_alteration      , uint16_t    , coords_t> CUndoMacroMapAlteration     ;

template<ZData data_select, const uint16_t msg_id, typename data_type, typename index_type>
CUndoDataAlteration<data_select, msg_id, data_type, index_type>::CUndoDataAlteration(const index_type index, const data_type& old_data, QUndoCommand* parent)
  : CUndoDataAlteration(index, old_data, GData::GetFileData().get_data<data_select, data_type, index_type>(index), parent)
{
}

template<ZData data_select, const uint16_t msg_id, typename data_type, typename index_type>
CUndoDataAlteration<data_select, msg_id, data_type, index_type>::CUndoDataAlteration(const index_type index, const data_type& old_data, const data_type& new_data, QUndoCommand* parent)
  : QUndoCommand(QString("Description: %1\nDetails: data select %2.\nold data: %3\nnew data: %4")
                 .arg(messages::strings[msg_id])
                 .arg(data_select)
                 .arg(old_data)
                 .arg(new_data), parent),
    m_index(index),
    m_old_data(old_data),
    m_new_data(new_data)
{
}


template<ZData data_index, const uint16_t msg_id, typename index_type, typename data_type>
void CUndoDataAlteration<data_index, msg_id, index_type, data_type>::undo(void)
{
  GData::GetFileData().get_data<data_index, index_type, data_type>() = m_old_data;
  GData::ReinitializeAllImages();
}

template<ZData data_index, const uint16_t msg_id, typename index_type, typename data_type>
void CUndoDataAlteration<data_index, msg_id, index_type, data_type>::redo(void)
{
  GData::GetFileData().get_data<data_index, index_type, data_type>() = m_new_data;
  GData::ReinitializeAllImages();
}

#endif // UNDO_DATA_ALTERATION_H
