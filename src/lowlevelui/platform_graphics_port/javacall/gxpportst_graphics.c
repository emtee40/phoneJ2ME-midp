/*
 *
 *
 * Copyright  1990-2008 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include <gxpport_graphics.h>
#include <gxpport_font.h>
#include <midp_logging.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <syslog.h>
#include "lfpport_gtk.h"

extern GtkWidget *main_canvas;
extern GtkWidget *main_window;

/**
 * @file
 *
 * Stubs of primitive graphics.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define     GXPPORT_GTK_FONT_ASCENT     10
#define     GXPPORT_GTK_FONT_DESCENT    2
#define     GXPPORT_GTK_FONT_LEADING    2
#define     GXPPORT_GTK_CHARS_WIDTH     4

// extern MidpError lfpport_get_font(PlatformFontPtr* fontPtr,
//                int face, int style, int size);


/**
 * Draw triangle
 *
 * @param pixel The packed pixel value
 */
extern void gxpport_fill_triangle(
                                 jint pixel, const jshort *clip,
                                 gxpport_mutableimage_native_handle dst, int dotted,
                                 int x1, int y1,
                                 int x2, int y2,
                                 int x3, int y3) {

    GtkForm *form;
    GtkWidget *da;
    GdkGC *gc;
    GdkColor gdkColor;
    GdkPixmap *gdk_pix_map;
    GdkLineStyle lineStyle;
    GdkPoint points[3];
    GdkRectangle clipRectangle;

    LIMO_TRACE(">>>%s\n", __FUNCTION__);

    if (!dst) {
        form = gtk_main_window_get_current_form(main_window);
        da = gtk_object_get_user_data(form);
        if (!GTK_IS_DRAWING_AREA(da)) {
            LIMO_TRACE("%s extracted unepxected not drawing area.  Continuing.\n", __FUNCTION__);
        }

        gdk_pix_map = gtk_object_get_user_data(da);
        gc = gdk_gc_new(da->window);

        gdkColor.pixel = pixel;    /* 0x00RRGGBB */
        gdkColor.red = gdkColor.green = gdkColor.blue = 0;
        gdk_gc_set_foreground(gc, &gdkColor);

        lineStyle = dotted ? GDK_LINE_ON_OFF_DASH : GDK_LINE_SOLID;

        points[0].x = x1;
        points[0].y = y1;
        points[1].x = x2;
        points[1].y = y2;
        points[2].x = x3;
        points[2].y = y3;

        gdk_gc_set_line_attributes(gc,
                                   1,   /* line_width */
                                   lineStyle,   /* line_style */
                                   GDK_CAP_BUTT, /* cap_style */
                                   GDK_JOIN_BEVEL); /* join_style */

        clipRectangle.x = clip[0];
        clipRectangle.y = clip[1];
        clipRectangle.width = clip[2] - clip[0];
        clipRectangle.height = clip[3] - clip[1];
        gdk_gc_set_clip_rectangle(gc, &clipRectangle);

        gdk_draw_polygon(gdk_pix_map,
                              gc,
                              TRUE,
                              points,
                              3);
    }
    else {
        //TODO:  implement drawing to form-specific canvas
    }

    LIMO_TRACE("<<<%s\n", __FUNCTION__);}

/**
 * Copy from a specify region to other region
 */
extern void gxpport_copy_area(
                             const jshort *clip, gxpport_mutableimage_native_handle dst,
                             int x_src, int y_src, int width, int height,
                             int x_dest, int y_dest) {

    LIMO_TRACE(">>>%s\n", __FUNCTION__);
    LIMO_TRACE("<<<%s\n", __FUNCTION__);


    /* Suppress unused parameter warnings */
    (void)clip;
    (void)dst;
    (void)x_src;
    (void)y_src;
    (void)width;
    (void)height;
    (void)x_dest;
    (void)y_dest;
}

/**
 * Draw image in RGB format
 */
extern void gxpport_draw_rgb(
                            const jshort *clip,
                            gxpport_mutableimage_native_handle dst, jint *rgbData,
                            jint offset, jint scanlen, jint x, jint y,
                            jint width, jint height, jboolean processAlpha) {

    LIMO_TRACE(">>>%s\n", __FUNCTION__);
    LIMO_TRACE("<<<%s\n", __FUNCTION__);

    /* Suppress unused parameter warnings */
    (void)clip;
    (void)dst;
    (void)rgbData;
    (void)offset;
    (void)scanlen;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)processAlpha;
}

