#include <stdio.h>

int main(int argc, char *argv[])//main takes 2 arguments, arg count and a pointer to a list
{
    //https://en.wikipedia.org/wiki/Bitwise_operations_in_C
    int i = 241;

    printf("Dec: %d\n", i);
    printf("Hex: %x\n", i);//use X for capital letters to print out.

    int j = 0xF1;//0x tells compiler the number is in hex.
    printf("Dec: %d\n", j);

    //to find out how many bytes are in an int - returns 4
    printf("Size of i: %d\n", sizeof(i));
    //can check the size of types
    printf("Size of int: %d\n", sizeof(char));
    printf("Size of long: %d\n", sizeof(long));

    //compiler will compile the below but not as expected
    char c = 41;
    printf("c in char is: %c\n", c);
    printf("c in int is: %d\n", c);

    char d = 65;
    printf("d in char is: %c\n", d);
    printf("d in int is: %d\n", d);

    int e = 1000000000;
    printf("e in char is: %c\n", e);// no output, char size 1 byte, too small to output e.
    printf("e in int is: %d\n", e);



    //should always return 0 from main if there hasn't been an error, if not incl
    //chaining programs on command line is not possible.
    return 0;
}