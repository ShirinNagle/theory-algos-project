#include<stdio.h>
#include<inttypes.h>

//Preprocessor directives
#define WORD uint32_t// possibly a problem as machine may be little endian
#define PF PRIX32//print format for the word - 32bit hex
#define W 32//length of a word
#define BYTE uint8_t//

//SHA256 works on blocks of 512 bits
union Block{//all memory variables stored in same location
 BYTE bytes[64];//8 * 64 = 512- dealing with blocks as bytes
 WORD words[16];//32 x 16 = 512 - dealing with blocks as words
 uint64_t sixf[8];// 8 * 64 - dealing with the last 64 bits of last block
};

//enum for keeping track of where we are with the input message/padding, like a flag
enum Status{
    READ, PAD, END
};

//loop through the preprocesses blocks - Returns 1 if it created a new block from original meassage or padding.
//Returns 0 if all padded message has already been consumed. 0 returns out of the loop.
int next_block(FILE *f, union Block *B, enum Status *S, uint64_t *nobits){ 
    //no of bytes to read
    size_t nobytes;

    if(*S == END){
        return 0;
    } else if(*S == READ){
        //Try to read 64 bytes from the input file
        //fread is a more accurate way to check if the end of file has been reached.in pref to feof
        nobytes = fread(B->bytes, 1, 64, f);//B is now a pointer to a union block
        //Calculate the total bits read so far.
        *nobits = *nobits + (8 * nobytes);//update nobits
        /* *nobits = *nobits + (8 * nobytes);
        on lhs of assignment *nobits means update whatever is at the address of nobits - find the address and update whatever is there
        on rhs of assignment *nobits means whatever nobits points at and find its value - get the value
        */
        //512 bits -> 512 - 65 same as 64 bytes -> 55 bytes
        //enough room for padding
        if(nobytes == 64){//if read 64 bytes - return 1
        // this happens when we can read 64 bytes from f.
            return 1;
        } else if(nobytes < 56){
            //Append a 1 bit(and seven 0 bits to make full byte)
            //this happens when there is enough room for all the padding.
            B->bytes[nobytes] = 0x80;//In bits: 10000000
            //Append enough 0 bits, leaving 64 at the end
            for(nobytes++;nobytes < 56; nobytes++){
                B->bytes[nobytes]= 0x00;//in bits: 0000000
            }
            //Append length of original input (CHECK ENDIANESS)
            B->sixf[7] = *nobits;
           //say this is the last block
           *S = END;
        } else{
            //Gotten to end of input message  - not enough room in this block for all the padding
            //Append a 1 bit(and 7 0 bits to make a full byte).
            B->bytes[nobytes++] = 0x80;
            //Append 0 bits.
            for(nobytes++; nobytes < 64; nobytes++){
                B->bytes[nobytes]= 0x00;//in bits: 0000000
            }
            // Change to staus to PAD
            *S = PAD;
        }
    } else if(*S == PAD){
        nobytes = 0;
        //Append 0 bits
        for(nobytes = 0; nobytes < 56; nobytes++){
                B->bytes[nobytes]= 0x00;//in bits: 0000000
            }
        //Append nobits as an integer. CHECK ENDIAN
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

    int i;
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
        //print the 16  32 bit words
        for(i=0; i < 16; i++){
        printf("%08" PF " ", B.words[i]);
        }
         printf("\n");
    
    }
    fclose(f);//close file when finished
    printf("Total bits read %d. \n", nobits);

     return 0;
}

//padding, look over it in documentation, look up union vs a struct.