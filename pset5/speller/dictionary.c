// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 75000;

// Hash table
node *table[N];

// Number of words loaded
unsigned int loadedWords = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Convert word to lowercase to be used for hashing
    char tmpWord[strlen(word) + 1];
    strcpy(tmpWord, word);
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (isupper(tmpWord[i]))
        {
            tmpWord[i] = tolower(word[i]);
        }
    }
    
    // Hash word to obtain hash value
    unsigned int index = hash(tmpWord);
    node *traverse = table[index];
    while (traverse != NULL)
    {
        if (strcmp(traverse->word, tmpWord) == 0)
        {
            return true;
        }
        else
        {
            traverse = traverse->next;
        }
    } 
    return false;
}

// FUNCTION: Hashes word to a number
// SOURCE: Uses the djb2 hash written by Daniel Bernstein
// Link: http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    // djb2 hash
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;    /* hash * 33 + c */
    }
    // Reduce hash to fit within the specified bucket size
    while (hash >= N)
    {
        hash = hash % N;
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open the dictionary for reading and assign it to a pointer
    FILE *dict = fopen(dictionary, "r");
    // Check if there's any error
    if (dict == NULL)
    {
        printf("Eror opening dictionary\n");
        return false;
    }
    
    char readBuffer[LENGTH + 1];
    
    // Read strings from the file one by one until end of file
    while (fscanf(dict, "%s", readBuffer) != EOF)
    {
        node *new = malloc(sizeof(node)); // Allocate memory for a new node
        if (new == NULL)
        {
            printf("Not enough memory to make new node.\n");
            return false;
        }
        strcpy(new->word, readBuffer); // Copy the word from the readBuffer into the new node
        new->next = NULL; // Make the next pointer of new nowhere
        unsigned int index = hash(new->word); // Hash the word to obtain a hash value
        
        // If no nodes exist there already, insert node into a hash table at table[index] 
        if (table[index] == NULL)
        {
            table[index] = new;
        }
        else // If nodes already exist
        {
            // Insert a node into the front of the linked list at table[index];
            new->next = table[index];
            table[index] = new;
        }
        loadedWords++; // Update number of words loaded
    }
    fclose(dict); // Close the dictionary for reading
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return loadedWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *temp = table[i];
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(temp);
            temp = cursor;
        }
    }
    return true;
}
