#include <stdint.h> // we include stdint.h because we need to use some data types that are defined in it like BYTE, DWORD, LONG, WORD

// These data types are essentially aliases for C/C++ primitive data types.
// They are used to make the code more readable and portable across different platforms.
// aliasess are like nicknames. so when we use BYTE, it is the same as using uint8_t. so BYTE is an alias for uint8_t

typedef uint8_t  BYTE; // unit8_t is an unsigned 8-bit integer.
typedef uint32_t DWORD; // uint32_t is an unsigned 32-bit integer.
typedef int32_t  LONG; // int32_t is a signed 32-bit integer.
typedef uint16_t WORD; // uint16_t is an unsigned 16-bit integer.

/*
BITMAPFILEHEADER is a struct that contains the first 14 bytes of a bmp file.
the first 14 bytes of a bmp file are the signature of the bmp file and the size of the file and the offset of the pixel
array and some other information about the file like the number of colors and the compression.
*/

typedef struct
{
    WORD   bfType;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;

/*
BITMAPINFOHEADER comes after the BITMAPFILEHEADER in a bmp file. the size of the BITMAPINFOHEADER is 40 bytes.
so it is 40 bytes after the BITMAPFILEHEADER.
BITMAPINFOHEADER contains the information about the image like the width and the height and the number of colors and
the compression type and some other information.

the diffrerence between BITMAPFILEHEADER and BITMAPINFOHEADER is that BITMAPFILEHEADER contains the information about
the file itself like the size of the file and the offset of the pixel array and the signature of the file but BITMAPINFOHEADER
contains the information about the image like the width and the height and the number of colors and the compression type.
*/
typedef struct
{
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;

/*
RGBTRIPLE is a struct that contains 3 bytes, one byte for the blue color, one byte for the green color,
and one byte for the red color. so it is used to represent a pixel in a color image. each pixel in a color image
*/

typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
