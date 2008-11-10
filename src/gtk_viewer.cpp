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

#include <string.h>
#include <gtkmm.h>
#include <libglademm/xml.h>
#include <glade/glade.h>
#include <iostream>
#include <gtkglmm.h>
#include <gtkmm/gl/widget.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "gtk_viewer_widget.hpp"
#include "gtk_viewer.hpp"

GtkViewer::GtkViewer()
{
}

GtkViewer::~GtkViewer()
{
}

void
GtkViewer::run()
{
  int    argc = 1;
  char** argv;

  argv = (char**)malloc(sizeof(char*) * 2);

  argv[0] = strdup("galapix");
  argv[1] = NULL;

  Gtk::Main kit(&argc, &argv);
  Gtk::GL::init(&argc, &argv);

  Glib::RefPtr<Gnome::Glade::Xml> xml = Gnome::Glade::Xml::create("galapix-gtk.glade");

  // start the event loop; exit when the specified window is closed
  Gtk::Window& window = dynamic_cast<Gtk::Window&>(*xml->get_widget("MainWindow"));

  Gtk::ScrolledWindow& hbox = dynamic_cast<Gtk::ScrolledWindow&>(*xml->get_widget("scrolledwindow1"));

  GtkViewerWidget viewer_widget;
  hbox.add(viewer_widget);
  viewer_widget.show();
      
  Gtk::Main::run(window);

  free(argv[0]);
}

/* EOF */
