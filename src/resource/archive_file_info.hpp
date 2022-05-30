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

#ifndef HEADER_GALAPIX_RESOURCE_ARCHIVE_FILE_INFO_HPP
#define HEADER_GALAPIX_RESOURCE_ARCHIVE_FILE_INFO_HPP

#include <string>

#include "database/row_id.hpp"
#include "resource/blob_info.hpp"

class ArchiveFileInfo
{
public:
  ArchiveFileInfo() :
    m_id(),
    m_path(),
    m_blob_info()
  {}

  ArchiveFileInfo(std::string const& path, BlobInfo const& blob_info) :
    m_id(),
    m_path(path),
    m_blob_info(blob_info)
  {}

  RowId get_id() const { return m_id; }
  std::string get_path() const { return m_path; }
  BlobInfo get_blob_info() const { return m_blob_info; }

private:
  RowId m_id;
  std::string m_path;
  BlobInfo m_blob_info;
};

#endif

/* EOF */
