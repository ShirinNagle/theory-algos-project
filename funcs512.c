#include<stdio.h>
#include <inttypes.h>

//define a macro called WORD, define is a preprocessor directive, replaces WORD before compile,does'nt run at compile time
#define WORD uint64_t//unsigned 64bit int 
#define PF PRIx64
#define W 64

//x, y and z need to be 64-bit words
//note the size of ints in different devices may be a different size to 64-bits. Ch() = choose
WORD Ch(WORD x, WORD y, WORD z)
{
    return (x & y)^(~x & z);
}

WORD Maj(WORD x, WORD y, WORD z){
    return (x & y)^(x & z)^(y & z);
}


int main(int argc, char *argv[]){

WORD x = 0xf0f0f0f0;
WORD y = 0x0f0f0f0f;
WORD z = 0xf0f0f0f0;

WORD ans = Ch(x,y,z);
//WORD ans1 = Maj(x,y,z);
 printf("Ch(%08"PF", %08"PF", %08"PF") = %08"PF"\n", x, y, z, ans );
 //printf("Maj(%08"PF", %08"PF", %08"PF") = %08"PF"\n", x, y, z, ans1 );
    return 0;
}