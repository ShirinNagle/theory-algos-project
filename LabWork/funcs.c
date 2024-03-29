#include<stdio.h>
#include <inttypes.h>

//define a macro called WORD, define is a preprocessor directive, doesn't run at compile time
#define WORD uint32_t//unsigned 32bit int 
#define PF PRIX32
#define W 32

//look at ROTR, SHR and Sigma large and sigma small.

/*The rotate right (circular right shift) operation ROTR n(x), where x is a w-bit word
and n is an integer with 0  n < w, is defined by
ROTR n(x)=(x >> n)  (x << w - n).
n = Number of bits to be rotated or shifted when a word is operated upon
w = Number of bits in a word. 32 bits in this case*/

/*
The right shift operation, where x is a w-bit word and n is an integer with 0
 n < w, is defined by SHR n(x)=x >> n.
SHR n(x)=x >> n.
n = Number of bits to be rotated or shifted when a word is operated upon.
*/

//x, y and z need to be 32-bit words
//note the size of ints in different devices may be a different size to 32-bits. Ch() = choose

//P 5 of the secure hash standard
#define ROTR(x,n) (x >> n)|(x << W - n)
#define SHR(x,n) (x >> n)

//P 10 of the secure hash standard
#define CH(x,y,z) (x & y)^(~x & z)
#define MAJ(x,y,z) (x & y)^(x & z)^(y & z)

#define SIG0(x) ROTR(x,2)^ROTR(x,13)^ROTR(x,22)
#define SIG1(x) ROTR(x,6)^ROTR(x,11)^ROTR(x,25)
#define Sig0(x) ROTR(x,7)^ROTR(x,18)^ROTR(x,3)
#define Sig1(x) ROTR(x,17)^ROTR(x,19)^ROTR(x,10)

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
//Initial hash values, part of preprocessing file
WORD H[] ={
    0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
    0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
};

int main(int argc, char *argv[]){

WORD x = 0xf1234567;
WORD y = 0x0a0a0a0a;
WORD z = 0xf0f0f0f0;

// Temporary WORD for answers
WORD ans;
ans = CH(x,y,z);

 printf("Ch(%08" PF ", %08" PF ", %08" PF ") = %08" PF "\n", x, y, z, ans );
 ans = MAJ(x,y,z);
 printf("Maj(%08" PF ", %08" PF ", %08" PF ") = %08" PF "\n", x, y, z, ans );

printf("ROTR(%08" PF " -> %08" PF ")\n",x, ROTR(x,4));
printf("SHR(%08" PF " -> %08" PF ")\n",x, SHR(x,4));
printf("SIG0(%08" PF " -> %08" PF ")\n",x, SIG0(x));
printf("SIG1(%08" PF " -> %08" PF ")\n",x, SIG1(x));
printf("Sig0(%08" PF " -> %08" PF ")\n",x, Sig0(x));
printf("Sig1(%08" PF " -> %08" PF ")\n",x, Sig1(x));

printf("K[0] = %08" PF "\tk[63] = %08" PF "\n", K[0], K[63]);

return 0;
}