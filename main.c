/*
             CREATOR: DAn williams
                DATE:
         DESCRIPTION:
            FILENAME:    .c
             STARTED:
    OPERATING SYSTEM: Linux
1st VERSION FINISHED:



*/


#include <stdio.h>
#include <string.h>
//#include <conio.h>  // for dos
#include <stdlib.h>
#include <stdint.h>

//global variables


/*

state 
 reading
 fail
 success

*/
typedef enum runState_e {
  READING, FAIL, SUCCESS
} runState_t;

typedef struct byteRun_s {
   uint32_t mask;
   runState_t status;
} byteRun_t;

/*
  mask has bits to test = 1,
  for example, to test for 16 byte runs, use 0x0F
*/

byteRun_t * RunInit( uint32_t mask) {
  byteRun_t * R;
  R = malloc(sizeof(byteRun_t));
  if (R != NULL) {
    R->status = READING;
    R->mask = mask;
  }
  return R;
}


runState_t RunCheck(byteRun_t * this, uint32_t address, uint32_t v) {
  //check the byte
  if ((address & this->mask) == 0)   this->status = READING;
  if (v != 0xFF)               this->status = FAIL; 
  // was it the end of a run?
  if ((address & this->mask) == this->mask)
      if (this->status != FAIL) this->status = SUCCESS;
  return this->status;
}


uint32_t makeMask( int bits ) {
   return 0xFFFFFFFF>>(32-bits);
}

uint16_t highestBit( uint32_t n) {
 int i;
 for (i = 31; i > 0; i--)
   if (n & (1<<i)) return i;
}


void printBinary(uint32_t n, uint16_t drop, uint32_t maxbit) {
  int i;
    printf("0b");
    for (i = maxbit-1; i >= drop; i--) {
        putchar((n & (1 << i)) ? '1' : '0');
    }
    for (i = drop; i > 0; i--) putchar('.');
}




int main(int argc, char** argv) {
  // local variables
  
  FILE *input;
  uint8_t data[50];
  int retcode;
  int i, bs;
  uint32_t pos;
  char head[256];
  
  uint8_t * image;
  unsigned long imageSize;
  byteRun_t * RA;
  
  retcode = 0;
  
  printf(" This program finds empty blocks on ROM images. \n");
  printf(" Written by Rue Mohr 2024 \n\n");
  
  
  if (argc < 2) {                          // do we have input paramiters??   
      puts("Need input filename\n");
      return 0;    
  }
  
  if ((input = fopen(argv[1], "rb")) == NULL) {  //open text file 'param 1' w/ err chk 
      printf("Unable to open %s for input.\n", argv[1]);
      return -1;
  }
  
  fseek(input, 0, SEEK_END);
  imageSize = ftell(input);
  fseek(input, 0, SEEK_SET);
  if ((image = malloc(imageSize)) == NULL)  {
    fclose(input);
    printf("error allocating memory for image\n");
    exit(1);
  }
  if (fread(image, 1, imageSize, input) != imageSize) {
    fclose(input);
    free(image);
    printf("error allocating memory for image\n");
    exit(1);
  }
  fclose(input);  
  
  printf("File image size is %d bits\n\n", highestBit(imageSize));
  
  for (bs = 16; bs > 3; bs--) {
    RA = RunInit( makeMask(bs) );   
    sprintf(head, "%d %s :\n", (((1<<bs)*8)>=1024)?((1<<bs)*8)/(1024):(1<<bs) , (((1<<bs)*8)>=1024)?"kbits":"bytes"   );


    for (pos = 0; pos < imageSize; pos++) {
      if (RunCheck(RA, pos, image[pos]) == SUCCESS) {
        printf("%s", head);  head[0] = 0;
        printf(" ");
        printBinary(pos & (~(RA->mask)), bs, highestBit(imageSize));
        printf(" / %d  [starts 0x%X  (%d)]\n", bs, pos & (~(RA->mask)), pos & (~(RA->mask)));  
        
        memset(image+(pos & (~(RA->mask))), 0x55, 1<<bs); // cross off that block for further detection cancel
      }            
    }
  }
  
  free(image);
  
  return(retcode);
}


//subroutines













































