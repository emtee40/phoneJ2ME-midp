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

#ifndef _JCAPP_EXPORT_H_
#define _JCAPP_EXPORT_H_

#include <kni.h>
/**
 * @defgroup highui_jcapp Javacall Application External Interface
 * @ingroup highui
 */

/**
 * @file
 * @ingroup highui_jcapp
 *
 * @brief Porting interface for javacall application.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the javacall native resources.
 *
 * @return <tt>0</tt> upon successful initialization, or
 *         <tt>other value</tt> otherwise
 */
extern int jcapp_init();

/**
 * Finalize the javacall native resources.
 */
extern void jcapp_finalize();

/**
 * Refresh the given area.  For double buffering purposes.
 */
extern void jcapp_refresh(int x, int y, int w, int h);

/**
 * set the screen mode either to fullscreen or normal.
 *
 * @param mode The screen mode
 */
extern void jcapp_set_fullscreen_mode(jboolean mode);

/**
 * Change screen orientation flag
 */
extern jboolean jcapp_reverse_orientation();

/**
 * Get screen orientation flag
 */
extern jboolean jcapp_get_reverse_orientation();


/**
 * Return screen width
 */
extern int jcapp_get_screen_width();

/**
 *  Return screen height
 */
extern int jcapp_get_screen_height();

/**
 * Checks if soft button layer is supported
 * 
 * @return KNI_TRUE if native softbutton is supported, KNI_FALSE - otherwise
 */

extern jboolean jcapp_is_native_softbutton_layer_supported();

/**
 * Request platform to draw a label in the soft button layer.
 * 
 * @param label Label to draw (UTF16)
 * @param len Length of the lable (0 will cause removal of current label)
 * @param index Index of the soft button in the soft button bar.
 */
extern void jcapp_set_softbutton_label_on_native_layer (unsigned short *label,
                                                         int len,
                                                         int index);


/*Disables the refresh of the screen*/
 void LCDUI_disable_refresh(void);
/*Enables the refresh of the screen*/
 void LCDUI_enable_refresh(void);

#ifdef __cplusplus
}
#endif

#endif /* _JCAPP_EXPORT_H_ */
