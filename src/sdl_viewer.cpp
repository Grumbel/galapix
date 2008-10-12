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

#include <boost/bind.hpp>
#include "../config.h"
#include "math/rgba.hpp"
#include "file_entry.hpp"
#include "workspace.hpp"
#include "sdl_framebuffer.hpp"
#include "viewer.hpp"
#include "sdl_viewer.hpp"
#include "tile_generator_thread.hpp"
#include "space_navigator.hpp"
#include "database_thread.hpp"

SDLViewer* SDLViewer::current_ = 0;

SDLViewer::SDLViewer(const Size& geometry, bool fullscreen, int  anti_aliasing)
  : geometry(geometry),
    fullscreen(fullscreen),
    anti_aliasing(anti_aliasing),
    quit(false)
{
  current_ = this;
}

SDLViewer::~SDLViewer()
{
}

void
SDLViewer::receive_file(const FileEntry& entry)
{
  file_queue.push(entry);
}

void
SDLViewer::process_event(const SDL_Event& event)
{
  //Uint8* keystates = SDL_GetKeyState(NULL);

  switch(event.type)
    {
      case SDL_QUIT:
        std::cout << "Viewer: SDL_QUIT received" << std::endl;
        quit = true;
        break;

      case SDL_VIDEOEXPOSE:
        break;

      case SDL_VIDEORESIZE:
        SDLFramebuffer::resize(event.resize.w, event.resize.h);
        break;

      default:
        viewer->process_event(event);
        break;
    }
}

int
SDLViewer::run()
{
  Workspace workspace;

  SDLFramebuffer::set_video_mode(geometry, fullscreen, anti_aliasing);

  workspace.layout_aspect(4,3);

  viewer = std::auto_ptr<Viewer>(new Viewer(&workspace));

  Uint32 ticks = SDL_GetTicks();

#ifdef HAVE_SPACE_NAVIGATOR
  SpaceNavigator space_navigator;
#endif

  while(!viewer->done())
    {     
      while (!file_queue.empty())
        {
          const FileEntry& entry = file_queue.front();
          workspace.add_image(entry);
          file_queue.pop();
        }

#ifdef HAVE_SPACE_NAVIGATOR
      space_navigator.poll(*viewer);
#endif

      SDL_Event event;
      while (SDL_PollEvent(&event))
        {
          process_event(event);
        }

      Uint32 cticks = SDL_GetTicks();
      float delta = (cticks - ticks) / 1000.0f;
      ticks = cticks;

      viewer->update(delta);
      viewer->draw();
      SDLFramebuffer::flip();

      SDL_Delay(30);
    }

  std::cout << "SDLViewer: done" << std::endl;

  return 0;
}

/* EOF */
