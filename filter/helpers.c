#include "helpers.h"
#include <stdio.h>
#include <math.h>

int min(float a, int b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            long double avg = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            avg = roundl(avg);
            image[i][j].rgbtBlue  = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed   = avg;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed   = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaBlue  = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            image[i][j].rgbtRed    = min(sepiaRed, 255);
            image[i][j].rgbtGreen  = min(sepiaGreen, 255);
            image[i][j].rgbtBlue   = min(sepiaBlue, 255);
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][(width - 1) - j];

            image[i][(width - 1) - j].rgbtRed = tmp.rgbtRed;
            image[i][(width - 1) - j].rgbtGreen = tmp.rgbtGreen;
            image[i][(width - 1) - j].rgbtBlue = tmp.rgbtBlue;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float red_total = 0, green_total = 0, blue_total = 0, counter = 0;

            for (int row = -1; row <= 1; row++)
            {
                if (i + row >= 0 && row + i < height)
                {
                    for (int column = -1; column <= 1; column++)
                    {
                        if (column + j >= 0 && column + j < width)
                        {
                            red_total   += copy[row + i][column + j].rgbtRed;
                            green_total += copy[row + i][column + j].rgbtGreen;
                            blue_total  += copy[row + i][column + j].rgbtBlue;
                            counter++;
                        }

                    }
                }
                image[i][j].rgbtRed   = round(red_total / counter);
                image[i][j].rgbtGreen = round(green_total / counter);
                image[i][j].rgbtBlue  = round(blue_total / counter);

            }
        }
    }
    return;
}

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    float g[3][3] = {{-1.0, 0.0, 1.0},
                     {-2.0, 0.0, 2.0},
                     {-1.0, 0.0, 1.0}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gxred_total = 0, gxgreen_total = 0, gxblue_total = 0;
            float gyred_total = 0, gygreen_total = 0, gyblue_total = 0;
            int counter = 0;

            for (int k = 0; k < 3 ; k++)
            {
                if (k + i - 1 >= 0 && k + i - 1 < height)
                {
                    for (int l = 0; l < 3; l++)
                    {
                        if (j - 1 + l >= 0 && j - 1 + l < width)
                        {
                            gxred_total   += g[k][l] * copy[k - 1 + i][l - 1 + j].rgbtRed;
                            gxgreen_total += g[k][l] * copy[k - 1 + i][l - 1 + j].rgbtGreen;
                            gxblue_total  += g[k][l] * copy[k - 1 + i][l - 1 + j].rgbtBlue;

                            gyred_total   += g[l][k] * copy[k - 1 + i][l - 1 + j].rgbtRed;
                            gygreen_total += g[l][k] * copy[k - 1 + i][l - 1 + j].rgbtGreen;
                            gyblue_total  += g[l][k] * copy[k - 1 + i][l - 1 + j].rgbtBlue;
                        }
                        counter++;
                    }
                }
            }
            float red   = sqrt(gxred_total * gxred_total + gyred_total * gyred_total);
            float green = sqrt(gxgreen_total * gxgreen_total + gygreen_total * gygreen_total);
            float blue  = sqrt(gxblue_total * gxblue_total + gyblue_total * gyblue_total);

            image[i][j].rgbtRed   = min(red, 255);
            image[i][j].rgbtGreen = min(green, 255);
            image[i][j].rgbtBlue  = min(blue, 255);
        }
    }
    return;
}

int min(float a, int b)
{
    if (a > b)
    {
        return b;
    }
    return round(a);
}
