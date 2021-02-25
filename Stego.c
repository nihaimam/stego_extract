/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * Stego.c: A program for manipulating images                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <string.h>
#include "image.h"

unsigned char getlsbs(unsigned char *p);
void setlsbs(unsigned char *p, unsigned char b0);

int main(int argc, char *argv[])
{  

  int i, j, cover_bits, bits;
  struct Buffer b = {NULL, 0, 0};
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
  //byte b0 = b.size;

  if (argc != 4) 
  {
    printf("\n%s <cover_file> <stego_file> <file_to_hide> \n", argv[0]);
    exit(1);
  }
  
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros
  ReadBinaryFile(argv[3],&b);    // Read binary data
 

  // hidden information

  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  if (!GetColor)
  {
    cover_bits = img.NofC*img.NofR;
  }
  else
  { 
    cover_bits = 3*img.NofC*img.NofR;    
  }
  bits = (8 + b.size)*8;
  if (bits > cover_bits)
  {
    printf("Cover file is not large enough %d (bits) > %d (cover_bits)\n",bits,cover_bits);
    exit(1);
  }

  byte b0 = b.size;

  int getctr = 0; // counter to keep track of get gray function call
  int setctr = 0; // counter to keep track of set gray function call
  
  // embed four size bytes for the Buffer's size field
  for (int i = 0; i < 4; i++)
  {
    unsigned char arr[8]; // create an array of eight unsigned chars
    for (j = 0; j < 8 ; j++)
    {
      arr[j] = GetGray(getctr); // get and save byte from the source image as an unsigned char in array
      getctr++; // increment the get gray counter by one
    }
    setlsbs(arr,b0); // replace the lsbs of each byte of array by the bits of b0
    for(j = 0; j < 8; j++)
    {
      SetGray(setctr, arr[j]); // set byte in the destination image with the unsigned char from array
      setctr++; // increment the set gray counter by one
    }
    b0 = b0 >> 8; // shift b0 to the right by 8
  }

  // embed the eight digits of your G# using 4 bits per digit
  int g[8] = {0,1,0,1,7,0,9,2}; // array of my g number
  int num = 0; 
  for (int i = 0; i < 4; i++)
  {
    unsigned char arr[8]; // create an array of eight unsigned chars
    for (j = 0; j < 8 ; j++)
    {       
      arr[j] = GetGray(getctr); // get and save byte from the source image as an unsigned char in array
      getctr++; // increment the get gray counter by one
    }
    int newnum  = g[num] << 4; //
    num++; // increment num by one
    int newbits = newnum| g[num]; //
    num++; // increment num by one
    setlsbs(arr,newbits); // replace the lsbs of each byte of array by the bits of newbits
    for(j = 0; j < 8; j++)
    {
      SetGray(setctr, arr[j]); // set byte in the destination image with the unsigned char from array
      setctr++; // increment the set gray counter by one
    }
    b0 = b0 >> 8; // shift b0 to the right by 8	
  }

  // embed information into the image one byte at the time
  // change only the least significant bits of the image  
  for (i = 0; i < b.size; i++)
  {
    unsigned char arr[8]; // create an array of eight unsigned chars
    for (j = 0; j < 8; j++)
    {
      arr[j] = GetGray(getctr); // get and save byte from the source image as an unsigned char in array
      getctr++; // increment the get gray counter by one
    }
    setlsbs(arr,(GetByte(i))); // replace the lsbs of each byte of array by getting the byte of i
    for (j = 0; j < 8; j++)
    {
      SetGray(setctr, arr[j]); // set byte in the destination image with the unsigned char from array
      setctr++; // increment the set gray counter by one
    }
  }

  WriteImage(argv[2],img);  // output stego file (cover_file + file_to_hide)

}
