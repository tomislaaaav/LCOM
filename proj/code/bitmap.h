/**
 * @file bitmap.h
 * @author Miguel Botelho and Antonio Ramadas
 * @brief The file containing the functions that handle the bitmaps
 *
 * @see http://difusal.blogspot.pt/2014/09/minixtutorial-8-loading-bmp-images.html
 */

#ifndef __BITMAP_H
#define __BITMAP_H

#include "lib.h"
#include "bitmap_macros.h"
#include "video_gr.h"
#include "bmpfile.h"
#include "global_variables.h"
#include "timer.h"

extern int screen_abs;
extern int screen_current;
#define Dx			27

#pragma once

/** @defgroup Bitmap Bitmap
 * @{
 * Functions for manipulating bitmaps
 */

typedef enum {
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
} Alignment;

typedef struct {
    unsigned short type; // specifies the file type
    unsigned int size; // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels largura
    int height; // specifies height in pixels altura
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bits; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} BitmapInfoHeader;

/// Represents a Bitmap
typedef struct {
    BitmapInfoHeader bitmapInfoHeader;
    unsigned char* bitmapData;
} Bitmap;

/**
 * @brief receives the color, and changes the red, green and blue argument
 *
 * @param red
 * @param green
 * @param blue
 * @param colour
 *
 */
void getrgbBitmap(int *red, int *green, int *blue, int rgbaaaa);

/**
 * @brief creates the bitmap for the right guess
 */
void createBitmap();

/**
 * @brief creates the bitmaps for the undo and redo functions
 */
void createBitmapsUndoRedo();

/**
 * @brief creates the bitmap for the current draw bitmap
 */
void createDBitmap();

/**
 * @brief Loads a bmp image (made by Henrique Ferrolho)
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 * @see http://difusal.blogspot.pt/2014/09/minixtutorial-8-loading-bmp-images.html
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position (made by Henrique Ferrolho, slightly changed by us)
 *
 * @param bmp bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 * @param char buffer
 * @see http://difusal.blogspot.pt/2014/09/minixtutorial-8-loading-bmp-images.html
 */
void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment, char * buffer);

/**
 * @brief Loads an image with transparency (green). This one was made by us
 *
 * @param bmp bitmap to be drawn
 * @param x x coord
 * @param y y coord
 * @param alignment image alignment
 * @param char buffer
 */
void drawMouse(Bitmap* bmp, int x, int y, Alignment alignment, char * buffer);

/**
 * @brief Destroys the given bitmap, freeing all resources used by it. (made by Henrique Ferrolho)
 *
 * @param bitmap bitmap to be destroyed
 * @see http://difusal.blogspot.pt/2014/09/minixtutorial-8-loading-bmp-images.html
 */
void deleteBitmap(Bitmap* bmp);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position, with a time delay (made by Henrique Ferrolho, slightly changed by us)
 *
 * @param bmp bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 * @param char buffer
 */
void drawBitmapDelay(Bitmap* bmp, int x, int y, Alignment alignment, char * buffer);

/**@}*/

#endif /* __BITMAP_H */
