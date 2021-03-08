#include<stdio.h>
#include<inttypes.h>

#define BYTE uint8_t

void bin_print(BYTE i){
    //number of bits in an integer
    int j = sizeof(BYTE) * 8;
    //temp variable
    int k;
    
    //loop over the number of bits in i
    for(j--;j >= 0; j--){
        //<< bitshift to the left & bitwise and
        k=((1 << j) & i) ? 1:0;//? : is a ternary operator. 
        //condition on the left, what to do if true, what to do if false

        //print k
        printf("%d",k);
    }
}

//on cli man 3 fopen, 3 means give c function fopen. man = manual page. Q to get out of a man pag

int main(int argc, char *argv[]){
    //Remember to error check, if no file etc. Or if file doesn't open properly
    //read docs about different possible errors
    BYTE b;

    FILE *f; //* = pointer, 
    f = fopen(argv[1], "r"); // path and mode

    size_t nobytes;//number of bytes

    nobytes = fread(&b,1, 1, f);//&b = memory address of b, read 1 byte, read one of those bytes, read from f
   
    while (nobytes)
    {
        bin_print(b);
        nobytes = fread(&b,1, 1, f);
    }
    fclose(f);//close file when finished
    printf("\n");




     return 0;
}