#include <stdio.h>
#include <cs50.h>

//Makes # towers based of a inputted height of between 1 to 8
int main(void)
{
    int n = 0;
    //Only allow heights of 1 to 8
    while (n < 1 || n > 8)
    {
        n = get_int("Height: ");
    }
    //The for loop with int i is for each row
    for (int i = 0; i < n; i++)
    {
        //Makes the spaces at the beginning of height - 1
        for (int j = 0; j < n - i - 1; j++)
        {
            printf(" ");
        }
        //Prints # blocks based on row number
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        //Double spacing in between towers
        printf("  ");
        //Prints # blocks based on row number
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}