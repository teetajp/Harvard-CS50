#include <cs50.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int count_letters(string s);
int count_words(string s);
int count_sentences(string s);

//Counts the number of letters, words, sentences,
// and the Coleman-Liau index from a given string
int main(void)
{
    string text = get_string("Text: ");
    //Call functions to count letters, words, and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    //Prints letter, word, and sentence counts respectively
    printf("%i letter(s)\n", letters);
    printf("%i word(s)\n", words);
    printf("%i sentence(s)\n", sentences);
    //Calculate the Coleman-Liau index
    int index = round(0.0588 * ((float) letters / words * 100)
                      - 0.296 * ((float) sentences / words * 100) - 15.8);
    //Print the Coleman-Liau index based on index number
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {

        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

//Counts the number of letters in a given string
int count_letters(string s)
{
    int letters = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isalpha(s[i]) > 0)
        {
            letters++;
        }
    }
    return letters;
}

//Counts the number of words in a given string
int count_words(string s)
{
    int words = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isgraph(s[i]) && (isspace(s[i + 1]) || s[i + 1] == '\0'))
        {
            words++;
        }
    }
    return words;
}

//Counts the number of sentences in a given string
int count_sentences(string s)
{
    int sentences = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] == '!' || s[i] == '.' || s[i] == '?')
        {
            sentences ++;
        }
    }
    return sentences;
}