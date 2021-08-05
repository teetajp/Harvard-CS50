#include <cs50.h>
#include <stdio.h>
#include <math.h>

/*
Asks the user how much change is owed and then prints
the minimum number of coins with which that change can be made.
*/
int main(void)
{
    float dollars;
    //Ensure that the float is non-negative.
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
    printf("\n");
    //Round and convert dollars to cents
    int cents = round(dollars * 100);
    int coins = 0;
    while (cents > 0)
    {
        //Subtract max number of quarters and add to counter
        for (int i = 0; cents - 25 >= 0; i++, coins++)
        {
            cents -= 25;
        }
        //Subtract max number of dimes and add to counter
        for (int i = 0; cents - 10 >= 0; i++, coins++)
        {
            cents -= 10;
        }
        //Subtract max number of nickels and add to counter
        for (int i = 0; cents - 5 >= 0; i++, coins++)
        {
            cents -= 5;
        }
        //Subtract max number of pennies and add to counter
        for (int i = 0; cents - 1 >= 0; i++, coins++)
        {
            cents -= 1;
        }
    }
    //Print out answer
    printf("%i\n", coins);
}