#include "helpers.h"
#include <math.h> // Needed for round function
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) // Iterate over each row
    {
        // Iterate over each pixel in the row
        for (int j = 0; j < width; j++)
        {
            // Calculate the average of the red, green, and blue values
            float average =
                (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;

            // Round the average to the nearest integer
            BYTE gray = round(average);

            // Set the red, green, and blue values to the average
            image[i][j].rgbtRed = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtBlue = gray;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over each row
    for (int i = 0; i < height; i++)
    {
        // Iterate over each pixel in the row
        for (int j = 0; j < width; j++)
        {
            // Store original color values
            BYTE originalRed = image[i][j].rgbtRed;
            BYTE originalGreen = image[i][j].rgbtGreen;
            BYTE originalBlue = image[i][j].rgbtBlue;

            // Calculate sepia values using the sepia formula
            int sepiaRed =
                round(0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue);
            int sepiaGreen =
                round(0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue);
            int sepiaBlue =
                round(0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue);

            // Ensure the sepia values do not exceed 255
            image[i][j].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over each row
    for (int i = 0; i < height; i++)
    {
        // Iterate over half the width of the row
        for (int j = 0; j < width / 2; j++)
        {
            // Swap the pixel with its horizontally mirrored counterpart
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary copy of the image to store the blurred pixels
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL)
    {
        // If memory allocation fails, exit the function
        return;
    }

    // Iterate over each row
    for (int i = 0; i < height; i++)
    {
        // Iterate over each pixel in the row
        for (int j = 0; j < width; j++)
        {
            int totalRed = 0;
            int totalGreen = 0;
            int totalBlue = 0;
            int count = 0;

            // Iterate over the neighboring pixels (including the current pixel)
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if the neighboring pixel is within the image boundaries
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        totalRed += image[ni][nj].rgbtRed;
                        totalGreen += image[ni][nj].rgbtGreen;
                        totalBlue += image[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            // Calculate the average color values
            temp[i][j].rgbtRed = round((float) totalRed / count);
            temp[i][j].rgbtGreen = round((float) totalGreen / count);
            temp[i][j].rgbtBlue = round((float) totalBlue / count);
        }
    }

    // Copy the blurred pixels back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    // Free the temporary memory
    free(temp);
}
