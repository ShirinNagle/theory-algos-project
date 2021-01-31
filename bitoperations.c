#include <stdio.h>


void bin_print(int i){
    //number of bits in an integer
    int j = sizeof(int) * 8;

    //temp variable
    int k;
    /*
    look at for loop and figure out what is going on
    */

    for(j--;j >= 0; j--){
        //<< bitshift to the left & bitwise and
        k=((1 << j) & i) ? 1:0;//? : is a ternary operator. 
        //condition on the left, what to do if true, what to do if false
        printf("%d",k);
    }
}

int main(int argc, char *argv[])//main takes 2 arguments, arg count and a pointer to a list
{
    //https://en.wikipedia.org/wiki/Bitwise_operations_in_C
    
    int i = 241;
    int j = 0xf1;
    bin_print(i);
    printf("\n");
    printf("j in binary from hex:\n");
    bin_print(j);
    printf("\n");

    //bit shifting example
    printf("Original:  ");
    bin_print(i);
    printf("\n");

    for(int j =0;j < 40; j++){
        printf("%3d << %2d: ", i, j);
        bin_print(i << j);
        printf("\n");
    }


    return 0;
}