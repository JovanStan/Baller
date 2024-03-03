#ifndef __bmp_h__
#define __bmp_h__

#include <windows.h>
#include <gl/glut.h>
// Bitmap file data structures are defined in <wingdi.h> under Windows
#include <wingdi.h>

/* The following maynot be defined in your compiler includes if you are not on Windows

typedef unsigned int WORD; 
typedef unsigned long DWORD; 
typedef unsigned char BYTE;
#define BI_RGB 0
#define BI_RLE8 1
#define BI_RLE4 2
#define BI_BITFIELDS 3


typedef struct tBITMAPFILEHEADER 
{ 
	WORD bfType; 
	DWORD bfSize; 
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits; 
} BITMAPFILEHEADER;


typedef struct tBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER;

typedef struct tRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;

typedef struct tBITMAPINFO {
    MYBITMAPINFOHEADER    bmiHeader;
    MYRGBQUAD            bmiColors[1];
} BITMAPINFO;

*/



extern GLubyte * ReadBitmap(const char *filename,BITMAPINFO **info);  
extern int SaveBitmap(const char *filename, BITMAPINFO *info,GLubyte    *bits); 
extern GLubyte * ReadBitmapFromScreen(BITMAPINFO **info);
#endif