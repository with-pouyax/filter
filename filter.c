#include <getopt.h> //we add getopt.h to use getopt function which is used to parse the command line arguments
#include <stdio.h> //we add stdio.h to use printf and FILE and fread and fwrite and fclose and fputc and fseek functions
#include <stdlib.h> //we add stdlib.h to use malloc and calloc and free functions

#include "helpers.h" //we include helpers.h to use the functions that are defined in helpers.h

int main(int argc, char *argv[])
{
    // Define allowable filters
    char *filters = "bgrs"; //we initialize the filters string and set it to "bgrs" which are the possible filters

    // Get filter flag and check validity
    char filter = getopt(argc, argv, filters); // we pass argc, argv, and the filters which is equal to "bgrs" to getopt.
                                               //and getopt will return the flag that was passed in the command line. so if
                                               //we pass ./filter -g it will return 'g' and if we pass ./filter -x it will return '?'
    if (filter == '?') //getopt returns '?' if the flag is not in the filters string
    {
        printf("Invalid filter.\n");
        return 1;
    }

    // Ensure only one filter
    if (getopt(argc, argv, filters) != -1) //getopt returns -1 in case we pass more than one flag in the command line
    {
        printf("Only one filter allowed.\n");
        return 2;
    }

    // Ensure proper usage
    if (argc != optind + 2) //optind came from getopt function and it is the index of the next element in argv that getopt will process. so if we pass ./filter -g infile outfile, optind will be 3. 3 is the index of the infile in argv. so argc should be equal to optind + 2
                            //so here argc != optind + 2 means, if we don't pass exactly 2 arguments after the flag, we will print the usage message. could we also write if (argc != 4) ? yes we could, but this way is more dynamic and it will work even if we add more flags in the future
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        return 3;
    }

    // Remember filenames
    char *infile = argv[optind]; //optind is the index of the first argument after the flag. so if we pass ./filter -g infile outfile, optind will be 2. 2 is the index of the infile in argv
    char *outfile = argv[optind + 1]; //optind + 1 is the index of the second argument after the flag. so if we pass ./filter -g infile outfile, optind + 1 will be 3. 3 is the index of the outfile in argv

    // Open input file
    FILE *inptr = fopen(infile, "r"); //we open the infile in read mode
    if (inptr == NULL) //if the file doesn't exist or we don't have the permission or any other error, fopen will return NULL
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w"); //we open the outfile in write mode, FILE is a struct that is defined in stdio.h
    if (outptr == NULL) //if the file doesn't exist or we don't have the permission or any other error, fopen will return NULL
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 5;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf; //we create a BITMAPFILEHEADER variable. this variable will store the BITMAPFILEHEADER of the infile, BITMAPFILEHEADER is a struct that is defined in bmp.h. it contains the first 14 bytes of the bmp file
                         //the first 14 bytes are the signature of the bmp file and the size of the file and the offset of the pixel array and some other information about the file
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr); //we read the first 14 bytes of the infile and store them in bf

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi; //we create a BITMAPINFOHEADER variable. this variable will store the BITMAPINFOHEADER of the infile, BITMAPINFOHEADER is a struct that is defined in bmp.h. it contains the information about the image like the width and the height and the number of colors and the compression type and some other information
                         //the BITMAPINFOHEADER is 40 bytes long
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr); //we read the 40 bytes of the infile and store them in bi

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0) /* we check if the bfType is equal to 0x4d42, that means the first two bytes of the file are 'BM' which is the signature of the bmp file which means the file is a bmp file
                                                        we check if the bfOffBits is equal to 54, that means the offset of the pixel array is 54 bytes which means the file is a BMP 4.0 file format which is the most common format
                                                        we check if the biSize is equal to 40, that means the size of the BITMAPINFOHEADER is 40 bytes which means the file is a BMP 4.0 file format
                                                        we check if the biBitCount is equal to 24, that means the image is 24-bit which means the image is a color image and each pixel is represented by 3 bytes, one byte for the blue color, one byte for the green color, and one byte for the red color
                                                        we check if the biCompression is equal to 0, that means the image is uncompressed which means the image is a BMP 4.0 file format
                                                      */
    {
        fclose(outptr); //we close the outptr file
        fclose(inptr); //we close the inptr file
        printf("Unsupported file format.\n");
        return 6;
    }

    // Get image's dimensions
    int height = abs(bi.biHeight); //abs is a function that is defined in stdlib.h. it returns the absolute value of an integer. so if the height is negative, we make it positive. the hight can be negative if the image is stored upside down
    int width = bi.biWidth;

    // Allocate memory for image
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE)); // RGBTRIPLE is a struct that is defined in bmp.h. it contains 3 bytes, one byte for the blue color, one byte for the green color, and one byte for the red color
                                                                          // RGBTRIPLE(*image)[width] is a pointer to a 2D array of RGBTRIPLE. using calloc we allocate memory for the image. the image is a 2D array of RGBTRIPLE.
                                                                          // the height of the image is height and the width of the image is width. so the image is height x width pixels.
                                                                          // in calloc the first argument is the number of elements we want to allocate memory for. the number of elements is height. the second argument is the size of each element. the size of each element is width * sizeof(RGBTRIPLE)
                                                                          //
    if (image == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4; //the padding is the number of bytes that are added to the end of each row to make the row a multiple of 4 bytes
                                                             //we need each row to be a multiple of 4 bytes because the BMP file format requires that.
                                                             //the padding is calculated by the formula (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4
                                                             //the width of the image is bi.biWidth. the size of each pixel is sizeof(RGBTRIPLE) which is 3 bytes. so the size of each row is width * sizeof(RGBTRIPLE)
                                                             //we divide the size of each row by 4 and get the remainder. then we subtract the remainder from 4. then we get the remainder of the result by 4
                                                             //the padding can be 0, 1, 2, or 3 bytes
                                                             //the padding is added to the end of each row to make the row a multiple of 4 bytes

    for (int i = 0; i < height; i++) //we loop over the rows of the image and read each row into the image array and skip over the padding
    {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR); //fseek is a function that is defined in stdio.h. it is used to move the file pointer to a specific position in the file
                                         //the first argument is the file pointer. the second argument is the number of bytes we want to move the file pointer. the third argument is the position we want to move the file pointer to. SEEK_CUR means we want to move the file pointer relative to the current position
                                         //so we move the file pointer to the end of the padding
    }

    switch (filter) //we switch on the filter flag that was passed in the command line and call the appropriate function.
                    //to explain how switch works, let's say the filter flag is 'g'. so the switch will jump to the case 'g' and call the grayscale function
                    // it is like if (filter == 'g') { grayscale(height, width, image); }
    {
        // Blur
        case 'b':
            blur(height, width, image);
            break;

        // Grayscale
        case 'g':
            grayscale(height, width, image);
            break;

        // Reflection
        case 'r':
            reflect(height, width, image);
            break;

        // Sepia
        case 's':
            sepia(height, width, image);
            break;
    // after the function is called, we will have the a modified image in the image array and the function will return the modified image as a 2D array of RGBTRIPLE called image
    }
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr); // first we write the BITMAPFILEHEADER of the modified image to the outfile

    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr); // then we write the BITMAPINFOHEADER of the modified image to the outfile

    // Write new pixels to outfile
    for (int i = 0; i < height; i++) // then we loop over the rows of the image and write each row to the outfile and add the padding at the end of each row
    {
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // Free memory for image
    free(image); // we free the memory that was allocated for the image

    // Close files
    fclose(inptr);
    fclose(outptr);
    return 0;
}
