#include<stdio.h>
#include<inttypes.h>

#define WORD uint64_t//unsigned 128bit int 
#define PF PRIx64
#define W 64//length of a word
#define BYTE uint8_t

// union - all of the member variables stored in the same location
//SHA512 works on blocks of 1024 bits
union Block{
    BYTE bytes[128];//8 * 128 = 1024- dealing with blocks as bytes - view into the block
    WORD words[16];//16 * 64 = 1024, dealing with blocks as words.Words of 64 bit length - words view
    uint64_t sixf[16];// 16 * 64 - dealing with the last 128 bits of last block
};

//enum for keeping track of where we are with the input message/padding, like a flag
enum Status{
    READ, PAD, END
};

//loop through the preprocesses blocks - Returns 1 if it created a new block from original meassage or padding.
//Returns 0 if all padded message has already been consumed. 0 returns out of the loop.
int next_block(FILE *f, union Block *B, enum Status *S, uint64_t *nobits)//uint64_t *nobits -keeps track of no of bits
{ 
    //no of bytes to read
    size_t nobytes;

    if(*S == END){
        return 0;
    }else if(*S == READ){
        //Try to read  128 bytes from input f,
        nobytes = fread(B->bytes, 1, 128, f);//B is now a pointer to a union block
        //Calculate the total bits read so far.
        *nobits = *nobits + (8 * nobytes);//L value = update what is at address of nobits. R value = go to nobits find actual value, 
        // need enough room for 128 + 1 bits, 1024 - 129, 
        if(nobytes == 128){
            //This happens when we can read 128 bytes from f
            return 1;
        }
        // need enough room for 128 + 1 bits, 1024 - 129,
        else if(nobytes < 112){//111
            //This happens when there is enough room for all the padding
            //Append a 1 bit and 7 0 bits to make the byte
            B->bytes[nobytes++] = 0x80;//in bits: 10000000//possibly 0x8000
            //Append enough 0 bits, leaving 128 at the end
            for(nobytes++; nobytes < 112; nobytes++){//111
                //Append 8 0's
                B->bytes[nobytes++] = 0x00;//in bits: 00000000//possibly 0x0000
            }
            //CHECK BIG ENDIAN
            //Append length of original input
            B->sixf[15] = *nobits;//this needs to be big endian
            //say this is the last block
            *S = END;
        }else{
            //Got to the end of the input message not enough room in this block for all the padding
            //append a 1 bit and 15 0 bits to make a 2 full bytes
            B->bytes[nobytes++] = 0x80;//possibly 0x8000
            //append 0 bits
            for (nobytes++; nobytes < 128; nobytes++)
            {
                 B->bytes[nobytes] = 0x00;//in bits: 00000000//possibly 0x0000
            }
            //Change the status to PAD
            *S = PAD;     
        }
        }else if(*S == PAD){
            nobytes = 0;
            //Append 0 bits
            for (nobytes = 0; nobytes < 112; nobytes++)
            {
                 B->bytes[nobytes] = 0x00;//in bits: 00000000//possibly 0x0000
            }
            //Append nobits as an integer - Check Endianess
             B->sixf[15] = *nobits;
             //Change the status to PAD
            *S = END; 
        }
    return 1;
    
    
    
 }

//on cli man 3 fopen, 3 means give c function fopen. man = manual page. Q to get out of a man pag

int main(int argc, char *argv[]){
    //Remember to error check, if no file etc. Or if file doesn't open properly
    //read docs about different possible errors
    BYTE b;

    int i;
    //the current block
    union Block B;
    
    //total number of bits read
    uint64_t nobits = 0;

    //current status of reading input
    enum Status S = READ;
   

    FILE *f; //* = pointer, 
    f = fopen(argv[1], "r"); // path and mode, argv[1] supply a command line argument or program will crash

    //nobytes = fread(&b,1, 1, f);//&b = memory address of b, read 1 byte, read one of those bytes, read from f
   
    //loop through the preprocessed blocks from the input
    while (next_block(f, &B, &S, &nobits))
    {
        //print the 16  64 bit words
        for(i=0; i < 16; i++){
        printf("%016" PF " ", B.words[i]);
        
        }
    }
    fclose(f);//close file when finished
    printf("Total bits read %d. \n", nobits);

     return 0;
}