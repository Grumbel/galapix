/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2013 Ingo Ruhnke <grumbel@gmx.de>
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

#include "blob_accessor.hpp"

#include <boost/filesystem.hpp>

BlobAccessor::BlobAccessor(const std::string& filename) :
  m_filename(filename),
  m_blob()
{
}

BlobAccessor::BlobAccessor(BlobPtr blob) :
  m_filename(),
  m_blob(blob)
{
}

bool
BlobAccessor::has_stdio_name() const
{
  return !m_filename.empty();
}

std::string
BlobAccessor::get_stdio_name()
{
  if (!m_filename.empty())
  {
    return m_filename;
  }
  else
  {
    boost::filesystem::path tmpfile = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path("%%%%-%%%%-%%%%-%%%%");
    m_filename = tmpfile.string();
    m_blob->write_to_file(m_filename);
    return m_filename;
  }
}

bool
BlobAccessor::has_blob() const
{
  return static_cast<bool>(m_blob);
}

BlobPtr
BlobAccessor::get_blob()
{
  if (m_blob)
  {
    return m_blob;
  }
  else
  {
    m_blob = Blob::from_file(m_filename);
    return m_blob;
  }
}

/* EOF */
