/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
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

#include "database/database.hpp"

#include "database/file_tile_database.hpp"
#include "database/sqlite_tile_database.hpp"
#include "database/cached_tile_database.hpp"
#include "util/filesystem.hpp"

Database::Database(const std::string& prefix) :
  m_db(),
  m_tile_db(),
  m_resources(),
  m_tiles()
{
  Filesystem::mkdir(prefix);

  m_db = std::make_unique<SQLiteConnection>(prefix + "/cache4.sqlite3");
  m_tile_db = std::make_unique<SQLiteConnection>(prefix + "/cache4_tiles.sqlite3");

  m_resources = std::make_unique<ResourceDatabase>(*m_db);

  if ((true))
  {
    m_tiles = std::make_unique<CachedTileDatabase>(std::make_unique<SQLiteTileDatabase>(*m_tile_db, *m_resources));
  }
  else
  {
    m_tiles = std::make_unique<CachedTileDatabase>(std::make_unique<FileTileDatabase>(prefix + "/tiles"));
  }
}

Database::~Database()
{
}

void
Database::delete_file_entry(const RowId& fileid)
{
  std::cout << "Begin Delete" << std::endl;
  m_db->exec("BEGIN;");
  m_tiles->delete_tiles(fileid);
  m_resources->delete_old_file_entry(fileid);
  m_db->exec("END;");
  std::cout << "End Delete" << std::endl;
}

void
Database::cleanup()
{
  m_db->vacuum();
}

/* EOF */
