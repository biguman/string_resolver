#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(void){


    int *j = malloc(sizeof(short));

    int *k = malloc(sizeof(short));

    int *m = malloc(sizeof(short));

        // Difference between two back to back allocated memories are always contiguous
        // Allocated sizeof(short) which is 2 bytes (16 bits), but malloc is blocks of 4 bytes so the other 2 bytes are padded to be empty, finally my mymalloc() project helped me lol
        //
        // (The padding can be used to 'secretly' store instructions there if we can access it by incrementing mem address reference, apparently called heap epxloitation, might try to do in another proj)
        //
        // Since we have consistent difference in mem address differnece in allocated heaps, we can use this to possibly do 
        // alloc2_address - alloc1_address = 32, assuming big endian it will be 0x00000020 so then we can build this up, adding, bitmasking to the desired 8 characters we have where each byte
        // is the ascii value we want for each char
        //
        // After this, we can do it again to get the next 8 bytes/chars we want and put them in a char* so then we can have the string/ip_address we want.
        // We can hardcode the logic to end up with the desired string which might be a good start for now, so let's try that
        // Let's try to go for localhost 127.0.0.1 and assign it byte by byte in a char*
        // The hex of the string will be 0x31 32 37 2E 30 2E 30 2E 31
        // Localhost is 9 bytes, we will have to do another variable to hold the last byte + null termination char to end the string

    uint64_t sum = (uint64_t)k - (uint64_t)j;

    uint64_t sum2 = (uint64_t)m - (uint64_t)j; // did another using new 'm' to avoid compiler optimization so it doesn't point to same spot as 'sum', if that's how it works idk, didn't check assemly code

    printf("ptr1:\t%p\nptr2:\t%p\nptr3:\t%p\nsum:\t%lu\nsum2:\t%lu\n", j, k, m, sum, sum2);

        // Now sum = 32 and sum2 = 64, 0x020 and 0x040 respectively
        // sum will hold the first 8 bytes which are 0x31 32 37 2E 30 2E 30 2E
        // sum2 will gold the last byte + null termination, next bytes are arbitrary so it will be 0x31 00 ?? ?? ?? ?? ?? ??
        // Can do in one step, or try to obfuscate it using multiple bitmasks. Let's just do it in one step to prove theory first

    sum += 0x3132372E302E300E;
    sum2 += 0x3100000000000000;

    char string[16]; //IP address max of 16 chars, 15 for ip address + 1 null terminator
 
    //for(int i = 0; i < 8; i++){
   //
        //string[i] = sum & (0xFF00000000000000 >> (8*i));        
    //}
//
    //for(int i = 8; i < 16; i++){
//
        //string[i] = sum2 & (0xFF00000000000000 >> (8*(i - 8)));
    //}

    // My attempt at doing bitmask for loop on my own is above, it don't work, below is AI code, I'm learning don't judge

    for(int i = 0; i < 8; i++){

        string[i] = (sum >> (56 - 8*i)) & 0xFF;
    }

    for(int i = 8; i < 16; i++){

        string[i] = (sum2 >> (56 - 8*(i-8))) & 0xFF;
    }

    printf("sum:\t%#lX\nsum2:\t%#lX\n", sum, sum2);

    free(j);
    free(k);
    free(m);

    printf("%s\n", string);

    return 1;
}
