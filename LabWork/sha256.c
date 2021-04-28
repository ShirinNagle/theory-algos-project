#include<stdio.h>
#include<inttypes.h>

//Endianess. Adapted from   https://developer.ibm.com/technologies/systems/articles/au-endianc/
#include<byteswap.h>
const int _i = 1;
#define is_lilendian() ((*(char*)&_i) !=0)

//words and bytes
#define WORD uint32_t
#define PF PRIx32//print format for the word - 32bit hex
#define BYTE uint8_t//

//P 5 of the secure hash standard
#define ROTL(_x,_n) ((_x << _n) | (_x >> ((sizeof(_x)*8) - _n)))
#define ROTR(_x,_n) ((_x >> _n) | (_x << ((sizeof(_x)*8) - _n)))
#define SHR(_x,_n) (_x >> _n)


//P 10 of the secure hash standard
#define CH(_x,_y,_z) ((_x & _y) ^ (~_x & _z))
#define MAJ(_x,_y,_z) ((_x & _y) ^ (_x & _z) ^ (_y & _z))

#define SIG0(_x) (ROTR(_x,2)  ^ ROTR(_x,13) ^ ROTR(_x,22))
#define SIG1(_x) (ROTR(_x,6)  ^ ROTR(_x,11) ^ ROTR(_x,25))
#define Sig0(_x) (ROTR(_x,7)  ^ ROTR(_x,18) ^ SHR(_x,3))
#define Sig1(_x) (ROTR(_x,17) ^ ROTR(_x,19) ^ SHR(_x,10))

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


//Global arrays
//Section 4.2.2
const WORD K[]={
0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

//Section 5.4.3 P15

//loop through the preprocesses blocks - Returns 1 if it created a new block from original meassage or padding.
//Returns 0 if all padded message has already been consumed. 0 returns out of the loop.
int next_block(FILE *f, union Block *M, enum Status *S, uint64_t *nobits){ 
    //no of bytes to read
    size_t nobytes;

    if(*S == END){
        return 0;
    } else if(*S == READ){
        //Try to read 64 bytes from the input file
        //fread is a more accurate way to check if the end of file has been reached.in pref to feof
        nobytes = fread(M->bytes, 1, 64, f);//B is now a pointer to a union block
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
            //return 1;//actually do nothing here.
        } else if(nobytes < 56){
            //Append a 1 bit(and seven 0 bits to make full byte)
            //this happens when there is enough room for all the padding.
            M->bytes[nobytes] = 0x80;//In bits: 10000000
            //Append enough 0 bits, leaving 64 at the end
            for(nobytes++;nobytes < 56; nobytes++){
                M->bytes[nobytes]= 0x00;//in bits: 0000000
            }
            //Append length of original input (CHECK ENDIANESS)
            M->sixf[7] = (is_lilendian() ? bswap_64(*nobits): *nobits);
           //say this is the last block
           *S = END;
        } else{
            //Gotten to end of input message  - not enough room in this block for all the padding
            //Append a 1 bit(and 7 0 bits to make a full byte).
            M->bytes[nobytes] = 0x80;
            //Append 0 bits.
            for(nobytes++; nobytes < 64; nobytes++){
                M->bytes[nobytes]= 0x00;//in bits: 0000000
            }
            // Change to staus to PAD
            *S = PAD;
        }
    } else if(*S == PAD){
        //nobytes = 0;
        //Append 0 bits
        for(nobytes = 0; nobytes < 56; nobytes++){
                M->bytes[nobytes]= 0x00;//in bits: 0000000
            }
        //Append nobits as an integer. CHECK ENDIAN
        M->sixf[7] = (is_lilendian() ? bswap_64(*nobits): *nobits);
        //Change the status to END
        *S = END;
    }
    //swap the byte order of the words if we're little endian
    if(is_lilendian())
        for(int i = 0; i < 16; i++)
            M->words[i] = bswap_32(M->words[i]);//reverses byte order, operations will happen big endian style
        
    
    return 1;
}

int next_hash(union Block *M, WORD H[])
{
  /* H_0 -> H_1 -> H_2 -> H_3 -> ....H_n
             ^      ^       ^         ^
             |      |       |         |
            M_1    M_2     M_3       M_n
    H goes from 0 up to n        
    message blocks M go from 1 up to n
    message is telling you how many intermediate hash valuse you have to calculate, n is the no of blocks you need once you pad ypur message
    */
   // message schedule, section 6.2.2
   WORD W[64];
   //iterator
   int t;

   //temp variables
   WORD a, b, c, d, e, f, g, h, T1, T2;
   //section 6.2.2, part 1
   for(t = 0; t < 16 ; t++)
       W[t] = M->words[t];
   
   for(t = 16; t < 64; t++)
       W[t] = Sig1(W[t-2]) + W[t-7] + Sig0( W[t-15]) + W[t-16];
   
    //section 6.2.2, part 2
    a = H[0]; b = H[1]; c = H[2]; d = H[3];
    e = H[4]; f = H[5]; g = H[6]; h = H[7];
    //section 6.2.2, part 3
    for(t = 0; t < 64; t++)
    {
        T1 = h + SIG1(e) + CH(e, f, g) + K[t] + W[t];
        T2 = SIG0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
        //printf("a: %08" PF "...", a);
    }
    //section 6.2.2, part 4
    H[0] = a + H[0]; H[1] = b + H[1]; H[2] = c + H[2]; H[3] = d + H[3];
    H[4] = e + H[4]; H[5] = f + H[5]; H[6] = g + H[6]; H[7] = h + H[7];

    return 0;
}



int sha256(FILE *f,WORD H[]){//H is a pointer to an array that is passed in
    //The function that performs the SHA256 algorithm on message f.
    
    //the current block
    union Block M;
    //total number of bits read
    uint64_t nobits = 0;
    //current status of reading input
    enum Status S = READ;
    while (next_block(f, &M, &S, &nobits))
    {
      next_hash(&M, H);//&M sends the address of M, H sends the address of the 1st item in H. as H is an array
    }
    return 0;
}


int main(int argc, char *argv[]){
    //Remember to error check, if no file etc. Or if file doesn't open properly
    //read docs about different possible errors

    //From section 5.3.4
    WORD H[] ={
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
    };
    //* = pointer, File pointer for reading
    FILE *f;  
    //open file from cli for reading
    f = fopen(argv[1], "r"); // path and mode

    //Calculate the SHA256 of f.
    sha256(f, H);
    
    // print the final SHA256
    for(int i= 0; i < 8; i++){
        printf("%08" PF, H[i]);
    }
    printf("\n");
    fclose(f);//close file when finished
    

     return 0;
}

//padding, look over it in documentation, look up union vs a struct.