#include<stdio.h>
#include<inttypes.h>

#define WORD uint128_t//unsigned 64bit int 
#define PF PRIx128
#define W 64//length of a word
#define BYTE uint8_t

// union - all of the member variables stored in the same location
// useful - if we want to read or write to memory in bytes, we can use bytes member var
// if we want to read or write to memory as 32 bit int(words), we can use words member var
union Block{
    BYTE bytes[128];
    WORD words[16];//16 words of 64 bit length
}


//on cli man 3 fopen, 3 means give c function fopen. man = manual page. Q to get out of a man pag

int main(int argc, char *argv[]){
    //Remember to error check, if no file etc. Or if file doesn't open properly
    //read docs about different possible errors
    BYTE b;

    FILE *f; //* = pointer, 
    f = fopen(argv[1], "r"); // path and mode, argv[1] supply a command line argument or program will crash

    size_t nobytes;//number of bytes

    nobytes = fread(&b,1, 1, f);//&b = memory address of b, read 1 byte, read one of those bytes, read from f
   
    while (nobytes)
    {
        bin_print(b);
        nobytes = fread(&b,1, 1, f);
    }
    fclose(f);//close file when finished
    printf("\n");




     return 0;
}