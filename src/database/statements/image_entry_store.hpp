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

#ifndef HEADER_GALAPIX_DATABASE_STATEMENTS_IMAGE_ENTRY_STORE_HPP
#define HEADER_GALAPIX_DATABASE_STATEMENTS_IMAGE_ENTRY_STORE_HPP

#include <iostream>
#include <assert.h>

#include "database/entries/image_entry.hpp"

class ImageEntryStore final
{
public:
  ImageEntryStore(SQLite::Database& db) :
    m_db(db),
    m_stmt(db, "INSERT OR REPLACE INTO image (resource_id, width, height) VALUES (?1, ?2, ?3);")
  {}

  void operator()(ImageEntry const& image)
  {
    m_stmt.bind(1, image.get_resource_id().get_id());
    m_stmt.bind(2, image.get_width());
    m_stmt.bind(3, image.get_height());

    m_stmt.exec();

    //return sqlite3_last_insert_rowid(m_db.get_db());
  }

private:
  SQLite::Database& m_db;
  SQLite::Statement   m_stmt;

private:
  ImageEntryStore(ImageEntryStore const&);
  ImageEntryStore& operator=(ImageEntryStore const&);
};

#endif

/* EOF */
