#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Takes a key and enciphers a given plaintext to return a ciphertext
//  argv[] gets the key
int main(int argc, string argv[])
{
    //Check key length
    if (argc != 2)
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }
    // Check key lenght to make sure its 26 characters
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (!isalpha(argv[1][i])) // Check for non-alphabetic characters
            {
                printf("Key must contain only alphabetic characters.\n");
                return 1;
            }
            else // Check for duplicate characters
            {
                for (int j = i; j < n - 1; j++)
                {
                    // Lowercase both characters and compare
                    if (tolower(argv[1][i]) == tolower(argv[1][j + 1]))
                    {
                        printf("The key can only contain one of each character.\n");
                        return 1;
                    }
                }
            }
        }
    }
    // Get plaintext from the user
    string text = get_string("plaintext: ");

    //Cipher the text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            if (islower(text[i]))
            {
                text[i] = tolower(argv[1][text[i] - 97]);
            }
            if (isupper(text[i]))
            {
                text[i] = toupper(argv[1][text[i] - 65]);
            }
        }
    }
    printf("ciphertext: %s\n", text);
    return 0;
}