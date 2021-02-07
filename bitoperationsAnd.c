#include <stdio.h>


void bin_print(unsigned int i){
    //number of bits in an integer
    int j = sizeof(unsigned int) * 8;
    //temp variable
    int k;
    
    //loop over the number of bits in i left to right
    for(j--;j >= 0; j--){
        //Pick out j^th bit of i, ie. k will evaluates to true when j is equal to 1
        k=((1 << j) & i) ? 1:0;//? : is a ternary operator. 
        //print k
        printf("%d",k);
    }
}
int main(int argc, char *argv[])//main takes 2 arguments, arg count and a pointer to a list
{
    //https://en.wikipedia.org/wiki/Bitwise_operations_in_C
    
    unsigned int i = 0x0f0f0f0f;//241;//4294967295
    //int j = 0xf1;//hex value of 241
    bin_print(i);
    printf("\n");
    
    printf("\n");

    //bit shifting example
    printf("Original:  ");
    //print in binary
    bin_print(i);
    printf("\n");
    printf("\t%x\t%u\n\n",i,i);

    // the below is 32
    int j =sizeof(unsigned int) * 8;

    for(j --;j >= 0; j--){
        //Prints out the value of i, shifted j positions. eg 241 << 1: 
        bin_print(1 << j);
        printf("\n");
        //1 shifted left j times & i & is bitwise logical AND, ie. 1+1 = 1, 1+0 = 0, 0+0=0
        bin_print(i);
        printf("\n");
        printf("-------------------------------- &\n");
        bin_print((1 << j) & i);
        printf("\n\n");
    }
    //All ok
    return 0;
}