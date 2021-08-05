#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Declare BYTE as a new type to store 8-bit unsigned integers
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    // Open Memory Card for reading 
    FILE *f = fopen(argv[1], "r");
    
    // Check if the file is invalid and tell user it is and return 1
    if (f == NULL)
    {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 1;
    }
    
    int foundJpeg = 0; // 0 for false, 1 for true
    int jpegNum = 0;
    char imgName[8]; // string to store output File name
    FILE *img = NULL; // output File location
   
    // The block size of a FAT file system is 512 Bytes
    int blockSize = 512;
    
    //Allocate memory for the buffer and make an array of BYTES with blockSize elements
    BYTE *buffer = calloc(blockSize, sizeof(BYTE));
    
    if (buffer == NULL)
    {
        printf("Not enough memory... Aborting.\n");
        return 2;
    }
    
    while (fread(buffer, blockSize, sizeof(BYTE), f) == 1)
    {

        // Read a 512 Byte block from the file

        // Check if first four bytes of the block is a JPEG header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If this is the first JPEG to be written
            if (jpegNum == 0)
            {
                // Set filename to ###.jpg where ### is the JPEG number
                sprintf(imgName, "%03i.jpg", jpegNum); 
                // if first JPEG, then start writing 000 into first file
                img = fopen(imgName, "w");
                // Write BYTES from the input file to the new JPEG located at img.
                fwrite(buffer, blockSize, sizeof(BYTE), img);
                jpegNum++;
                foundJpeg = 1;
            }
            // If this is not the first JPEG to be written, close other file and write new one
            else
            {
                fclose(img);
                // Set filename to ###.jpg where ### is the JPEG number
                sprintf(imgName, "%03i.jpg", jpegNum); 
                img = fopen(imgName, "w");
                printf("%s\n", imgName);
                fwrite(buffer, blockSize, sizeof(BYTE), img);
                jpegNum++;
            }
        }
        else if (foundJpeg == 1) // if already found jpeg, keep writing to jpeg
        {
            fwrite(buffer, blockSize, sizeof(BYTE), img);
        }
    }
    
    // Deallocate memory from buffer
    free(buffer);
    
    // Close any remaining file that was opened for writing
    if (img != NULL)
    {
        fclose(img);
    }
    
    // Close the input file for reading
    fclose(f);
}
