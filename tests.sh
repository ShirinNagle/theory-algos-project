#!/bin/bash

#If there is time consider adding a for loop to calculate value for a list of files files="$(ls)"

echo "Checking file input.txt."
expec="$(sha512sum input.txt)"
actual="$(./512SHA input.txt)  input.txt"
echo "Expected:" $expec
echo "Actual: "  $actual

if [[ $actual == $expec ]];then
    echo "Pass"
else 
    echo "Fail"
fi

#checks if input txt equals the hash output - keeping below as a reminder about spacing
#if [[ "$(./512SHA input.txt)" == "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f" ]];then
#    exho "Pass"
#else
#    echo "Fail"
#fi 


#./tests.sh 
