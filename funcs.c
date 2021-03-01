#include<stdio.h>
#include <inttypes.h>

//define a macro called WORD, define is a preprocessor directive, doesn't run at compile time
#define WORD uint32_t
#define PF PRIX32
#define W 32

//look at ROTR, SHR and Sigma large and sigma small.

/*The rotate right (circular right shift) operation ROTR n(x), where x is a w-bit word
and n is an integer with 0  n < w, is defined by
ROTR n(x)=(x >> n)  (x << w - n).
n = Number of bits to be rotated or shifted when a word is operated upon
w = Number of bits in a word. 32 bits in this case*/
WORD ROTR(WORD n, WORD x){
    return (x >> n)|(x << W - n);
}

/*
The right shift operation, where x is a w-bit word and n is an integer with 0
 n < w, is defined by SHR n(x)=x >> n.
SHR n(x)=x >> n.
n = Number of bits to be rotated or shifted when a word is operated upon.
*/
WORD SHR(WORD n, WORD x){
    return(x >> n);
}



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
 printf("Maj(%08"PF", %08"PF", %08"PF") = %08"PF"\n", x, y, z, ans1 );
    return 0;
}