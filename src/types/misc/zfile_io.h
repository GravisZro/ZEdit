#ifndef ZFILE_IO_H
#define ZFILE_IO_H

#include <QIODevice>

#include <stdint.h>
#include <vector>
#include <memory>

#include "matrix.h"
#include "pce_types.h"
#include "structs.h"

enum ZValues : uint16_t
{
  sprite_size        = 0x0080,
  tile_size          = 0x0020,
  bytes_in_palette   = 0x0020,
  colors_in_palette  = 0x0010,
  sprite_mask        = 0x8000,
  image_mask         = 0x7FFF,

  sector_size        = 0x00000800,


  max_image_palettes = 32,
  max_images = 512,
};

enum ZData : uint16_t
{
  HeaderData,
  Palettes,

  TilePatterns,
  TilePalettes,
  TileIndexes,
  TileMasks,

  SpritePatterns,
  SpritePalettes,
  SpriteIndexes,
  SpriteMasks,

  IndexMap,
  MaskMap,
  TileMap,
  MacroMap,
};

struct block_mask_t
{
  uint8_t mask;
};

struct properties_t
{
  uint8_t has_tiles      : 1; // 0 = no tile data
                              // 1 = contains tile data

  uint8_t has_sprites    : 1; // 0 = no sprite data
                              // 1 = contains sprite data

  uint8_t use_index_map  : 1; // 0 = use tiles.indexes
                              // 1 = use maps.indexes

  uint8_t use_mask_map   : 1; // 0 = use tiles.masks
                              // 1 = use maps.masks

  uint8_t contains_music : 2; // 0 = no music
                              // 1 = contains MML music data for the level
                              // 2 = use cd track for music
                              // 3 = use current music

  uint8_t                : 2; // unused bits
};

struct header_t;

class ZFile
{
private:
  std::vector<char> m_header_data;
  header_t*         m_header;

public:
  ZFile(void);

  void read (QIODevice* device);
  void write(QIODevice* device);

  template<ZData data_index, typename index_type, typename data_type>
  data_type& get_data(index_type index_value);


  uint16_t get_image_pal_idx(uint16_t image_id)
  {
    if(image_id & ZValues::sprite_mask)
      return sprites.indexes[image_id & image_mask];
    return tiles.indexes[image_id];
  }

  void set_image_pal_idx(uint16_t image_id, const uint16_t value)
  {
    if(image_id & ZValues::sprite_mask)
      sprites.indexes[image_id & image_mask] = value;
    else
      tiles.indexes[image_id] = value;
  }

  CPaletteData& get_palette(uint16_t pal_id)
  {
    if(pal_id & ZValues::sprite_mask)
      return sprites.palettes[pal_id & ZValues::image_mask];
    return tiles.palettes[pal_id];
  }

// ======================== types ========================



  struct macro_entry_t        // a single tile/sprite in the macro
  {
    uint16_t     pattern;     // tile/sprite pattern id
    uint16_t     palette;     // palette number
    block_mask_t mask   ;     // properties mask (collision mask, sprite size, etc.)
  };

  struct macro_block_t        // the collection of sprite/tile entries
  {
    dimensions_t dimensions; // number of tiles/sprites wide/high
    macro_entry_t entries[0]; // num entries = width * height
  };

  struct warp_point_t
  {
    coords_t source        ;  // location of the warp point
    coords_t destination   ;  // location to warp to
    uint8_t  level_warp : 1;  // warp to different map?
    uint8_t  overlay    : 7;  // number of the overlay to load
  };

  //=================================================
  template<typename TPatternData>
  struct blocks_t
  {
    std::vector<TPatternData > patterns;  // actual pixel data
    std::vector<CPaletteData > palettes;  // every palette for this type of block
    std::vector<uint16_t     > indexes ;  // block #X uses palette #Y
    std::vector<block_mask_t > masks   ;  // block #X has property A, B and C. (e.g. collision with (other) sprites)
    std::vector<macro_block_t> macros  ;  // used to create compound blocks
  };

  blocks_t<CSpriteData> sprites;
  blocks_t<CTileData  > tiles;

  //=================================================
  struct allmaps_t
  {
    CMatrix<uint16_t    > indexes; // palette indexes - one entry per 8x8 tile
    CMatrix<block_mask_t> masks  ; // tile masks - one entry per 8x8 tile
    CMatrix<uint16_t    > tiles  ; // tile numbers - one entry per 8x8 tile
    CMatrix<uint16_t    > macros ; // unsorted list of macros in the map (editor use only)
  } maps;

  std::vector<warp_point_t> warp_points;

  dimensions_t& map_size   ; // total size of the map (in tiles)
  dimensions_t& screen_size; // size of each screen (in tiles)
  properties_t& properties ;

};

template<> CPaletteData& ZFile::get_data<ZData::Palettes, uint16_t, CPaletteData>(uint16_t index_value)
{
  if(index_value & ZValues::sprite_mask)
    return sprites.palettes[index_value & ZValues::image_mask];
  return tiles.palettes[index_value];
}

template<> CTileData   & ZFile::get_data<ZData::TilePatterns  , uint16_t, CTileData   >(uint16_t index_value) { return tiles.patterns [index_value]; }
template<> CPaletteData& ZFile::get_data<ZData::TilePalettes  , uint16_t, CPaletteData>(uint16_t index_value) { return tiles.palettes [index_value]; }
template<> uint16_t    & ZFile::get_data<ZData::TileIndexes   , uint16_t, uint16_t    >(uint16_t index_value) { return tiles.indexes  [index_value]; }
template<> block_mask_t& ZFile::get_data<ZData::TileMasks     , uint16_t, block_mask_t>(uint16_t index_value) { return tiles.masks    [index_value]; }

template<> CSpriteData & ZFile::get_data<ZData::SpritePatterns, uint16_t, CSpriteData >(uint16_t index_value) { return sprites.patterns[index_value]; }
template<> CPaletteData& ZFile::get_data<ZData::SpritePalettes, uint16_t, CPaletteData>(uint16_t index_value) { return sprites.palettes[index_value]; }
template<> uint16_t    & ZFile::get_data<ZData::SpriteIndexes , uint16_t, uint16_t    >(uint16_t index_value) { return sprites.indexes [index_value]; }
template<> block_mask_t& ZFile::get_data<ZData::SpriteMasks   , uint16_t, block_mask_t>(uint16_t index_value) { return sprites.masks   [index_value]; }

template<> uint16_t    & ZFile::get_data<ZData::IndexMap, coords_t, uint16_t    >(coords_t index_value) { return maps.indexes [index_value.x][index_value.y]; }
template<> block_mask_t& ZFile::get_data<ZData::MaskMap , coords_t, block_mask_t>(coords_t index_value) { return maps.masks   [index_value.x][index_value.y]; }
template<> uint16_t    & ZFile::get_data<ZData::TileMap , coords_t, uint16_t    >(coords_t index_value) { return maps.tiles   [index_value.x][index_value.y]; }
template<> uint16_t    & ZFile::get_data<ZData::MacroMap, coords_t, uint16_t    >(coords_t index_value) { return maps.macros  [index_value.x][index_value.y]; }


#endif
