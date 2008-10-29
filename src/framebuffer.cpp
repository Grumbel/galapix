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

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <math.h>
#include "../config.h"
#include "math/rgb.hpp"
#include "math/rgba.hpp"
#include "math/rect.hpp"
#include "sdl_framebuffer.hpp"
#include "framebuffer.hpp"

#ifndef assert_gl
void assert_gl(const char* message)
{
  GLenum error = glGetError();
  if(error != GL_NO_ERROR) {
    std::ostringstream msg;
    msg << "OpenGLError while '" << message << "': "
        << gluErrorString(error);
    throw std::runtime_error(msg.str());
  }
}
#endif

void 
Framebuffer::init()
{
  // Init Glew 
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      std::ostringstream str;
      str << "Error: " << glewGetErrorString(err) << std::endl;
      throw std::runtime_error(str.str());
    }
  
  if (!GLEW_ARB_texture_rectangle)
    {
      throw std::runtime_error("OpenGL ARB_texture_rectangle extension not found, but required");
    }
}

void
Framebuffer::reshape(const Size& size)
{ 
  glViewport(0, 0, size.width, size.height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, size.width, size.height, 0.0, 1000.0, -1000.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // Magic Voodoo to get pixel perfect graphics, see:
  //   http://www.opengl.org/resources/faq/technical/transformations.htm
  // On Nvidia this seems to break things, instead of fixing them, so
  // we don't use it, since the results are already perfect even without it.
  // glTranslated(0.375, 0.375, 0.0);
}

void
Framebuffer::clear(const RGBA& rgba)
{
  glClearColor(rgba.r/255.0f, rgba.g/255.0, rgba.b/255.0f, rgba.a/255.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
Framebuffer::draw_rect(const Rectf& rect, const RGB& rgb)
{
  glDisable(GL_TEXTURE_RECTANGLE_ARB);
    
  glColor3ub(rgb.r, rgb.g, rgb.b);

  glBegin(GL_LINE_LOOP);
  glVertex2f(rect.left,  rect.top);
  glVertex2f(rect.right, rect.top);
  glVertex2f(rect.right, rect.bottom);
  glVertex2f(rect.left,  rect.bottom);
  glEnd();
}

void
Framebuffer::fill_rect(const Rectf& rect, const RGB& rgb)
{
  glDisable(GL_TEXTURE_RECTANGLE_ARB);

  glColor3ub(rgb.r, rgb.g, rgb.b);
  glBegin(GL_QUADS);
  glVertex2f(rect.left,  rect.top);
  glVertex2f(rect.right, rect.top);
  glVertex2f(rect.right, rect.bottom);
  glVertex2f(rect.left,  rect.bottom);
  glEnd();
}

void
Framebuffer::draw_grid(int num_cells)
{
  glDisable(GL_TEXTURE_RECTANGLE_ARB);
 
  glBegin(GL_LINES);
  //  if (grid_color)
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
  //else
  //  glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

    int cell_width = Framebuffer::get_width()/num_cells;
    for(int x = 1; x < num_cells; ++x)
      {
        glVertex2f(x*cell_width, 0);
        glVertex2f(x*cell_width, Framebuffer::get_height());
      }

    int cell_height = Framebuffer::get_height()/num_cells;
    for(int y = 1; y < num_cells; ++y)
      {
        glVertex2f(0, y*cell_height);
        glVertex2f(Framebuffer::get_width(), y*cell_height);
      }

  glEnd();
}

void
Framebuffer::draw_grid(const Vector2f& offset, const Sizef& size, const RGBA& rgba)
{
  glDisable(GL_TEXTURE_RECTANGLE_ARB);
 
  glBegin(GL_LINES);
  glColor4ub(rgba.r, rgba.g, rgba.b, rgba.a);

  float start_x = fmodf(offset.x, size.width);
  float start_y = fmodf(offset.y, size.height);

  for(float x = start_x; x < Framebuffer::get_width(); x += size.width)
    {
      glVertex2f(x, 0);
      glVertex2f(x, Framebuffer::get_height());
    }

  for(float y = start_y; y < Framebuffer::get_height(); y += size.height)
    {
      glVertex2f(0, y);
      glVertex2f(Framebuffer::get_width(), y);
    }

  glEnd();  
}

int
Framebuffer::get_width()
{
  return SDLFramebuffer::get_width();
}

int
Framebuffer::get_height()
{
  return SDLFramebuffer::get_height();
}

SoftwareSurface
Framebuffer::screenshot()
{
  SoftwareSurface surface(SoftwareSurface::RGB_FORMAT, get_size());
  glReadPixels(0, 0, surface.get_width(), surface.get_height(), GL_RGB, GL_UNSIGNED_BYTE, surface.get_data());
  return surface.vflip();
}

/* EOF */
