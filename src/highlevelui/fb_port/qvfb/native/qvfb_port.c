/*
 *   
 *
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
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

/**
 * @file
 * Implementation of the porting layer for QVFb application
 */

#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gxj_putpixel.h>
#include <midp_logging.h>
#include <midpMalloc.h>
#include <midp_constants_data.h>

#include "qvfb_port.h"

/** @def PERROR Prints diagnostic message. */
#define PERROR(msg) REPORT_ERROR2(0, "%s: %s", msg, strerror(errno))

/** The file descriptor for reading the mouse */
static int mouseFd = -1;
/** The file descriptor for reading the keyboard */
static int keyboardFd = -1;

/** Return file descriptor of keyboard device, or -1 in none */
int getKeyboardFd(int hardwareId) {
  (void)hardwareId;
    return keyboardFd;
}
/** Return file descriptor of mouse device, or -1 in none */
int getMouseFd(int hardwareId) {
  (void)hardwareId;
    return mouseFd;
}

/** QVFb header data */
static QVFbHeader *hdr = NULL;
/** QVFb video buffer for direct drawing */
static gxj_pixel_type *qvfbPixels = NULL;

/** System offscreen buffer */
gxj_screen_buffer gxj_system_screen_buffer;


/** System screens */
typedef struct _SystemScreen {
    int hardwareId;
    int isFullScreen;
    jboolean reverse_orientation;
    gxj_screen_buffer buffer;
} SystemScreen;



SystemScreen* system_screens; 
int num_of_screens;

void initScreenList(int num) {
  int size = sizeof(SystemScreen) * num;
  system_screens = (SystemScreen*)midpMalloc(size);
  if (system_screens != NULL) {
    memset(system_screens, 0, size);
  } 

  printf("init system screen system_screens[0].hardwareId = %d fs=%d ro = %d\n", system_screens[0].hardwareId,  system_screens[0].isFullScreen, system_screens[0].reverse_orientation );
  printf("init system screen system_screens[1].hardwareId = %d fs=%d ro = %d\n", system_screens[1].hardwareId,  system_screens[1].isFullScreen, system_screens[1].reverse_orientation );
}

SystemScreen* getScreenById(int hardwareId) {
  int i;
  for (i = 0; i < num_of_screens; i++) {
    if (system_screens[i].hardwareId == hardwareId)
      return &system_screens[i];
  }
  return NULL;
}

void initBuffer(int width, int height, gxj_screen_buffer* buffer) {
  int size = sizeof(gxj_pixel_type) * width * height;
  printf("initBuffer\n");
  buffer->width = width;
  buffer->height = height;
  buffer->alphaData = NULL;
  buffer->pixelData =
    (gxj_pixel_type *)midpMalloc(size);
  printf("initBuffer malloc\n");
  if (buffer->pixelData != NULL) {
    memset(buffer->pixelData, 0, size);
  } 
  printf("initBuffer memset\n");
}
                      

/** Allocate and init system screen */
void initSystemScreen(int id, int isFullScreen, int reverse_orientation, int width, int height) {
  SystemScreen *screen = getScreenById(id);
  if (screen == NULL) {
    return;
  }
  printf("init system screen id = %d fs=%d ro = %d width = %d height=%d\n", screen->hardwareId,  screen->isFullScreen, screen->reverse_orientation, width, height );

  screen->hardwareId = id;
  screen->isFullScreen = isFullScreen;
  screen->reverse_orientation = reverse_orientation;
      
  initBuffer(width, height, &(screen->buffer)); 
  
  printf("after initBuffer /n");
  printf("system screen: harwareId= %d", screen->hardwareId);
  
  printf("isFullScreen = %d", screen->isFullScreen);
  printf("reverse_orientation = %d ", screen->reverse_orientation);
  printf("width = %d height = %d", width,height);
  if (id ==0) {
    gxj_system_screen_buffer = screen->buffer;
  }
}              
void clearScreenList() {
  int i;
  for (i = 0; i < num_of_screens; i++ ) {
      gxj_screen_buffer buff = system_screens[i].buffer;
      if (buff.pixelData != NULL) {
        midpFree(buff.pixelData);
        buff.pixelData = NULL;
      }
      if (buff.alphaData != NULL) {
        midpFree(buff.alphaData);
        buff.alphaData = NULL;
      }
    }
}          


