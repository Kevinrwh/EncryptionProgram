/*
 Kevin Ramos
 CIS3360
 HW1
 */

/*
    This programs reads plaintext and key files and encrypts the message using a Hill Cipher method.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSIZE 10000

int main(int argc, char **argv) {

    int i, j, letter, size, charsStored = 0;
    char c;
    char plaintext[MAXSIZE];
    
    // Create our files
    char* fname1 = argv[ 1 ];
    char* fname2 = argv[ 2 ];
    
    // Open the files in read mode
    FILE *file1 = fopen( fname1, "r" ); // p.txt
    FILE *file2 = fopen( fname2, "r" ); // k.txt
    
    // Read and store the characters needed for our plaintext array.
    while( ( c = getc(file1) ) != EOF)
    {
        if( !isspace(c) && !ispunct(c) && !isnumber(c) ) // ignore spaces, punctuation, and numbers
        {
            letter = tolower(c);
            plaintext[charsStored] = letter;
            charsStored++;
        }
    }
    
    // Get the dimensions of the key from the file
    fscanf(file2, "%d", &size);
    
    // Determine the amount of padding required, which can be found by subtracting the remainder from the dimension
    int remainder = charsStored%size;
    int numPadding = size - remainder;
    
    // Determine the length after padding and do the padding.
    int afterPaddingLength = charsStored + numPadding;
    for(i = charsStored; i < afterPaddingLength; i++)
    {
        plaintext[i] = 'x';
    }
    printf("\n");
    
    // Determine the number of blocks/vectors
    int numBlocks = afterPaddingLength / size;

    // Read the key matrix, given the dimensions.
    printf("Key Matrix:\n");
    int key[size][size];
    
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            fscanf(file2, "%d", &key[i][j]);
        }
    }
    
    // Output the key matrix.
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            printf("%d ", key[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    
    // Print the new plaintext, 80 rows per line.
    printf("Plaintext:\n");
    for(i = 0; i < afterPaddingLength; i++)
    {
        printf("%c", plaintext[i]);
        if(i != 0 && i%80 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    
    /*
        Encryption portion. We take into account that letters 'a'-'z' have ASCII values 97-122, respectively.
        Subtracting 97 from the letter value gives us a desired value 0-25. We perform linear algebra on
        plaintext vectors of size size against the key matrix.
     */
    
    int length = afterPaddingLength, value = 0;
    int k = 0, l = 0, m = 0, o = 0;
    int tempMatrix[size][size];
    char encMessage[afterPaddingLength];
    char ch;
    int sum = 0, count = 0, keyVal = 0;
    i = 0;
    int blocks = 0;

    // Iterate through the length of the plaintext and the number of blocks determined earlier.
    while(i < length && blocks < numBlocks)
    {
        // Create a temporary matrix for the plaintext
        for(j = 0; j<size; j++) // rows
        {
            for(k = 0; k < size; k++) // columns
            {
                value = plaintext[i] - 97; // get the value to store in the tempMatrix from the plaintext
                tempMatrix[j][k] = value;
                i++; // get the next plaintext value
            }
        }
        
        // Iterate through our temporary matrix and the key matrix to get our encrypted characters.
        for(o = 0; o < size; o++) // iterate through vectors
        {
            for(l = 0; l < size; l++) // iterate through keyMatrix rows
            {
                sum = 0; // set a running sum to 0
                for(m = 0; m < size; m++) // iterate through keyMatrix columns
                {
                    keyVal = key[l][m]; // retrieve the value at the proper location in keyMatrix
                    sum += keyVal*tempMatrix[o][m]; // linear algebra portion
                }
                
                // Mod the sum by 26
                sum %=26;
                
                // Add 97 to get our lowercase char ASCII value
                sum += 97;

                // Append the letter to our encrypted message.
                ch = sum;
                encMessage[count] = ch;
                count++;
            }
            blocks++; // completion of one of the blocks
        }
    }
    
    printf("\nCiphertext:\n");
    
    // print out our encrypted message!
    for(int w = 0; w < afterPaddingLength; w++)
    {
        printf("%c", encMessage[w]);
        if(w != 0 && w%80 == 0)
        {
            printf("\n");
        }
    }
    
    printf("\n");
    
    return 0;
}
