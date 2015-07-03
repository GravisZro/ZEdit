#include "zfile_io.h"

template<typename unsigned_num>
constexpr int64_t to_sector(const unsigned_num sector_num) { return ZValues::sector_size * sector_num; }

template<typename unsigned_num>
constexpr int64_t from_sector(const unsigned_num byte_count) { return byte_count / ZValues::sector_size + (byte_count % ZValues::sector_size ? 1 : 0); }

struct vector_loc_t
{
  uint16_t count;
  uint16_t sector;
};

struct matrix_loc_t
{
  dimensions_t dimensions;
  uint16_t sector;
};

// block == sprite or tile
struct blocks_vector_locs_t
{
  vector_loc_t patterns;       // actual pixel data
  vector_loc_t palettes;       // every palette for this type of block
  vector_loc_t indexes ;       // block #X uses palette #Y
  vector_loc_t masks   ;       // block #X has property A, B and C. (e.g. collision with (other) sprites)
  vector_loc_t macros  ;       // used to create compound blocks
};

//=================================================
struct header_t
{
  char magic[6]; // magic number is: Zelda\0

  properties_t properties;

  struct
  {
    uint8_t number;     // level number
    char    name[0x20]; // level name
    void setname(std::string new_name) { memcpy(name, new_name.data(), sizeof(name)); }
  } identifiers;

  struct
  {
    dimensions_t map   ; // total size of the map (in tiles)
    dimensions_t screen; // size of each screen (in tiles)
  } dimensions;

  blocks_vector_locs_t sprites;
  blocks_vector_locs_t tiles;

  struct
  {
    matrix_loc_t indexes; // palette indexes - one entry per 8x8 tile
    matrix_loc_t masks;   // tile masks - one entry per 8x8 tile
    matrix_loc_t tiles;   // tile numbers - one entry per 8x8 tile
    matrix_loc_t macros;  // unsorted list of macros in the map (editor use only)
  } maps; // each has

  vector_loc_t warp_points; // an array of warp_point_t data

  struct
  {
    uint8_t   cd_track_num;
    vector_loc_t MML_data;
  } music;
};




ZFile::ZFile(void)
  : m_header_data(sizeof(header_t)),
    m_header(reinterpret_cast<header_t*>(m_header_data.data())),
    map_size(m_header->dimensions.map),
    screen_size(m_header->dimensions.screen),
    properties(m_header->properties)
{
}

template<typename data_type>
static void read_vector_loc(QIODevice* device, std::vector<data_type>& data, const vector_loc_t& location)
{
  data.resize(location.count);
  device->seek(to_sector(location.sector));
  device->read(reinterpret_cast<char*>(data.data()), sizeof(data_type) * location.count);
}

template<typename data_type>
static void read_matrix_loc(QIODevice* device, CMatrix<data_type>& data, const matrix_loc_t& location)
{
  data.resize(location.dimensions.columns, location.dimensions.rows);
  device->seek(to_sector(location.sector));
  for(auto pos = data.begin(); pos != data.end(); ++pos)
    device->read(reinterpret_cast<char*>(pos->data()), sizeof(data_type) * location.dimensions.columns);
}

void ZFile::read (QIODevice* device)
{
  if(!device->isOpen())
    device->open(QIODevice::ReadOnly);
  Q_ASSERT(device->isOpen());

  device->read(m_header_data.data(), sizeof(header_t));

  read_vector_loc(device, tiles.patterns, m_header->tiles.patterns);
  read_vector_loc(device, tiles.palettes, m_header->tiles.palettes);
  read_vector_loc(device, tiles.indexes , m_header->tiles.indexes );
  read_vector_loc(device, tiles.masks   , m_header->tiles.masks   );
  read_vector_loc(device, tiles.macros  , m_header->tiles.macros  );

  read_vector_loc(device, sprites.patterns, m_header->sprites.patterns);
  read_vector_loc(device, sprites.palettes, m_header->sprites.palettes);
  read_vector_loc(device, sprites.indexes , m_header->sprites.indexes );
  read_vector_loc(device, sprites.masks   , m_header->sprites.masks   );
  read_vector_loc(device, sprites.macros  , m_header->sprites.macros  );

  read_matrix_loc(device, maps.indexes, m_header->maps.indexes);
  read_matrix_loc(device, maps.masks  , m_header->maps.masks  );
  read_matrix_loc(device, maps.tiles  , m_header->maps.tiles  );
  read_matrix_loc(device, maps.macros , m_header->maps.macros );

  read_vector_loc(device, warp_points, m_header->warp_points);

  device->close();
}

template<typename data_type>
static void write_vector_loc(QIODevice* device, int64_t& sector, std::vector<data_type>& data, vector_loc_t& location)
{
  location.sector = sector;
  location.count = data.size();
  sector += from_sector(location.count * sizeof(data_type));

  data.resize(location.count);
  device->seek(to_sector(location.sector));
  device->write(reinterpret_cast<const char*>(data.data()), sizeof(data_type) * location.count);
}

template<typename data_type>
static void write_matrix_loc(QIODevice* device, int64_t& sector, CMatrix<data_type>& data, matrix_loc_t& location)
{
  location.sector = sector;
  location.dimensions.rows    = data.rows();
  location.dimensions.columns = data.columns();
  sector += from_sector( location.dimensions.rows *
                         location.dimensions.columns *
                         sizeof(data_type));

  data.resize(location.dimensions.columns, location.dimensions.rows);
  device->seek(to_sector(location.sector));
  for(auto pos = data.begin(); pos != data.end(); ++pos)
    device->write(reinterpret_cast<const char*>(pos->data()), sizeof(data_type) * location.dimensions.columns);
}

void ZFile::write(QIODevice* device)
{
  if(!device->isOpen())
    device->open(QIODevice::WriteOnly);
  Q_ASSERT(device->isOpen());

  int64_t sector = 0;

  write_vector_loc(device, sector, tiles.patterns, m_header->tiles.patterns);
  write_vector_loc(device, sector, tiles.palettes, m_header->tiles.palettes);
  write_vector_loc(device, sector, tiles.indexes , m_header->tiles.indexes );
  write_vector_loc(device, sector, tiles.masks   , m_header->tiles.masks   );
  write_vector_loc(device, sector, tiles.macros  , m_header->tiles.macros  );

  write_vector_loc(device, sector, sprites.patterns, m_header->sprites.patterns);
  write_vector_loc(device, sector, sprites.palettes, m_header->sprites.palettes);
  write_vector_loc(device, sector, sprites.indexes , m_header->sprites.indexes );
  write_vector_loc(device, sector, sprites.masks   , m_header->sprites.masks   );
  write_vector_loc(device, sector, sprites.macros  , m_header->sprites.macros  );

  write_matrix_loc(device, sector, maps.indexes, m_header->maps.indexes);
  write_matrix_loc(device, sector, maps.masks  , m_header->maps.masks  );
  write_matrix_loc(device, sector, maps.tiles  , m_header->maps.tiles  );
  write_matrix_loc(device, sector, maps.macros , m_header->maps.macros );

  write_vector_loc(device, sector, warp_points, m_header->warp_points);

  device->seek(0);
  device->write(m_header_data.data(), sizeof(header_t));

  device->close();
}
