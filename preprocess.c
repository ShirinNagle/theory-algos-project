#include<stdio.h>
#include<inttypes.h>

#define WORD uint32_t
#define PF PRIX32
#define W 32
#define BYTE uint8_t

union Block{//all memory variables stored in same location
 BYTE bytes[64];
 WORD words[16];
};

//on cli man 3 fopen, 3 means give c function fopen. man = manual page. Q to get out of a man pag

int main(int argc, char *argv[]){
    //Remember to error check, if no file etc. Or if file doesn't open properly
    //read docs about different possible errors

    //the current block
    union Block B;
    
    //total number of bits read
    uint64_t nobits =0;
   
    //* = pointer, File pointer for reading
    FILE *f;  
    //open file from cli for reading
    f = fopen(argv[1], "r"); // path and mode
    //number of bytes to read
    size_t nobytes;
    //try to read bytes
    nobytes = fread(B.bytes,1, 64, f);//&b = memory address of b, read 1 byte, read 64 times, read from f
    printf("Read %d bytes. \n", nobytes);
    //update of bits read
    nobits = nobits +(8 * nobytes);
    //Print 16 x 32bit words to screen
    for(int i=0; i < 16; i++){
        printf("%08" PF " ", B.words[i]);
        if(i+1 % 8 == 0)
         printf("\n");
    }

    while (!feof(f))
    {
        //do same here
        nobytes = fread(&B.bytes,1, 64, f);
        printf("Read %d bytes: \n", nobytes);
        nobits = nobits +(8 * nobytes);

        for(int i=0; i < 16; i++){
        printf("%08" PF " ", B.words[i]);
        if(i+1 % 8 == 0)
         printf("\n");
    }
    }
    fclose(f);//close file when finished
    printf("Total bits read %d. \n", nobits);

     return 0;
}

//padding, look over it in documentation, look up union vs a struct.