/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2013 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_GALAPIX_DATABASE_STATEMENTS_IMAGE_INFO_STORE_HPP
#define HEADER_GALAPIX_DATABASE_STATEMENTS_IMAGE_INFO_STORE_HPP

class ImageInfoStore
{
public:
  ImageInfoStore(SQLiteConnection& db) :
    m_db(db),
    m_stmt(db, "INSERT OR REPLACE INTO image (resource_id, width, height) SELECT ?1, ?2, ?3;")
  {}

  RowId operator()(const ImageInfo& image_info)
  {
    m_stmt.bind_int64(1, image_info.get_resource_id().get_id());
    m_stmt.bind_int(2, image_info.get_width());
    m_stmt.bind_int(3, image_info.get_height());
    m_stmt.execute();

    return {sqlite3_last_insert_rowid(m_db.get_db())};
  }

private:
  SQLiteConnection& m_db;
  SQLiteStatement   m_stmt;

private:
  ImageInfoStore(const ImageInfoStore&) = delete;
  ImageInfoStore& operator=(const ImageInfoStore&) = delete;
};

#endif

/* EOF */