/**
 * Obtain the color that will be final shown
 * on the screen after the system processed it.
 */
extern jint gxpport_get_displaycolor(jint color) {

    REPORT_CALL_TRACE1(LC_LOWUI, "LF:STUB:gxpport_get_displaycolor(%d)\n",
                       color);

    return color; /* No change */
}


/**
 * Draw a line between two points (x1,y1) and (x2,y2).
 */
extern void gxpport_draw_line(
                             jint pixel, const jshort *clip,
                             gxpport_mutableimage_native_handle dst,
                             int dotted, int x1, int y1, int x2, int y2)
{
    GtkForm *form;
    GtkWidget *da;
    GdkGC *gc;
    GdkColor gdkColor;
    GdkPixmap *gdk_pix_map;
    GdkLineStyle lineStyle;
    GdkRectangle clipRectangle;

    LIMO_TRACE(">>>%s\n", __FUNCTION__);

    if (!dst) {
        form = gtk_main_window_get_current_form(main_window);
        da = gtk_object_get_user_data(form);
        if (!GTK_IS_DRAWING_AREA(da)) {
            LIMO_TRACE("%s extracted unepxected not drawing area.  Continuing.\n", __FUNCTION__);
        }

        gdk_pix_map = gtk_object_get_user_data(da);
        gc = gdk_gc_new(da->window);

        gdkColor.pixel = pixel;    /* 0x00RRGGBB */
        gdkColor.red = gdkColor.green = gdkColor.blue = 0;
        gdk_gc_set_foreground(gc, &gdkColor);

        lineStyle = dotted ? GDK_LINE_ON_OFF_DASH : GDK_LINE_SOLID;

        gdk_gc_set_line_attributes(gc,
                                   1,   /* line_width */
                                   lineStyle,   /* line_style */
                                   GDK_CAP_BUTT, /* cap_style */
                                   GDK_JOIN_BEVEL); /* join_style */
        clipRectangle.x = clip[0];
        clipRectangle.y = clip[1];
        clipRectangle.width = clip[2] - clip[0];
        clipRectangle.height = clip[3] - clip[1];
        gdk_gc_set_clip_rectangle(gc, &clipRectangle);
        gdk_draw_line(gdk_pix_map,
                              gc,
                              x1,
                              y1,
                              x2,
                              y2);
    }
    else {
        //TODO:  implement drawing to form-specific canvas
    }

    LIMO_TRACE("<<<%s\n", __FUNCTION__);
}

/**
 * Draw a rectangle at (x,y) with the given width and height.
 *
 * @note x, y sure to be >=0
 *       since x,y is quan. to be positive (>=0), we don't
 *       need to test for special case anymore.
 */
extern void gxpport_draw_rect(
                             jint pixel, const jshort *clip,
                             gxpport_mutableimage_native_handle dst,
                             int dotted, int x, int y, int width, int height)
{
    GtkForm *form;
    GtkWidget *da;
    GdkGC *gc;
    GdkColor gdkColor;
    GdkPixmap *gdk_pix_map;
    GdkLineStyle lineStyle;
    GdkRectangle clipRectangle;

    LIMO_TRACE(">>>%s\n", __FUNCTION__);
    LIMO_TRACE(">>>%s clip is %d, %d, %d, %d\n", __FUNCTION__, clip[0], clip[1], clip[2], clip[3]);

    if (!dst) {
        form = gtk_main_window_get_current_form(main_window);
        da = gtk_object_get_user_data(form);
        if (!GTK_IS_DRAWING_AREA(da)) {
            LIMO_TRACE("%s extracted unepxected not drawing area.  Continuing.\n", __FUNCTION__);
        }

        gdk_pix_map = gtk_object_get_user_data(da);
        gc = gdk_gc_new(da->window);

        gdkColor.pixel = pixel;    /* 0x00RRGGBB */
        gdkColor.red = gdkColor.green = gdkColor.blue = 0;
        gdk_gc_set_foreground(gc, &gdkColor);

        lineStyle = dotted ? GDK_LINE_ON_OFF_DASH : GDK_LINE_SOLID;


        gdk_gc_set_line_attributes(gc,
                                   1,   /* line_width */
                                   lineStyle,   /* line_style */
                                   GDK_CAP_BUTT, /* cap_style */
                                   GDK_JOIN_BEVEL); /* join_style */

        clipRectangle.x = clip[0];
        clipRectangle.y = clip[1];
        clipRectangle.width = clip[2] - clip[0];
        clipRectangle.height = clip[3] - clip[1];
        gdk_gc_set_clip_rectangle(gc, &clipRectangle);

        gdk_draw_rectangle(gdk_pix_map,
                              gc,
                              FALSE,
                              x, y,
                              width,
                              height);
    }
    else {
        //TODO:  implement drawing to form-specific canvas
    }

    LIMO_TRACE("<<<%s\n", __FUNCTION__);
}

