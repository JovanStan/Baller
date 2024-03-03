#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include "bmp.h"

/* read bitmap information. Returns the final bitmap pixel values */
GLubyte *                         
ReadBitmap(const char *filename, 
             BITMAPINFO **info)    
{
  FILE             *fp;          /* Open file pointer */
  GLubyte          *pixels;        /* Bitmap pixel bits */
  int              imgsize;      /* Size of bitmap image*/
  int              infosize;     /* Size of header information */
  BITMAPFILEHEADER header;       /* File header */


  // Try opening the file; use "rb" mode to read a binary file. 
  if ((fp = fopen(filename, "rb")) == NULL)
    return (NULL);

  // Read the file header
  if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)
  {
  // Couldn't read the file header 
	  fclose(fp);
    return (NULL);
  }

  if (header.bfType != 'MB')	/* Check for BM reversed... */
  {
  // Not a bitmap file
    fclose(fp);
    return (NULL);
  }

  infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
  if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL)
  {
    fclose(fp);
    return (NULL);
  }

  if (fread(*info, 1, infosize, fp) < infosize)
  {
    free(*info);
    fclose(fp);
    return (NULL);
  }

  /* Now that we have all the header info read in, allocate memory for *
   * the bitmap and read it in...  
   */
  imgsize = (*info)->bmiHeader.biSizeImage;
  // sometimes imagesize is not set in files
  if (imgsize == 0)
     imgsize = ((*info)->bmiHeader.biWidth *
                (*info)->bmiHeader.biBitCount + 7) / 8 *
  	             abs((*info)->bmiHeader.biHeight);

  if ((pixels = (unsigned char *)malloc(imgsize)) == NULL)
  {
    free(*info);
    fclose(fp);
    return (NULL);
  }

  if (fread(pixels, 1, imgsize, fp) < imgsize)
  {
    free(*info);
    free(pixels);
    fclose(fp);
    return (NULL);
   }

   fclose(fp);
   return (pixels);
}


/*
 * 'SaveBitmap()' - Save a BMP file to disk.
 *
 * Returns 0 on success or -1 on failure...
 */

int                                
SaveBitmap(const char *filename,  /* File to save to */
            BITMAPINFO *info,     /* Bitmap information */
	          GLubyte    *bits)     /*Bitmap data */
    {
    FILE             *fp;          
    int              size, infosize, bitsize;  
    BITMAPFILEHEADER header;   


    /* Try opening the file; use "wb" mode to write this *binary* file. */
    if ((fp = fopen(filename, "wb")) == NULL)
        return (-1);

    /* Figure out the bitmap size */
    if (info->bmiHeader.biSizeImage == 0)
	    bitsize = (info->bmiHeader.biWidth *
        	   info->bmiHeader.biBitCount + 7) / 8 * fabs(double(info->bmiHeader.biHeight));
    else
	    bitsize = info->bmiHeader.biSizeImage;

    /* Figure out the header size */
    infosize = sizeof(BITMAPINFOHEADER);
    switch (info->bmiHeader.biCompression)
	  {
	  
	  case BI_RGB :
      if (info->bmiHeader.biBitCount > 8 &&
        	info->bmiHeader.biClrUsed == 0)
	      break;
	  
	  }

    size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;

    /* Write the file header, bitmap information, and bitmap pixel data... */
    header.bfType      = 'MB'; 
    header.bfSize      = size;
    header.bfReserved1 = 0;
    header.bfReserved2 = 0;
    header.bfOffBits   = sizeof(BITMAPFILEHEADER) + infosize;

    if (fwrite(&header, 1, sizeof(BITMAPFILEHEADER), fp) < sizeof(BITMAPFILEHEADER))
        {
        /* Couldn't write the file header - return... */
        fclose(fp);
        return (-1);
        }

    if (fwrite(info, 1, infosize, fp) < infosize)
        {
        /* Couldn't write the bitmap header - return... */
        fclose(fp);
        return (-1);
        }

    if (fwrite(bits, 1, bitsize, fp) < bitsize)
        {
        /* Couldn't write the bitmap - return... */
        fclose(fp);
        return (-1);
        }

    fclose(fp);
    return (0);
    }

GLubyte * 
ReadBitmapFromScreen(BITMAPINFO **info)
{
  long i,j,bitsize, width;
  GLint viewport[4];
  GLubyte *bits;
  GLubyte *rgb, tmp;

  // get the extents of the viewport of the window
  glGetIntegerv(GL_VIEWPORT, viewport);

  if ((*info = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER))) == NULL)
    return NULL;
  width = viewport[2]*3;
  width = (width+3)& ~3;
  bitsize = width*viewport[3];

  if ((bits = (unsigned char *)calloc(bitsize,1)) ==NULL)
    return NULL;

  glPixelStorei(GL_PACK_ALIGNMENT,4);
  glPixelStorei(GL_PACK_ROW_LENGTH,0);
  glPixelStorei(GL_PACK_SKIP_ROWS,0);
  glPixelStorei(GL_PACK_SKIP_PIXELS,0);

  glReadPixels(0,0,viewport[2], viewport[3], GL_BGR_EXT, GL_UNSIGNED_BYTE,bits);

  (*info)->bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
  (*info)->bmiHeader.biWidth    = viewport[2];
  (*info)->bmiHeader.biHeight   = viewport[3];
  (*info)->bmiHeader.biPlanes   = 1;
  (*info)->bmiHeader.biBitCount = 24;
  (*info)->bmiHeader.biCompression = 0L; // BI_RGB;
  (*info)->bmiHeader.biSizeImage = bitsize;
  (*info)->bmiHeader.biXPelsPerMeter = 2952;
  (*info)->bmiHeader.biYPelsPerMeter = 2952;
  (*info)->bmiHeader.biClrUsed = 0;
  (*info)->bmiHeader.biClrImportant = 0;

  return(bits);

}