jboolean setFullScreenMode(int id, int mode, int width, int height) {
    SystemScreen *screen = getScreenById(id);
    int updated = 0;
    if (screen = NULL) {
      return FALSE;
    }
    if (screen->isFullScreen != mode) {
      gxj_screen_buffer buff = screen->buffer;
      screen->isFullScreen = mode;
      
      if (buff.pixelData != NULL) {
        if (buff.width == width &&
	    buff.height == height) {
        } else {

	  if (buff.pixelData != NULL) {
	    midpFree(buff.pixelData);
	    buff.pixelData = NULL;
	  }
	  if (buff.alphaData != NULL) {
	    midpFree(buff.alphaData);
	    buff.alphaData = NULL;
	  }

	  initBuffer(width, height, &(screen->buffer));
        }
      }
      updated = 1;
    }
    return updated;
}

int getReverseOrientation(int id) {
    SystemScreen *screen = getScreenById(id);
    int ret = 0;
    if (screen != NULL) {
      ret = screen->reverse_orientation;
    }
    return ret;
}

jboolean isFullScreenMode(int id) {
    SystemScreen *screen = getScreenById(id);
    jboolean ret = 0;
    if (screen != NULL) {
      ret = screen->isFullScreen;
    }
    return ret;
}

/**
 * Change screen orientation to landscape or portrait,
 * depending on the current screen mode
 */
jboolean reverseScreenOrientation(int id) {
    int height;
    SystemScreen *screen = getScreenById(id);
    if (screen != NULL) {
      return FALSE;
    }

    screen->reverse_orientation = !screen->reverse_orientation;

    
    if (screen->buffer.pixelData != NULL) {
      int size = sizeof(gxj_pixel_type) *
	screen->buffer.width *
	screen->buffer.height;
      
        memset(screen->buffer.pixelData, 0, size);
    }

    height = screen->buffer.height;
    screen->buffer.height = screen->buffer.width;
    screen->buffer.width = height;

    return screen->reverse_orientation;
}

/** On i386, connect to the QVFB virtual frame buffer */
void connectFrameBuffer(int width, int height) {
    int displayId = 0;
    char buff[30];
    key_t key;
    int shmId;
    unsigned char *shmrgn;
    char *env;

    
    // System screen buffer geometry
    int bufWidth = width;;
    int bufHeight = height;


    if ((env = getenv("QWS_DISPLAY")) != NULL) {
      displayId = atoi(env + 1); /* skip the leading colon */
    }

    sprintf(buff, "/tmp/.qtvfb_mouse-%d", displayId);
    if ((mouseFd = open(buff, O_RDONLY | O_NONBLOCK, 0)) < 0) {
      fprintf(stderr, "open of %s failed\n", buff);
      exit(1);
    }
    
    if ((key = ftok(buff, 'b')) == -1) {
      PERROR("ftok() failed");
      exit(1);
    }
    
    if ((shmId = shmget(key, 0, 0)) == -1) {
      PERROR("shmget() failed");
      exit(1);
    }
    
    shmrgn = (unsigned char *)shmat(shmId, 0, 0);
    if ((int)shmrgn == -1 || shmrgn == 0) {
      PERROR("shmat() failed");
      exit(1);
    }
      
    hdr = (QVFbHeader *) shmrgn;
    qvfbPixels = (gxj_pixel_type*)(shmrgn + hdr->dataOffset);
    
    fprintf(stderr, "QVFB info: %dx%d, depth=%d\n",
	    hdr->width, hdr->height, hdr->depth);
      
    if (hdr->width < bufWidth || hdr->height < bufHeight) {
      fprintf(stderr, "QVFB screen too small. Need %dx%d\n",
	      bufWidth, bufHeight);
      exit(1);
    }
    if (hdr->depth != 16) {
      fprintf(stderr, "QVFB depth must be 16. Please run qvfb -depth 16\n");
      exit(1);
    }
    
    sprintf(buff, "/tmp/.qtvfb_keyboard-%d", displayId);
    if ((keyboardFd = open(buff, O_RDONLY, 0)) < 0) {
      fprintf(stderr, "open of %s failed\n", buff);
      exit(1);
    }
    
    memset(qvfbPixels, 0, sizeof(gxj_pixel_type) * hdr->width * hdr->height);
}

/** Clear screen content */
void clearScreen() {
    memset(qvfbPixels, 0, 
	sizeof(gxj_pixel_type) * 
	    hdr->width * hdr->height);
}


/** Check if screen buffer is not bigger than frame buffer device */
static void checkScreenBufferSize(int width, int height) {
    // Check if frame buffer is big enough
    if (hdr->width < width || hdr->height < height) {
        fprintf(stderr, "QVFB screen too small. Need %dx%d\n",
            width, height);
        exit(1);
    }
}

