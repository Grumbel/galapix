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

#ifndef HEADER_GALAPIX_DATABASE_STATEMENTS_URL_INFO_STORE_HPP
#define HEADER_GALAPIX_DATABASE_STATEMENTS_URL_INFO_STORE_HPP

class URLInfoStore
{
public:
  URLInfoStore(SQLite::Database& db) :
    m_db(db),
    m_stmt(db, "INSERT OR REPLACE INTO url (url, mtime, content_type, blob_id) SELECT ?1, ?2, ?3, blob.id FROM blob WHERE blob.sha1 = ?4;")
  {}

  RowId operator()(URLInfo const& url_info)
  {
    m_stmt.bind(1, url_info.get_url());
    m_stmt.bind(2, url_info.get_mtime());
    m_stmt.bind(3, url_info.get_content_type());
    m_stmt.bind(4, url_info.get_blob_info().get_sha1().str());
    m_stmt.exec();

    return RowId(m_db.getLastInsertRowid());
  }

private:
  SQLite::Database& m_db;
  SQLite::Statement   m_stmt;

private:
  URLInfoStore(URLInfoStore const&);
  URLInfoStore& operator=(URLInfoStore const&);
};

#endif

/* EOF */
