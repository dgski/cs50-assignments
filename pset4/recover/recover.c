#include "stdio.h"
#include "stdbool.h"

bool is_bit_set(unsigned char c, int n)
{
    return (1 & (c >> n));
}





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
    
   int counter = 0;

   bool fileopen = false;



   //Output file pointer
   FILE *output_file;
    
   //New file name
   char* new_filename[10];
   sprintf(new_filename, "00%d.jpeg", counter);
   

   //Read in one block at a time
   while(fread(buffer, 1, 512, input_file) == 512)
   {
       for(int i = 0; i < 512; i++)
       {    


           //Check if start of file
           if( (buffer[i] == (char)0xff) && 
             (buffer[i+1] == (char)0xd8) &&
             (buffer[i+2] == (char)0xff) &&
             (is_bit_set(buffer[i+3],7)) && 
             (is_bit_set(buffer[i+3],6)) &&
             (is_bit_set(buffer[i+3],6)))
           {
               
               if(!fileopen)
               {
                   printf("CREATING NEW FILE\n");
                   sprintf(new_filename,"00%d.jpeg", counter);
                   output_file = fopen(new_filename, "w");
                   fileopen = true; 
               }
               else
               {


                   fwrite(buffer, i, 1, output_file), 
                   printf("ClOSING FILE\n");
                   fclose(output_file);
                   fileopen = false;
               }
               
               counter++;

           }
           

       }
       if(fileopen)
       {
           fwrite(buffer, 512, 1, output_file);
           printf("WROTE BLOCK\n");
       }
   }

   printf("Number of Files: %d\n", counter);
   fclose(input_file);


}

