/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * StegoExtract.c: A program for manipulating images                    *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <string.h>
#include "image.h"

unsigned char getlsbs(unsigned char *p);

int main(int argc, char *argv[])
{  
  int i, j, cover_bits, bits;
  struct Buffer b = {NULL, 0, 0};
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
  //byte size = 0;
   
  if (argc != 3) 
    {
      printf("\n%s <stego_file> <file_to_extract> \n", argv[0]);
      exit(1);
    }
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros

  // hidden information 
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else 
    cover_bits = 3*img.NofC*img.NofR;

  byte size = 0;  
  
  // extract four size bytes for the Buffer's size field
  int getctr = 0; // counter to keep track of get gray function call

  for (i = 0; i < 4; i++)
  {
    unsigned char arr[8]; // create an array of eight unsigned chars
    for (j = 0; j < 8; j++)
    {
      arr[j]= GetGray(getctr); // get and save byte from the source image as an unsigned char in array
      getctr++; // increment the get gray counter by one	
    }			
    size = getlsbs(arr) + size; // increment size by the return of getlsbs of array
  }

  // extract the eight digits of your G# using 4 bits per digit
  int gnum = 0; //initial g number
  printf("G number is "); // print g number
  for (i = 0; i < 4; i++)
  {
    unsigned char arr[8]; // create an array of eight unsigned chars
    for (j = 0; j < 8; j++)
    {
      arr[j] = GetGray(getctr); // get and save byte from the source image as an unsigned char in array
      getctr++; // increment the get gray counter by one
    }
    gnum = getlsbs(arr);
    int first = gnum >> 4; // shift g number right by four
    int second = gnum ^ (first << 4); // raise g number to the power of first
    printf("%d%d", first, second); // print g number
  }
  printf("\n");
	
  // here you extract information from the image and write to the output file
  FILE *infile;
  infile = fopen(argv[2], "wb"); // open infile 
  if (infile == NULL) // check if infile is null or not
  {
    printf("error opening the file\n");
    exit(0);
  }
  char c[size]; // create an array of chars with length size
  for (i = 0; i < size; i++)
  {
    unsigned char arr[8]; // create an array of eight unsigned chars
    for (j = 0; j < 8; j++)
    {
      arr[j] = GetGray(getctr); // get and save byte from the source image as an unsigned char in array
      getctr++;	// increment the get gray counter by one
    }
    c[i] = getlsbs(arr); // store the extracted data in the char arrya	
  }

  fwrite(c, sizeof(size), (sizeof(c)/sizeof(c[0])), infile); // write to the output payload file
  
  fclose(infile); // close the file
  return 0;
}
