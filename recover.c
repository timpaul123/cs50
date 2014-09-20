/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <cs50.h>

#define RAW "card.raw"

int main(int argc, char* argv[])
{
    FILE* raw = fopen(RAW, "r");
    FILE* output;
    
    char fileName[8];
    
    uint8_t sig1[4] = {0xff, 0xd8, 0xff, 0xe1};
    uint8_t sig2[4] = {0xff, 0xd8, 0xff, 0xe0};
    
    int count = 0;
    bool jpg_found = false;
    
    uint8_t buffer[512];
    uint8_t check[4];
  
    while(fread(buffer, 512, 1, raw) > 0)
    {
       
        // store the first 4 bytes of the data
        for(int i = 0; i < 4; i++)
        {
            check[i] = buffer[i];
        }
        
        if(memcmp(check, sig1, 4) == 0 || memcmp(check, sig2, 4) == 0)
        {
            if(jpg_found == false)
            {
                jpg_found = true;
            
                sprintf(fileName, "%03i.jpg", count);
            
                output = fopen(fileName, "w"); 
                
                fwrite(buffer, sizeof(buffer), 1, output);
                
                count++;
            }
            
            else if(jpg_found)
            {
                fclose(output);
            
                sprintf(fileName, "%03i.jpg", count);
            
                output = fopen(fileName, "w"); 
                
                fwrite(buffer, sizeof(buffer), 1, output);
            
                count++;   
            }
        }
        
        else 
        {
            if(jpg_found)
            {    
                fwrite(buffer, sizeof(buffer), 1, output);
            }
        }
    }
    
    fclose(raw);
    if(output)
    {
        fclose(output);
    }
    
    return 0;
}


