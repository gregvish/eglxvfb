/* Copyright 1985, 1986, Massachusetts Institute of Technology */

/*
Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in advertising or
publicity pertaining to distribution of the software without specific,
written prior permission.  M.I.T. makes no representations about the
suitability of this software for any purpose.  It is provided "as is"
without express or implied warranty.
*/

/*
 * XWDFile.h	MIT Project Athena, X Window system window raster
 *		image dumper, dump file format header file.
 *
 *  Author:	Tony Della Fera, DEC
 *		27-Jun-85
 *
 * Modifier:    William F. Wyatt, SAO
 *              18-Nov-86  - version 6 for saving/restoring color maps
 */

#ifndef __XWDFILE_H__
#define __XWDFILE_H__


#define XWD_FILE_VERSION 7
#define sz_XWDheader 100
#define sz_XWDColor 12


typedef struct _xwd_file_header {
    uint32_t header_size;

    uint32_t file_version;        /* = XWD_FILE_VERSION above */
    uint32_t pixmap_format;        /* ZPixmap or XYPixmap */
    uint32_t pixmap_depth;        /* Pixmap depth */
    uint32_t pixmap_width;        /* Pixmap width */
    uint32_t pixmap_height;        /* Pixmap height */
    uint32_t xoffset;            /* Bitmap x offset, normally 0 */
    uint32_t byte_order;        /* of image data: MSBFirst, LSBFirst */

    /* bitmap_unit applies to bitmaps (depth 1 format XY) only.
     * It is the number of bits that each scanline is padded to. */
    uint32_t bitmap_unit;

    uint32_t bitmap_bit_order;    /* bitmaps only: MSBFirst, LSBFirst */

    /* bitmap_pad applies to pixmaps (non-bitmaps) only.
     * It is the number of bits that each scanline is padded to. */
    uint32_t bitmap_pad;

    uint32_t bits_per_pixel;        /* Bits per pixel */

    /* bytes_per_line is pixmap_width padded to bitmap_unit (bitmaps)
     * or bitmap_pad (pixmaps).  It is the delta (in bytes) to get
     * to the same x position on an adjacent row. */
    uint32_t bytes_per_line;
    uint32_t visual_class;        /* Class of colormap */
    uint32_t red_mask;        /* Z red mask */
    uint32_t green_mask;        /* Z green mask */
    uint32_t blue_mask;        /* Z blue mask */
    uint32_t bits_per_rgb;        /* Log2 of distinct color values */
    uint32_t colormap_entries;    /* Number of entries in colormap; not used? */
    uint32_t ncolors;            /* Number of XWDColor structures */
    uint32_t window_width;        /* Window width */
    uint32_t window_height;        /* Window height */
    uint32_t window_x;        /* Window upper left X coordinate */
    uint32_t window_y;        /* Window upper left Y coordinate */
    uint32_t window_bdrwidth;        /* Window border width */
} XWDFileHeader;

/* Null-terminated window name follows the above structure. */

/* Next comes XWDColor structures, at offset XWDFileHeader.header_size in
 * the file.  XWDFileHeader.ncolors tells how many XWDColor structures
 * there are.
 */

typedef struct {
    uint32_t    pixel;
    uint16_t    red;
    uint16_t    green;
    uint16_t    blue;
    uint8_t    flags;
    uint8_t    pad;
} XWDColor;

/* Last comes the image data in the format described by XWDFileHeader. */

#endif /* __XWDFILE_H__ */
