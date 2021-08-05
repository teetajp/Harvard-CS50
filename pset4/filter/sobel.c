#include <stdio.h>

int main(void)
{
    int sobel[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    printf("Printing Gx\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i ", sobel[i][j]);
        }
        printf("\n");
    }
    printf("Printing Gy\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i ", sobel[j][i]);
        }
        printf("\n");
    }
}