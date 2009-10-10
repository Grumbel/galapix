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

#include "database/file_database.hpp"

#include <iostream>

#include "database/file_entry.hpp"
#include "database/database.hpp"
#include "util/software_surface.hpp"
#include "util/software_surface_factory.hpp"
#include "util/filesystem.hpp"

FileDatabase::FileDatabase(Database& db) :
  m_db(db),
  m_file_table(m_db.get_db()),
  m_file_entry_get_all(m_db.get_db()),
  m_file_entry_get_by_fileid(m_db.get_db()),
  m_file_entry_get_by_pattern(m_db.get_db()),
  m_file_entry_get_by_url(m_db.get_db()),
  m_file_entry_store(m_db.get_db())
{
}

FileEntry
FileDatabase::store_file_entry(const FileEntry& entry)
{
  return store_file_entry(entry.get_url(), entry.get_image_size());
}
 
FileEntry
FileDatabase::store_file_entry(const URL& url, const Size& size)
{
  return store_file_entry_without_cache(url, size);
}

FileEntry
FileDatabase::store_file_entry_without_cache(const URL& url, const Size& size)
{
  return m_file_entry_store(url, size);
}

FileEntry
FileDatabase::get_file_entry(const URL& url)
{
  return m_file_entry_get_by_url(url);
}

void
FileDatabase::get_file_entries(const std::string& pattern, std::vector<FileEntry>& entries_out)
{
  m_file_entry_get_by_pattern(pattern, entries_out);
}

void
FileDatabase::get_file_entries(std::vector<FileEntry>& entries_out)
{
  m_file_entry_get_all(entries_out);
}

void
FileDatabase::update_file_entry(FileEntry& entry)
{
  // UPDATE files SET mtime = ?entry.get_mtime() WHERE fileid = ?entry.fileid
}

void
FileDatabase::check()
{
  std::vector<FileEntry> entries;
  get_file_entries(entries);

  std::cout << "Checking File Existance:" << std::endl;
  for(std::vector<FileEntry>::iterator i = entries.begin(); i != entries.end(); ++i)
    {
      if (!Filesystem::exist(i->get_url().get_stdio_name()))
        {
          std::cout << i->get_url() << ": does not exist" << std::endl;
        }
      else
        {
          std::cout << i->get_url() << ": ok" << std::endl;
        }
    }
}

/* EOF */
