#include <stdio.h>
#include <inttypes.h>

//Endianess
// https://developer.ibm.com/technologies/systems/articles/au-endianc/
#include <byteswap.h>
const int _i = 1;
#define islilend() ((*(char*)&_i) != 0)

// Words and bytes.
#define WORD uint32_t
#define PF PRIx32
#define BYTE uint8_t

// Page 5 of the secure hash standard.
#define ROTL(_x,_n) ((_x << _n) | (_x >> ((sizeof(_x)*8) - _n)))
#define ROTR(_x,_n) ((_x >> _n) | (_x << ((sizeof(_x)*8) - _n)))
#define SHR(_x,_n) (_x >> _n)

// Page 10 of the secure hash standard.
#define CH(_x,_y,_z) ((_x & _y) ^ (~_x & _z))
#define MAJ(_x,_y,_z) ((_x & _y) ^ (_x & _z) ^ (_y & _z))

#define SIG0(_x) (ROTR(_x,2)  ^ ROTR(_x,13) ^ ROTR(_x,22))
#define SIG1(_x) (ROTR(_x,6)  ^ ROTR(_x,11) ^ ROTR(_x,25))
#define Sig0(_x) (ROTR(_x,7)  ^ ROTR(_x,18) ^ SHR(_x,3))
#define Sig1(_x) (ROTR(_x,17) ^ ROTR(_x,19) ^ SHR(_x,10))


// SHA256 works on blocks of 512 bits.
union Block {
    // 8 x 64 = 512 - dealing with block as bytes.
    BYTE bytes[64];
    // 32 x 16 = 512 - dealing with block as words.
    WORD words[16];
    // 64 x 8 = 512 - dealing with the last 64 bits of last block.
    uint64_t sixf[8];
};

// For keeping track of where we are with the input message/padding.
enum Status {
    READ, PAD, END
};

const WORD K[] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 
    0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 
    0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 
    0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 
    0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 
    0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 
    0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 
    0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 
    0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};

// Returns 1 if it created a new block from original message or padding.
// Returns 0 if all padded message has already been consumed.
int next_block(FILE *f, union Block *B, enum Status *S, uint64_t *nobits) {
     // Number of bytes read.
    size_t nobytes;

    if (*S == END) {
        return 0;
    } else if (*S == READ) {
        // Try to read 64 bytes from the input file.
        nobytes = fread(B->bytes, 1, 64, f);
        // Calculate the total bits read so far.
        *nobits = *nobits + (8 * nobytes);
        // Enough room for padding.
        if (nobytes == 64) {
            // This happens when we can read 64 bytes from f.
            //return 1;
        } else if (nobytes < 56) {
            // This happens when we have enough roof for all the padding.
            // Append a 1 bit (and seven 0 bits to make a full byte).
            B->bytes[nobytes] = 0x80; // In bits: 1000000
            // Append enough 0 bits, leaving 64 at the end.
            for (nobytes++; nobytes < 56; nobytes++) {
                B->bytes[nobytes] = 0x00; // In bits: 00000000
            }
            // Append length of original input (CHECK ENDIANESS).
            B->sixf[7] = (islilend() ? bswap_64(*nobits) : *nobits);
            // Say this is the last block.
            *S = END;
        } else {
            // Got to the end of the input message and not enough room
            // in this block for all padding.
            // Append a 1 bit (and seven 0 bits to make a full byte.)
            B->bytes[nobytes] = 0x80;
            // Append 0 bits.
            for (nobytes++; nobytes < 64; nobytes++) {
                // Error: trying to write to 
                B->bytes[nobytes] = 0x00; // In bits: 00000000
            }
            // Change the status to PAD.
            *S = PAD;
        }
    } else if (*S == PAD) {
        // Append 0 bits.
        for (nobytes = 0; nobytes < 56; nobytes++) {
            B->bytes[nobytes] = 0x00; // In bits: 00000000
        }
        // Append nobits as a big endian integer
        B->sixf[7] = (islilend() ? bswap_64(*nobits) : *nobits);
        // Change the status to END.
        *S = END;
    } 
    // Swap order of the words if were little endian.
    if(islilend())
        for (int i = 0; i < 16; i++)
            B -> words[i] = bswap_32(B -> words[i]);
        
    return 1;
}

int next_hash(union Block *M, WORD H[]) {
  
    // Message schedule, Section 6.2.2
    WORD W[64];
    // Iterator.
    int t;
    // Temporary variables.
    WORD a, b, c, d, e, f, g, h, T1, T2;

    // Section 6.2.2, part 1.
    for (t = 0; t < 16; t++)
        W[t] = M->words[t];
    for (t = 16; t < 64; t++)
        W[t] = Sig1(W[t-2]) + W[t-7] + Sig0(W[t-15]) + W[t-16];

    // Section 6.2.2, part 2.
    a = H[0]; b = H[1]; c = H[2]; d = H[3];
    e = H[4]; f = H[5]; g = H[6]; h = H[7];

    // Section 6.2.2, part 3.
    for (t = 0; t < 64; t++) {
        T1 = h + SIG1(e) + CH(e, f, g) + K[t] + W[t];
        T2 = SIG0(a) + MAJ(a, b, c);
        h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;
    }

    // Section 6.2.2, part 4.
    H[0] = a + H[0]; H[1] = b + H[1]; H[2] = c + H[2]; H[3] = d + H[3];
    H[4] = e + H[4]; H[5] = f + H[5]; H[6] = g + H[6]; H[7] = h + H[7];

    return 0;
}

int sha256(FILE *f, WORD H[]) {
    // The function that performs/orchestrates the SHA256 algorithm on
    // message f.

    // The current block.
    union Block M;

    // Total number of bits read.
    uint64_t nobits = 0;

    // Current status of reading input.
    enum Status S = READ;

    // Loop through the (preprocessed) blocks.
    while (next_block(f, &M, &S, &nobits)) {
        next_hash(&M, H);
    }

    return 0;
}

int main(int argc, char *argv[]){
    // Section 5.3.4
    WORD H[] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // Reading - File pointer
    FILE *f;

    // Open the file with Cmd Line
    f = fopen(argv[1], "r");

    // Calculate the SHA256 of f.
    sha256(f, H);

    // Print the final SHA256 hash.
    for (int i = 0; i < 8; i++)
        printf("%08" PF, H[i]);
    printf("\n");

    // Close the file.
    fclose(f);

    return 0;
}
