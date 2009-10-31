/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmx.de>
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

#include "jobs/file_entry_generation_job.hpp"

#include <boost/bind.hpp>

#include "jobs/tile_generator.hpp"
#include "plugins/jpeg.hpp"
#include "util/filesystem.hpp"
#include "util/log.hpp"
#include "util/software_surface_factory.hpp"

FileEntryGenerationJob::FileEntryGenerationJob(const JobHandle& job_handle, const URL& url) :
  Job(job_handle),
  m_url(url),
  m_sig_file_callback(),
  m_sig_tile_callback()
{
}

void
FileEntryGenerationJob::run()
{
  try 
  {
    SoftwareSurfacePtr surface;
    Size size;
    int min_scale;
    int max_scale;
    FileEntry file_entry;

    if (m_url.has_stdio_name() && JPEG::filename_is_jpeg(m_url.get_stdio_name()))
    {
      size = JPEG::get_size(m_url.get_stdio_name());

      // FIXME: On http:// transfer mtime and size must be got from the transfer itself, not afterwards
      file_entry = FileEntry::create_without_fileid(m_url, m_url.get_size(), m_url.get_mtime(), 
                                                    size.width, size.height);

      // FIXME: here we are just guessing which tiles might be useful,
      // there might be a better way to pick \a min_scale
      min_scale = std::max(0, file_entry.get_thumbnail_scale() - 3);
      max_scale = file_entry.get_thumbnail_scale();

      // 2^3 is the highest scale JPEG supports, so we limit the
      // min_scale to that
      min_scale = Math::min(min_scale, 3);

      // FIXME: recalc min_scale from jpeg scale
      surface = JPEG::load_from_file(m_url.get_stdio_name(), Math::pow2(min_scale));
    }
    else
    {
      // FIXME: On http:// transfer mtime and size must be got from the transfer itself, not afterwards
      file_entry = FileEntry::create_without_fileid(m_url, m_url.get_size(), m_url.get_mtime(), 
                                                    size.width, size.height);

      surface = SoftwareSurfaceFactory::from_url(m_url);
      size = surface->get_size();
      min_scale = 0;
      max_scale = file_entry.get_thumbnail_scale();
    }

    m_sig_file_callback(file_entry);
    
    TileGenerator::cut_into_tiles(surface, size, min_scale, max_scale, 
                                  boost::bind(&FileEntryGenerationJob::process_tile, this, file_entry, _1));
  }
  catch(const std::exception& err)
  {
    log_error << "Error while processing " << m_url << std::endl;
    log_error << "  Exception: " << err.what() << std::endl;
  }
}

void
FileEntryGenerationJob::process_tile(const FileEntry& file_entry, const Tile& tile)
{
  m_sig_tile_callback(file_entry, tile);
}

/* EOF */
