#include <stdio.h>
#include <inttypes.h>

// Run on Linux:
// 1: navigate to folder: cd Labs
// 2. create executable: make 09d_preprocessing
// 3: run: ./09d_preprocessing test_file.txt
// 4. change contents of test_file.txt to "abc" and it will print output "a" "b" "c" as per Section 5.1.2

// Define macros
#define BYTE uint8_t
#define WORD uint32_t
#define PX PRIX32
#define PU PRIu32
#define W 32 // number of bits in a word

// union - all of the member variables stored in the same location
// useful - if we want to read or write to memory in bytes, we can use bytes member var
// if we want to read or write to memory as 32 bit int(words), we can use words member var
union Block { // used as per Section 5.2.1
  BYTE bytes[64];
  WORD words[16];
};

int main(int argc, char *argv[]) {

  // the current block
  union Block B;

  // Total number of bits read - last 64 bits - section 5.1.1
  uint64_t number_of_bits = 0;

  // File pointer for reading
  FILE *fPointer;
  // Open file from command line for reading
  fPointer = fopen(argv[1], "r");

  // Number of current bytes read
  size_t number_of_bytes;

  if (fPointer == NULL) {
    printf("File not found\n");
    return 1;
  }

  // https://www.debugcn.com/en/article/61281011.html
  for (;;) {
    if (feof(fPointer)) break;
    number_of_bytes = fread(B.bytes, 1, 64, fPointer); // read 1 byte 64 times to B ... Section 5.2.1
    // Tell the command line how many we read
    printf("Read %lu bytes\n", number_of_bytes);
    // Update the number of bits read
    number_of_bits = number_of_bits + (8 * number_of_bytes);
    // Print the 16 32-bit words
    for (int i = 0; i < 16; i++) {
      printf("%08" PX " ", B.words[i]);
      if ((i + 1) % 8 == 0) {
        printf("\n"); // first 4 bytes: 00636261 -> 0 c b a ... the rest is gibberish
      }
    }
  }
  // Close the file
  fclose(fPointer);
  // Print total number of bits read
  printf("Total bits read: %lu\n", number_of_bits);

  return 0;
}
