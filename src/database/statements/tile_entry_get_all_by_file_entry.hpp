// Galapix - an image viewer for large image collections
// Copyright (C) 2008-2019 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_GALAPIX_DATABASE_STATEMENTS_TILE_ENTRY_GET_ALL_BY_FILE_ENTRY_HPP
#define HEADER_GALAPIX_DATABASE_STATEMENTS_TILE_ENTRY_GET_ALL_BY_FILE_ENTRY_HPP

#include <assert.h>

#include "database/entries/old_file_entry.hpp"
#include "database/entries/tile_entry.hpp"
#include "surface/software_surface_factory.hpp"
#include "plugins/png.hpp"
#include "plugins/jpeg.hpp"

class TileEntryGetAllByFileEntry final
{
public:
  TileEntryGetAllByFileEntry(SQLiteConnection& db) :
    m_stmt(db, "SELECT * FROM tile WHERE image_id = ?1;")
  {}

  void operator()(const RowId& fileid, std::vector<TileEntry>& tiles)
  {
    if (fileid)
    {
      m_stmt.bind_int64(1, fileid.get_id());

      SQLiteReader reader = m_stmt.execute_query();
      while(reader.next())
      {
        TileEntry tile(fileid,
                       reader.get_int(1), // scale
                       Vector2i(reader.get_int (2),  // x
                                reader.get_int (3)), // y
                       reader.get_blob(4),
                       static_cast<TileEntry::Format>(reader.get_int(6)));

        // FIXME: TileEntry shouldn't contain a SoftwareSurface, but a
        // Blob, so we don't do encode/decode when doing a database
        // merge
        Blob blob = tile.get_blob();
        switch(tile.get_format())
        {
          case TileEntry::JPEG_FORMAT:
            tile.set_surface(surf::JPEG::load_from_mem(blob));
            break;

          case TileEntry::PNG_FORMAT:
            tile.set_surface(surf::PNG::load_from_mem(blob));
            break;

          default:
            assert(false && "never reached");
        }

        tiles.push_back(tile);
      }
    }
  }

private:
  SQLiteStatement m_stmt;

private:
  TileEntryGetAllByFileEntry(const TileEntryGetAllByFileEntry&);
  TileEntryGetAllByFileEntry& operator=(const TileEntryGetAllByFileEntry&);
};

#endif

/* EOF */
