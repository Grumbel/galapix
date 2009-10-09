/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "database/tile_database.hpp"

#include <iostream>

#include "database/tile_entry.hpp"
#include "database/file_entry.hpp"
#include "database/database.hpp"
#include "plugins/jpeg.hpp"
#include "plugins/png.hpp"
#include "util/software_surface_factory.hpp"

TileDatabase::TileDatabase(Database& db)
  : m_db(db),
    m_tiles_table(m_db.get_db()),
    m_tile_entry_store(m_db.get_db()),
    m_tile_entry_get_all_by_file_entry(m_db.get_db()),
    m_tile_entry_has(m_db.get_db()),
    m_tile_entry_get_by_file_entry(m_db.get_db()),
    tile_cache()
{}

TileDatabase::~TileDatabase()
{
  flush_cache();
}

bool
TileDatabase::has_tile(const FileEntry& file_entry, const Vector2i& pos, int scale)
{
  if (m_tile_entry_has(file_entry, pos, scale))
  {
    return true;
  }
  else
  {
    // Check cache
    for(std::vector<TileEntry>::iterator i = tile_cache.begin(); i != tile_cache.end(); ++i)
    {
      if (i->get_fileid() == file_entry.get_fileid() &&
          i->get_scale()  == scale  &&
          i->get_pos()    == pos)
      {
        return true;
      }
    }

    return false;
  }
}

void
TileDatabase::get_tiles(const FileEntry& file_entry, std::vector<TileEntry>& tiles_out)
{
  m_tile_entry_get_all_by_file_entry(file_entry, tiles_out);

  // Check cache
  for(std::vector<TileEntry>::iterator i = tile_cache.begin(); i != tile_cache.end(); ++i)
  {
    if (i->get_fileid() == file_entry.get_fileid())
    {
      tiles_out.push_back(*i);
    }
  }
}

bool
TileDatabase::get_tile(const FileEntry& file_entry, int scale, const Vector2i& pos, TileEntry& tile)
{
  if (m_tile_entry_get_by_file_entry(file_entry, scale, pos, tile))
  {
    return true;
  }
  else
  {
    // Check cache
    for(std::vector<TileEntry>::iterator i = tile_cache.begin(); i != tile_cache.end(); ++i)
    {
      if (i->get_fileid() == file_entry.get_fileid() &&
          i->get_scale()  == scale  &&
          i->get_pos()    == pos)
      {
        tile = *i;
        return true;
      }
    }

    // Tile missing
    return false;
  }
}

void
TileDatabase::store_tile_in_cache(const TileEntry& tile)
{
  tile_cache.push_back(tile);

  // A single tile is ~10KB
  if (tile_cache.size() > 256)
    flush_cache();
}

void
TileDatabase::store_tiles(const std::vector<TileEntry>& tiles)
{
  m_db.get_db().exec("BEGIN;");
  for(std::vector<TileEntry>::const_iterator i = tiles.begin(); i != tiles.end(); ++i)
    {
      store_tile(*i);
    }
  m_db.get_db().exec("COMMIT;");
}

void
TileDatabase::store_tile(const TileEntry& tile)
{
  m_tile_entry_store(tile);
}

void
TileDatabase::check()
{
#if 0
  SQLiteReader reader = get_all_stmt.execute_query();

  while(reader.next())
    {
      /*
        int64_t fileid = reader.get_int64(0);
        int scale  = reader.get_int(1);
        int x      = reader.get_int(2);
        int y      = reader.get_int(3);
        BlobHandle blob  = reader.get_blob(4);
      */
    }
#endif
}

void
TileDatabase::flush_cache()
{
  std::cout << "Flushing TileCache" << std::endl;

  if (!tile_cache.empty())
    {
      store_tiles(tile_cache);
      tile_cache.clear();
    }
}

/* EOF */