/** Get x-coordinate of screen origin */
int getScreenX(int screenRotated, int width) {
    // System screen buffer geometry
    int bufWidth = width;
    int x = 0;
    int LCDwidth = screenRotated ? hdr->height : hdr->width;
    if (LCDwidth > bufWidth) {
        x = (LCDwidth - bufWidth) / 2;
    }
    return x;
}

/** Get y-coordinate of screen origin */
int getScreenY(int screenRotated, int height) {
  int bufHeight = height;
    int y = 0;
    int LCDheight = screenRotated ? hdr->width : hdr->height;
    if (LCDheight > bufHeight) {
        y = (LCDheight - bufHeight) / 2;
    }
    return y;
}



/** Refresh screen with offscreen bufer content */
void refreshScreenNormal(gxj_screen_buffer *screen_buffer, int x1, int y1, int x2, int y2) {
    // QVFB feature: a number of bytes per line can be different from
    // screenWidth * pixelSize, so lineStep should be used instead.
    int lineStep = hdr->lineStep / sizeof(gxj_pixel_type);
    int dstWidth =  hdr->lineStep / sizeof(gxj_pixel_type);
    gxj_pixel_type *dst  = (gxj_pixel_type *)qvfbPixels;
    gxj_pixel_type *src  = screen_buffer->pixelData;

    int srcWidth = x2 - x1;

    // System screen buffer geometry
    int bufWidth = screen_buffer->width;
    int bufHeight = screen_buffer->height;

    REPORT_CALL_TRACE4(LC_HIGHUI, "LF:fbapp_refresh(%3d, %3d, %3d, %3d )\n",
                       x1, y1, x2, y2);

    // Check if frame buffer is big enough
    checkScreenBufferSize(bufWidth, bufHeight);

    // Center the LCD output area
    if (hdr->width > bufWidth) {
        dst += (hdr->width - bufWidth) / 2;
    }
    if (hdr->height > bufHeight) {
        dst += (hdr->height - bufHeight) * lineStep / 2;
    }

    src += y1 * bufWidth + x1;
    dst += y1 * dstWidth + x1;

    for (; y1 < y2; y1++) {
        memcpy(dst, src, srcWidth * sizeof(gxj_pixel_type));
        src += bufWidth;
        dst += dstWidth;
    }

    hdr->dirty_x1 = 0;
    hdr->dirty_y1 = 0;
    hdr->dirty_x2 = hdr->width;
    hdr->dirty_y2 = hdr->height;
    hdr->is_dirty = 1;
}

/** Refresh rotated screen with offscreen bufer content */
void refreshScreenRotated(gxj_screen_buffer *screen_buffer, int x1, int y1, int x2, int y2) {

    gxj_pixel_type *src = screen_buffer->pixelData;
    gxj_pixel_type *dst = (gxj_pixel_type *)qvfbPixels;
    int srcWidth, srcHeight;
    int lineStep =  hdr->lineStep / sizeof(gxj_pixel_type);

    // System screen buffer geometry
    int bufWidth = screen_buffer->width;
    int bufHeight = screen_buffer->height;

    int x;
    int srcInc;
    int dstInc;

    // Check if frame buffer is big enough
    checkScreenBufferSize(bufHeight, bufWidth);

    srcWidth = x2 - x1;
    srcHeight = y2 - y1;

    // Center the LCD output area
    if (bufWidth < hdr->height) {
        dst += (hdr->height - bufWidth) / 2 * lineStep;
    }
    if (bufHeight < hdr->width) {
        dst += ((hdr->width - bufHeight) / 2);
    }

    src += x1 + y1 * bufWidth;
    dst += y1 + (bufWidth - x1 - 1) * lineStep;

    srcInc = bufWidth - srcWidth;      // increment for src pointer at the end of row
    dstInc = srcWidth * lineStep + 1;  // increment for dst pointer at the end of column

    while(y1++ < y2) {
        for (x = x1; x < x2; x++) {
            *dst = *src++;
            dst -= lineStep;
         }
         dst += dstInc;
         src += srcInc;
    }

    hdr->dirty_x1 = 0;
    hdr->dirty_y1 = 0;
    hdr->dirty_x2 = hdr->width;
    hdr->dirty_y2 = hdr->height;
    hdr->is_dirty = 1;
}

void refreshScreen(int id, int x1, int y1, int x2, int y2) {
  SystemScreen *screen = getScreenById(id);
  if (screen != NULL) {
    if (!screen->reverse_orientation) {
      refreshScreenNormal(&screen->buffer, x1, y1, x2, y2);
    } else {
      refreshScreenRotated(&screen->buffer, x1, y1, x2, y2);
    }
  }
}