/**
 * Fill a rectangle at (x,y) with the given width and height.
 */
extern void gxpport_fill_rect(
                             jint pixel, const jshort *clip,
                             gxpport_mutableimage_native_handle dst,
                             int dotted, int x, int y, int width, int height) {
    GtkForm *form;
    GtkWidget *da;
    GdkGC *gc;
    GdkColor gdkColor;
    GdkPixmap *gdk_pix_map;
    GdkLineStyle lineStyle;
    GdkRectangle clipRectangle;

    LIMO_TRACE(">>>%s\n", __FUNCTION__);
    if (!dst) {
        form = gtk_main_window_get_current_form(main_window);
        da = gtk_object_get_user_data(form);
        if (!GTK_IS_DRAWING_AREA(da)) {
            LIMO_TRACE("%s extracted unepxected not drawing area.  Continuing.\n", __FUNCTION__);
        }

        gdk_pix_map = gtk_object_get_user_data(da);
        gc = gdk_gc_new(da->window);

        gdkColor.pixel = pixel;    /* 0x00RRGGBB */
        gdkColor.red = gdkColor.green = gdkColor.blue = 0;
        gdk_gc_set_foreground(gc, &gdkColor);

        lineStyle = dotted ? GDK_LINE_ON_OFF_DASH : GDK_LINE_SOLID;

        gdk_gc_set_line_attributes(gc,
                                   1,   /* line_width */
                                   lineStyle,   /* line_style */
                                   GDK_CAP_BUTT, /* cap_style */
                                   GDK_JOIN_BEVEL); /* join_style */

        clipRectangle.x = clip[0];
        clipRectangle.y = clip[1];
        clipRectangle.width = clip[2] - clip[0];
        clipRectangle.height = clip[3] - clip[1];
        gdk_gc_set_clip_rectangle(gc, &clipRectangle);

        LIMO_TRACE("%s clip.x=%d clip.y=%d clip.width=%d clip.height=%d\n",
                   __FUNCTION__, clip[0], clip[1], clip[2], clip[3]);
        gdk_draw_rectangle(gdk_pix_map,
                              gc,
                              TRUE,
                              x, y,
                              width,
                              height);
    }
    else {
        //TODO:  implement drawing to form-specific canvas
    }

    LIMO_TRACE("<<<%s\n", __FUNCTION__);
}

/**
 * Draw a rectangle at (x,y) with the given width and height. arcWidth and
 * arcHeight, if nonzero, indicate how much of the corners to round off.
 */
extern void gxpport_draw_roundrect(
                                  jint pixel, const jshort *clip,
                                  gxpport_mutableimage_native_handle dst,
                                  int dotted,
                                  int x, int y, int width, int height,
                                  int arcWidth, int arcHeight)
{

    LIMO_TRACE(">>>%s\n", __FUNCTION__);
    LIMO_TRACE("<<<%s\n", __FUNCTION__);

    /* Suppress unused parameter warnings */
    (void)pixel;
    (void)clip;
    (void)dst;
    (void)dotted;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)arcWidth;
    (void)arcHeight;
}

/**
 * Fill a rectangle at (x,y) with the given width and height. arcWidth and
 * arcHeight, if nonzero, indicate how much of the corners to round off.
 */
extern void gxpport_fill_roundrect(
                                  jint pixel, const jshort *clip,
                                  gxpport_mutableimage_native_handle dst,
                                  int dotted,
                                  int x, int y, int width, int height,
                                  int arcWidth, int arcHeight)
{

    LIMO_TRACE(">>>%s\n", __FUNCTION__);
    LIMO_TRACE("<<<%s\n", __FUNCTION__);

    /* Suppress unused parameter warnings */
    (void)pixel;
    (void)clip;
    (void)dst;
    (void)dotted;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)arcWidth;
    (void)arcHeight;
}

