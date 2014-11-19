/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"
#define LENGTH 45
#define TableSize 1000

/**
* Create the node structure for seperate chaining
*/
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}node;

// create the hash table
node* table[TableSize] = {NULL}; 

// global variable for dictionary size
int dictSize = 0; 

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // store the length of the word
    int length = strlen(word);
    
    // temp buffer to store the word
    char buffer[LENGTH + 1];
    
    // store the lower case version of the text word in buffer
    for(int i = 0; i < length; i++)
    {
        buffer[i] = tolower(word[i]);
    }
    // append the end of string character to the end of the string
    buffer[length] = '\0';
    
    // find the key to search in hash table by hashing the word
    int key = hash_function(buffer, length);
    
    // point to that index of the hash table
    node* curr = table[key];
    
    // traverse the linked list of that index in the hash table and look for the text word
    while(curr != NULL)
    {
        if(strcmp(buffer, curr -> word) == 0)
        {
            return true;
        }
              
        curr = curr -> next;
            
   }
    
    // if the loop ends without returning, the word is not in the dictionary and so misspelled
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
   
    // open file
    FILE *fp;
    fp = fopen(dictionary, "r");
    
    // ensure file is opened properly
    if(fp == NULL)
        return false;
    
    while(!feof(fp))
    {
        // allocate space on the heap for a new node - a new dict word 
        node* new_node = malloc(sizeof(node));
        
        // read word from file and store it in the node
        fscanf(fp, "%s", new_node -> word);
        
        // increment the dictionary size variable
        dictSize++;
        
        // hash new_node -> word
        int key = hash_function(new_node -> word, strlen(new_node->word));
        
        // store the node in the first index of the hash table if it is empty, other wise append that linked list
        if(table[key] == NULL)
        {
            table[key] = new_node;
            new_node -> next = NULL;
        }
        else
        {
            new_node -> next = table[key];
            table[key] = new_node;
        }
        
    }    
    
    // close file
    fclose(fp);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dictSize - 1;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    node* current;
    node* prev;    
    
    for(int i = 0; i < TableSize; i++)
    {
        current = table[i];
        while(current != NULL)
        {
            prev = current;
            current = current -> next;
            free(prev);
        }
    }
    return true;
}

/*
* Hash function courtesy of wikipedia, jenkins one at a time algorithm. I decided to search algorithms
* using bitwise operators as they drastically improved speed
*/
int hash_function(const char *word, int len)
{
    unsigned int hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += word[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % TableSize;
}
