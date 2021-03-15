#include<stdio.h>
#include<inttypes.h>

#define WORD uint32_t
#define PF PRIX32
#define W 32
#define BYTE uint8_t

union Block{//all memory variables stored in same location
 BYTE bytes[64];
 WORD words[16];
 uint64_t sixf[8];
};

enum Status{
    READ, PAD, END
};

//loop through the preprocesses blocks
int next_block(FILE *f, union Block *B, enum Status *S, uint64_t *nobits){ 
    //no of bytes to read
    size_t nobytes;

    if(*S == END){
        return 0;
    } else if(*S == READ){
        //Try to read 64 bytes
        nobytes = fread(B->bytes, 1, 64, f);//B is now a pointer to a union block
        //Calculate the total bits read so far.
        *nobits = *nobits + (8 * nobytes);
        //512 bits -> 512 - 65 same as 64 bytes -> 55 bytes
        //enough room for padding
        if(nobytes == 64){
            return 1;
        } else if(nobytes <= 55){
            //Append a 1 bit(and seven 0 bits to make full byte)
            B->bytes[nobytes++] = 0x80;//In bits: 1000000
            //Append enough 0 bits, leaving 64 at the end
            while(nobytes++ < 56){
                B->bytes[nobytes]= 0x00;//in bits: 0000000
            }
            //Append length of original input (CHECK ENDIANESS)
            B->sixf[7] = *nobits;
           //say this is the last block
           *S = END;
        } else{
            //Gotten to end of input message  - not em=nough room in this block for all the padding
            //Append a 1 bit(and 7 0 bits to make a full byte).
            B->bytes[nobytes] = 0x80;
            //Append 0 bits.
            while(nobytes++ < 64){
                B->bytes[nobytes]= 0x00;//in bits: 0000000
            }
            // Change to staus to PAD
            *S = PAD;
        }
    } else if(*S == PAD){
        nobytes = 0;
        //Append 0 bits
        while(nobytes++ <56){
                B->bytes[nobytes]= 0x00;//in bits: 0000000
            }
        //Append nobits as an integer
        B->sixf[7] = *nobits;
        //Change the status to END
        *S = END;


    }
    return 1;
}

//on cli man 3 fopen, 3 means give c function fopen. man = manual page. Q to get out of a man pag

int main(int argc, char *argv[]){
    //Remember to error check, if no file etc. Or if file doesn't open properly
    //read docs about different possible errors

    //the current block
    union Block B;
    
    //total number of bits read
    uint64_t nobits =0;

    //current status of reading input
    enum Status S = READ;
   
    //* = pointer, File pointer for reading
    FILE *f;  
    //open file from cli for reading
    f = fopen(argv[1], "r"); // path and mode
    //number of bytes to read
    

    //loop through the preprocesses blocks
    while (next_block(f, &B, &S, &nobits))
    {
        
        for(int i=0; i < 16; i++){
        printf("%08" PF " ", B.words[i]);
        }
         printf("\n");
    
    }
    fclose(f);//close file when finished
    printf("Total bits read %d. \n", nobits);

     return 0;
}

//padding, look over it in documentation, look up union vs a struct.