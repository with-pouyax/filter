# Filter


## Description

Filter is a C program designed to apply various effects to BMP images. Each image is composed of a grid of pixels, where each pixel represents a color. This project involves reading BMP files, processing pixel data, and writing the modified image back to a file.

## Understanding BMP Images

- **Pixels and Colors**
  - Each image is made up of a grid of dots called **pixels**.
  - Each pixel represents a color. In black and white images, pixels represent either black or white.
  - In color images, especially those supporting 24-bit color like BMP, JPG, or PNG, each pixel uses 24 bits to represent color:
    - **8 bits for Red (R)**
    - **8 bits for Green (G)**
    - **8 bits for Blue (B)**
  - The values for each R, G, and B component are in hexadecimal. For example:
    - `0xff, 0x00, 0x00` represents a purely red pixel (255, 0, 0).

- **File Structure**
  - A file is essentially a sequence of bits.
  - A **24-bit BMP file** contains metadata followed by the actual bitmap data:
    - **BITMAPFILEHEADER (14 bytes):** Contains information like the file type, size, and offset where the pixel array begins.
    - **BITMAPINFOHEADER (40 bytes):** Provides details about the image, such as its width, height, color depth, and compression type.
    - **Bitmap Data:** An array of bytes where each set of three bytes represents a pixel's color in BGR (Blue, Green, Red) format.
  - **Example of a 24-bit Smiley BMP:**
  
    ```
    ffffff ffffff 0000ff 0000ff 0000ff 0000ff ffffff ffffff

    ffffff 0000ff ffffff ffffff ffffff ffffff 0000ff ffffff

    0000ff ffffff 0000ff ffffff ffffff 0000ff ffffff 0000ff

    0000ff ffffff ffffff ffffff ffffff ffffff ffffff 0000ff

    0000ff ffffff 0000ff ffffff ffffff 0000ff ffffff 0000ff

    0000ff ffffff ffffff 0000ff 0000ff ffffff ffffff 0000ff

    ffffff 0000ff ffffff ffffff ffffff ffffff 0000ff ffffff

    ffffff ffffff 0000ff 0000ff 0000ff 0000ff ffffff ffffff
    ```

## What Does It Mean to Filter an Image?

Filtering an image involves modifying each pixel of the original image to produce a particular effect in the resulting image. This can include converting the image to grayscale, applying a sepia tone, reflecting the image horizontally, or blurring the image.

## Files

### `bmp.h`

- **Header Definitions:**
  - **`BITMAPFILEHEADER` and `BITMAPINFOHEADER`:** Structures that store metadata about BMP files, such as image dimensions and color depth.
  - **Data Types:** Defines `BYTE`, `DWORD`, `LONG`, and `WORD` as aliases for standard C/C++ primitive data types, enhancing code readability and portability.
  - **`RGBTRIPLE`:** A structure that encapsulates three bytes representing a pixel's color in the order of Blue, Green, and Red (BGR).

- **Purpose of Structs:**
  - Structs allow us to interpret a sequence of bytes in a file as meaningful data structures. Instead of handling raw byte arrays, structs enable easier access and manipulation of file data by assigning meaningful names to byte sequences.

### `filter.c` & `helpers.c`

- **`filter.c`:** Contains the `main` function, handling command-line arguments, file operations, and invoking the appropriate filter functions based on user input.
- **`helpers.c`:** Implements the filter functions (`grayscale`, `sepia`, `reflect`, and `blur`) that manipulate the pixel data to achieve various image effects.

### `Makefile`

Automates the compilation process, ensuring that all source files are correctly compiled and linked to produce the final executable.

## Compilation

To compile the project, navigate to the project directory in your terminal and run:

```bash
make
