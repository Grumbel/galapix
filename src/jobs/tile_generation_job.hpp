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

#ifndef HEADER_TILE_GENERATION_JOB_HPP
#define HEADER_TILE_GENERATION_JOB_HPP

#include <boost/function.hpp>
#include "../job.hpp"

class TileEntry;

class TileGenerationJob : public Job
{
private: 
  FileEntry entry;
  int       min_scale;
  int       max_scale;
  
  boost::function<void (TileEntry)> callback;

public:
  TileGenerationJob(const FileEntry& file_entry, int min_scale, int max_scale,
                    const boost::function<void (TileEntry)>& callback);
  void run();

private:
  TileGenerationJob (const TileGenerationJob&);
  TileGenerationJob& operator= (const TileGenerationJob&);
};

#endif

/* EOF */
