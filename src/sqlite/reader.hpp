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

#ifndef HEADER_SQLITE_READER_HPP
#define HEADER_SQLITE_READER_HPP

//#include <sqlite3.h>
//#include <string>
#include "connection.hpp"
#include "util/blob.hpp"

class SQLiteReader
{
private:
  SQLiteConnection* db;
  sqlite3_stmt*   stmt;
  
public:
  SQLiteReader(SQLiteConnection* db, sqlite3_stmt* stmt);
  ~SQLiteReader();

  bool next();

  bool        is_null(int column);
  int         get_type(int column);
  int         get_int(int column);
  int64_t     get_int64(int column);
  std::string get_text(int column);
  Blob        get_blob(int column);

  std::string get_column_name(int column);

private:
  //  SQLiteReader(const SQLiteReader&);
  //  SQLiteReader& operator=(const SQLiteReader&);
};

#endif

/* EOF */
