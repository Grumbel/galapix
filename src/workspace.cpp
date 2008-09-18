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

#include "math/rect.hpp"
#include "file_entry.hpp"
#include "math.hpp"
#include "framebuffer.hpp"
#include "workspace.hpp"

Workspace::Workspace()
{
  next_pos = Vector2i(0, 0);
  row_width = 20;
  progress = 0.0f;
}

std::vector<Image>
Workspace::get_images(const Rectf& rect)
{
  std::vector<Image> result;
  for(Images::iterator i = images.begin(); i != images.end(); ++i)
    {
      if (rect.contains(i->get_image_rect()))
        {
          result.push_back(*i);
        }
    }  
  return result;
}

Image
Workspace::get_image(const Vector2f& pos)
{
  for(Images::reverse_iterator i = images.rbegin(); i != images.rend(); ++i)
    {
      if (i->overlaps(pos))
        return *i;
    }  
  return Image();
}

void
Workspace::add_image(const FileEntry& file_entry)
{
  Image image(file_entry);
  images.push_back(image);
  image.set_scale(Math::min(1000.0f / file_entry.get_width(),
                            1000.0f / file_entry.get_height()));

  image.set_pos(next_pos * 1024.0f);

  // FIXME: Ugly, instead we should relayout once a second or so
  next_pos.x += 1;
  if (next_pos.x >= row_width)
    {
      next_pos.x  = 0;
      next_pos.y += 1;
    }
}

void
Workspace::layout(float aspect_w, float aspect_h)
{
  if (!images.empty())
    {     
      int w = int(Math::sqrt(aspect_w * images.size() / aspect_h));
      
      row_width = w;

      for(int i = 0; i < int(images.size()); ++i)
        {
          float target_scale = Math::min(1000.0f / images[i].get_original_width(),
                                         1000.0f / images[i].get_original_height());

          images[i].set_target_scale(target_scale);

          if ((i/w) % 2 == 0)
            {
              images[i].set_target_pos(Vector2f((i % w) * 1024.0f,
                                                (i / w) * 1024.0f));
            }
          else
            {
              images[i].set_target_pos(Vector2f((w - (i % w)-1) * 1024.0f,
                                                (i / w)         * 1024.0f));
            }

          next_pos = Vector2i(i % w, i / w);
        }
    }

  progress = 0.0f;
}

void
Workspace::tight_layout()
{
  float    width = Math::sqrt(images.size()) * 1024.0f;;
  Vector2f pos(0.0f, 0.0f);
  Vector2f last_pos(0.0f, 0.0f);
  bool go_right = true;
  
  float spacing = 24.0f;

  for(Images::iterator i = images.begin(); i != images.end(); ++i)
    {
      Image& image = *i;

      const float scale = 1000.0f / image.get_original_height();
      image.set_target_scale(scale);

      if (go_right)
        {
          if (pos.x + (image.get_original_width()*scale) > width)
            {
              pos.x = last_pos.x;
              pos.y += 1000.0f + spacing;   
              
              go_right = false;

              last_pos = pos;
              image.set_target_pos(pos + Vector2f(image.get_original_width(),
                                                  image.get_original_height()) * scale / 2.0f);
            }
          else
            {
              last_pos = pos;
              image.set_target_pos(pos + Vector2f(image.get_original_width(),
                                                  image.get_original_height()) * scale / 2.0f);
            
              pos.x += image.get_original_width()*scale + spacing;
            }
        }
      else
        {
          if (pos.x - (image.get_original_width()*scale) < 0)
            {
              //pos.x = 0;
              pos.y += 1000.0f + spacing;   
              go_right = true;

              last_pos = pos;
              image.set_target_pos(pos + Vector2f(image.get_original_width(),
                                                  image.get_original_height()) * scale / 2.0f);

              pos.x += image.get_original_width()*scale + spacing;
            }
          else
            {
              pos.x -= image.get_original_width()*scale + spacing;

              last_pos = pos;
              image.set_target_pos(pos + Vector2f(image.get_original_width(),
                                                  image.get_original_height()) * scale / 2.0f);
            }
        }
    }

  // Cause a smooth relayout
  progress = 0.0f;
}

void
Workspace::random_layout()
{
  int width = static_cast<int>(Math::sqrt(float(images.size())) * 1500.0f);
  for(Images::iterator i = images.begin(); i != images.end(); ++i)
    {
      i->set_target_pos(Vector2f(rand()%width, rand()%width));
      i->set_target_scale((rand()%1000) / 1000.0f + 0.25f); // FIXME: Make this relative to image size
    }
  progress = 0.0f;
}

void
Workspace::draw(const Rectf& cliprect, float scale)
{
  //std::cout << Math::clamp(1, static_cast<int>(1.0f / scale), 32) << " -> " << scale << std::endl;

  for(Images::iterator i = images.begin(); i != images.end(); ++i)
    {
      i->draw(cliprect, scale);
    }  

  for(Selection::iterator i = selection.begin(); i != selection.end(); ++i)
    {
      i->draw_mark();
    }
}

