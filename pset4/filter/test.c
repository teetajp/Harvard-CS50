#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Prototype for Swap function that takes two int paths as parameter
void swap(BYTE *a, BYTE *b);

// Compute G by square rooting Gx^2 + Gy^2
int computeG(int Gx, int Gy);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // for each pixel, add each channel value to shade
    // then divide shade by 3 and set all channels to shade
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int shade = 0;
            shade += image[h][w].rgbtBlue;
            shade += image[h][w].rgbtGreen;
            shade += image[h][w].rgbtRed;
            shade = round((float) shade / 3);
            image[h][w].rgbtBlue = shade;
            image[h][w].rgbtGreen = shade;
            image[h][w].rgbtRed = shade;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // For each row, swap pixels on horizontally opposite sides
    // Middle pixel in odd rows stay at the same spot
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            // Swap BLUE channel
            swap(&image[h][w].rgbtBlue, &image[h][width - w - 1].rgbtBlue);
            // Swap GREEN channel
            swap(&image[h][w].rgbtGreen, &image[h][width - w - 1].rgbtGreen);
            // Swap RED channel
            swap(&image[h][w].rgbtRed, &image[h][width - w - 1].rgbtRed);

        }
    }
    return;
}

// Blur image using box blur
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for an empty copy of image
    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (copy == NULL)
    {
        printf("Not enough memory to store temp image.\n");
        return;
    }
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Make temporary integer values to store the channel values as channel values can't exceed 255
            int tempBlue = 0;
            int tempGreen = 0;
            int tempRed = 0;
            int pixels = 0;
            // Make a loop for the box blur which encapsulates the grid of 3x3 around the pixel[h][w]
            for(int i = h - 1, n = h + 1; i <= n; i++)
            {
                if (i >= 0 && i < height)
                {
                    for(int j = w - 1, m = w + 1; j <= m; j++)
                    {
                        if (j >= 0 && j < width)
                        {
                            tempBlue += image[i][j].rgbtBlue;
                            tempGreen += image[i][j].rgbtGreen;
                            tempRed += image[i][j].rgbtRed; 
                            pixels++; // Add to pixel counter
                        }
                    }
                }
            }
            // Set the blurred copy of the image to be the average value of the pixels around [h][w]
            copy[h][w].rgbtBlue = round((float) tempBlue / pixels);
            copy[h][w].rgbtGreen = round((float) tempGreen / pixels);
            copy[h][w].rgbtRed = round((float) tempRed / pixels);
        }
    }

    // Overwrite the original image with the blurred image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtBlue = copy[h][w].rgbtBlue;
            image[h][w].rgbtGreen = copy[h][w].rgbtGreen;
            image[h][w].rgbtRed = copy[h][w].rgbtRed;
        }
    }
    free(copy); // Deallocate memory from copy now that we have overwritten the original
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for an empty copy of image
    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (copy == NULL)
    {
        printf("Not enough memory to store temp image.\n");
        return;
    }
    int sobel[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} }; // Make a Gx matrix from the Sobel operator
    for (int h = 0; h < 2; h++)
    {
        for (int w = 0; w < 2; w++)
        {
            // Make temporary integer values to store the channel values as channel values can't exceed 255
            // The gColor[] array will hold Gx in tempColor[0], Gy in tempColor[1], and G in tempColor[2];
            int gBlue[] = {0, 0, 0};
            int gGreen[] = {0, 0, 0};
            int gRed[] = {0, 0, 0};
            
            printf("INITIAL: image[%i][%i].rgbtBlue==%i\n", h, w, image[h][w].rgbtBlue);
            
            // Make a loop for the box blur which encapsulates the grid of 3x3 around the pixel[h][w]
            for(int i = h - 1, n = h + 1, x = 0; i <= n; i++, x++)
            {
                if (i >= 0 && i < height)
                {
                    for(int j = w - 1, m = w + 1, y = 0; j <= m; j++, y++)
                    {
                        if (j >= 0 && j < width)
                        {
                            printf("i:%i | j: %i | SobelX[%i][%i]=%i, SobelY[%i][%i]=%i\n", i, j, x, y, sobel[x][y], y, x, sobel[y][x]);
                            gBlue[0] += image[i][j].rgbtBlue * sobel[x][y];
                            gBlue[1] += image[i][j].rgbtBlue * sobel[y][x];
                            printf("%i * %i = gBlue[Gx] = %i\n", image[i][j].rgbtBlue, sobel[x][y], gBlue[0]);
                            printf("%i * %i = gBlue[Gy] = %i\n\n", image[i][j].rgbtBlue, sobel[y][x], gBlue[1]);
                            gGreen[0] += image[i][j].rgbtGreen * sobel[x][y];
                            gGreen[1] += image[i][j].rgbtGreen * sobel[y][x];
                            gRed[0] += image[i][j].rgbtRed * sobel[x][y]; 
                            gRed[1] += image[i][j].rgbtRed * sobel[y][x];
                        }
                    }
                }
            }
            printf("Gx=%i, Gy=%i\n",gBlue[0],gBlue[1]);
            gBlue[2] = computeG(gBlue[0], gBlue[1]);
            gGreen[2] = computeG(gGreen[0], gGreen[1]);
            gRed[2] = computeG(gGreen[0], gGreen[1]);
            // Set the blurred copy of the image to be the average value of the pixels around [h][w]
            copy[h][w].rgbtBlue = gBlue[2];
            copy[h][w].rgbtGreen = gGreen[2];
            copy[h][w].rgbtRed = gRed[2];
            printf("AFTER: copy[%i][%i].rgbtBlue==%i\n\n", h, w, copy[h][w].rgbtBlue);
        }
    }

    // Overwrite the original image with the blurred image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtBlue = copy[h][w].rgbtBlue;
            image[h][w].rgbtGreen = copy[h][w].rgbtGreen;
            image[h][w].rgbtRed = copy[h][w].rgbtRed;
        }
    }
    free(copy); // Deallocate memory from copy now that we have overwritten the original
    return;
}

// Swap channel values of two pixels
void swap(BYTE *a, BYTE *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int computeG(int Gx, int Gy)
{
    return round(sqrt(pow((float)Gx, 2) + pow((float)Gy, 2)));
}