/**
 *
 * Draw an elliptical arc centered in the given rectangle. The
 * portion of the arc to be drawn starts at startAngle (with 0 at the
 * 3 o'clock position) and proceeds counterclockwise by <arcAngle>
 * degrees.  arcAngle may not be negative.
 *
 * @note: check for width, height <0 is done in share layer
 */
extern void gxpport_draw_arc(
                            jint pixel, const jshort *clip,
                            gxpport_mutableimage_native_handle dst,
                            int dotted, int x, int y, int width, int height,
                            int startAngle, int arcAngle)
{
    GtkForm *form;
    GtkWidget *da;
    GdkGC *gc;
    GdkColor gdkColor;
    GdkPixmap *gdk_pix_map;
    GdkLineStyle lineStyle;
    GdkRectangle clipRectangle;

    LIMO_TRACE(">>>%s\n", __FUNCTION__);

    if (!dst) {
        form = gtk_main_window_get_current_form(main_window);
        da = gtk_object_get_user_data(form);
        if (!GTK_IS_DRAWING_AREA(da)) {
            LIMO_TRACE("%s extracted unepxected not drawing area.  Continuing.\n", __FUNCTION__);
        }

        gdk_pix_map = gtk_object_get_user_data(da);
        gc = gdk_gc_new(da->window);

        gdkColor.pixel = pixel;    /* 0x00RRGGBB */
        gdkColor.red = gdkColor.green = gdkColor.blue = 0;
        gdk_gc_set_foreground(gc, &gdkColor);

        lineStyle = dotted ? GDK_LINE_ON_OFF_DASH : GDK_LINE_SOLID;


        gdk_gc_set_line_attributes(gc,
                                   1,   /* line_width */
                                   lineStyle,   /* line_style */
                                   GDK_CAP_BUTT, /* cap_style */
                                   GDK_JOIN_BEVEL); /* join_style */

        clipRectangle.x = clip[0];
        clipRectangle.y = clip[1];
        clipRectangle.width = clip[2] - clip[0];
        clipRectangle.height = clip[3] - clip[1];
        gdk_gc_set_clip_rectangle(gc, &clipRectangle);

        gdk_draw_arc(gdk_pix_map,
                          gc,
                          FALSE,
                          x,
                          y,
                          width,
                          height,
                          startAngle,
                          arcAngle);
    }
    else {
        //TODO:  implement drawing to form-specific canvas
    }

    LIMO_TRACE("<<<%s\n", __FUNCTION__);
}

/**
 * Fill an elliptical arc centered in the given rectangle. The
 * portion of the arc to be drawn starts at startAngle (with 0 at the
 * 3 o'clock position) and proceeds counterclockwise by <arcAngle>
 * degrees.  arcAngle may not be negative.
 */
extern void gxpport_fill_arc(
                            jint pixel, const jshort *clip,
                            gxpport_mutableimage_native_handle dst,
                            int dotted, int x, int y, int width, int height,
                            int startAngle, int arcAngle)
{

    GtkForm *form;
    GtkWidget *da;
    GdkGC *gc;
    GdkColor gdkColor;
    GdkPixmap *gdk_pix_map;
    GdkLineStyle lineStyle;
    GdkRectangle clipRectangle;

    LIMO_TRACE(">>>%s\n", __FUNCTION__);

    if (!dst) {
        form = gtk_main_window_get_current_form(main_window);
        da = gtk_object_get_user_data(form);
        if (!GTK_IS_DRAWING_AREA(da)) {
            LIMO_TRACE("%s extracted unepxected not drawing area.  Continuing.\n", __FUNCTION__);
        }

        gdk_pix_map = gtk_object_get_user_data(da);
        gc = gdk_gc_new(da->window);

        gdkColor.pixel = pixel;    /* 0x00RRGGBB */
        gdkColor.red = gdkColor.green = gdkColor.blue = 0;
        gdk_gc_set_foreground(gc, &gdkColor);

        lineStyle = dotted ? GDK_LINE_ON_OFF_DASH : GDK_LINE_SOLID;


        gdk_gc_set_line_attributes(gc,
                                   1,   /* line_width */
                                   lineStyle,   /* line_style */
                                   GDK_CAP_BUTT, /* cap_style */
                                   GDK_JOIN_BEVEL); /* join_style */

        clipRectangle.x = clip[0];
        clipRectangle.y = clip[1];
        clipRectangle.width = clip[2] - clip[0];
        clipRectangle.height = clip[3] - clip[1];
        gdk_gc_set_clip_rectangle(gc, &clipRectangle);

        gdk_draw_arc(gdk_pix_map,
                          gc,
                          TRUE,
                          x,
                          y,
                          width,
                          height,
                          startAngle,
                          arcAngle);
    }
    else {
        //TODO:  implement drawing to form-specific canvas
    }

    LIMO_TRACE("<<<%s\n", __FUNCTION__);
}

