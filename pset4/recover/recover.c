#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"


//Check if the bit is set
bool is_bit_set(unsigned char c, int n)
{
    return (1 & (c >> n));
}

//Generate a file name with three digits
void generate_filename(char* filename, int counter)
{
    if(counter < 10)
        sprintf(filename, "00%d.jpeg", counter);
    else if(counter < 100)
        sprintf(filename, "0%d.jpeg", counter);
    else if(counter < 1000)
        sprintf(filename, "%d.jpeg", counter);
}

//Main functions
int main(int argc, char* argv[])
{
    //Check command line arguments
    if(argc != 2)
    {
        fprintf(stderr,"Usage: ./recover filename\n");
        return 1;
    }


   //Prepare file pointer
   FILE *input_file;
   
   //Open file for reading
   input_file = fopen(argv[1], "r");

    if(!input_file)
    {
        fprintf(stderr, "Could not open the specified file");
        return 1;
    }

   //Prepare buffer
   char buffer[512] = {};
   
   //Count for filenames 
   int counter = 0;

   //Is a file currently open
   bool fileopen = false;

   //Output file pointer
   FILE *output_file;
    
   //New file name
   char* new_filename = malloc(sizeof(char) * 10);
   generate_filename(new_filename, counter);

   //Read in one block at a time
   while(fread(buffer, 1, 512, input_file) == 512)
   { 

      //Check if start of file
       if(( (buffer[0] == (char)0xff) && 
         (buffer[1] == (char)0xd8) &&
         (buffer[2] == (char)0xff) &&
         (is_bit_set(buffer[3],7)) && 
         (is_bit_set(buffer[3],6)) &&
         (is_bit_set(buffer[3],6))))
       {
           
           //If file open close it 
           if(fileopen)
           {
               fclose(output_file);
               fileopen = false;
           }   
           
           //Create New file
           generate_filename(new_filename, counter);
           output_file = fopen(new_filename, "w");
           fileopen = true;
           counter++;
          
       }
       
       //Write out block 
       if(fileopen)
       {
           fwrite(buffer, 512, 1, output_file);
       }
   }
   
   
   //Free memory used for string  
   free(new_filename);
   
   //Close input file
   fclose(input_file);


}

