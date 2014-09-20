/*********************************************************************************************************
This is an implementation of the Vigenere cipher. The user enters a key through the command line a text
to be encrypted through the terminal. The main implementaion here is a loop which converts each plaintext
character to its alphabetical index (0 - 25), and carries out a caeser cipher on that character. The key
is a string, in which the ith character is used to ecrypt the ith character of p. This is done by using 
the keys letters, turning them into their alphabetical equivalent, and "shifting" each p(i) by k(i)
**********************************************************************************************************/
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if((argc < 1 || argc > 2)|| argv[1] == NULL)
    {
        printf("Please enter only one cmd line arg\n");
        return 1;
    }
    
    string key = argv[1];//the key for the cipher
    string plainText = GetString();//get the plaintext to be encrypted
    int keyIndex = 0;//index to keep track of the key[i]
    int keyLength = strlen(key);//stores length of key
    
    for(int i = 0, n = strlen(key); i < n; i++)//validates key
    {
        if(isdigit(key[i]))
        {
            printf("Only alphabetical characters can be used as keys\n");
            return 1;
        }
    }//end for loop    
    
    for(int i = 0, n = strlen(key); i < n; i++)
    {
        if(islower(key[i]))
            key[i] -= 97;
        else
            key[i] -= 65;
    }//end for loop
    
    for(int i = 0, k = strlen(plainText); i < k; i++)//main implementation of the cipher
    {   
        if(isalpha(plainText[i]) != 0)
        {
            if(islower(plainText[i]))
            {
                plainText[i] -= 97;
                plainText[i] = (plainText[i] + key[keyIndex]) % 26;//implement caeser cipher 
                plainText[i] += 97;
            }
            else if(isupper(plainText[i]))//if p is an upper case
            {
                plainText[i] -= 65;//set p to its alphabetical index
                plainText[i] = (plainText[i] + key[keyIndex]) % 26;//implement caeser cipher on p(i) k(i)
                plainText[i] += 65;//return p to its ASCII index
            }
            keyIndex++;//index key[j]
        }
        
        if(keyIndex == keyLength)//if the value of keyIndex is larger than the length of key set ith index of key to zero
            keyIndex -= keyLength;//set key back to key[0]
    }//end for loop
    
    printf("%s\n", plainText);
    return 0;
}//end main
