#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

int check_ram(void);
int check_disk(void);
void trusted_machine(void);
void XOR_maker(uint64_t*);

int main(void){

    FILE *fp = fopen("/proc/cpuinfo", "r");
    if(!fp){
        perror("fopen");
        return 1;
    }

    bool AMD_CPU = false;
    bool INTEL_CPU = false;

    char line[256];

        // Random test to enumerate CPU model, just messing around, skip to line 128 for the start of the proper string and XOR_key obf
    while(fgets(line, sizeof(line), fp)){

        // strstr returns a pointer to the match, NULL if not found
        if(strstr(line, "model name")){

            // value is after the ": "
            char *val = strchr(line, ':');

            if(val){
                val += 2; // skip ": "
 
                if(strstr(val, "AMD")){AMD_CPU = true;} else{INTEL_CPU = true;}

                printf("Model: %s", val);
            }
            
            break; // remove break if you want all logical processors
        }
    }


    int *j = malloc(sizeof(short)); // These are useless legacy code but if i delete them I will have to redo XOR key n i dont wanna do that so this stays, just ignore these 3 mallocs :)

    int *k = malloc(sizeof(short));

    int *m = malloc(sizeof(short));

        // Difference between two back to back allocated memories are always contiguous
        // Allocated sizeof(short) which is 2 bytes (16 bits), but malloc is blocks of 32 bytes so the other 30 bytes are padded to be empty but only 24 bytes are usable 
        // found through expirement of using malloc_usable_size()
        //
        // Finally my mymalloc() project helped me
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

    uint64_t sum = (uint64_t)k - (uint64_t)j;   // Same thing in relation to the above mallocs, ignore these 2 sums

    uint64_t sum2 = (uint64_t)m - (uint64_t)j; // did another using new 'm' to avoid compiler optimization so it doesn't point to same spot as 'sum', if that's how it works idk, didn't check assemly code


    if(AMD_CPU){printf("AMD CPU model\n");}
    
    else{printf("Intel CPU model\n");}

        // Now sum = 32 and sum2 = 64, 0x020 and 0x040 respectively
        // sum will hold the first 8 bytes which are 0x31 32 37 2E 30 2E 30 2E
        // sum2 will gold the last byte + null termination, next bytes are arbitrary so it will be 0x31 00 ?? ?? ?? ?? ?? ??
        // Can do in one step, or try to obfuscate it using multiple bitmasks. Let's just do it in one step to prove theory first

    sum += 0x3132372E302E300E;
    sum2 += 0x3100000000000000;

    char string[16]; //IP address max of 16 chars, 15 for ip address + 1 null terminator. This will store 127.0.0.1
 
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

        if(string[i] == '\0'){break;}
    }


    // Above loop is to have string "127.0.0.1" dynamically, ignore ig.

    free(j);
    free(k);
    free(m);

    //printf("%s\n", string);
    
    short check = check_ram();
    short check2 = check_disk();


    if(check_ram() && check_disk()){
        

        trusted_machine();
    }
    
    fclose(fp);

    return 1;
}



int check_ram(void){

    FILE *mem_file = fopen("/proc/meminfo", "r");

    char search[256];
    
    fgets(search, sizeof(search), mem_file);

    char *match = strstr(search, "MemTotal:");

    uint64_t ram_kb;

    if(match){
        
        sscanf(match, "MemTotal: %lu kB", &ram_kb);
        
        ram_kb /= (1024*1024); // Change from kB to GB
    }

    if(ram_kb <7){return 0;} // If 7gb then return false, implying sandbox

    return 1; // RAM is 7gb or more meaning good chance of real pc. made 7 instead of 8 to assume after conversion from kB to GB it might round down due to integer.

}

int check_disk(void){

    FILE *disk_file = fopen("/proc/partitions", "r");

    char line[256];
    uint64_t disk_size;
   while(fgets(line, sizeof(line), disk_file)){

       char name[32];
            if(sscanf(line, "%*u %*u %lu %s", &disk_size, name) == 2){
            
                if(strcmp(name, "sdd") == 0){
                    break;
            }
        }
    }

   
   disk_size /= (1024);
   

   if(disk_size >= 230){return 1;} // Return true if disk size 230gb or greater, indicating real system
    else{return 0;} // Disk space less than 230gb
}



    // This is supposed to print "real pc" by the end of it.
void trusted_machine(void){

    uint64_t XOR_key;                           // our XOR decoding key for our "ascii constant", like the one we used earlier called "sum" and "sum2". Just another layer of obfuscation. 
                                                //Will calculate key using heap mem allocation difference like we did with "sum" pointers.

    uint64_t encoded_hex = 0xc0a8bef5b4bdba16;

    char *output = malloc(sizeof(char) * 8);
  

    XOR_maker(&XOR_key);

    encoded_hex ^= XOR_key;


    for(int i = 0; i < 8; i++){
    
        output[i] = (encoded_hex >> (8 * i)) & (0xFF);        
    }


    output[8] = '\0';

    printf("%s\n", output);


//    int size = malloc_usable_size(output) - 16; // Since we know malloc_usable_size returns 24, we adjust to make size = 8 to use in for loop. No fucking idea if this actually helps in obfuscation.
                                                // Will test later if this is useful "obfuscation"
    
//    int Number_of_ptrs;                         // int which contains number of pointers we will allocate
//    if(size%8 == 0){
//        
//        Number_of_ptrs = size/8;
//    }
//    else{Number_of_ptrs = (size/8) + 1;}        // Essentially a ceiling function


//    if(size%8 == 0){
//        int **ptr_list = malloc(sizeof(int*) * size);   // Using malloc_usable_size() purely for obfuscation, size of it will be computed during runtime instead of constant 8
//    }

//    for(int i = 0; i < Number_of_ptrs; i++){            // Assign a pointer (64 bit addressing) for each 8 bytes in the size of the 
//
//        *(ptr_list + i) = malloc(sizeof(short));
//    }
    

}


void XOR_maker(uint64_t* key){
 
      int **ptr_list = malloc(sizeof(int*) * 9);
      uint64_t sum_list[8];
 
      for(int i = 0; i < 9; i++){
 
          if((i%2) == 0){
              *(ptr_list + i) = malloc(sizeof(short) * i * 13);
          }
          else{
              *(ptr_list + i) = malloc(sizeof(char) * 29);
          }
      }
 
      for(int i = 0; i < 8; i++){
 
          sum_list[i] = (uint64_t)ptr_list[8] - ((uint64_t)ptr_list[i] >> 4);
      }
 
 
      for(int i = 0; i < 8; i++){
 
          *key |= (sum_list[i] & 0xFF) << (i * 8);
      }
 
 
  }



// With each visit of this code, I realize how mentally deficient i really am, every revisit is convincing me to make my repos private until they are fully complete
