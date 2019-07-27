/*
 * Copyright (C) 2018-2019 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zrythm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <math.h>

#include "audio/bus_track.h"
#include "audio/channel.h"
#include "audio/instrument_track.h"
#include "audio/midi_note.h"
#include "audio/track.h"
#include "gui/widgets/arranger.h"
#include "gui/widgets/center_dock.h"
#include "gui/widgets/main_window.h"
#include "gui/widgets/midi_arranger.h"
#include "gui/widgets/automation_region.h"
#include "gui/widgets/region.h"
#include "gui/widgets/ruler.h"
#include "gui/widgets/timeline_arranger.h"
#include "utils/cairo.h"

G_DEFINE_TYPE (AutomationRegionWidget,
               automation_region_widget,
               REGION_WIDGET_TYPE)


#define Y_PADDING 6.f
#define Y_HALF_PADDING 3.f

static gboolean
automation_region_draw_cb (
  GtkWidget * widget,
  cairo_t *cr,
  AutomationRegionWidget * self)
{
  int i, j;
  REGION_WIDGET_GET_PRIVATE (self);
  guint width, height;
  GtkStyleContext *context;

  context =
    gtk_widget_get_style_context (widget);

  width =
    gtk_widget_get_allocated_width (widget);
  height =
    gtk_widget_get_allocated_height (widget);

  gtk_render_background (
    context, cr, 0, 0, width, height);

  cairo_set_source_rgba (
    cr, 1, 1, 1, 1);

  Region * r = rw_prv->region;
  Region * main_region =
    region_get_main_region (r);
  int num_loops =
    region_get_num_loops (r, 1);
  long ticks_in_region =
    region_get_full_length_in_ticks (r);
  double x_start, y_start, x_end, y_end;

  /* draw automation */
  long loop_end_ticks =
    position_to_ticks (&r->loop_end_pos);
  long loop_ticks =
    region_get_loop_length_in_ticks (r);
  long clip_start_ticks =
    position_to_ticks (&r->clip_start_pos);
  AutomationPoint * ap, * next_ap;
  AutomationCurve * ac;
  for (i = 0; i < main_region->num_aps; i++)
    {
      ap = main_region->aps[i];
      if (i == main_region->num_aps - 1)
        {
          ac = NULL;
          next_ap = NULL;
        }
      else
        {
          ac = main_region->acs[i];
          next_ap = main_region->aps[i + 1];
        }

      ap =
        (AutomationPoint *)
        arranger_object_info_get_visible_counterpart (
          &ap->obj_info);

      /* get ratio (0.0 - 1.0) on x where midi note starts
       * & ends */
      int ap_start_ticks =
        position_to_ticks (&ap->pos);
      int ap_end_ticks = ap_start_ticks;
      if (next_ap)
        ap_end_ticks =
          position_to_ticks (&next_ap->pos);
      int tmp_start_ticks, tmp_end_ticks;

      /* if before loop end */
      if (position_compare (
            &ap->pos, &r->loop_end_pos) < 0)
        {
          for (j = 0; j < num_loops; j++)
            {
              /* if ap started before loop start
               * only draw it once */
              if (position_is_before (
                    &ap->pos,
                    &r->loop_start_pos) &&
                  j != 0)
                break;

              /* calculate draw endpoints */
              tmp_start_ticks =
                ap_start_ticks + loop_ticks * j;

              /* if should be clipped */
              if (next_ap &&
                  position_is_after_or_equal (
                    &next_ap->pos,
                    &r->loop_end_pos))
                tmp_end_ticks =
                  loop_end_ticks + loop_ticks * j;
              else
                tmp_end_ticks =
                  ap_end_ticks + loop_ticks * j;

              /* adjust for clip start */
              tmp_start_ticks -= clip_start_ticks;
              tmp_end_ticks -= clip_start_ticks;

              x_start =
                (float) tmp_start_ticks /
                ticks_in_region;
              x_end =
                (float) tmp_end_ticks /
                ticks_in_region;

              /* get ratio (0.0 - 1.0) on y where
               * midi note is */
              y_start =
                1.0 -
                automation_point_get_normalized_value (
                  ap);
              if (next_ap)
                y_end =
                  1.0 -
                  automation_point_get_normalized_value (
                    next_ap);
              else
                y_end = y_start;

              double x_start_real =
                x_start * width;
              /*double x_end_real =*/
                /*x_end * width;*/
              double y_start_real =
                y_start * height;
              double y_end_real =
                y_end * height;

              /* draw ap */
              int padding = 1;
              cairo_rectangle (
                cr,
                x_start_real - padding,
                y_start_real - padding,
                2 * padding,
                2 * padding);
              cairo_fill (cr);

              /* draw ac */
              if (ac)
                {
                  double new_x, ap_y, new_y;
                  double ac_height =
                    fabs (y_end - y_start);
                  ac_height *= height;
                  double ac_width =
                    fabs (x_end - x_start);
                  ac_width *= width;
                  /*g_message ("ac height %f",*/
                             /*ac_height);*/
                  /*g_message ("ac width %f",*/
                             /*ac_width);*/
                  /*g_message ("y start real %f",*/
                             /*y_start_real);*/
                  for (double j = x_start_real;
                       j < (x_start_real) + ac_width;
                       j += 0.6)
                    {
                      /* in pixels, higher values are lower */
                      ap_y =
                        1.0 -
                        automation_curve_get_normalized_value (
                          ac,
                          (j - x_start_real) /
                            ac_width);
                      ap_y *= ac_height;

                      new_x = j;
                      if (y_start > y_end)
                        new_y = ap_y + y_end_real;
                      else
                        new_y = ap_y + y_start_real;

                      if (j == 0.0)
                        {
                          cairo_move_to (
                            cr, new_x, new_y);
                        }

                      cairo_line_to (
                        cr, new_x, new_y);
                    }
                  cairo_stroke (cr);
                }
            }
        }
    }

  region_widget_draw_name (
    Z_REGION_WIDGET (self), cr);

  return FALSE;
}

AutomationRegionWidget *
automation_region_widget_new (
  Region * region)
{
  AutomationRegionWidget * self =
    g_object_new (
      AUTOMATION_REGION_WIDGET_TYPE,
      NULL);

  region_widget_setup (
    Z_REGION_WIDGET (self), region);
  REGION_WIDGET_GET_PRIVATE (self);

  /* connect signals */
  g_signal_connect (
    G_OBJECT (rw_prv->drawing_area), "draw",
    G_CALLBACK (automation_region_draw_cb), self);

  return self;
}

static void
automation_region_widget_class_init (
  AutomationRegionWidgetClass * klass)
{
}

static void
automation_region_widget_init (
  AutomationRegionWidget * self)
{
}