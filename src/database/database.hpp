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

#ifndef HEADER_GALAPIX_DATABASE_DATABASE_HPP
#define HEADER_GALAPIX_DATABASE_DATABASE_HPP

#include <memory>

#include "database/memory_tile_database.hpp"
#include "database/resource_database.hpp"
#include "database/tile_database_interface.hpp"

class Database
{
public:
  static Database create(const std::string& prefix);

public:
  Database(std::unique_ptr<SQLiteConnection>&& db,
           std::unique_ptr<SQLiteConnection>&& tile_db,
           std::unique_ptr<ResourceDatabase>&& resources,
           std::unique_ptr<TileDatabaseInterface>&& tiles);
  Database(Database&&) = default;
  ~Database();

  ResourceDatabase& get_resources() { return *m_resources; }
  TileDatabaseInterface& get_tiles() { return *m_tiles; }

  void delete_file_entry(const RowId& fileid);

  void cleanup();

private:
  std::unique_ptr<SQLiteConnection> m_db;
  std::unique_ptr<SQLiteConnection> m_tile_db;
  std::unique_ptr<ResourceDatabase> m_resources;
  std::unique_ptr<TileDatabaseInterface> m_tiles;

private:
  Database(const Database&) = delete;
  Database& operator=(const Database&) = delete;
};

#endif

/* EOF */
