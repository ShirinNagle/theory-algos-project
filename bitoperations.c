#include <stdio.h>


void bin_print(unsigned int i){
    //number of bits in an integer
    int j = sizeof(unsigned int) * 8;
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
int main(int argc, char *argv[])//main takes 2 arguments, arg count and a pointer to a list
{
    //https://en.wikipedia.org/wiki/Bitwise_operations_in_C
    
    unsigned int i = 0xffffffff;//241;//4294967295
    int j = 0xf1;//hex value of 241
    bin_print(i);
    printf("\n");
    printf("j in binary from hex:\n");
    bin_print(j);
    printf("\n");

    //bit shifting example
    printf("Original:  ");
    //print in binary
    bin_print(i);
    printf("\n");
    printf("\t%x\t%d\n",i,i);

    for(int j =0;j < 40; j++){
        //Prints out the value of i, shifted j positions. eg 241 << 1: 
        printf("%3u << %2d: ", i, j);
        //i shifted left j times
        bin_print(i << j);
        printf("\n");
    }
    //All ok
    return 0;
}