void
Workspace::update(float delta)
{
  if (progress != 1.0f)
    {
      progress += delta * 2.0f;

      if (progress > 1.0f)
        progress = 1.0f;

      for(Images::iterator i = images.begin(); i != images.end(); ++i)
        {
          i->update_pos(progress);
        }
    }
}

struct ImageSorter
{
  bool operator()(const Image& lhs, const Image& rhs)
  {
    return lhs.get_url() < rhs.get_url();
  }
};

void
Workspace::sort()
{
  std::sort(images.begin(), images.end(), ImageSorter());
}

void
Workspace::random_shuffle()
{
  std::random_shuffle(images.begin(), images.end());
}

void
Workspace::clear_cache()
{
  for(Images::iterator i = images.begin(); i != images.end(); ++i)
    {
      i->clear_cache();
    }  
}

void
Workspace::cache_cleanup()
{
  for(Images::iterator i = images.begin(); i != images.end(); ++i)
    {
      i->cache_cleanup();
    }   
}

void
Workspace::print_info()
{
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "Workspace Info:" << std::endl;
  std::cout << "  Number of Images: " << images.size() << std::endl;
  for(Images::iterator i = images.begin(); i != images.end(); ++i)
    {
      i->print_info();
    }
  std::cout << "-------------------------------------------------------" << std::endl;
}

void
Workspace::print_images(const Rectf& rect)
{
  std::cout << "-- Visible images --------------------------------------" << std::endl;
  for(Images::iterator i = images.begin(); i != images.end(); ++i)
    {
      if (i->overlaps(rect))
        {
          std::cout << i->get_url() << " "
                    << i->get_original_width() << "x" << i->get_original_height()
                    << std::endl;
        }
    }
  std::cout << "--------------------------------------------------------" << std::endl;
}

void
Workspace::set_row_width(int w)
{
  row_width = w;
}

void
Workspace::select_images(const std::vector<Image>& lst)
{
  selection.clear();
  selection.add_images(lst);
}

bool
Workspace::selection_clicked(const Vector2f& pos)
{
  for(Selection::iterator i = selection.begin(); i != selection.end(); ++i)
    {
      if (i->overlaps(pos))
        return true;
    }
  return false;
}

void
Workspace::clear_selection()
{
  selection.clear();
}

void
Workspace::move_selection(const Vector2f& rel)
{
  for(Selection::iterator i = selection.begin(); i != selection.end(); ++i)
    {
      i->set_pos(i->get_pos() + rel);
    }
}

void
Workspace::isolate_selection()
{
  images = selection.get_images();
  selection.clear();
}

struct ImagesMemberOf
{
  Selection selection;

  ImagesMemberOf(const Selection& selection)
    : selection(selection)
  {}

  bool operator()(const Image& image)
  {
    for(Selection::iterator i = selection.begin(); i != selection.end(); ++i)
      {
        if (*i == image)
          return true;
      }
    return false;
  }
};

void
Workspace::delete_selection()
{
  images.erase(std::remove_if(images.begin(), images.end(), ImagesMemberOf(selection)),
               images.end());
  selection.clear();
}

void
Workspace::solve_overlaps()
{
  // FIXME: This function is extremely slow and doesn't work
  // dynamically (no animation on position setting)
  int num_overlappings = 1;

  while(num_overlappings)
    {
      num_overlappings = 0;
      // Use QuadTree to make this fast
      for(std::vector<Image>::iterator i = images.begin(); i != images.end(); ++i)
        {
          for(std::vector<Image>::iterator j = i+1; j != images.end(); ++j)
            {
              Rectf irect = i->get_image_rect();
              Rectf jrect = j->get_image_rect();

              if (irect.is_overlapped(jrect))
                {
                  num_overlappings += 1;
                  
                  Rectf clip = irect.clip_to(jrect);

                  // FIXME: This only works if one rect isn't completly within the other
                  if (clip.get_width() > clip.get_height())
                    {
                      i->set_pos(i->get_pos() - Vector2f(0.0f, clip.get_height()/2 + 16.0f));
                      j->set_pos(j->get_pos() + Vector2f(0.0f, clip.get_height()/2 + 16.0f));
                    }
                  else
                    {
                      i->set_pos(i->get_pos() - Vector2f(clip.get_width()/2 + 16.0f, 0.0f));
                      j->set_pos(j->get_pos() + Vector2f(clip.get_width()/2 + 16.0f, 0.0f));
                    }
                }
            }          
        }
      std::cout << "NumOverlappings: " << num_overlappings << std::endl; 
    }
}

void
Workspace::save(const std::string& /*filename*/)
{

}

/* EOF */
