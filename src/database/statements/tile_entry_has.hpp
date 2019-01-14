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

#ifndef HEADER_GALAPIX_DATABASE_STATEMENTS_TILE_ENTRY_HAS_HPP
#define HEADER_GALAPIX_DATABASE_STATEMENTS_TILE_ENTRY_HAS_HPP

class TileEntryHas final
{
public:
  TileEntryHas(SQLiteConnection& db) :
    m_stmt(db, "SELECT (rowid) FROM tile WHERE image_id = ?1 AND scale = ?2 AND x = ?3 AND y = ?4;")
  {}

  bool operator()(const RowId& fileid, const Vector2i& pos, int scale)
  {
    if (!fileid)
    {
      return false;
    }
    else
    {
      m_stmt.bind_int64(1, fileid.get_id());
      m_stmt.bind_int(2, scale);
      m_stmt.bind_int(3, pos.x);
      m_stmt.bind_int(4, pos.y);

      SQLiteReader reader = m_stmt.execute_query();

      if (reader.next())
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }

private:
  SQLiteStatement m_stmt;

private:
  TileEntryHas(const TileEntryHas&);
  TileEntryHas& operator=(const TileEntryHas&);
};

#endif

/* EOF */
