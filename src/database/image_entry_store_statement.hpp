/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2011 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_GALAPIX_DATABASE_IMAGE_ENTRY_STORE_STATEMENT_HPP
#define HEADER_GALAPIX_DATABASE_IMAGE_ENTRY_STORE_STATEMENT_HPP

#include <iostream>
#include <assert.h>

#include "database/image_entry.hpp"

class ImageEntryStoreStatement
{
private:
  SQLiteConnection& m_db;
  SQLiteStatement   m_stmt;

public:
  ImageEntryStoreStatement(SQLiteConnection& db) :
    m_db(db),
    m_stmt(db, "INSERT OR REPLACE INTO images (fileid, width, height, format) VALUES (?1, ?2, ?3, ?4);")
  {}
  
  void operator()(const ImageEntry& image)
  {
    m_stmt.bind_int64(1, image.get_fileid().get_id());
    m_stmt.bind_int(2, image.get_width());
    m_stmt.bind_int(3, image.get_height());
    m_stmt.bind_int(4, image.get_format());

    m_stmt.execute();
    
    //return sqlite3_last_insert_rowid(m_db.get_db());
  }

private:
  ImageEntryStoreStatement(const ImageEntryStoreStatement&);
  ImageEntryStoreStatement& operator=(const ImageEntryStoreStatement&);
};

#endif

/* EOF */