/**
 * Return the pixel value.
 */
extern jint gxpport_get_pixel(
                             jint rgb, int gray, int isGray) {

    REPORT_CALL_TRACE3(LC_LOWUI, "LF:STUB:gxpport_getPixel(%x, %x, %d)\n",
                       rgb, gray, isGray);

    /* Suppress unused parameter warnings */
    (void)gray;
    (void)isGray;

    return rgb;
}

/*
 * Draws the first n characters specified using the current font,
 * color, and anchor point.
 *
 * <p>
 * <b>Reference:</b>
 * Related Java declaration:
 * <pre>
 *     drawString(Ljava/lang/String;III)V
 * </pre>
 *
 * @param pixel Device-dependent pixel value
 * @param clip Clipping information
 * @param dst Platform dependent destination information
 * @param dotted The stroke style to be used
 * @param face The font face to be used (Defined in <B>Font.java</B>)
 * @param style The font style to be used (Defined in
 * <B>Font.java</B>)
 * @param size The font size to be used. (Defined in <B>Font.java</B>)
 * @param x The x coordinate of the anchor point
 * @param y The y coordinate of the anchor point
 * @param anchor The anchor point for positioning the text
 * @param chararray Pointer to the characters to be drawn
 * @param n The number of characters to be drawn
 */
extern void gxpport_draw_chars(
                              jint pixel, const jshort *clip,
                              gxpport_mutableimage_native_handle dst,
                              int dotted,
                              int face, int style, int size,
                              int x, int y, int anchor,
                              const jchar *charArray, int n) {

    LIMO_TRACE(">>>%s\n", __FUNCTION__);
    LIMO_TRACE("<<<%s\n", __FUNCTION__);

    /* Suppress unused parameter warnings */
    (void)pixel;
    (void)clip;
    (void)dst;
    (void)dotted;
    (void)face;
    (void)size;
    (void)style;
    (void)x;
    (void)y;
    (void)anchor;
    (void)charArray;
    (void)n;
}

/**
 * Obtains the ascent, descent and leading info for the font indicated.
 *
 * @param face The face of the font (Defined in <B>Font.java</B>)
 * @param style The style of the font (Defined in <B>Font.java</B>)
 * @param size The size of the font (Defined in <B>Font.java</B>)
 * @param ascent The font's ascent should be returned here.
 * @param descent The font's descent should be returned here.
 * @param leading The font's leading should be returned here.
 */
extern void gxpport_get_fontinfo(
  int face, int style, int size,
  int *ascent, int *descent, int *leading) {

    REPORT_CALL_TRACE(LC_LOWUI, "LF:STUB:gxpport_getFontInfo()\n");

    /* Suppress unused parameter warnings */
    (void)face;
    (void)size;
    (void)style;

    /* Return hard-coded values */
    *ascent = GXPPORT_GTK_FONT_ASCENT;
    *descent = GXPPORT_GTK_FONT_DESCENT;
    *leading = GXPPORT_GTK_FONT_LEADING;
}

/**
 * Gets the advance width for the first n characters in charArray if
 * they were to be drawn in the font indicated by the parameters.
 *
 * <p>
 * <b>Reference:</b>
 * Related Java declaration:
 * <pre>
 *     charWidth(C)I
 * </pre>
 *
 * @param face The font face to be used (Defined in <B>Font.java</B>)
 * @param style The font style to be used (Defined in
 * <B>Font.java</B>)
 * @param size The font size to be used. (Defined in <B>Font.java</B>)
 * @param charArray The string to be measured
 * @param n The number of character to be measured
 * @return The total advance width in pixels (a non-negative value)
 */
extern int gxpport_get_charswidth(
                                 int face, int style, int size,
                                 const jchar *charArray, int n) {

    LIMO_TRACE(">>>%s\n", __FUNCTION__);
    LIMO_TRACE("<<<%s\n", __FUNCTION__);

    /* Suppress unused parameter warnings */
    (void)face;
    (void)size;
    (void)style;
    (void)charArray;
    (void)n;

    return GXPPORT_GTK_CHARS_WIDTH;
}
