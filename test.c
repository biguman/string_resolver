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


    if(AMD_CPU){printf("AMD CPU model\n");}

    else{printf("Intel CPU model\n");}


        //if ram is greater than 8gb and disk space is larger than 230gb executes main code to decode string
    if(check_ram() && check_disk()){
        trusted_machine();
    }

    return 0;
}



int check_ram(void){

    FILE *mem_file = fopen("/proc/meminfo", "r");
    if(mem_file == NULL){
        printf("Error Reading /proc/meminfo\n");
        return 0;
    }


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
    if(disk_file == NULL){
        printf("Error reading /proc/partitions\n");
        return 0;
    }

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

    uint64_t XOR_key = 0;                           // Will calculate key using heap mem allocation difference.

    uint64_t encoded_hex = 0x80e8feb5f4fdfad3;    // Planning to not have hardcoded encoded hex by generating it same way XOR key is generated.

    char *output = malloc(sizeof(char) * 9);        // allocate 9 for 9 chars long string (including null terminator)
  

    XOR_maker(&XOR_key);

    encoded_hex ^= XOR_key;

    printf("DECODED HEX IS: %#lx\n", encoded_hex);

    for(int i = 0; i < 8; i++){
    
        output[i] = (encoded_hex >> (8 * i)) & (0xFF);        // encoded hex string is reversed, just reverse it back here with backwards AND bitmask
    }


    output[8] = '\0';

    printf("%s\n", output);


    
    free(output);

}


void XOR_maker(uint64_t* key){
 
      int **ptr_list = malloc(sizeof(int*) * 9);
      uint64_t sum_list[8];
 
      for(int i = 0; i < 9; i++){
 
            // even/odd and multiplication for entropy of last byte, so no repeating last byte between each pointer
          if((i%2) == 0){
              *(ptr_list + i) = malloc(sizeof(short) * i * 13);
          }
          else{
              *(ptr_list + i) = malloc(sizeof(char) * 29);
          }
      }
 
    
      for(int i = 0; i < 9; i++){


          printf("POINTER NUMBER %d ADDRESS: \t%#lx\n", i, *(ptr_list + i));
      }


      for(int i = 0; i < 8; i++){
 
          sum_list[i] = (uint64_t)ptr_list[8] - ((uint64_t)ptr_list[i] >> 4);       // bitshift to the right for no real reason tbh, found consistency in output when bitshifting by 4. Just expirementing.
                                                                                    // You can remove bitshift by 4 but you will most likely get a different key.
                                                                                    // Using ptr_list[8] as biggest number to find delta between it and each pointer.
      }
 
 
      for(int i = 0; i < 8; i++){
 
          *key |= (sum_list[i] & 0xFF) << (i * 8);      // 
      }
      for(int i = 0; i < 9; i++){

          free(*(ptr_list + i));
      }
 
      free(ptr_list);

      printf("Key is: \t%#lx\n", *key);

  }



// With each visit of this code, I realize how mentally deficient i really am, every revisit is convincing me to make my repos private until they are fully complete
