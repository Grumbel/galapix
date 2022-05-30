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

#ifndef HEADER_GALAPIX_GALAPIX_WORKSPACE_DESC_HPP
#define HEADER_GALAPIX_GALAPIX_WORKSPACE_DESC_HPP

#include <string>

class ImageDesc
{
public:
  std::string filename;
  Vector2f    pos;
  float       scale;
};

class WorkspaceDesc
{
public:
  void add_image(std::string const& filename);

public:
  std::vector<ImageDesc> images;
};

#endif

/* EOF */
