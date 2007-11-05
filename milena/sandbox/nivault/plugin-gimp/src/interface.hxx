/* GIMP Plug-in Template
 * Copyright (C) 2000-2004  Michael Natterer <mitch@gimp.org> (the "Author").
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Author of the
 * Software shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the Author.
 */

#include "config.h"

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "interface.hh"

#include "plugin-intl.hh"


/*  Constants  */

#define SCALE_WIDTH        180
#define SPIN_BUTTON_WIDTH   75
#define RANDOM_SEED_WIDTH  100

/*  Give access to drawing areas  */
static GtkWidget *drawing_area = NULL;

/*  Public functions  */

gboolean
dialog (gint32              image_ID,
	GimpDrawable       *drawable)
{
  GtkWidget *dlg;
  GtkWidget *main_vbox;
  GtkWidget *image_layout;
  GtkWidget *frame; 
//   GtkWidget *scroll; 
  
  GtkAdjustment *hadjust = NULL;
  GtkAdjustment *vadjust = NULL;
  GimpRGB color;

  gimp_ui_init (PLUGIN_NAME, TRUE);

  dlg = gimp_dialog_new (_("Milena toolbox !"), PLUGIN_NAME,
                         NULL, (GtkDialogFlags)0,
			 gimp_standard_help_func, "plug-in-milena", NULL);

  main_vbox = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 12);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dlg)->vbox), main_vbox);

  /*
  ** Image Layout.
  */

  frame = gimp_frame_new (_("Display box :"));
  gtk_box_pack_start (GTK_BOX (main_vbox), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);
  gimp_rgb_set(&color, 0, 0, 0);
  
//   scroll = gtk_scrolled_window_new((GtkAdjustment *)hadjust,
// 				   (GtkAdjustment *)vadjust);

//   gtk_container_set_border_width (GTK_CONTAINER (scroll), 10);
//   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
// 				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
//   gtk_widget_show (scroll);


  drawing_area = gtk_drawing_area_new();
  gtk_container_add (GTK_CONTAINER (frame), drawing_area);
  gtk_widget_set_size_request(drawing_area,
			      mln::geom::ncols(get_ima()),
			      mln::geom::nrows(get_ima()));
  g_signal_connect (G_OBJECT (drawing_area),
		    "expose_event",
		    G_CALLBACK (draw_milena_image),
		    G_OBJECT (NULL));
//   gtk_scrolled_window_add_with_viewport (
//     GTK_SCROLLED_WINDOW (scroll), drawing_area);
  gtk_widget_show (drawing_area);

  gtk_widget_show (main_vbox);
  gtk_widget_show (dlg);

  gimp_dialog_run (GIMP_DIALOG (dlg));
  gtk_widget_destroy (dlg);

  return TRUE;
}
