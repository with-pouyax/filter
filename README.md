# BMP image filters

A command-line C program for processing 24-bit BMP images. It reads pixel data, applies one transformation and writes a new BMP.

| Flag | Transformation |
| --- | --- |
| `-g` | Grayscale |
| `-s` | Sepia |
| `-r` | Horizontal reflection |
| `-b` | Blur |

## Run

```sh
make
./filter -g images/input.bmp output.bmp
```

Choose an existing input under [`images/`](images). The program expects one filter flag, an input path and an output path. Source: [`filter.c`](filter.c) handles BMP I/O and arguments; [`helpers.c`](helpers.c) transforms pixel arrays; [`bmp.h`](bmp.h) defines the file structures.

**Scope:** Supports the BMP layout validated in the source; it is not a general JPEG/PNG editor. [License](LICENSE).
