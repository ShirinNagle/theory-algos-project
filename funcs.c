#include<stdio.h>
#include <inttypes.h>

//define a macro called WORD, define is a preprocessor directive, doesn't run at compile time
#define WORD uint32_t
#define PF PRIX32

//look at ROTR, SHR and Sigma large and sigma small.

//x, y and z need to be 32-bit words
//note the size of ints in different devices may be a different size to 32-bits. Ch() = choose
WORD Ch(WORD x, WORD y, WORD z)
{
    return (x & y)^(~x & z);
}

WORD Maj(WORD x, WORD y, WORD z)
{
    return(x & y)^(x & z)^(y & z);
}

int main(int argc, char *argv[]){

WORD x = 0x0f0f0f0f;
WORD y = 0x0a0a0a0a;
WORD z = 0xf0f0f0f0;

WORD ans = Ch(x,y,z);
WORD ans1 = Maj(x,y,z);
 printf("Ch(%08"PF", %08"PF", %08"PF") = %08"PF"\n", x, y, z, ans );
 printf("Ch(%08"PF", %08"PF", %08"PF") = %08"PF"\n", x, y, z, ans1 );
    return 0;
}