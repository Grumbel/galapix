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

#include "database/resource_database.hpp"

#include <iostream>

#include "database/entries/file_entry.hpp"
#include "database/database.hpp"
#include "util/software_surface.hpp"
#include "util/software_surface_factory.hpp"
#include "util/filesystem.hpp"
#include "util/log.hpp"

ResourceDatabase::ResourceDatabase(SQLiteConnection& db) :
  m_db(db),

  m_blob_table(m_db),
  m_file_table(m_db),
  m_image_table(m_db),
  m_archive_table(m_db),
  m_video_table(m_db),

  m_file_entry_get_all(m_db),
  m_file_entry_get_by_fileid(m_db),
  m_file_entry_get_by_pattern(m_db),
  m_file_entry_get_by_url(m_db),
  m_file_entry_store(m_db),
  m_file_entry_delete(m_db),
  m_image_entry_store(m_db),
  m_image_entry_get(m_db),
  m_resource_entry_get_by_blob_id(m_db)
{
}

ResourceDatabase::~ResourceDatabase()
{
}
 
FileEntry
ResourceDatabase::store_file_entry(const URL& url, int size, int mtime, FileEntry::Handler handler)
{
  RowId file_id = m_file_entry_store(url, SHA1(), size, mtime, handler);
  return FileEntry(file_id, url, size, mtime, handler);
}

FileEntry
ResourceDatabase::store_file_entry(const URL& url, const SHA1& sha1, int size, int mtime, FileEntry::Handler handler, const RowId& archive_id)
{
  RowId file_id = m_file_entry_store(url, sha1, size, mtime, handler);
  return FileEntry(file_id, url, size, mtime, handler);
}

ImageEntry
ResourceDatabase::store_image_entry(const ImageEntry& image)
{
  m_image_entry_store(image);
  return image;
}

bool
ResourceDatabase::get_image_entry(const FileEntry& entry, ImageEntry& image_out)
{
  return m_image_entry_get(entry.get_blob_entry().get_id(), image_out);
}

bool
ResourceDatabase::get_file_entry(const URL& url, FileEntry& entry_out)
{
  return m_file_entry_get_by_url(url, entry_out);
}

void
ResourceDatabase::get_file_entries(const std::string& pattern, std::vector<FileEntry>& entries_out)
{
  m_file_entry_get_by_pattern(pattern, entries_out);
}

void
ResourceDatabase::get_file_entries(std::vector<FileEntry>& entries_out)
{
  m_file_entry_get_all(entries_out);
}

boost::optional<ResourceEntry>
ResourceDatabase::get_resource_entry(const RowId& blob_id)
{
  //m_resource_entry_get_by_blob_id();
  return boost::optional<ResourceEntry>();
}

void
ResourceDatabase::delete_file_entry(const RowId& file_id)
{
  m_file_entry_delete(file_id);
}

/* EOF */
